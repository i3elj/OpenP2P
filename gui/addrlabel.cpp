#include "gui/addrlabel.h"
#include <qthread.h>

AddrLabel::AddrLabel(QObject *parent)
    : QObject{parent}
{
    QThread *thread = new QThread;
    IPv6AddrResolver *ipr = new IPv6AddrResolver;

    ipr->moveToThread(thread);

    connect(thread, &QThread::started, ipr, &IPv6AddrResolver::resolve);
    connect(ipr, &IPv6AddrResolver::finished, this, &AddrLabel::ipReceived);
    connect(ipr, &IPv6AddrResolver::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, ipr, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();
}
