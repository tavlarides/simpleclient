#pragma once

#include <QObject>

#include <surveillance/domain.h>

class EventProcessor : public QObject {
    Q_OBJECT

public slots:
    void process(const QByteArray &payload, const QString &topic);

signals:
    void incidentReady(const surveillance::Incident &incident);
    void rejected(const QString &reason);
};
