#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "message.h"
#include "peer.h"
#include "self.h"
#include "peerlistmodel.h"
#include <QAbstractItemModel>
#include <QObject>

class SessionManager : public QObject {
  Q_OBJECT
  Q_PROPERTY(PeerListModel* peers READ peers CONSTANT)

private:
  const QString m_chatsFilePath = "/OpenP2P/chats";
  PeerListModel *m_peerModel;
  QHash<PeerId, Peer*> m_peerMap;
  Self *m_user;

  bool loadSavedPeers();

public:
  explicit SessionManager(Self *user, QObject *parent = nullptr);
  void addPeer(Peer *peer);
  void deletePeer(Peer *peer);
  void activatePeer(PeerId id, QTcpSocket *conn);
  Peer* getPeer(PeerId id);

  Q_INVOKABLE PeerListModel* peers() const;
  Q_INVOKABLE void saveChat(Peer *peer, QAbstractListModel *msgModel);
  Q_INVOKABLE QList<Message> loadChat(Peer *peer);
  Q_INVOKABLE void addNewMsgTo(Peer *peer, bool sent, QString msg);

signals:
};

#endif // SESSIONMANAGER_H
