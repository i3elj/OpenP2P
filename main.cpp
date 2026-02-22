#include "gui/addrlabel.h"
#include "peer.h"
#include "self.h"
#include "server.h"
#include "sessionmanager.h"
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[]) {
  QCoreApplication::setOrganizationName("i3elj");
  QCoreApplication::setOrganizationDomain("i3elj.com");
  QCoreApplication::setApplicationName("OpenP2P");

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  qmlRegisterType<Peer>("App", 1, 0, "Peer");
  qmlRegisterType<PeerListModel>("App", 1, 0, "PeerListModel");
  qmlRegisterType<AddrLabel>("App", 1, 0, "AddrLabel");

  Self self(&app);
  SessionManager sm(&self, &app);
  Server server(&self, &sm, &app);
  server.initTcpSocket();

  QQmlContext *qmlContext = engine.rootContext();
  qmlContext->setContextProperty("self", &self);
  qmlContext->setContextProperty("sessionManager", &sm);
  qmlContext->setContextProperty("server", &server);

  engine.loadFromModule("p2pcom", "Main");

  return app.exec();
}
