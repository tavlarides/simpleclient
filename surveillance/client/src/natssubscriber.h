#pragma once

#include <QObject>
#include <QTcpSocket>

#include <surveillance/domain.h>

class NatsSubscriber : public QObject {
    Q_OBJECT

public:
    explicit NatsSubscriber(QString host, quint16 port, QObject *parent = nullptr);

    void start();

signals:
    void incidentReceived(const surveillance::Incident &incident);
    void statusChanged(const QString &status);

private:
    void consumeBuffer();

    QTcpSocket m_socket;
    QByteArray m_buffer;
    QString m_host;
    quint16 m_port;
    qsizetype m_expectedPayloadSize = -1;
    bool m_handshakeSent = false;
};
