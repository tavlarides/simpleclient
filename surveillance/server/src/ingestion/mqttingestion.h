#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QString>

class MqttIngestionPrivate;

class MqttIngestion : public QObject {
    Q_OBJECT

public:
    explicit MqttIngestion(QString host, quint16 port, QString topic, QObject *parent = nullptr);
    ~MqttIngestion() override;

    void start();

signals:
    void rawEventReceived(const QByteArray &payload, const QString &topic);
    void statusChanged(const QString &status);

private:
    Q_DECLARE_PRIVATE(MqttIngestion)
    QScopedPointer<MqttIngestionPrivate> d_ptr;
};
