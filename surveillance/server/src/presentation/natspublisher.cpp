#include "natspublisher.h"

#include <QJsonDocument>
#include <QJsonObject>

NatsPublisher::NatsPublisher(QString host, quint16 port, QObject *parent)
    : QObject(parent)
    , m_host(std::move(host))
    , m_port(port) {
    connect(&m_socket, &QTcpSocket::readyRead, this, [this] {
        while (m_socket.canReadLine()) {
            const QByteArray line = m_socket.readLine().trimmed();
            if (line.startsWith("INFO") && !m_handshakeSent) {
                m_handshakeSent = true;
                m_socket.write("CONNECT {\"verbose\":false,\"pedantic\":false}\r\nPING\r\n");
            } else if (line == "PONG") {
                m_ready = true;
                emit statusChanged(QStringLiteral("NATS ready to publish incidents"));
            } else if (line.startsWith("-ERR")) {
                emit statusChanged(QStringLiteral("NATS error: %1").arg(QString::fromUtf8(line)));
            }
        }
    });
    connect(&m_socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        emit statusChanged(QStringLiteral("NATS socket error: %1").arg(m_socket.errorString()));
    });
}

void NatsPublisher::start() {
    m_handshakeSent = false;
    m_ready = false;
    emit statusChanged(QStringLiteral("Connecting to NATS"));
    m_socket.connectToHost(m_host, m_port);
}

void NatsPublisher::publishIncident(const surveillance::Incident &incident) {
    if (!m_ready) {
        emit statusChanged(QStringLiteral("NATS is not ready; incident remains stored in PostgreSQL"));
        return;
    }
    QJsonObject payload {
        { QStringLiteral("incidentId"), incident.incidentId },
        { QStringLiteral("eventId"), incident.eventId },
        { QStringLiteral("cameraId"), incident.cameraId },
        { QStringLiteral("eventType"), incident.eventType },
        { QStringLiteral("severity"), incident.severity },
        { QStringLiteral("zoneId"), incident.zoneId },
        { QStringLiteral("status"), incident.status },
        { QStringLiteral("occurredAt"), incident.occurredAt.toString(Qt::ISODate) },
        { QStringLiteral("latitude"), incident.location.latitude },
        { QStringLiteral("longitude"), incident.location.longitude }
    };
    const QByteArray body = QJsonDocument(payload).toJson(QJsonDocument::Compact);
    const QByteArray command = "PUB security.incidents." + incident.severity.toUtf8() + ' ' + QByteArray::number(body.size()) + "\r\n" + body + "\r\n";
    m_socket.write(command);
}
