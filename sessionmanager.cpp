#include "sessionmanager.h"
#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include "self.h"

/**
 * Private functions.
 */

bool SessionManager::loadSavedPeers() {
  QJsonArray peersArray = loadPeersFromFile();

  for (const QJsonValueRef &entry : peersArray) {
    QJsonObject json = entry.toObject();
    Peer *peer = new Peer(m_user, this);
    peer->setName(json[Self::SettingsKeys::Name].toString());
    peer->setAddr(json[Self::SettingsKeys::Addr].toString());
    peer->setPort(json[Self::SettingsKeys::Port].toInt());
    m_peerMap.insert(peer->id(), peer);
    m_peerModel->addPeer(peer);
  }

  return true;
}

bool SessionManager::savePeersToFile(QJsonArray peers) {
  QFile file(Self::savedPeersFilePath);

  if (!file.open(QFile::WriteOnly)) {
    qWarning() << "Can't open file to save peers:" << file.errorString();
    return false;
  }

  auto bytesWritten = file.write(QJsonDocument(peers).toJson(QJsonDocument::Indented));

  if (bytesWritten == -1) {
    qWarning() << "Couldn't save peers:" << file.errorString();
    return false;
  }

  file.close();
  return true;
}

QJsonArray SessionManager::loadPeersFromFile() {
  QFile file(Self::savedPeersFilePath);

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "Something went wrong while reading saved files:" << file.errorString();
    return QJsonArray();
  }

  QByteArray data = file.readAll();
  QJsonParseError error;
  QJsonDocument doc(QJsonDocument::fromJson(data, &error));

  if (doc.isNull()) {
    qWarning() << "JSON document is empty:" << error.errorString();
    return QJsonArray();
  }

  return doc.array();
}

/**
 * Public functions.
 */

SessionManager::SessionManager(Self *user, QObject *parent)
  : QObject{parent}
  , m_user(user)
  , m_peerModel(new PeerListModel(this))
{
  loadSavedPeers();
}

SessionManager::~SessionManager() { saveAllPeers(); }

bool SessionManager::contains(PeerId peerid) const {
  return m_peerMap.contains(peerid);
}

void SessionManager::addPeer(Peer *peer) {
  peer->setParent(this);
  m_peerMap.insert(peer->id(), peer);
  m_peerModel->addPeer(peer);

  if (!savePeer(peer)) {
    qWarning() << "Couldn't save peer";
  }
}

void SessionManager::deletePeer(Peer *peer) { m_peerMap.remove(peer->id()); }

QHash<PeerId, Peer *> SessionManager::getAllPeers() const { return m_peerMap; }

Peer *SessionManager::getPeer(PeerId id) const {
  return m_peerMap.value(id, nullptr);
}

bool SessionManager::saveAllPeers() {
  QJsonArray arr;

  for (const auto &peer : m_peerMap) {
    QJsonObject json{{Self::SettingsKeys::Name, peer->name()},
                     {Self::SettingsKeys::Addr, peer->addr()},
                     {Self::SettingsKeys::Port, peer->port()}};
    arr.append(json);
  }

  return savePeersToFile(arr);
}

bool SessionManager::savePeer(Peer *peer) {
  QJsonArray peers = loadPeersFromFile();
  QJsonObject jsonPeer{{Self::SettingsKeys::Name, peer->name()},
                       {Self::SettingsKeys::Addr, peer->addr()},
                       {Self::SettingsKeys::Port, peer->port()}};

  if (!peers.contains(jsonPeer)) {
    peers.append(jsonPeer);
    return savePeersToFile(peers);
  }

  return false;
}

/**
 * Q_INVOKABLE Functions.
 */

PeerListModel *SessionManager::peers() const { return m_peerModel; }

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
    Message msg(sent, text);
    chat.append(msg.toJson());
  }

  // save chat to file
  QDir dir(Self::userConfigDir);

  if (!dir.exists())
    dir.mkpath(".");

  QFile file(Self::userConfigDir + peer->name());

  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    qErrnoWarning("Can't open file");
    return;
  }

  QJsonDocument doc(chat);
  file.write(doc.toJson());
  file.close();
}

bool SessionManager::loadChats() {
  for (const auto &peer : m_peerMap) {
    QFile file(Self::userConfigDir + peer->name());
    QList<Message> messages;

    if (!file.exists())
      continue;

    if (!file.open(QIODevice::ReadOnly)) {
      qErrnoWarning("Can't open file");
      return false;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    QJsonArray chat = doc.array();

    for (const QJsonValue &json : chat) {
      Message msg(json[Self::SettingsKeys::Sent].toBool(),
                  json[Self::SettingsKeys::Text].toString());
      msg.setType(static_cast<Message::Type>(json[Self::SettingsKeys::Type].toInt()));
      messages.append(msg);
    }

    peer->loadMessages(messages);
  }

  return true;
}

void SessionManager::addNewMsgTo(Peer *peer, bool sent, QString msg) {
  // todo
}
