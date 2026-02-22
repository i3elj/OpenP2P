#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include "peerid.h"

class Message
{
public:
  enum Type { Reject = 0, Accept = 1, Common = 2};

  explicit Message();
  explicit Message(PeerId peerId, bool sent, QString msg);
  explicit Message(QByteArray data);
  QByteArray toBytes();
  QString message();
  QJsonObject json() const;
  Message::Type type() const;

private:
  PeerId m_peerId;
  QString m_data;
  bool m_sent;
  QJsonObject m_json;
  Message::Type m_type;
};

#endif // MESSAGE_H
