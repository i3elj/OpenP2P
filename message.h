#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include "peerid.h"

class Message
{
private:
  PeerId m_peerId;
  QString m_data;
  bool m_sent;
  QJsonObject m_json;

public:
  explicit Message();
  explicit Message(PeerId peerId, bool sent, QString msg);
  explicit Message(QByteArray data);
  QByteArray toBytes();
  QString message();
  QJsonObject json() const;
};

#endif // MESSAGE_H
