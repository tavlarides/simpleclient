#include "incidentsmodel.h"

IncidentsModel::IncidentsModel(QObject *parent)
    : QAbstractListModel(parent) {}

int IncidentsModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_incidents.size();
}

QVariant IncidentsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_incidents.size()) {
        return {};
    }
    const surveillance::Incident &incident = m_incidents.at(index.row());
    switch (role) {
    case IncidentIdRole: return incident.incidentId;
    case CameraIdRole: return incident.cameraId;
    case EventTypeRole: return incident.eventType;
    case SeverityRole: return incident.severity;
    case ZoneIdRole: return incident.zoneId;
    case StatusRole: return incident.status;
    case OccurredAtRole: return incident.occurredAt.toLocalTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    case LatitudeRole: return incident.location.latitude;
    case LongitudeRole: return incident.location.longitude;
    default: return {};
    }
}

QHash<int, QByteArray> IncidentsModel::roleNames() const {
    return {
        { IncidentIdRole, "incidentId" },
        { CameraIdRole, "cameraId" },
        { EventTypeRole, "eventType" },
        { SeverityRole, "severity" },
        { ZoneIdRole, "zoneId" },
        { StatusRole, "status" },
        { OccurredAtRole, "occurredAt" },
        { LatitudeRole, "latitude" },
        { LongitudeRole, "longitude" }
    };
}

void IncidentsModel::append(const surveillance::Incident &incident) {
    beginInsertRows({}, 0, 0);
    m_incidents.prepend(incident);
    endInsertRows();
}
