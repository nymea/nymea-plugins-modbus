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

#include "amtronhcc3discovery.h"
#include "extern-plugininfo.h"

AmtronHCC3Discovery::AmtronHCC3Discovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent) :
    QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery}
{
    m_gracePeriodTimer.setSingleShot(true);
    m_gracePeriodTimer.setInterval(3000);
    connect(&m_gracePeriodTimer, &QTimer::timeout, this, [this](){
        qCDebug(dcMennekes()) << "Discovery: Grace period timer triggered.";
        finishDiscovery();
    });
}

void AmtronHCC3Discovery::startDiscovery()
{
    qCInfo(dcMennekes()) << "Discovery: Searching for AMTRON wallboxes in the network...";
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &AmtronHCC3Discovery::checkNetworkDevice);

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcMennekes()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";
        m_gracePeriodTimer.start();
        discoveryReply->deleteLater();
    });
}

QList<AmtronHCC3Discovery::AmtronDiscoveryResult> AmtronHCC3Discovery::discoveryResults() const
{
    return m_discoveryResults;
}

void AmtronHCC3Discovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    if (networkDeviceInfo.macAddressManufacturer() != "GIGA-BYTE TECHNOLOGY CO.,LTD.") {
        return;
    }

    int port = 502;
    int slaveId = 0xff;
    qCDebug(dcMennekes()) << "Checking network device:" << networkDeviceInfo << "Port:" << port << "Slave ID:" << slaveId;

    AmtronHCC3ModbusTcpConnection *connection = new AmtronHCC3ModbusTcpConnection(networkDeviceInfo.address(), port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &AmtronHCC3ModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected...ok, let's try to initialize it!
        connect(connection, &AmtronHCC3ModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcMennekes()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString();
                cleanupConnection(connection);
                return;
            }
            AmtronDiscoveryResult result;
            result.wallboxName = connection->name();
            result.serialNumber = connection->serialNumber();
            result.networkDeviceInfo = networkDeviceInfo;
            m_discoveryResults.append(result);

            qCDebug(dcMennekes()) << "Discovery: --> Found" << result.wallboxName
                                << "Serial number:" << result.serialNumber
                                << result.networkDeviceInfo;


            // Done with this connection
            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcMennekes()) << "Discovery: Unable to initialize connection on" << networkDeviceInfo.address().toString();
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &AmtronHCC3ModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcMennekes()) << "Discovery: Checking reachability failed on" << networkDeviceInfo.address().toString();
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void AmtronHCC3Discovery::cleanupConnection(AmtronHCC3ModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void AmtronHCC3Discovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Cleanup any leftovers...we don't care any more
    foreach (AmtronHCC3ModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcMennekes()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.count()
                       << "AMTRON wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();
}
