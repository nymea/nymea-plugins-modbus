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

#include "smamodbusbatteryinverterdiscovery.h"
#include "extern-plugininfo.h"

#include "sma.h"

SmaModbusBatteryInverterDiscovery::SmaModbusBatteryInverterDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, quint16 modbusAddress, QObject *parent):
    QObject(parent),
    m_networkDeviceDiscovery{networkDeviceDiscovery},
    m_port(port),
    m_modbusAddress(modbusAddress)
{
    m_gracePeriodTimer.setSingleShot(true);
    m_gracePeriodTimer.setInterval(3000);
    connect(&m_gracePeriodTimer, &QTimer::timeout, this, [this](){
        qCDebug(dcSma()) << "Discovery: Grace period timer triggered.";
        finishDiscovery();
    });

}

void SmaModbusBatteryInverterDiscovery::startDiscovery()
{
    qCInfo(dcSma()) << "Discovery: Searching for SMA battery inverters in the network...";
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &SmaModbusBatteryInverterDiscovery::checkNetworkDevice);

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcSma()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";
        m_gracePeriodTimer.start();
        discoveryReply->deleteLater();
    });
}

QList<SmaModbusBatteryInverterDiscovery::Result> SmaModbusBatteryInverterDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void SmaModbusBatteryInverterDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    qCInfo(dcSma()) << "Checking network device:" << networkDeviceInfo << "Port:" << m_port << "Slave ID:" << m_modbusAddress;

    SmaBatteryInverterModbusTcpConnection *connection = new SmaBatteryInverterModbusTcpConnection(networkDeviceInfo.address(), m_port, m_modbusAddress, this);
    m_connections.append(connection);

    connect(connection, &SmaBatteryInverterModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connect(connection, &SmaBatteryInverterModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCInfo(dcSma()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString() << "Skipping result...";;
                cleanupConnection(connection);
                return;
            }

            if (connection->deviceClass() != Sma::DeviceClassBatteryInverter) {
                qCInfo(dcSma()) << "Discovery: Initialization successful for" << networkDeviceInfo.address().toString() << "but the device class is not a battery inverter. Skipping result...";;
                cleanupConnection(connection);
                return;
            }

            Result result;
            result.deviceName = connection->deviceName();
            result.serialNumber = QString::number(connection->serialNumber());
            result.port = m_port;
            result.modbusAddress = m_modbusAddress;
            result.softwareVersion = Sma::buildSoftwareVersionString(connection->softwarePackage());
            result.networkDeviceInfo = networkDeviceInfo;
            m_discoveryResults.append(result);

            qCInfo(dcSma()) << "Discovery: --> Found";
            qCInfo(dcSma()) << "  Device name:" << result.deviceName;
            qCInfo(dcSma()) << "  Serial number:" << result.serialNumber;
            qCInfo(dcSma()) << "  Software version:" << result.softwareVersion;
            qCInfo(dcSma()) << " " << result.networkDeviceInfo;

            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcSma()) << "Discovery: Unable to initialize connection on" << networkDeviceInfo.address().toString();
            cleanupConnection(connection);
        }
    });

    connect(connection, &SmaBatteryInverterModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcSma()) << "Discovery: Checking reachability failed on" << networkDeviceInfo.address().toString();
        cleanupConnection(connection);
    });

    connection->connectDevice();

}

void SmaModbusBatteryInverterDiscovery::cleanupConnection(SmaBatteryInverterModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void SmaModbusBatteryInverterDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Cleanup any leftovers...we don't care any more
    foreach (SmaBatteryInverterModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcSma()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.count()
                       << "SMA battery inverters in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();

}
