#pragma once

#include <QDateTime>
#include <QMetaType>
#include <QString>

namespace surveillance {

struct GeoPoint {
    double latitude = 0.0;
    double longitude = 0.0;
};

struct CameraEvent {
    QString eventId;
    QString cameraId;
    QString eventType;
    QString sourceTopic;
    QDateTime occurredAt;
    GeoPoint location;
    double confidence = 0.0;
};

struct Incident {
    QString incidentId;
    QString eventId;
    QString cameraId;
    QString eventType;
    QString severity;
    QString zoneId;
    QString status;
    QDateTime occurredAt;
    GeoPoint location;
};

QString zoneFor(const GeoPoint &location);
QString severityFor(const CameraEvent &event, const QString &zoneId);

} // namespace surveillance

Q_DECLARE_METATYPE(surveillance::CameraEvent)
Q_DECLARE_METATYPE(surveillance::Incident)
