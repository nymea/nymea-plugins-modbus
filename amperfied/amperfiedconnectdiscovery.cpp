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

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &AmperfiedConnectDiscovery::checkNetworkDevice);

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcAmperfied()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";
        m_gracePeriodTimer.start();
        discoveryReply->deleteLater();
    });
}

QList<AmperfiedConnectDiscovery::Result> AmperfiedConnectDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void AmperfiedConnectDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    int port = 502;
    int slaveId = 1;
    qCDebug(dcAmperfied()) << "Checking network device:" << networkDeviceInfo << "Port:" << port << "Slave ID:" << slaveId;

    AmperfiedModbusTcpConnection *connection = new AmperfiedModbusTcpConnection(networkDeviceInfo.address(), port, slaveId, this);
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
                qCDebug(dcAmperfied()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString();
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
            result.networkDeviceInfo = networkDeviceInfo;
            m_discoveryResults.append(result);

            qCDebug(dcAmperfied()) << "Discovery: --> Found"
                                << result.modelName
                                << "Version:" << result.firmwareVersion
                                << result.networkDeviceInfo;


            // Done with this connection
            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcAmperfied()) << "Discovery: Unable to initialize connection on" << networkDeviceInfo.address().toString();
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &AmperfiedModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcAmperfied()) << "Discovery: Checking reachability failed on" << networkDeviceInfo.address().toString();
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

    // Cleanup any leftovers...we don't care any more
    foreach (AmperfiedModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcAmperfied()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.count()
                       << "Amperfied wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();
}
