#include "sessionmanager.h"
#include "self.h"
#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <qabstractitemmodel.h>

SessionManager::SessionManager(Self *self, QObject *parent)
  : QObject{parent}, m_self(self) {
  m_peerListModel->setPeers(self->loadSavedPeers());
}

void SessionManager::saveChat(Peer *peer, QAbstractListModel *msgModel) {
  QHash<int, QByteArray> roles = msgModel->roleNames();
  int sentAttr = roles.key("sent");
  int textAttr = roles.key("text");

  QJsonArray chat{};
  int chatSize = msgModel->rowCount();

  for (int i = 0; i < chatSize; ++i) {
    QModelIndex idx = msgModel->index(i, 0);
    bool sent = msgModel->data(idx, sentAttr).toBool();
    QString text = msgModel->data(idx, textAttr).toString();
    Message msg(peer->id(), sent, text);
    chat.append(msg.json());
  }

  // save chat to file
  QDir dir(Self::userConfigDir);

  if (!dir.exists())
    dir.mkpath(".");

  QFile file(Self::userConfigDir + "/" + peer->name());

  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    qErrnoWarning("Can't open file");
    return;
  }

  QJsonDocument doc(chat);
  file.write(doc.toJson());
  file.close();
}

QList<Message> SessionManager::loadChat(Peer *peer) {
  QFile file(Self::userConfigDir + "/" + peer->name());
  QList<Message> messages;

  if (!file.open(QIODevice::ReadOnly)) {
    qErrnoWarning("Can't open file");
    return messages;
  }

  QByteArray fileData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(fileData);
  QJsonArray chat = doc.array();

  for (const QJsonValue &json : chat) {
    Message msg(peer->id(), json["sent"].toBool(), json["data"].toString());
    messages.append(msg);
  }

  return messages;
}

void SessionManager::addNewMsgTo(Peer *peer, bool sent, QString msg) {
  // todo
}
