#pragma once

#include <QAbstractListModel>
#include <QScopedPointer>

#include <surveillance/domain.h>

class IncidentsModelPrivate;

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
    ~IncidentsModel() override;

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void append(const surveillance::Incident &incident);

private:
    Q_DECLARE_PRIVATE(IncidentsModel)
    QScopedPointer<IncidentsModelPrivate> d_ptr;
};
