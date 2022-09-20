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
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &PhoenixDiscovery::checkNetworkDevice);

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcPhoenixConnect()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";
        m_gracePeriodTimer.start();
        discoveryReply->deleteLater();
    });
}

QList<PhoenixDiscovery::Result> PhoenixDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void PhoenixDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    if (networkDeviceInfo.macAddressManufacturer() != "wallbe GmbH" && networkDeviceInfo.macAddressManufacturer() != "Phoenix") {
        return;
    }

    int port = 502;
    int slaveId = 0xff;
    qCDebug(dcPhoenixConnect()) << "Checking network device:" << networkDeviceInfo << "Port:" << port << "Slave ID:" << slaveId;

    PhoenixModbusTcpConnection *connection = new PhoenixModbusTcpConnection(networkDeviceInfo.address(), port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &PhoenixModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connect(connection, &PhoenixModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcPhoenixConnect()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString();
                cleanupConnection(connection);
                return;
            }
            Result result;
            result.firmwareVersion = connection->firmwareVersion();
            result.model = connection->deviceName();
            result.serialNumber = connection->serial();
            result.networkDeviceInfo = networkDeviceInfo;
            m_discoveryResults.append(result);

            qCDebug(dcPhoenixConnect()) << "Discovery: Found wallbox with firmware version:" << result.firmwareVersion << result.networkDeviceInfo;

            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcPhoenixConnect()) << "Discovery: Unable to initialize connection on" << networkDeviceInfo.address().toString();
            cleanupConnection(connection);
        }
    });

    connect(connection, &PhoenixModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcPhoenixConnect()) << "Discovery: Checking reachability failed on" << networkDeviceInfo.address().toString();
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

    // Cleanup any leftovers...we don't care any more
    foreach (PhoenixModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcPhoenixConnect()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.count()
                       << "Phoenix connect wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();
}
