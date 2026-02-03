#include "sessionmanager.h"
#include <QBuffer>
#include <QFile>
#include "message.h"

SessionManager::SessionManager(QObject *parent) : QObject{parent} {}

void SessionManager::saveChat(Peer *peer, QAbstractListModel *msgModel)
{
  QHash<int, QByteArray> roles = msgModel->roleNames();
  int sentAttr = roles.key("sent");
  int textAttr = roles.key("text");

  QVector<Message> chat;
  int chatSize = msgModel->rowCount();

  for (int i = 0; i < chatSize; ++i) {
    QModelIndex idx = msgModel->index(i, 0);
    Message msg = {.sent = msgModel->data(idx, sentAttr).toBool(),
                   .data = msgModel->data(idx, textAttr).toString()};
    chat.append(msg);
  }

  // save chat to file
  QFile file("~/OpenP2P/chats/" + peer->name());

  if (!file.open(QIODevice::WriteOnly)) {
    qErrnoWarning("Can't open file");
    return;
  }

  QBuffer buf;
  buf.open(QBuffer::WriteOnly);
  for (auto msg : chat) {
    // buf.write(std::format("{} {}", msg.sent, msg.data.toStdString()));
  }
}
