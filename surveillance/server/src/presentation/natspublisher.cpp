#include "natspublisher.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>

class NatsPublisherPrivate {
    Q_DECLARE_PUBLIC(NatsPublisher)

public:
    NatsPublisherPrivate(NatsPublisher *q, QString serverHost, quint16 serverPort)
        : q_ptr(q)
        , host(std::move(serverHost))
        , port(serverPort) {}

    NatsPublisher *q_ptr;
    QTcpSocket socket;
    QString host;
    quint16 port;
    bool handshakeSent = false;
    bool ready = false;
};

NatsPublisher::NatsPublisher(QString host, quint16 port, QObject *parent)
    : QObject(parent)
    , d_ptr(new NatsPublisherPrivate(this, std::move(host), port)) {
    Q_D(NatsPublisher);
    connect(&d->socket, &QTcpSocket::readyRead, this, [this] {
        Q_D(NatsPublisher);
        while (d->socket.canReadLine()) {
            const QByteArray line = d->socket.readLine().trimmed();
            if (line.startsWith("INFO") && !d->handshakeSent) {
                d->handshakeSent = true;
                d->socket.write("CONNECT {\"verbose\":false,\"pedantic\":false}\r\nPING\r\n");
            } else if (line == "PONG") {
                d->ready = true;
                emit statusChanged(QStringLiteral("NATS ready to publish incidents"));
            } else if (line.startsWith("-ERR")) {
                emit statusChanged(QStringLiteral("NATS error: %1").arg(QString::fromUtf8(line)));
            }
        }
    });
    connect(&d->socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        Q_D(NatsPublisher);
        emit statusChanged(QStringLiteral("NATS socket error: %1").arg(d->socket.errorString()));
    });
}

NatsPublisher::~NatsPublisher() = default;

void NatsPublisher::start() {
    Q_D(NatsPublisher);
    d->handshakeSent = false;
    d->ready = false;
    emit statusChanged(QStringLiteral("Connecting to NATS"));
    d->socket.connectToHost(d->host, d->port);
}

void NatsPublisher::publishIncident(const surveillance::Incident &incident) {
    Q_D(NatsPublisher);
    if (!d->ready) {
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
    d->socket.write(command);
}
