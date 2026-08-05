// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TQDM100DISCOVERY_H
#define TQDM100DISCOVERY_H

#include <QObject>
#include <QTimer>
#include <QLoggingCategory>

#include <network/networkdevicediscovery.h>

#include "bendertqdm100modbustcpconnection.h"

class TqDm100Discovery : public QObject
{
    Q_OBJECT
public:
    explicit TqDm100Discovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);

    struct Result {
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    };

    void startDiscovery();
    QList<Result> discoveryResults() const;

signals:
    void discoveryFinished();

private:
    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(BenderTqDm100ModbusTcpConnection *connection);
    void finishDiscovery();

    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    QTimer m_gracePeriodTimer;
    QDateTime m_startDateTime;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<BenderTqDm100ModbusTcpConnection *> m_connections;
    QList<Result> m_discoveryResults;
};

#endif // TQDM100DISCOVERY_H
