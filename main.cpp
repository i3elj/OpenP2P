#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include "gui/addrlabel.h"
#include "serverthread.h"

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

    ServerThread serverThread(&app);
    serverThread.start();

    engine.loadFromModule("p2pcom", "Main");

    return app.exec();
}
