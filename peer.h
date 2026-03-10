#ifndef PEER_H
#define PEER_H

#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>
#include "message.h"
#include "peerid.h"

class Self;

class Peer : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QString addr READ addr CONSTANT)
  Q_PROPERTY(int port READ port CONSTANT)
  Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)

private:
  PeerId m_id;
  QString m_name;
  QHostAddress m_addr;
  int m_port;
  QTcpSocket *m_conn;
  bool m_active;

public:
  explicit Peer(QTcpSocket *conn, QObject *parent = nullptr);
  explicit Peer(QObject *parent = nullptr);

  PeerId id() const;
  QString name() const;
  QString addr() const;
  int port() const;
  bool isActive() const;
  QTcpSocket *conn() const;

  void setName(QString n);
  bool setAddr(QString address);
  void setPort(int port);
  void setConn(QTcpSocket *conn = new QTcpSocket);

  void setup();
  void close();

  bool setAddressAndPort(QString address, int port);
  void connectToHost();

  Q_INVOKABLE void sendMsg(QString txt);
  void sendMsg(Message msg);

public slots:
  void handle();
  void activate();
  void deactivate();

signals:
  void nameChanged();
  void activeChanged();
  void newMsg(QString msg);
  void msgSent(QString msg, bool success);
  void accepted(Peer *p);
  void rejected(Peer *p);
};

#endif // PEER_H
