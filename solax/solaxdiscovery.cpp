 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2023, nymea GmbH
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
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &SolaxDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcSolax()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";

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

void SolaxDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    // Create a Solax connection and try to initialize it.
    // Only if initialized successfully and all information have been fetched correctly from
    // the device we can assume this is what we are locking for (ip, port, modbus address, correct registers).
    // We cloud tough also filter the result only for certain software versions, manufactueres or whatever...

    SolaxModbusTcpConnection *connection = new SolaxModbusTcpConnection(networkDeviceInfo.address(), m_port, m_modbusAddress, this);
    connection->modbusTcpMaster()->setTimeout(500);
    connection->modbusTcpMaster()->setNumberOfRetries(0);
    m_connections.append(connection);

    connect(connection, &SolaxModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected and reachable call successed, let's try to initialize it!
        connect(connection, &SolaxModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcSolax()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString() << "Continue...";;
                cleanupConnection(connection);
                return;
            }

            qCInfo(dcSolax()) << "Discovery: Initialized successfully" << networkDeviceInfo << connection->factoryName() << connection->serialNumber();

            // Let's make sure the information are correct
            if (connection->factoryName().toLower().contains("solax")) {
                SolaxDiscoveryResult result;
                result.productName = connection->moduleName();
                result.manufacturerName = connection->factoryName();
                result.serialNumber = connection->serialNumber();
                result.networkDeviceInfo = networkDeviceInfo;
                m_discoveryResults.append(result);

                qCInfo(dcSolax()) << "Discovery: --> Found" << result.manufacturerName << result.productName
                                  << "Serial number:" << result.serialNumber
                                  << result.networkDeviceInfo;
            }

            connection->disconnectDevice();
        });

        qCDebug(dcSolax()) << "Discovery: The host" << networkDeviceInfo << "is reachable, trying to initialize...";
        if (!connection->initialize()) {
            qCDebug(dcSolax()) << "Discovery: Unable to initialize connection on" << networkDeviceInfo.address().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionStateChanged, this, [=](bool connected){
        if (connected) {
            qCDebug(dcSolax()) << "Discovery: Connected with" << networkDeviceInfo.address().toString() << m_port;
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcSolax()) << "Discovery: Connection error on" << networkDeviceInfo.address().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &SolaxModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcSolax()) << "Discovery: Check reachability failed on" << networkDeviceInfo.address().toString() << "Continue...";;
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

    // Cleanup any leftovers...we don't care any more
    foreach (SolaxModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcSolax()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.count() << "Solax Inverters in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
