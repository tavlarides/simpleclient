#include "mqttingestion.h"

#include <QMqttClient>
#include <QMqttTopicFilter>

class MqttIngestionPrivate {
    Q_DECLARE_PUBLIC(MqttIngestion)

public:
    explicit MqttIngestionPrivate(MqttIngestion *q)
        : q_ptr(q) {}

    MqttIngestion *q_ptr;
    QMqttClient client;
    QString topic;
};

MqttIngestion::MqttIngestion(QString host, quint16 port, QString topic, QObject *parent)
    : QObject(parent)
    , d_ptr(new MqttIngestionPrivate(this)) {
    Q_D(MqttIngestion);
    d->topic = std::move(topic);
    d->client.setHostname(std::move(host));
    d->client.setPort(port);

    connect(&d->client, &QMqttClient::messageReceived, this,
            [this](const QByteArray &payload, const QMqttTopicName &topic) {
                emit rawEventReceived(payload, topic.name());
            });
    connect(&d->client, &QMqttClient::stateChanged, this, [this](QMqttClient::ClientState state) {
        Q_D(MqttIngestion);
        if (state == QMqttClient::Connected) {
            if (!d->client.subscribe(QMqttTopicFilter(d->topic))) {
                emit statusChanged(QStringLiteral("MQTT subscription failed"));
                return;
            }
            emit statusChanged(QStringLiteral("MQTT subscribed to %1").arg(d->topic));
        }
    });
    connect(&d->client, &QMqttClient::errorChanged, this, [this](QMqttClient::ClientError error) {
        if (error != QMqttClient::NoError) {
            emit statusChanged(QStringLiteral("MQTT error %1").arg(static_cast<int>(error)));
        }
    });
}

MqttIngestion::~MqttIngestion() = default;

void MqttIngestion::start() {
    Q_D(MqttIngestion);
    emit statusChanged(QStringLiteral("Connecting to MQTT broker"));
    d->client.connectToHost();
}
