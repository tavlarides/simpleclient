#pragma once

#include <QAbstractListModel>

#include <surveillance/domain.h>

class IncidentsModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Role {
        IncidentIdRole = Qt::UserRole + 1,
        CameraIdRole,
        EventTypeRole,
        SeverityRole,
        ZoneIdRole,
        StatusRole,
        OccurredAtRole,
        LatitudeRole,
        LongitudeRole
    };
    Q_ENUM(Role)

    explicit IncidentsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void append(const surveillance::Incident &incident);

private:
    QList<surveillance::Incident> m_incidents;
};
