#include "src/gui/chatlistmodel.hpp"

ChatListModel::ChatListModel(QObject *parent) : QAbstractListModel{parent} {}

void ChatListModel::add(Message msg) {
  int listSize = m_msgs.size();
  beginInsertRows(QModelIndex(), listSize, listSize);
  m_msgs.append(msg);
  endInsertRows();
}

void ChatListModel::reset(QList<Message> messages) {
  beginResetModel();
  m_msgs = messages;
  endResetModel();
}

int ChatListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_msgs.size();
}

QVariant ChatListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() >= m_msgs.size())
    return {};

  Message msg = m_msgs.at(index.row());

  switch (role) {
  case TextRole:
    return msg.text();
  case SentRole:
    return msg.sent();
  default:
    return -1;
  }
}

QHash<int, QByteArray> ChatListModel::roleNames() const {
  return {{TextRole, "text"}, {SentRole, "sent"}};
}
