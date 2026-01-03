#include "ipv6addrresolver.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QThread>

IPv6AddrResolver::IPv6AddrResolver(QObject *parent)
    : QObject{parent}
{}

QStringList IPv6AddrResolver::resolve()
{
    QStringList list;

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
                list.append(addr.toString());
            }
        }
    }

    emit finished(list);
    return list;
}
