#ifndef SELF_H
#define SELF_H

#include <QHostAddress>
#include <QObject>
#include <config.h>
#include <typedefs.h>

class Self : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

private:
  QString m_name;
  AddressList m_ipv6list;
  Config *m_config;

public:
  explicit Self(Config *cfg, QObject *parent = nullptr);
  QString name() const;
  void setName(QString newName);

signals:
  void nameChanged();
};

#endif // SELF_H
