#include "eventprocessor.h"

#include <surveillance/domain.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>

void EventProcessor::process(const QByteArray &payload, const QString &topic) {
    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(payload, &error);
    if (error.error != QJsonParseError::NoError || !document.isObject()) {
        emit rejected(QStringLiteral("Invalid JSON: %1").arg(error.errorString()));
        return;
    }

    const QJsonObject object = document.object();
    const QString eventId = object.value(u"eventId").toString();
    const QString cameraId = object.value(u"cameraId").toString();
    const QString eventType = object.value(u"eventType").toString();
    const QDateTime occurredAt = QDateTime::fromString(object.value(u"occurredAt").toString(), Qt::ISODate);
    const double latitude = object.value(u"latitude").toDouble(999.0);
    const double longitude = object.value(u"longitude").toDouble(999.0);

    if (eventId.isEmpty() || cameraId.isEmpty() || eventType.isEmpty() || !occurredAt.isValid()
        || latitude < -90.0 || latitude > 90.0 || longitude < -180.0 || longitude > 180.0) {
        emit rejected(QStringLiteral("Event misses a required field or has invalid GIS coordinates"));
        return;
    }

    const surveillance::CameraEvent event {
        eventId,
        cameraId,
        eventType,
        topic,
        occurredAt.toUTC(),
        { latitude, longitude },
        object.value(u"confidence").toDouble()
    };
    const QString zoneId = surveillance::zoneFor(event.location);
    emit incidentReady({
        QUuid::createUuid().toString(QUuid::WithoutBraces),
        event.eventId,
        event.cameraId,
        event.eventType,
        surveillance::severityFor(event, zoneId),
        zoneId,
        QStringLiteral("new"),
        event.occurredAt,
        event.location
    });
}
