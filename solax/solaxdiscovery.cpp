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

#include "solaxdiscovery.h"
#include "extern-plugininfo.h"

SolaxDiscovery::SolaxDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, quint16 modbusAddress, QObject *parent) :
    QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery},
    m_port{port},
    m_modbusAddress{modbusAddress}
{

}

void SolaxDiscovery::startDiscovery()
{
    qCInfo(dcSolax()) << "Discovery: Start searching for Solax inverters in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &SolaxDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [this, discoveryReply](){
        qCDebug(dcSolax()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "network devices";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();

        // Give the last connections added right before the network discovery finished a chance to check the device...
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcSolax()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

QList<SolaxDiscovery::SolaxDiscoveryResult> SolaxDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void SolaxDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    // Create a Solax connection and try to initialize it.
    // Only if initialized successfully and all information have been fetched correctly from
    // the device we can assume this is what we are locking for (ip, port, modbus address, correct registers).
    // We cloud tough also filter the result only for certain software versions, manufactueres or whatever...

    SolaxModbusTcpConnection *connection = new SolaxModbusTcpConnection(address, m_port, m_modbusAddress, this);
    connection->modbusTcpMaster()->setTimeout(500);
    connection->modbusTcpMaster()->setNumberOfRetries(0);
    m_connections.append(connection);

    connect(connection, &SolaxModbusTcpConnection::reachableChanged, this, [this, connection, address](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected and reachable call successed, let's try to initialize it!
        connect(connection, &SolaxModbusTcpConnection::initializationFinished, this, [this, connection, address](bool success){
            if (!success) {
                qCDebug(dcSolax()) << "Discovery: Initialization failed on" << address.toString() << "Continue...";;
                cleanupConnection(connection);
                return;
            }

            qCInfo(dcSolax()) << "Discovery: Initialized successfully" << address.toString() << connection->factoryName() << connection->serialNumber();

            // Let's make sure the information are correct
            if (connection->factoryName().toLower().contains("solax")) {
                SolaxDiscoveryResult result;
                result.productName = connection->moduleName();
                result.manufacturerName = connection->factoryName();
                result.serialNumber = connection->serialNumber();
                result.address = address;
                m_discoveryResults.append(result);

                qCInfo(dcSolax()) << "Discovery: --> Found" << result.manufacturerName << result.productName
                                  << "Serial number:" << result.serialNumber
                                  << result.networkDeviceInfo;
            }

            connection->disconnectDevice();
        });

        qCDebug(dcSolax()) << "Discovery: The host" << address << "is reachable, trying to initialize...";
        if (!connection->initialize()) {
            qCDebug(dcSolax()) << "Discovery: Unable to initialize connection on" << address.toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionStateChanged, this, [this, address](bool connected){
        if (connected) {
            qCDebug(dcSolax()) << "Discovery: Connected with" << address.toString() << m_port;
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [this, connection, address](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcSolax()) << "Discovery: Connection error on" << address.toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &SolaxModbusTcpConnection::checkReachabilityFailed, this, [this, connection, address](){
        qCDebug(dcSolax()) << "Discovery: Check reachability failed on" << address.toString() << "Continue...";;
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void SolaxDiscovery::cleanupConnection(SolaxModbusTcpConnection *connection)
{
    qCDebug(dcSolax()) << "Discovery: Cleanup connection" << connection->modbusTcpMaster();
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void SolaxDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Fill in all network device infos we have
    for (int i = 0; i < m_discoveryResults.length(); i++)
        m_discoveryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_discoveryResults.at(i).address);

    // Cleanup any leftovers...we don't care any more
    foreach (SolaxModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcSolax()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.length()
                      << "Solax Inverters in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
