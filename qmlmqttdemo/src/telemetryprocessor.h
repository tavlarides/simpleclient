#pragma once

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QtMqtt/QMqttTopicName>

class TelemetryProcessor : public QObject {
  Q_OBJECT
public:
  explicit TelemetryProcessor(QObject *parent = nullptr);

public slots:
  void processMessage(const QByteArray &message, const QMqttTopicName &topic);

signals:
  void messageReady(const QString &topic, const QString &payload);
};
