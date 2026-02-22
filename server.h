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

public:
  explicit Server(Self *user, SessionManager *sm, QObject *parent = nullptr);
  void initTcpSocket();

  Q_INVOKABLE void setupPeer(Peer *peer);
  Q_INVOKABLE void acceptPeer(Peer *peer);
  Q_INVOKABLE void rejectPeer(Peer *peer);
  Q_INVOKABLE void startNewConn(QString address, int port);

public slots:
  void handleNewConnection();
  void destroyPeer(Peer *peer);

signals:
  void newConnection(Peer *peer);
  void newMsg(Peer *from, QString msg);
  void wrongAddress(QString address);
};

#endif // SERVER_H
