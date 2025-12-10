// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef AMTRONECUDISCOVERY_H
#define AMTRONECUDISCOVERY_H

#include <QObject>
#include <QTimer>

#include <network/networkdevicediscovery.h>

#include "phoenixmodbustcpconnection.h"

class PhoenixDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit PhoenixDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);
    struct Result {
        QString firmwareVersion;
        QString model;
        QString serialNumber;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    };

    void startDiscovery();

    QList<Result> discoveryResults() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;

    QTimer m_gracePeriodTimer;
    QDateTime m_startDateTime;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<PhoenixModbusTcpConnection *> m_connections;

    QList<Result> m_discoveryResults;

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(PhoenixModbusTcpConnection *connection);

    void finishDiscovery();
};

#endif // AMTRONECUDISCOVERY_H
