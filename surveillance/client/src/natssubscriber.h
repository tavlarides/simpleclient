#pragma once

#include <QObject>
#include <QScopedPointer>
#include <surveillance/domain.h>

class NatsSubscriberPrivate;

class NatsSubscriber : public QObject {
  Q_OBJECT

public:
  explicit NatsSubscriber(QString host, quint16 port, QObject *parent = nullptr);
  ~NatsSubscriber() override;

  void start();

signals:
  void incidentReceived(const surveillance::Incident &incident);
  void statusChanged(const QString &status);

private:
  void consumeBuffer();

  Q_DECLARE_PRIVATE(NatsSubscriber)
  QScopedPointer<NatsSubscriberPrivate> d_ptr;
};
