/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
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

    // Imedialty check any new device gets discovered
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &SmaModbusSolarInverterDiscovery::checkNetworkDevice);

    // Check what might be left on finished
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcSma()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";

        // Send a report request to nework device info not sent already...
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
            if (!m_verifiedNetworkDeviceInfos.contains(networkDeviceInfo)) {
                checkNetworkDevice(networkDeviceInfo);
            }
        }

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

void SmaModbusSolarInverterDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    // Create a kostal connection and try to initialize it.
    // Only if initialized successfully and all information have been fetched correctly from
    // the device we can assume this is what we are locking for (ip, port, modbus address, correct registers).
    // We cloud tough also filter the result only for certain software versions, manufactueres or whatever...

    if (m_verifiedNetworkDeviceInfos.contains(networkDeviceInfo))
        return;

    SmaSolarInverterModbusTcpConnection *connection = new SmaSolarInverterModbusTcpConnection(networkDeviceInfo.address(), m_port, m_modbusAddress, this);
    m_connections.append(connection);
    m_verifiedNetworkDeviceInfos.append(networkDeviceInfo);

    connect(connection, &SmaSolarInverterModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected...ok, let's try to initialize it!
        connect(connection, &SmaSolarInverterModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcSma()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString() << "Continue...";;
                cleanupConnection(connection);
                return;
            }

            if (connection->deviceClass() != Sma::DeviceClassSolarInverter) {
                qCDebug(dcSma()) << "Discovery: Initialization successfull for" << networkDeviceInfo.address().toString() << "but the device class is not an inverter. Continue...";;
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
            result.networkDeviceInfo = networkDeviceInfo;
            m_discoveryResults.append(result);

            qCDebug(dcSma()) << "Discovery: --> Found" << result.productName;
            qCDebug(dcSma()) << "Discovery:  Device name:" << result.deviceName;
            qCDebug(dcSma()) << "Discovery:  Serial number:" << result.serialNumber;
            qCDebug(dcSma()) << "Discovery:  Software version:" << result.softwareVersion;
            qCDebug(dcSma()) << "Discovery:  " << result.networkDeviceInfo;

            // Done with this connection
            cleanupConnection(connection);
        });

        // Initializing...
        if (!connection->initialize()) {
            qCDebug(dcSma()) << "Discovery: Unable to initialize connection on" << networkDeviceInfo.address().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcSma()) << "Discovery: Connection error on" << networkDeviceInfo.address().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &SmaSolarInverterModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcSma()) << "Discovery: Check reachability failed on" << networkDeviceInfo.address().toString() << "Continue...";;
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

    // Cleanup any leftovers...we don't care any more
    foreach (SmaSolarInverterModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcSma()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.count() << "SMA inverters in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
