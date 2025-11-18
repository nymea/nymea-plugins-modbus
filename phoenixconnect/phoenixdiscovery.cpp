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

#include "phoenixdiscovery.h"
#include "extern-plugininfo.h"

PhoenixDiscovery::PhoenixDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent) :
    QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery}
{
    m_gracePeriodTimer.setSingleShot(true);
    m_gracePeriodTimer.setInterval(3000);
    connect(&m_gracePeriodTimer, &QTimer::timeout, this, [this](){
        qCDebug(dcPhoenixConnect()) << "Discovery: Grace period timer triggered.";
        finishDiscovery();
    });
}

void PhoenixDiscovery::startDiscovery()
{
    qCInfo(dcPhoenixConnect()) << "Discovery: Searching for PhoenixConnect wallboxes in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &PhoenixDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcPhoenixConnect()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "network devices";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();
        discoveryReply->deleteLater();
        m_gracePeriodTimer.start();
    });
}

QList<PhoenixDiscovery::Result> PhoenixDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void PhoenixDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    int port = 502;
    int slaveId = 0xff;

    qCDebug(dcPhoenixConnect()) << "Discovery: Checking network device:" << address << "Port:" << port << "Slave ID:" << slaveId;

    PhoenixModbusTcpConnection *connection = new PhoenixModbusTcpConnection(address, port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &PhoenixModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connect(connection, &PhoenixModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcPhoenixConnect()) << "Discovery: Initialization failed on" << address.toString();
                cleanupConnection(connection);
                return;
            }
            Result result;
            result.firmwareVersion = connection->firmwareVersion();
            result.model = connection->deviceName();
            result.serialNumber = connection->serial();
            result.address = address;
            m_discoveryResults.append(result);

            qCDebug(dcPhoenixConnect()) << "Discovery: Found wallbox with firmware version:" << result.firmwareVersion << result.networkDeviceInfo;

            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcPhoenixConnect()) << "Discovery: Unable to initialize connection on" << address.toString();
            cleanupConnection(connection);
        }
    });

    connect(connection, &PhoenixModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcPhoenixConnect()) << "Discovery: Checking reachability failed on" << address.toString();
        cleanupConnection(connection);
    });

    connection->connectDevice();
}

void PhoenixDiscovery::cleanupConnection(PhoenixModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void PhoenixDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Fill in all network device infos we have
    for (int i = 0; i < m_discoveryResults.length(); i++)
        m_discoveryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_discoveryResults.at(i).address);

    // Cleanup any leftovers...we don't care any more
    foreach (PhoenixModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcPhoenixConnect()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.length()
                       << "Phoenix connect wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();
    emit discoveryFinished();
}
