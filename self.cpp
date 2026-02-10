#include "self.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

Self::Self(QObject *parent)
  : QObject{parent}
  , m_name("")
  , m_settings(new QSettings(this))
{}

QString Self::name() const
{
  return m_name == "" ? m_settings->value("name", "").toString() : m_name;
}

void Self::setName(QString newName) {
  if (m_name != newName) {
    m_name = newName;
    m_settings->setValue("name", m_name);
    emit nameChanged();
  }
}
