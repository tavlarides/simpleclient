#include "operatorcontroller.h"

namespace {
QString environment(const char *name, const QString &fallback) {
    const QString value = qEnvironmentVariable(name);
    return value.isEmpty() ? fallback : value;
}
} // namespace

OperatorController::OperatorController(QObject *parent)
    : QObject(parent)
    , m_incidents(this)
    , m_subscriber(environment("NATS_HOST", QStringLiteral("localhost")),
                   environment("NATS_PORT", QStringLiteral("4222")).toUShort(), this) {
    connect(&m_subscriber, &NatsSubscriber::incidentReceived, &m_incidents, &IncidentsModel::append);
    connect(&m_subscriber, &NatsSubscriber::statusChanged, this, [this](const QString &status) {
        if (m_connectionStatus == status) {
            return;
        }
        m_connectionStatus = status;
        emit connectionStatusChanged();
    });
    m_subscriber.start();
}

QAbstractItemModel *OperatorController::incidentModel() {
    return &m_incidents;
}

QString OperatorController::connectionStatus() const {
    return m_connectionStatus;
}
