#include "incidentsmodel.h"

class IncidentsModelPrivate {
    Q_DECLARE_PUBLIC(IncidentsModel)

public:
    explicit IncidentsModelPrivate(IncidentsModel *q)
        : q_ptr(q) {}

    IncidentsModel *q_ptr;
    QList<surveillance::Incident> incidents;
};

IncidentsModel::IncidentsModel(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new IncidentsModelPrivate(this)) {}

IncidentsModel::~IncidentsModel() = default;

int IncidentsModel::rowCount(const QModelIndex &parent) const {
    Q_D(const IncidentsModel);
    return parent.isValid() ? 0 : d->incidents.size();
}

QVariant IncidentsModel::data(const QModelIndex &index, int role) const {
    Q_D(const IncidentsModel);
    if (!index.isValid() || index.row() < 0 || index.row() >= d->incidents.size()) {
        return {};
    }
    const surveillance::Incident &incident = d->incidents.at(index.row());
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
    Q_D(IncidentsModel);
    beginInsertRows({}, 0, 0);
    d->incidents.prepend(incident);
    endInsertRows();
}
