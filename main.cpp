#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include "gui/addrlabel.h"
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

    Server s;

    engine.loadFromModule("p2pcom", "Main");

    return app.exec();
}
