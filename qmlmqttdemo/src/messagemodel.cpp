#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent)
    : QAbstractListModel(parent) {}

int MessageModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return m_messages.size();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_messages.size())
    return {};

  const MessageRecord &record = m_messages.at(index.row());
  switch (role) {
  case TopicRole:
    return record.topic;
  case PayloadRole:
    return record.payload;
  case TimestampRole:
    return record.timestamp;
  default:
    return {};
  }
}

QHash<int, QByteArray> MessageModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TopicRole]     = "topic";
  roles[PayloadRole]   = "payload";
  roles[TimestampRole] = "timestamp";
  return roles;
}

void MessageModel::addMessage(const QString &topic, const QString &payload, const QString &timestamp) {
  beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());
  m_messages.append({ topic, payload, timestamp });
  endInsertRows();
}

void MessageModel::clear() {
  beginResetModel();
  m_messages.clear();
  endResetModel();
}
