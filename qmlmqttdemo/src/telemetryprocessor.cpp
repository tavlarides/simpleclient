#include "telemetryprocessor.h"

#include <QtMqtt/QMqttTopicName>

TelemetryProcessor::TelemetryProcessor(QObject *parent)
    : QObject(parent) {}

void TelemetryProcessor::processMessage(const QByteArray &message, const QMqttTopicName &topic) {
  const QString payload = QString::fromUtf8(message);
  emit          messageReady(topic.name(), payload);
}
