// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ABBTERRATCPDISCOVERY_H
#define ABBTERRATCPDISCOVERY_H

#include <QObject>
#include <QDateTime>

#include <network/networkdevicediscovery.h>

#include "abbterramodbustcpconnection.h"

class AbbTerraTcpDiscovery : public QObject
{
    Q_OBJECT
public:
    struct Result {
        NetworkDeviceInfo networkDeviceInfo;
        QString serialNumber;
        QString productName;
        QString firmwareVersion;
    };

    explicit AbbTerraTcpDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);

    void startDiscovery();
    QList<Result> results() const;

signals:
    void discoveryFinished();

private:
    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(AbbTerraModbusTcpConnection *connection);
    void finishDiscovery();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    QDateTime m_startDateTime;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<AbbTerraModbusTcpConnection *> m_connections;
    QList<Result> m_results;
    QList<Result> m_temporaryResults;
};

#endif // ABBTERRATCPDISCOVERY_H
