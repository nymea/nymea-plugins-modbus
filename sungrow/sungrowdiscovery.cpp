/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2024, nymea GmbH
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

#include "sungrowdiscovery.h"
#include "extern-plugininfo.h"

SungrowDiscovery::SungrowDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, quint16 modbusAddress, QObject *parent) :
    QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery},
    m_port{port},
    m_modbusAddress{modbusAddress}
{

}

void SungrowDiscovery::startDiscovery()
{
    qCDebug(dcSungrow()) << "Discovery: Start searching for Sungrow inverters in the network";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &SungrowDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=] () {
        qCDebug(dcSungrow()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();

        // Give the last connections added right before the network discovery finished a chance to check the device...
        QTimer::singleShot(3000, this, [this] () {
            qCDebug(dcSungrow()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

QList<SungrowDiscovery::SungrowDiscoveryResult> SungrowDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void SungrowDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    /* Create a Sungrow connection and try to initialize it.
    * Only if initialized successfully and all information have been fetched correctly from
    * the device we can assume this is what we are locking for (ip, port, modbus address, correct registers).
    */

    qCDebug(dcSungrow()) << "Creating Sungrow Modbus TCP connection for" << address << "Port:" << m_port << "Slave Address" << m_modbusAddress;
    SungrowModbusTcpConnection *connection = new SungrowModbusTcpConnection(address, m_port, m_modbusAddress, this);
    connection->modbusTcpMaster()->setTimeout(5000);
    connection->modbusTcpMaster()->setNumberOfRetries(0);
    m_connections.append(connection);

    connect(connection, &SungrowModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        qCDebug(dcSungrow()) << "Sungrow Modbus TCP Connection reachable changed:" << reachable;
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }
        qCDebug(dcSungrow()) << "Connected, proceeding with initialization";

        connect(connection, &SungrowModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcSungrow()) << "Discovery: Initialization failed on" << address.toString() << "Continue...";
                cleanupConnection(connection);
                return;
            }

            qCDebug(dcSungrow()) << "Discovery: Initialized successfully" << address.toString() << connection->serialNumber();
            qCDebug(dcSungrow()) << "    - Protocol number:" << connection->protocolNumber();
            qCDebug(dcSungrow()) << "    - Protocol version:" << connection->protocolVersion();
            qCDebug(dcSungrow()) << "    - ARM software version:" << connection->armSoftwareVersion();
            qCDebug(dcSungrow()) << "    - DSP software version:" << connection->dspSoftwareVersion();

            if (connection->deviceTypeCode() >= 0xd00 &&  connection->deviceTypeCode() <= 0xeff) {
                SungrowDiscoveryResult result;
                result.address = address;
                result.serialNumber = connection->serialNumber();
                result.nominalOutputPower = connection->nominalOutputPower();
                result.deviceType = connection->deviceTypeCode();
                m_discoveryResults.append(result);
            }

            connection->disconnectDevice();
        });

        qCDebug(dcSungrow()) << "Discovery: The host" << address << "is reachable. Starting with initialization.";
        if (!connection->initialize()) {
            qCDebug(dcSungrow()) << "Discovery: Unable to initialize connection on" << address.toString() << "Continue...";
            cleanupConnection(connection);
        }
    });

    // In case of an error skip the host
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionStateChanged, this, [=](bool connected){
        if (connected) {
            qCDebug(dcSungrow()) << "Discovery: Connected with" << address.toString() << m_port;
        }
    });

    // In case of an error skip the host
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcSungrow()) << "Discovery: Connection error on" << address.toString() << "Continue...";
            cleanupConnection(connection);
        }
    });

    // If the reachability check failed skip the host
    connect(connection, &SungrowModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcSungrow()) << "Discovery: Check reachability failed on" << address.toString() << "Continue...";
        cleanupConnection(connection);
    });

    connection->connectDevice();
}

void SungrowDiscovery::cleanupConnection(SungrowModbusTcpConnection *connection)
{
    qCDebug(dcSungrow()) << "Discovery: Cleanup connection" << connection->modbusTcpMaster();
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void SungrowDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Fill in all network device infos we have
    for (int i = 0; i < m_discoveryResults.count(); i++)
        m_discoveryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_discoveryResults.at(i).address);

    foreach (SungrowModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCDebug(dcSungrow()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.count() << "Sungrow inverters in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
