#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>

class Config : public QObject
{
  Q_OBJECT

private:
  QSettings m_settings;
  QString m_username;
  int m_port;

public:
  explicit Config(QObject *parent = nullptr);
};

#endif // CONFIG_H
