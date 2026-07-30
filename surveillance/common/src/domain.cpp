#include <surveillance/domain.h>

namespace surveillance {

QString zoneFor(const GeoPoint &location) {
    // Demo geofence around a fictional restricted perimeter in Athens.
    if (location.latitude >= 37.9720 && location.latitude <= 37.9780
        && location.longitude >= 23.7280 && location.longitude <= 23.7380) {
        return QStringLiteral("restricted-perimeter");
    }
    return QStringLiteral("unassigned");
}

QString severityFor(const CameraEvent &event, const QString &zoneId) {
    if (event.eventType == QStringLiteral("tamper") || event.eventType == QStringLiteral("offline")) {
        return QStringLiteral("critical");
    }
    if (event.eventType == QStringLiteral("motion") && zoneId == QStringLiteral("restricted-perimeter")) {
        return QStringLiteral("high");
    }
    return QStringLiteral("medium");
}

} // namespace surveillance
