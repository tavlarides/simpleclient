#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QString>

#include <surveillance/domain.h>

class DataStorePrivate;

class DataStore : public QObject {
    Q_OBJECT

public:
    explicit DataStore(QString connectionName, QString databaseUrl);
    ~DataStore() override;

public slots:
    void initialize();
    void store(const surveillance::Incident &incident);

signals:
    void incidentStored(const surveillance::Incident &incident);
    void storageError(const QString &message);

private:
    Q_DECLARE_PRIVATE(DataStore)
    QScopedPointer<DataStorePrivate> d_ptr;
};
