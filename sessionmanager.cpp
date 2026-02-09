#include "sessionmanager.h"
#include <QBuffer>
#include <QDir>
#include <QFile>

SessionManager::SessionManager(QObject *parent)
  : QObject{parent}
  , m_activePeers()
{}

void SessionManager::addPeer(PeerId id, Peer *peer)
{
  m_activePeers.insert(id, peer);
}

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
                   .text = msgModel->data(idx, textAttr).toString()};
    chat.append(msg);
  }

  // save chat to file
  QString path = QDir::homePath() + m_chatsFilePath;
  QDir dir(path);

  if (!dir.exists(path) && dir.isReadable()) {
    dir.mkpath(path);
  }

  QFile file(path + "/" + peer->name());

  if (!file.open(QIODevice::WriteOnly)) {
    qErrnoWarning("Can't open file");
    return;
  }

  std::string buffer;

  for (const Message &msg : chat) {
    buffer += std::format("{} {}", msg.sent, msg.text.toStdString());
  }

  file.write(buffer.data());
  file.close();
}

QList<Message> SessionManager::loadChat(Peer *peer)
{
  QFile file(QDir::homePath() + m_chatsFilePath + "/" + peer->name());
  QList<Message> messages;

  if (!file.open(QIODevice::ReadOnly)) {
    qErrnoWarning("Can't open file");
    return messages;
  }

  QTextStream in(&file);

  while (!file.atEnd()) {
    QString line = in.readLine();
    Message msg = {.sent = line.at(0) == '1', .text = line.slice(1)};
    messages.append(msg);
  }

  file.close();
  return messages;
}
