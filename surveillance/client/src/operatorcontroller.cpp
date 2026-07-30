#include "operatorcontroller.h"

#include "incidentsmodel.h"
#include "natssubscriber.h"

namespace {
QString environment(const char *name, const QString &fallback) {
    const QString value = qEnvironmentVariable(name);
    return value.isEmpty() ? fallback : value;
}
} // namespace

class OperatorControllerPrivate {
    Q_DECLARE_PUBLIC(OperatorController)

public:
    explicit OperatorControllerPrivate(OperatorController *q)
        : q_ptr(q)
        , incidents(q)
        , subscriber(environment("NATS_HOST", QStringLiteral("localhost")),
                     environment("NATS_PORT", QStringLiteral("4222")).toUShort(), q) {}

    OperatorController *q_ptr;
    IncidentsModel incidents;
    NatsSubscriber subscriber;
    QString connectionStatus = QStringLiteral("Starting");
};

OperatorController::OperatorController(QObject *parent)
    : QObject(parent)
    , d_ptr(new OperatorControllerPrivate(this)) {
    Q_D(OperatorController);
    connect(&d->subscriber, &NatsSubscriber::incidentReceived, &d->incidents, &IncidentsModel::append);
    connect(&d->subscriber, &NatsSubscriber::statusChanged, this, [this](const QString &status) {
        Q_D(OperatorController);
        if (d->connectionStatus == status) {
            return;
        }
        d->connectionStatus = status;
        emit connectionStatusChanged();
    });
    d->subscriber.start();
}

OperatorController::~OperatorController() = default;

QAbstractItemModel *OperatorController::incidentModel() {
    Q_D(OperatorController);
    return &d->incidents;
}

QString OperatorController::connectionStatus() const {
    Q_D(const OperatorController);
    return d->connectionStatus;
}
