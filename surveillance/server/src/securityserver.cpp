#include "securityserver.h"

#include "ingestion/mqttingestion.h"
#include "presentation/natspublisher.h"
#include "processing/eventprocessor.h"
#include "storage/datastore.h"

#include <QCoreApplication>
#include <QLoggingCategory>
#include <QThread>

Q_LOGGING_CATEGORY(serverLog, "surveillance.server")

namespace {
QString environment(const char *name, const QString &fallback) {
    const QString value = qEnvironmentVariable(name);
    return value.isEmpty() ? fallback : value;
}
} // namespace

class SecurityServerPrivate {
    Q_DECLARE_PUBLIC(SecurityServer)

public:
    explicit SecurityServerPrivate(SecurityServer *q)
        : q_ptr(q)
        , ingestion(environment("MQTT_HOST", QStringLiteral("localhost")),
                    environment("MQTT_PORT", QStringLiteral("1883")).toUShort(),
                    environment("MQTT_TOPIC", QStringLiteral("cameras/+/events")))
        , natsPublisher(environment("NATS_HOST", QStringLiteral("localhost")),
                        environment("NATS_PORT", QStringLiteral("4222")).toUShort()) {}

    SecurityServer *q_ptr;
    MqttIngestion ingestion;
    NatsPublisher natsPublisher;
    QThread processingThread;
    QThread storageThread;
    EventProcessor *processor = nullptr;
    DataStore *dataStore = nullptr;
};

SecurityServer::SecurityServer(QObject *parent)
    : QObject(parent)
    , d_ptr(new SecurityServerPrivate(this)) {
    Q_D(SecurityServer);
    d->processor = new EventProcessor;
    d->processor->moveToThread(&d->processingThread);
    d->dataStore = new DataStore(QStringLiteral("security-server-store"),
                                 environment("DATABASE_URL", QStringLiteral("postgresql://security:security@localhost:5432/security")));
    d->dataStore->moveToThread(&d->storageThread);

    connect(&d->processingThread, &QThread::finished, d->processor, &QObject::deleteLater);
    connect(&d->storageThread, &QThread::finished, d->dataStore, &QObject::deleteLater);
    connect(&d->ingestion, &MqttIngestion::rawEventReceived, d->processor, &EventProcessor::process, Qt::QueuedConnection);
    connect(d->processor, &EventProcessor::incidentReady, d->dataStore, &DataStore::store, Qt::QueuedConnection);
    connect(d->dataStore, &DataStore::incidentStored, &d->natsPublisher, &NatsPublisher::publishIncident, Qt::QueuedConnection);
    connect(d->processor, &EventProcessor::rejected, this, [](const QString &reason) { qCWarning(serverLog) << "Rejected camera event:" << reason; });
    connect(d->dataStore, &DataStore::storageError, this, [](const QString &message) { qCCritical(serverLog) << "Storage failure:" << message; });
    connect(&d->ingestion, &MqttIngestion::statusChanged, this, [](const QString &status) { qCInfo(serverLog) << status; });
    connect(&d->natsPublisher, &NatsPublisher::statusChanged, this, [](const QString &status) { qCInfo(serverLog) << status; });
}

SecurityServer::~SecurityServer() {
    Q_D(SecurityServer);
    d->processingThread.quit();
    d->storageThread.quit();
    d->processingThread.wait();
    d->storageThread.wait();
}

void SecurityServer::start() {
    Q_D(SecurityServer);
    qRegisterMetaType<surveillance::Incident>();
    d->processingThread.start();
    d->storageThread.start();
    QMetaObject::invokeMethod(d->dataStore, &DataStore::initialize, Qt::QueuedConnection);
    d->natsPublisher.start();
    d->ingestion.start();
}
