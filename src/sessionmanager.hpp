#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "src/peer.hpp"
#include "src/self.hpp"
#include "src/gui/peerlistmodel.hpp"
#include <QAbstractItemModel>
#include <QObject>

class SessionManager : public QObject {
  Q_OBJECT
  Q_PROPERTY(PeerListModel* peers READ peers CONSTANT)

private:
  const QString m_chatsFilePath = "/OpenP2P/chats";
  PeerListModel *m_peerModel;
  QHash<PeerId, Peer*> m_peerMap;
  Self *m_self;

  bool loadSavedPeers();
  bool savePeersToFile(QJsonArray peers);
  QJsonArray loadPeersFromFile();

public:
  explicit SessionManager(Self *user, QObject *parent = nullptr);
  ~SessionManager();
  bool contains(PeerId peerid) const;
  void addPeer(Peer *peer);
  Peer* getPeer(PeerId id) const;
  QHash<PeerId, Peer*> getAllPeers() const;
  bool saveAllPeers();
  bool savePeer(Peer *peer);

  Q_INVOKABLE PeerListModel* peers() const;
  Q_INVOKABLE void deletePeer(Peer *peer);
  Q_INVOKABLE void saveChat(Peer *peer, QAbstractListModel *msgModel);
  Q_INVOKABLE bool loadChats();
  Q_INVOKABLE void addNewMsgTo(Peer *peer, bool sent, QString msg);

signals:
};

#endif // SESSIONMANAGER_H
