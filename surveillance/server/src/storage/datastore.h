#pragma once

#include <QObject>
#include <QString>

#include <surveillance/domain.h>

class DataStore : public QObject {
    Q_OBJECT

public:
    explicit DataStore(QString connectionName, QString databaseUrl);

public slots:
    void initialize();
    void store(const surveillance::Incident &incident);

signals:
    void incidentStored(const surveillance::Incident &incident);
    void storageError(const QString &message);

private:
    QString m_connectionName;
    QString m_databaseUrl;
};
