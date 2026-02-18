#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "message.h"
#include "peer.h"
#include "self.h"
#include "typedefs.h"
#include "peerlistmodel.h"
#include <QAbstractItemModel>
#include <QObject>

class SessionManager : public QObject {
  Q_OBJECT
  Q_PROPERTY(QList<QAbstractItemModel> savedPeers READ listPeers)

private:
  const QString m_chatsFilePath = "/OpenP2P/chats";
  PeerListModel *m_peerListModel;
  Self *m_self;

public:
  explicit SessionManager(Self *self, QObject *parent = nullptr);
  void addPeer(Peer *peer);
  PeerListModel listPeers();
  Q_INVOKABLE void saveChat(Peer *peer, QAbstractListModel *msgModel);
  Q_INVOKABLE QList<Message> loadChat(Peer *peer);
  Q_INVOKABLE void addNewMsgTo(Peer *peer, bool sent, QString msg);

signals:
};

#endif // SESSIONMANAGER_H
