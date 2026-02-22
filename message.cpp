#include "message.h"

Message::Message()
{}

Message::Message(PeerId peerId, bool sent, QString msg)
  : m_peerId(peerId)
  , m_data(msg)
  , m_sent(sent)
  , m_json{{"id", m_peerId.toJson()}, {"sent", m_sent}, {"data", m_data}}
{}

Message::Message(QByteArray data)
  : m_json(QJsonDocument::fromJson(data).object())
{
  m_peerId = PeerId::fromJson(m_json["id"]);
}

QByteArray Message::toBytes()
{
  return QJsonDocument(m_json).toJson(QJsonDocument::Compact);
}

QString Message::message()
{
  return m_json["data"].toString();
}

QJsonObject Message::json() const
{
  return m_json;
}

Message::Type Message::type() const {
  return m_type;
}
