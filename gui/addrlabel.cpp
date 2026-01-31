#include "gui/addrlabel.h"

AddrLabel::AddrLabel(QObject *parent)
    : QObject{parent}
    , ipr(new IPv6AddrResolver())
    , thread(new QThread(this))
{
    ipr->moveToThread(thread);

    connect(thread, &QThread::started, ipr, &IPv6AddrResolver::resolve);
    connect(ipr, &IPv6AddrResolver::finished, this, &AddrLabel::ipsReceived);
    connect(ipr, &IPv6AddrResolver::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, ipr, &IPv6AddrResolver::deleteLater);

    thread->start();
}

AddrLabel::~AddrLabel()
{
    if (thread && thread->isRunning()) {
        thread->quit();
        thread->wait();
    }
}
