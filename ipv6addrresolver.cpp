#include "ipv6addrresolver.h"
#include <QNetworkInterface>

IPv6AddrResolver::IPv6AddrResolver(QObject *parent)
    : QObject{parent}
{}

QList<QHostAddress> IPv6AddrResolver::resolve()
{
    QList<QHostAddress> list;
    QStringList emittedList;

    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces()) {
        auto flags = iface.flags();

        if (!(flags & QNetworkInterface::IsUp) || !(flags & QNetworkInterface::IsRunning)) {
            continue;
        }

        for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
            QHostAddress addr = entry.ip();
            bool isNotLocal = !addr.isLinkLocal();
            bool isIPv6 = addr.protocol() == QAbstractSocket::IPv6Protocol;

            if (isIPv6 && isNotLocal && addr.toString() != "::1") {
                list.append(addr);
            }
        }
    }

    emittedList.reserve(list.size());

    std::transform(list.begin(),
                   list.end(),
                   std::back_inserter(emittedList),
                   [](const QHostAddress &a) { return a.toString(); });

    emit finished(emittedList);

    return list;
}
