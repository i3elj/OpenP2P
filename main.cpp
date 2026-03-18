#include <QDir>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QQuickStyle>
#include "gui/addrlabel.h"
#include "peer.h"
#include "self.h"
#include "server.h"
#include "sessionmanager.h"

#define APP "OpenP2P"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

#ifdef __WIN32
  QQuickStyle::setStyle("FluentWinUI3");
#endif

  QCoreApplication::setOrganizationName("i3elj");
  QCoreApplication::setOrganizationDomain("i3elj.com");

  int instance = 0;

  for (int i = 0; i < argc; i++) {
    if (QString(argv[i]).startsWith("--instance=")) {
      instance = QString(argv[i]).sliced(11).toInt();
      break;
    }
  }

  QString appname = APP + (instance != 0 ? QString("-") + QString::number(instance) : "");
  QCoreApplication::setApplicationName(appname);

  Self::userConfigDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
  Self::savedPeersFilePath = Self::userConfigDir + "saved_peers.json";
  Self::createFiles();

  Self self(&app);
  SessionManager sm(&self, &app);
  Server server(&self, &sm, &app);
  server.initTcpSocket();

  QQmlApplicationEngine engine;

  QQmlContext *qmlContext = engine.rootContext();
  qmlContext->setContextProperty("self", &self);
  qmlContext->setContextProperty("sessionManager", &sm);
  qmlContext->setContextProperty("server", &server);

  qmlRegisterType<Peer>("App", 1, 0, "Peer");
  qmlRegisterType<PeerListModel>("App", 1, 0, "PeerListModel");
  qmlRegisterType<AddrLabel>("App", 1, 0, "AddrLabel");

  QObject::connect(
    &engine,
    &QQmlApplicationEngine::objectCreationFailed,
    &app,
    []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);

  engine.loadFromModule("p2pcom", "Main");

  return app.exec();
}
