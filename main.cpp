#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "config.h"
#include "gui/addrlabel.h"
#include "peer.h"
#include "self.h"
#include "server.h"
#include "sessionmanager.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  QObject::connect(
    &engine,
    &QQmlApplicationEngine::objectCreationFailed,
    &app,
    []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);

  qmlRegisterType<Peer>("App", 1, 0, "Peer");
  qmlRegisterType<AddrLabel>("App", 1, 0, "AddrLabel");

  Config config(&app);
  Self self(&config, &app);
  SessionManager sm(&app);
  Server server(&sm, &app);
  server.initTcpSocket();

  QQmlContext *qmlContext = engine.rootContext();
  qmlContext->setContextProperty("self", &self);
  qmlContext->setContextProperty("sessionManager", &sm);
  qmlContext->setContextProperty("server", &server);

  engine.loadFromModule("p2pcom", "Main");

  return app.exec();
}
