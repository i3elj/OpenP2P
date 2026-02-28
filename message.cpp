#include "message.h"

Message::Message() {}

Message::Message(Type t) : m_type(t) {}

Message::Message(PeerId peerId, bool sent, QString msg)
  : m_peerId(peerId)
  , m_text(msg)
  , m_sent(sent)
  , m_type(Type::Common)
{}

Message::Message(QByteArray data)
{
  QJsonObject json(QJsonDocument::fromJson(data).object());
  m_type = Type(json["type"].toInt());

  if (m_type == Type::Common) {
    m_peerId = PeerId::fromJson(json["id"]);
    m_text = json["text"].toString();
    m_sent = json["sent"].toBool();
  }
}

QJsonObject Message::toJson()
{
  return QJsonObject{
    {"id", m_peerId.toJson()},
    {"text", m_text},
    {"sent", m_sent},
    {"type", m_type},
  };
}

QByteArray Message::toBytes()
{
  return QJsonDocument(toJson()).toJson(QJsonDocument::Compact);
}

QString Message::text()
{
  return m_text;
}

Message::Type Message::type() const
{
  return m_type;
}
