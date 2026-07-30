#include "mqttingestion.h"

#include <QMqttTopicFilter>

MqttIngestion::MqttIngestion(QString host, quint16 port, QString topic, QObject *parent)
    : QObject(parent)
    , m_topic(std::move(topic)) {
    m_client.setHostname(std::move(host));
    m_client.setPort(port);

    connect(&m_client, &QMqttClient::messageReceived, this,
            [this](const QByteArray &payload, const QMqttTopicName &topic) {
                emit rawEventReceived(payload, topic.name());
            });
    connect(&m_client, &QMqttClient::stateChanged, this, [this](QMqttClient::ClientState state) {
        if (state == QMqttClient::Connected) {
            if (!m_client.subscribe(QMqttTopicFilter(m_topic))) {
                emit statusChanged(QStringLiteral("MQTT subscription failed"));
                return;
            }
            emit statusChanged(QStringLiteral("MQTT subscribed to %1").arg(m_topic));
        }
    });
    connect(&m_client, &QMqttClient::errorChanged, this, [this](QMqttClient::ClientError error) {
        if (error != QMqttClient::NoError) {
            emit statusChanged(QStringLiteral("MQTT error %1").arg(static_cast<int>(error)));
        }
    });
}

void MqttIngestion::start() {
    emit statusChanged(QStringLiteral("Connecting to MQTT broker"));
    m_client.connectToHost();
}
