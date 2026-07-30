#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>

struct MessageRecord {
  QString topic;
  QString payload;
  QString timestamp;
};

class MessageModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum Roles { TopicRole = Qt::UserRole + 1, PayloadRole, TimestampRole };

  explicit MessageModel(QObject *parent = nullptr);

  int                    rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant               data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  void addMessage(const QString &topic, const QString &payload, const QString &timestamp);
  void clear();

private:
  QList<MessageRecord> m_messages;
};
