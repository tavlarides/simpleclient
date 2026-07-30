#pragma once

#include "messagemodel.h"
#include "telemetryprocessor.h"

#include <QObject>
#include <QString>
#include <QThread>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttTopicFilter>
#include <memory>
#include <mutex>

class AppController : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
  Q_PROPERTY(QString lastPayload READ lastPayload NOTIFY lastPayloadChanged)
  Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
  Q_PROPERTY(int messageCount READ messageCount NOTIFY messageCountChanged)
  Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
  Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
  Q_PROPERTY(QString topic READ topic WRITE setTopic NOTIFY topicChanged)
public:
  explicit AppController(QObject *parent = nullptr);
  ~AppController() override;

  QString connectionStatus() const;
  QString lastPayload() const;
  QString errorMessage() const;
  int     messageCount() const;
  QString host() const;
  void    setHost(const QString &host);
  int     port() const;
  void    setPort(int port);
  QString topic() const;
  void    setTopic(const QString &topic);

  QObject *messageModel() const;

public slots:
  void connectToBroker();
  void disconnectFromBroker();
  void subscribeToTopic(const QString &topic);
  void publishMessage(const QString &message);
  void clearMessages();

signals:
  void connectionStatusChanged();
  void lastPayloadChanged();
  void errorMessageChanged();
  void messageCountChanged();
  void hostChanged();
  void portChanged();
  void topicChanged();

private slots:
  void onStateChanged(QMqttClient::ClientState state);
  void onErrorChanged(QMqttClient::ClientError error);
  void handleIncomingMessage(const QString &topic, const QString &payload);

private:
  void updateStatus(const QString &status);
  void updateError(const QString &error);

  std::unique_ptr<QThread>            m_workerThread;
  std::unique_ptr<TelemetryProcessor> m_processor;
  std::unique_ptr<MessageModel>       m_messageModel;
  QMqttClient                        *m_client = nullptr;

  std::mutex m_mutex;
  QString    m_connectionStatus;
  QString    m_lastPayload;
  QString    m_errorMessage;
  QString    m_host         = QStringLiteral("localhost");
  int        m_port         = 1883;
  QString    m_topic        = QStringLiteral("qtmqtt/demo");
  int        m_messageCount = 0;
};
