#include "sessionmanager.h"
#include <QBuffer>
#include <QDir>
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
  QString path = QDir::homePath() + "/OpenP2P/chats/";
  QDir dir(path);

  if (!dir.exists(path) && dir.isReadable()) {
    dir.mkpath(path);
  }

  QFile file(path + peer->name());

  if (!file.open(QIODevice::WriteOnly)) {
    qErrnoWarning("Can't open file");
    return;
  }

  std::string buffer;

  for (const Message &msg : chat) {
    buffer += std::format("{} {}", msg.sent, msg.data.toStdString());
  }

  file.write(buffer.data());
  file.close();
}
