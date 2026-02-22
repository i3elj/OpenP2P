#ifndef PEER_H
#define PEER_H

#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>
#include "peerid.h"

class Self;

class Peer : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QString addr READ addr CONSTANT)
  Q_PROPERTY(int port READ port CONSTANT)

private:
  PeerId m_id;
  QTcpSocket *m_conn;
  QHostAddress m_addr;
  int m_port;
  QString m_name;
  Self *m_user;

public:
  explicit Peer(Self *user, QTcpSocket *conn, PeerId id, QObject *parent = nullptr);
  explicit Peer(Self *user, QObject *parent = nullptr);

  PeerId id() const;
  QTcpSocket *conn() const;
  void setConn(QTcpSocket *conn);
  QString addr() const;
  int port() const;
  QString name() const;
  void setName(QString n);

  void setup();
  void close();

  bool setAddressAndPort(QString address, int port);
  void connectToHost();

  Q_INVOKABLE void sendMsg(QString msg);

public slots:
  void handleMessage();

signals:
  void nameChanged();
  void newMsg(Peer *from, QString msg);
  void msgSent(Peer *to, QString msg, bool success);
  void accepted(Peer *p);
  void rejected(Peer *p);
};

#endif // PEER_H
