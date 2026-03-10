#ifndef SERVER_H
#define SERVER_H

#include "ipv6addrresolver.h"
#include "peer.h"
#include "sessionmanager.h"
#include <QTcpServer>
#include <QThread>
#include <self.h>

class Server : public QTcpServer {
  Q_OBJECT

private:
  IPv6AddrResolver *m_ipr;
  SessionManager *m_session;
  Self *m_user;

  void onRemoteRejected(Peer *peer);
  void onRemoteAccepted(Peer *peer, Message acceptMsg);
  void finalizePeerConnection(Peer *peer);
  void exchangeData(Peer *peer);

public:
  explicit Server(Self *user, SessionManager *sm, QObject *parent = nullptr);
  void initTcpSocket();

  Q_INVOKABLE void acceptIncomingPeer(Peer *peer);
  Q_INVOKABLE void rejectIncomingPeer(Peer *peer);
  Q_INVOKABLE void startNewConn(QString address, int port);

public slots:
  void handleNewConnection();

signals:
  void newConnection(Peer *peer);
  void wrongAddress(QString address);
  void peerAlreadyConnected(PeerId id);
  void peerConnectionFinalized(Peer* peer);
};

#endif // SERVER_H
