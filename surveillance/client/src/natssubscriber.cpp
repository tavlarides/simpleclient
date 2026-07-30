#include "natssubscriber.h"

#include <QJsonDocument>
#include <QJsonObject>

NatsSubscriber::NatsSubscriber(QString host, quint16 port, QObject *parent)
    : QObject(parent)
    , m_host(std::move(host))
    , m_port(port) {
    connect(&m_socket, &QTcpSocket::readyRead, this, [this] {
        m_buffer.append(m_socket.readAll());
        consumeBuffer();
    });
    connect(&m_socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        emit statusChanged(QStringLiteral("NATS socket error: %1").arg(m_socket.errorString()));
    });
}

void NatsSubscriber::start() {
    m_handshakeSent = false;
    emit statusChanged(QStringLiteral("Connecting to NATS"));
    m_socket.connectToHost(m_host, m_port);
}

void NatsSubscriber::consumeBuffer() {
    while (true) {
        if (m_expectedPayloadSize >= 0) {
            if (m_buffer.size() < m_expectedPayloadSize + 2) {
                return;
            }
            const QByteArray payload = m_buffer.left(m_expectedPayloadSize);
            m_buffer.remove(0, m_expectedPayloadSize + 2); // payload and CRLF
            m_expectedPayloadSize = -1;

            const QJsonObject object = QJsonDocument::fromJson(payload).object();
            const QDateTime occurredAt = QDateTime::fromString(object.value(u"occurredAt").toString(), Qt::ISODate);
            if (!object.isEmpty() && occurredAt.isValid()) {
                emit incidentReceived({
                    object.value(u"incidentId").toString(),
                    object.value(u"eventId").toString(),
                    object.value(u"cameraId").toString(),
                    object.value(u"eventType").toString(),
                    object.value(u"severity").toString(),
                    object.value(u"zoneId").toString(),
                    object.value(u"status").toString(),
                    occurredAt,
                    { object.value(u"latitude").toDouble(), object.value(u"longitude").toDouble() }
                });
            }
            continue;
        }

        const qsizetype lineEnd = m_buffer.indexOf("\r\n");
        if (lineEnd < 0) {
            return;
        }
        const QList<QByteArray> parts = m_buffer.left(lineEnd).split(' ');
        m_buffer.remove(0, lineEnd + 2);
        if (parts.first() == "INFO" && !m_handshakeSent) {
            m_handshakeSent = true;
            m_socket.write("CONNECT {\"verbose\":false,\"pedantic\":false}\r\nSUB security.incidents.> 1\r\nPING\r\n");
        } else if (parts.size() == 4 && parts.first() == "MSG") {
            bool validSize = false;
            const qsizetype payloadSize = parts.at(3).toLongLong(&validSize);
            if (validSize && payloadSize >= 0) {
                m_expectedPayloadSize = payloadSize;
            }
        } else if (parts.first() == "PONG") {
            emit statusChanged(QStringLiteral("Subscribed to NATS incidents"));
        } else if (parts.first() == "-ERR") {
            emit statusChanged(QStringLiteral("NATS error: %1").arg(QString::fromUtf8(parts.join(' '))));
        }
    }
}
