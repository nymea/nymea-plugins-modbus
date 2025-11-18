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

#ifndef AMTRONHCC3DISCOVERY_H
#define AMTRONHCC3DISCOVERY_H

#include <QObject>
#include <QTimer>

#include <network/networkdevicediscovery.h>

#include "amtronhcc3modbustcpconnection.h"

class AmtronHCC3Discovery : public QObject
{
    Q_OBJECT
public:
    typedef struct AmtronDiscoveryResult {
        QString wallboxName;
        QString serialNumber;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    } AmtronDiscoveryResult;

    explicit AmtronHCC3Discovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);

    void startDiscovery();

    QList<AmtronDiscoveryResult> discoveryResults() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    QTimer m_gracePeriodTimer;
    QDateTime m_startDateTime;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<AmtronHCC3ModbusTcpConnection *> m_connections;
    QList<AmtronDiscoveryResult> m_discoveryResults;

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(AmtronHCC3ModbusTcpConnection *connection);

    void finishDiscovery();
};

#endif // AMTRONHCC3DISCOVERY_H
