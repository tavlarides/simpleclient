#include "natssubscriber.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>

class NatsSubscriberPrivate {
  Q_DECLARE_PUBLIC(NatsSubscriber)

public:
  NatsSubscriberPrivate(NatsSubscriber *q, QString serverHost, quint16 serverPort)
      : q_ptr(q)
      , host(std::move(serverHost))
      , port(serverPort) {}

  NatsSubscriber *q_ptr;
  QTcpSocket      socket;
  QByteArray      buffer;
  QString         host;
  quint16         port;
  qsizetype       expectedPayloadSize = -1;
  bool            handshakeSent       = false;
};

NatsSubscriber::NatsSubscriber(QString host, quint16 port, QObject *parent)
    : QObject(parent)
    , d_ptr(new NatsSubscriberPrivate(this, std::move(host), port)) {
  Q_D(NatsSubscriber);
  connect(&d->socket, &QTcpSocket::readyRead, this, [this] {
    Q_D(NatsSubscriber);
    d->buffer.append(d->socket.readAll());
    consumeBuffer();
  });
  connect(&d->socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
    Q_D(NatsSubscriber);
    emit statusChanged(QStringLiteral("NATS socket error: %1").arg(d->socket.errorString()));
  });
}

NatsSubscriber::~NatsSubscriber() = default;

void NatsSubscriber::start() {
  Q_D(NatsSubscriber);
  d->handshakeSent = false;
  emit statusChanged(QStringLiteral("Connecting to NATS"));
  d->socket.connectToHost(d->host, d->port);
}

void NatsSubscriber::consumeBuffer() {
  Q_D(NatsSubscriber);
  while (true) {
    if (d->expectedPayloadSize >= 0) {
      if (d->buffer.size() < d->expectedPayloadSize + 2) {
        return;
      }
      const QByteArray payload = d->buffer.left(d->expectedPayloadSize);
      d->buffer.remove(0, d->expectedPayloadSize + 2);   // payload and CRLF
      d->expectedPayloadSize = -1;

      const QJsonObject object     = QJsonDocument::fromJson(payload).object();
      const QDateTime   occurredAt = QDateTime::fromString(object.value(u"occurredAt").toString(), Qt::ISODate);
      if (!object.isEmpty() && occurredAt.isValid()) {
        // TODO: create a proper Incident
        // push it in a queue
        // emit a signal with that queue after a time or after a queue size
        // clean queue

        emit incidentReceived({ object.value(u"incidentId").toString(),
                                object.value(u"eventId").toString(),
                                object.value(u"cameraId").toString(),
                                object.value(u"eventType").toString(),
                                object.value(u"severity").toString(),
                                object.value(u"zoneId").toString(),
                                object.value(u"status").toString(),
                                occurredAt,
                                { object.value(u"latitude").toDouble(), object.value(u"longitude").toDouble() } });
      }
      continue;
    }

    const qsizetype lineEnd = d->buffer.indexOf("\r\n");
    if (lineEnd < 0) {
      return;
    }
    const QList<QByteArray> parts = d->buffer.left(lineEnd).split(' ');
    d->buffer.remove(0, lineEnd + 2);
    if (parts.first() == "INFO" && !d->handshakeSent) {
      d->handshakeSent = true;
      d->socket.write("CONNECT {\"verbose\":false,\"pedantic\":false}\r\nSUB security.incidents.> 1\r\nPING\r\n");
    } else if (parts.size() == 4 && parts.first() == "MSG") {
      bool            validSize   = false;
      const qsizetype payloadSize = parts.at(3).toLongLong(&validSize);
      if (validSize && payloadSize >= 0) {
        d->expectedPayloadSize = payloadSize;
      }
    } else if (parts.first() == "PONG") {
      emit statusChanged(QStringLiteral("Subscribed to NATS incidents"));
    } else if (parts.first() == "-ERR") {
      emit statusChanged(QStringLiteral("NATS error: %1").arg(QString::fromUtf8(parts.join(' '))));
    }
  }
}
