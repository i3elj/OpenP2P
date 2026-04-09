#ifndef MESSAGE_H
#define MESSAGE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

class Message
{
public:
  enum Type { Reject = 0, Accept = 1, Common = 2, DataExchange = 3 };

  explicit Message();
  explicit Message(Type t);
  explicit Message(QString name);
  explicit Message(bool sent, QString msg);
  explicit Message(QByteArray data);

  QJsonObject toJson();
  QByteArray toBytes();
  void setMetaData(QString name, int port, QString publicKey);

  QString text() const;
  bool sent() const;
  QString hostName() const;
  int port() const;
  Message::Type type() const;
  QString publicKey() const;

  void setText(QString txt);
  void setSent(bool sent);
  void setHostName(QString name);
  void setPort(int port);
  void setType(Message::Type type);

private:
  QString m_text;
  bool m_sent;
  QString m_hostName;
  int m_port;
  QString m_publicKey;
  Message::Type m_type;
};

#endif // MESSAGE_H
