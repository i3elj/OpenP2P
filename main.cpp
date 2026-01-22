#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gui/addrlabel.h"
#include "peer.h"
#include "peerid.h"
#include "server.h"

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

    qmlRegisterType<AddrLabel>("App", 1, 0, "AddrLabel");

    QHash<PeerId, Peer *> ACTIVE_PEERS;

    Server *server = new Server(&ACTIVE_PEERS, &app);
    server->initTcpSocket();

    QQmlContext *qmlContext = engine.rootContext();
    qmlContext->setContextProperty("server", server);

    engine.loadFromModule("p2pcom", "Main");

    return app.exec();
}
