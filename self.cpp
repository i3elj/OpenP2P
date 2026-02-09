#include "self.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include "config.h"

Self::Self(Config *cfg, QObject *parent)
  : QObject{parent}
  , m_config(cfg)
  , m_name("")
{}

QString Self::name() const
{
  return m_name;
}

void Self::setName(QString newName)
{
  if (m_name != newName) {
    m_name = newName;
    emit nameChanged();
  }
}
