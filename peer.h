#ifndef PEER_H
#define PEER_H

#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>
#include "message.h"
#include "peerid.h"
#include "chatlistmodel.h"

class Self;

class Peer : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QString addr READ addr CONSTANT)
  Q_PROPERTY(int port READ port CONSTANT)
  Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)
  Q_PROPERTY(ChatListModel *chatModel READ chatModel CONSTANT)

private:
  PeerId m_id;
  QString m_name;
  QHostAddress m_addr;
  int m_port;
  QTcpSocket *m_conn;
  bool m_active;
  ChatListModel* m_chatModel;

public:
  explicit Peer(QTcpSocket *conn, QObject *parent = nullptr);
  explicit Peer(QObject *parent = nullptr);
  ~Peer();

  PeerId id() const;
  QString name() const;
  QString addr() const;
  int port() const;
  QTcpSocket *conn() const;
  bool isActive() const;
  ChatListModel *chatModel() const;

  void setName(QString n);
  bool setAddr(QString address);
  void setPort(int port);
  void setConn(QTcpSocket *conn = new QTcpSocket);

  void setupHandler();
  void close();

  bool setAddressAndPort(QString address, int port);
  void connectToHost();

  void sendMsg(Message msg);
  void loadMessages(QList<Message> messages);

  Q_INVOKABLE void sendMsg(QString txt);
  Q_INVOKABLE void saveChat();

public slots:
  void handle();
  void activate();
  void deactivate();
  void setupConnection();

signals:
  void nameChanged();
  void activeChanged();
  void connectionChanged();
  void newMsg(Message msg);
  void msgSent(Message msg);
  void accepted(Peer *p);
  void rejected(Peer *p);
};

#endif // PEER_H
