#pragma once

#include <QObject>
#include <QQmlEngine>

#include "incidentsmodel.h"
#include "natssubscriber.h"

class OperatorController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QAbstractItemModel *incidentModel READ incidentModel CONSTANT)
    Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)

public:
    explicit OperatorController(QObject *parent = nullptr);

    QAbstractItemModel *incidentModel();
    QString connectionStatus() const;

signals:
    void connectionStatusChanged();

private:
    IncidentsModel m_incidents;
    NatsSubscriber m_subscriber;
    QString m_connectionStatus = QStringLiteral("Starting");
};
