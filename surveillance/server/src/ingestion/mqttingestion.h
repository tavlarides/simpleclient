#pragma once

#include <QMqttClient>
#include <QObject>
#include <QString>

class MqttIngestion : public QObject {
    Q_OBJECT

public:
    explicit MqttIngestion(QString host, quint16 port, QString topic, QObject *parent = nullptr);

    void start();

signals:
    void rawEventReceived(const QByteArray &payload, const QString &topic);
    void statusChanged(const QString &status);

private:
    QMqttClient m_client;
    QString m_topic;
};
