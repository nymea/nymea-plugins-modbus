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

#ifndef SMAMODBUSBATTERYINVERTERDISCOVERY_H
#define SMAMODBUSBATTERYINVERTERDISCOVERY_H

#include <network/networkdevicediscovery.h>

#include <QObject>

#include "smabatteryinvertermodbustcpconnection.h"

class SmaModbusBatteryInverterDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit SmaModbusBatteryInverterDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port = 502, quint16 modbusAddress = 3, QObject *parent = nullptr);

    struct Result {
        QString deviceName;
        QString serialNumber;
        int port;
        int modbusAddress;
        QString softwareVersion;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    };

    void startDiscovery();

    QList<Result> discoveryResults() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    uint m_port;
    uint m_modbusAddress;

    QTimer m_gracePeriodTimer;
    QDateTime m_startDateTime;

    NetworkDeviceInfos m_networkDeviceInfos;
    QList<SmaBatteryInverterModbusTcpConnection *> m_connections;
    QList<Result> m_discoveryResults;

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(SmaBatteryInverterModbusTcpConnection *connection);

    void finishDiscovery();

};

#endif // SMAMODBUSBATTERYINVERTERDISCOVERY_H
