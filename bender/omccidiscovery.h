// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OMCCIDISCOVERY_H
#define OMCCIDISCOVERY_H

#include <QObject>
#include <QTimer>
#include <QLoggingCategory>

#include <network/networkdevicediscovery.h>

#include "benderomccimodbustcpconnection.h"

class OmcciDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit OmcciDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);

    struct Result {
        QString serialNumber;
        QString firmwareVersion;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    };

    void startDiscovery();
    QList<Result> discoveryResults() const;

signals:
    void discoveryFinished();

private:
    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(BenderOmcciModbusTcpConnection *connection);
    void finishDiscovery();

    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    QTimer m_gracePeriodTimer;
    QDateTime m_startDateTime;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<BenderOmcciModbusTcpConnection *> m_connections;
    QList<Result> m_discoveryResults;
};

#endif // OMCCIDISCOVERY_H
