#pragma once

#include <QObject>
#include <QScopedPointer>

#include <surveillance/domain.h>

class NatsPublisherPrivate;

class NatsPublisher : public QObject {
    Q_OBJECT

public:
    explicit NatsPublisher(QString host, quint16 port, QObject *parent = nullptr);
    ~NatsPublisher() override;

    void start();

public slots:
    void publishIncident(const surveillance::Incident &incident);

signals:
    void statusChanged(const QString &status);

private:
    Q_DECLARE_PRIVATE(NatsPublisher)
    QScopedPointer<NatsPublisherPrivate> d_ptr;
};
