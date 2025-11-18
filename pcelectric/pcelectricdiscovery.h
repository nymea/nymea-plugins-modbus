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

#ifndef PCELECTRICDISCOVERY_H
#define PCELECTRICDISCOVERY_H

#include <QObject>

#include <network/networkdevicediscovery.h>

#include "ev11modbustcpconnection.h"

class PcElectricDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit PcElectricDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, quint16 modbusAddress, QObject *parent = nullptr);

    typedef struct Result {
        QString serialNumber;
        QString firmwareRevision;
        QHostAddress address;
        MacAddress registerMacAddress;
        NetworkDeviceInfo networkDeviceInfo;
    } Result;

    QList<PcElectricDiscovery::Result> results() const;

public slots:
    void startDiscovery();

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    quint16 m_port;
    quint16 m_modbusAddress;
    QDateTime m_startDateTime;

    QList<EV11ModbusTcpConnection *> m_connections;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<Result> m_potentialResults;

    QList<Result> m_results;

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(EV11ModbusTcpConnection *connection);

    void finishDiscovery();
};

#endif // PCELECTRICDISCOVERY_H
