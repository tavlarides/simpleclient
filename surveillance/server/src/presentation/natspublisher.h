#pragma once

#include <QObject>
#include <QTcpSocket>

#include <surveillance/domain.h>

class NatsPublisher : public QObject {
    Q_OBJECT

public:
    explicit NatsPublisher(QString host, quint16 port, QObject *parent = nullptr);

    void start();

public slots:
    void publishIncident(const surveillance::Incident &incident);

signals:
    void statusChanged(const QString &status);

private:
    QTcpSocket m_socket;
    QString m_host;
    quint16 m_port;
    bool m_handshakeSent = false;
    bool m_ready = false;
};
