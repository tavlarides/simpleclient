#pragma once

#include <QObject>
#include <QThread>

#include "ingestion/mqttingestion.h"
#include "presentation/natspublisher.h"

class EventProcessor;
class DataStore;

class SecurityServer : public QObject {
    Q_OBJECT

public:
    explicit SecurityServer(QObject *parent = nullptr);
    ~SecurityServer() override;

    void start();

private:
    MqttIngestion m_ingestion;
    NatsPublisher m_natsPublisher;
    QThread m_processingThread;
    QThread m_storageThread;
    EventProcessor *m_processor = nullptr;
    DataStore *m_dataStore = nullptr;
};
