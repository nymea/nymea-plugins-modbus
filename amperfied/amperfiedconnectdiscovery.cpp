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

#include "amperfiedconnectdiscovery.h"
#include "extern-plugininfo.h"

AmperfiedConnectDiscovery::AmperfiedConnectDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent) :
    QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery}
{
    m_gracePeriodTimer.setSingleShot(true);
    m_gracePeriodTimer.setInterval(3000);
    connect(&m_gracePeriodTimer, &QTimer::timeout, this, [this](){
        qCDebug(dcAmperfied()) << "Discovery: Grace period timer triggered.";
        finishDiscovery();
    });
}

void AmperfiedConnectDiscovery::startDiscovery(const QString &nameFilter)
{
    qCInfo(dcAmperfied()) << "Discovery: Searching for Amperfied" << nameFilter << "wallboxes in the network...";
    m_nameFilter = nameFilter;
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &AmperfiedConnectDiscovery::checkNetworkDevice);

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcAmperfied()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "network devices";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();
        m_gracePeriodTimer.start();
        discoveryReply->deleteLater();
    });
}

QList<AmperfiedConnectDiscovery::Result> AmperfiedConnectDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void AmperfiedConnectDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    int port = 502;
    int slaveId = 1;

    qCDebug(dcAmperfied()) << "Checking network device:" << address.toString() << "Port:" << port << "Slave ID:" << slaveId;

    AmperfiedModbusTcpConnection *connection = new AmperfiedModbusTcpConnection(address, port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &AmperfiedModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected...ok, let's try to initialize it!
        connect(connection, &AmperfiedModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcAmperfied()) << "Discovery: Initialization failed on" << address.toString();
                cleanupConnection(connection);
                return;
            }

            if (connection->version() < 0x100 || connection->version() > 0x2ff) {
                qCInfo(dcAmperfied()) << "Skipping invalid/unsupported AMPERFIED version" << connection->version();
                cleanupConnection(connection);
                return;
            }

            if (!m_nameFilter.isEmpty() && connection->logisticString() != m_nameFilter) {
                qCInfo(dcAmperfied()) << "Skipping" << connection->modbusTcpMaster()->hostAddress().toString() << "as name" << connection->logisticString() << "does not match filter" << m_nameFilter;
                cleanupConnection(connection);
                return;
            }

            Result result;
            result.firmwareVersion = connection->version();
            result.modelName = connection->logisticString();
            result.address = address;
            m_discoveryResults.append(result);

            qCDebug(dcAmperfied()) << "Discovery: --> Found"
                                << result.modelName
                                << "Version:" << result.firmwareVersion
                                << result.address.toString();

            // Done with this connection
            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcAmperfied()) << "Discovery: Unable to initialize connection on" << address.toString();
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &AmperfiedModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcAmperfied()) << "Discovery: Checking reachability failed on" << address.toString();
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void AmperfiedConnectDiscovery::cleanupConnection(AmperfiedModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void AmperfiedConnectDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Fill in finished network device information
    for (int i = 0; i < m_discoveryResults.length(); i++)
        m_discoveryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_discoveryResults.value(i).address);

    // Cleanup any leftovers...we don't care any more
    foreach (AmperfiedModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcAmperfied()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.length()
                       << "Amperfied wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();
}
