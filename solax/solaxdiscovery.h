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

#ifndef SOLAXDISCOVERY_H
#define SOLAXDISCOVERY_H

#include <QObject>
#include <QTimer>

#include <network/networkdevicediscovery.h>

#include "solaxmodbustcpconnection.h"

class SolaxDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit SolaxDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port = 502, quint16 modbusAddress = 1, QObject *parent = nullptr);
    typedef struct SolaxDiscoveryResult {
        QString productName;
        QString manufacturerName;
        QString serialNumber;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    } SolaxDiscoveryResult;

    void startDiscovery();

    QList<SolaxDiscoveryResult> discoveryResults() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    quint16 m_port;
    quint16 m_modbusAddress;
    QDateTime m_startDateTime;
    QList<SolaxModbusTcpConnection *> m_connections;
    QList<SolaxDiscoveryResult> m_discoveryResults;
    NetworkDeviceInfos m_networkDeviceInfos;

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(SolaxModbusTcpConnection *connection);

    void finishDiscovery();
};

#endif // SOLAXDISCOVERY_H
