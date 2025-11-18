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

#include "smamodbussolarinverterdiscovery.h"
#include "extern-plugininfo.h"

#include "sma.h"


SmaModbusSolarInverterDiscovery::SmaModbusSolarInverterDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, quint16 modbusAddress,QObject *parent)
    : QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery},
    m_port{port},
    m_modbusAddress{modbusAddress}
{

}

void SmaModbusSolarInverterDiscovery::startDiscovery()
{
    qCInfo(dcSma()) << "Discovery: Start searching for SMA modbus inverters in the network...";
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    m_startDateTime = QDateTime::currentDateTime();

    // Imedialty check any new device gets discovered
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &SmaModbusSolarInverterDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [this, discoveryReply](){
        qCDebug(dcSma()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "network devices";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();

        // Give the last connections added right before the network discovery finished a chance to check the device...
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcSma()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

QList<SmaModbusSolarInverterDiscovery::SmaModbusDiscoveryResult> SmaModbusSolarInverterDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void SmaModbusSolarInverterDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    SmaSolarInverterModbusTcpConnection *connection = new SmaSolarInverterModbusTcpConnection(address, m_port, m_modbusAddress, this);
    m_connections.append(connection);

    connect(connection, &SmaSolarInverterModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected...ok, let's try to initialize it!
        connect(connection, &SmaSolarInverterModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcSma()) << "Discovery: Initialization failed on" << address.toString() << "Continue...";;
                cleanupConnection(connection);
                return;
            }

            if (connection->deviceClass() != Sma::DeviceClassSolarInverter) {
                qCDebug(dcSma()) << "Discovery: Initialization successfull for" << address.toString() << "but the device class is not an inverter. Continue...";;
                cleanupConnection(connection);
                return;
            }

            SmaModbusDiscoveryResult result;
            result.productName = Sma::getModelName(connection->modelIdentifier());
            result.deviceName = connection->deviceName();
            result.serialNumber = QString::number(connection->serialNumber());
            result.port = m_port;
            result.modbusAddress = m_modbusAddress;
            result.softwareVersion = Sma::buildSoftwareVersionString(connection->softwarePackage());
            result.address = address;
            m_discoveryResults.append(result);

            qCDebug(dcSma()) << "Discovery: --> Found" << result.productName;
            qCDebug(dcSma()) << "Discovery: Device name:" << result.deviceName;
            qCDebug(dcSma()) << "Discovery: Serial number:" << result.serialNumber;
            qCDebug(dcSma()) << "Discovery: Software version:" << result.softwareVersion;
            qCDebug(dcSma()) << "Discovery:" << result.networkDeviceInfo;

            // Done with this connection
            cleanupConnection(connection);
        });

        // Initializing...
        if (!connection->initialize()) {
            qCDebug(dcSma()) << "Discovery: Unable to initialize connection on" << address.toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcSma()) << "Discovery: Connection error on" << address.toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &SmaSolarInverterModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcSma()) << "Discovery: Check reachability failed on" << address.toString() << "Continue...";;
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void SmaModbusSolarInverterDiscovery::cleanupConnection(SmaSolarInverterModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void SmaModbusSolarInverterDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Fill in all network device infos we have
    for (int i = 0; i < m_discoveryResults.length(); i++)
        m_discoveryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_discoveryResults.at(i).address);

    // Cleanup any leftovers...we don't care any more
    foreach (SmaSolarInverterModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcSma()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.length()
                    << "SMA inverters in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
