#include "appcontroller.h"

#include <QDateTime>
#include <QMetaObject>
#include <QVariant>
#include <QtMqtt/QMqttTopicName>

AppController::AppController(QObject *parent)
    : QObject(parent) {
  m_client = new QMqttClient(this);
  m_client->setHostname(m_host);
  m_client->setPort(static_cast<quint16>(m_port));

  m_messageModel = std::make_unique<MessageModel>(this);
  m_workerThread = std::make_unique<QThread>(this);
  m_processor    = std::make_unique<TelemetryProcessor>();
  m_processor->moveToThread(m_workerThread.get());
  m_workerThread->start();

  connect(m_client, &QMqttClient::stateChanged, this, &AppController::onStateChanged);
  connect(m_client, &QMqttClient::errorChanged, this, &AppController::onErrorChanged);
  connect(m_client,
          &QMqttClient::messageReceived,
          m_processor.get(),
          &TelemetryProcessor::processMessage,
          Qt::QueuedConnection);
  connect(m_processor.get(),
          &TelemetryProcessor::messageReady,
          this,
          &AppController::handleIncomingMessage,
          Qt::QueuedConnection);

  updateStatus(QStringLiteral("Disconnected"));
}

AppController::~AppController() {
  disconnectFromBroker();
  if (m_workerThread && m_workerThread->isRunning()) {
    m_workerThread->quit();
    m_workerThread->wait();
  }
}

QString AppController::connectionStatus() const {
  return m_connectionStatus;
}

QString AppController::lastPayload() const {
  return m_lastPayload;
}

QString AppController::errorMessage() const {
  return m_errorMessage;
}

int AppController::messageCount() const {
  return m_messageCount;
}

QString AppController::host() const {
  return m_host;
}

void AppController::setHost(const QString &host) {
  if (m_host == host)
    return;

  m_host = host;
  m_client->setHostname(host);
  emit hostChanged();
}

int AppController::port() const {
  return m_port;
}

void AppController::setPort(int port) {
  if (m_port == port)
    return;

  m_port = port;
  m_client->setPort(static_cast<quint16>(port));
  emit portChanged();
}

QString AppController::topic() const {
  return m_topic;
}

void AppController::setTopic(const QString &topic) {
  if (m_topic == topic)
    return;

  m_topic = topic;
  emit topicChanged();
}

QObject *AppController::messageModel() const {
  return m_messageModel.get();
}

void AppController::connectToBroker() {
  if (m_client->state() == QMqttClient::Connected) {
    updateStatus(QStringLiteral("Connected"));
    return;
  }

  m_client->setHostname(m_host);
  m_client->setPort(static_cast<quint16>(m_port));
  m_client->connectToHost();
}

void AppController::disconnectFromBroker() {
  if (m_client->state() != QMqttClient::Disconnected)
    m_client->disconnectFromHost();
}

void AppController::subscribeToTopic(const QString &topic) {
  if (m_client->state() != QMqttClient::Connected) {
    updateError(QStringLiteral("Connect to the broker before subscribing"));
    return;
  }

  const QMqttTopicFilter topicFilter(topic);
  m_client->subscribe(topicFilter);
  updateStatus(QStringLiteral("Subscribed to %1").arg(topic));
}

void AppController::publishMessage(const QString &message) {
  if (m_client->state() != QMqttClient::Connected) {
    updateError(QStringLiteral("Connect to the broker before publishing"));
    return;
  }

  m_client->publish(QMqttTopicName(m_topic), message.toUtf8());
  updateStatus(QStringLiteral("Published to %1").arg(m_topic));
}

void AppController::clearMessages() {
  m_messageModel->clear();
}

void AppController::onStateChanged(QMqttClient::ClientState state) {
  switch (state) {
  case QMqttClient::Connected:
    updateStatus(QStringLiteral("Connected"));
    break;
  case QMqttClient::Connecting:
    updateStatus(QStringLiteral("Connecting"));
    break;
  case QMqttClient::Disconnected:
    updateStatus(QStringLiteral("Disconnected"));
    break;
  default:
    updateStatus(QStringLiteral("Idle"));
    break;
  }
}

void AppController::onErrorChanged(QMqttClient::ClientError error) {
  if (error == QMqttClient::NoError)
    return;

  updateError(QStringLiteral("MQTT error: %1").arg(error));
}

void AppController::handleIncomingMessage(const QString &topic, const QString &payload) {
  std::lock_guard<std::mutex> lock(m_mutex);
  m_lastPayload = payload;
  m_messageCount += 1;
  emit lastPayloadChanged();
  emit messageCountChanged();

  m_messageModel->addMessage(topic, payload, QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss")));
}

void AppController::updateStatus(const QString &status) {
  if (m_connectionStatus == status)
    return;

  m_connectionStatus = status;
  emit connectionStatusChanged();
}

void AppController::updateError(const QString &error) {
  m_errorMessage = error;
  emit errorMessageChanged();
}
