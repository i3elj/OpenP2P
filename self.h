#ifndef SELF_H
#define SELF_H

#include <QHostAddress>
#include <QObject>
#include <QSettings>
#include "typedefs.h"

class Self : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

private:
  QSettings *m_settings;
  QString m_name;
  int m_port;
  AddressList m_ipv6list;

public:
  static QString userConfigDir;
  static QString savedPeersFilePath;
  explicit Self(QObject *parent = nullptr);
  QString name() const;
  void setName(QString newName);
  int port() const;
  void setPort(int port);
  void createFiles() const;

signals:
  void nameChanged();
  void portChanged();
};

#endif // SELF_H
