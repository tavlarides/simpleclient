#include "datastore.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QUrl>

DataStore::DataStore(QString connectionName, QString databaseUrl)
    : m_connectionName(std::move(connectionName))
    , m_databaseUrl(std::move(databaseUrl)) {}

void DataStore::initialize() {
    const QUrl url(m_databaseUrl);
    if (!url.isValid() || url.scheme() != QStringLiteral("postgresql")) {
        emit storageError(QStringLiteral("DATABASE_URL must use postgresql://user:password@host:port/database"));
        return;
    }

    QSqlDatabase database = QSqlDatabase::addDatabase(QStringLiteral("QPSQL"), m_connectionName);
    database.setHostName(url.host());
    database.setPort(url.port(5432));
    database.setDatabaseName(url.path().mid(1));
    database.setUserName(url.userName());
    database.setPassword(url.password());
    if (!database.open()) {
        emit storageError(database.lastError().text());
        return;
    }

    QSqlQuery query(database);
    if (!query.exec(QStringLiteral(
            "CREATE TABLE IF NOT EXISTS incidents ("
            "incident_id UUID PRIMARY KEY, event_id TEXT UNIQUE NOT NULL, camera_id TEXT NOT NULL, "
            "event_type TEXT NOT NULL, severity TEXT NOT NULL, zone_id TEXT NOT NULL, status TEXT NOT NULL, "
            "occurred_at TIMESTAMPTZ NOT NULL, latitude DOUBLE PRECISION NOT NULL, longitude DOUBLE PRECISION NOT NULL)"))) {
        emit storageError(query.lastError().text());
    }
}

void DataStore::store(const surveillance::Incident &incident) {
    const QSqlDatabase database = QSqlDatabase::database(m_connectionName);
    if (!database.isOpen()) {
        emit storageError(QStringLiteral("PostgreSQL connection is not open"));
        return;
    }

    QSqlQuery query(database);
    query.prepare(QStringLiteral(
        "INSERT INTO incidents (incident_id, event_id, camera_id, event_type, severity, zone_id, status, occurred_at, latitude, longitude) "
        "VALUES (:incidentId, :eventId, :cameraId, :eventType, :severity, :zoneId, :status, :occurredAt, :latitude, :longitude) "
        "ON CONFLICT (event_id) DO NOTHING"));
    query.bindValue(QStringLiteral(":incidentId"), incident.incidentId);
    query.bindValue(QStringLiteral(":eventId"), incident.eventId);
    query.bindValue(QStringLiteral(":cameraId"), incident.cameraId);
    query.bindValue(QStringLiteral(":eventType"), incident.eventType);
    query.bindValue(QStringLiteral(":severity"), incident.severity);
    query.bindValue(QStringLiteral(":zoneId"), incident.zoneId);
    query.bindValue(QStringLiteral(":status"), incident.status);
    query.bindValue(QStringLiteral(":occurredAt"), incident.occurredAt);
    query.bindValue(QStringLiteral(":latitude"), incident.location.latitude);
    query.bindValue(QStringLiteral(":longitude"), incident.location.longitude);
    if (!query.exec()) {
        emit storageError(query.lastError().text());
        return;
    }
    if (query.numRowsAffected() > 0) {
        emit incidentStored(incident);
    }
}
