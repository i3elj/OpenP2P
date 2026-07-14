#ifndef CHATLISTMODEL_H
#define CHATLISTMODEL_H

#include <QAbstractListModel>
#include "src/message.hpp"

class ChatListModel : public QAbstractListModel
{
  Q_OBJECT

private:
  QList<Message> m_msgs;

public:
  enum Roles { TextRole, SentRole };

  explicit ChatListModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  void add(Message msg);
  void reset(QList<Message> messages);

signals:
};

#endif // CHATLISTMODEL_H
