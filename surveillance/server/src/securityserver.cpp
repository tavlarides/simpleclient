#include "securityserver.h"

#include "processing/eventprocessor.h"
#include "storage/datastore.h"

#include <QCoreApplication>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(serverLog, "surveillance.server")

namespace {
QString environment(const char *name, const QString &fallback) {
    const QString value = qEnvironmentVariable(name);
    return value.isEmpty() ? fallback : value;
}
} // namespace

SecurityServer::SecurityServer(QObject *parent)
    : QObject(parent)
    , m_ingestion(environment("MQTT_HOST", QStringLiteral("localhost")),
                  environment("MQTT_PORT", QStringLiteral("1883")).toUShort(),
                  environment("MQTT_TOPIC", QStringLiteral("cameras/+/events")))
    , m_natsPublisher(environment("NATS_HOST", QStringLiteral("localhost")),
                      environment("NATS_PORT", QStringLiteral("4222")).toUShort()) {
    m_processor = new EventProcessor;
    m_processor->moveToThread(&m_processingThread);
    m_dataStore = new DataStore(QStringLiteral("security-server-store"),
                                environment("DATABASE_URL", QStringLiteral("postgresql://security:security@localhost:5432/security")));
    m_dataStore->moveToThread(&m_storageThread);

    connect(&m_processingThread, &QThread::finished, m_processor, &QObject::deleteLater);
    connect(&m_storageThread, &QThread::finished, m_dataStore, &QObject::deleteLater);
    connect(&m_ingestion, &MqttIngestion::rawEventReceived, m_processor, &EventProcessor::process, Qt::QueuedConnection);
    connect(m_processor, &EventProcessor::incidentReady, m_dataStore, &DataStore::store, Qt::QueuedConnection);
    connect(m_dataStore, &DataStore::incidentStored, &m_natsPublisher, &NatsPublisher::publishIncident, Qt::QueuedConnection);
    connect(m_processor, &EventProcessor::rejected, this, [](const QString &reason) { qCWarning(serverLog) << "Rejected camera event:" << reason; });
    connect(m_dataStore, &DataStore::storageError, this, [](const QString &message) { qCCritical(serverLog) << "Storage failure:" << message; });
    connect(&m_ingestion, &MqttIngestion::statusChanged, this, [](const QString &status) { qCInfo(serverLog) << status; });
    connect(&m_natsPublisher, &NatsPublisher::statusChanged, this, [](const QString &status) { qCInfo(serverLog) << status; });
}

SecurityServer::~SecurityServer() {
    m_processingThread.quit();
    m_storageThread.quit();
    m_processingThread.wait();
    m_storageThread.wait();
}

void SecurityServer::start() {
    qRegisterMetaType<surveillance::Incident>();
    m_processingThread.start();
    m_storageThread.start();
    QMetaObject::invokeMethod(m_dataStore, &DataStore::initialize, Qt::QueuedConnection);
    m_natsPublisher.start();
    m_ingestion.start();
}
