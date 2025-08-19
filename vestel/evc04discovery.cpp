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

#include "evc04discovery.h"
#include "extern-plugininfo.h"

EVC04Discovery::EVC04Discovery(NetworkDeviceDiscovery *networkDeviceDiscovery, const QLoggingCategory &dc, QObject *parent) :
    QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery},
    m_dc{dc.categoryName()}
{
    m_gracePeriodTimer.setSingleShot(true);
    m_gracePeriodTimer.setInterval(3000);
    connect(&m_gracePeriodTimer, &QTimer::timeout, this, [this](){
        qCDebug(m_dc) << "Discovery: Grace period timer triggered.";
        finishDiscovery();
    });
}

void EVC04Discovery::startDiscovery()
{
    qCInfo(m_dc()) << "Discovery: Searching for Vestel EVC04 wallboxes in the network...";
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    m_startDateTime = QDateTime::currentDateTime();

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &EVC04Discovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(m_dc()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "network devices";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();
        m_gracePeriodTimer.start();
    });
}

QList<EVC04Discovery::Result> EVC04Discovery::discoveryResults() const
{
    return m_discoveryResults;
}

void EVC04Discovery::checkNetworkDevice(const QHostAddress &address)
{
    int port = 502;
    int slaveId = 0xff;
    qCDebug(m_dc()) << "Discovery: Checking network device:" << address << "Port:" << port << "Slave ID:" << slaveId;

    EVC04ModbusTcpConnection *connection = new EVC04ModbusTcpConnection(address, port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &EVC04ModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connect(connection, &EVC04ModbusTcpConnection::initializationFinished, this, [=](bool success){
            qCDebug(m_dc()) << "Discovery: Discovered device on" << address.toString() << connection->brand() << connection->model() << connection->firmwareVersion();
            qCDebug(m_dc()) << connection;
            if (!success) {
                qCDebug(m_dc()) << "Discovery: Initialization failed on" << address.toString();
                cleanupConnection(connection);
                return;
            }

            Result result;
            result.chargepointId = QString(QString::fromUtf16(connection->chargepointId().data(), connection->chargepointId().length()).toUtf8()).trimmed();
            result.brand = QString(QString::fromUtf16(connection->brand().data(), connection->brand().length()).toUtf8()).trimmed();
            result.model = QString(QString::fromUtf16(connection->model().data(), connection->model().length()).toUtf8()).trimmed();
            result.firmwareVersion = QString(QString::fromUtf16(connection->firmwareVersion().data(), connection->firmwareVersion().length()).toUtf8()).trimmed();
            result.address = address;

            if (result.chargepointId.isEmpty() && result.brand.isEmpty() && result.model.isEmpty() && result.firmwareVersion.isEmpty()) {
                qCDebug(m_dc()) << "Discovery: Found modbus device with valid register set (initialized successfully), but the information seem not to be valid. "
                                   "This is probably some other device. Ignoring" << address.toString();
                cleanupConnection(connection);
                return;
            }

            m_discoveryResults.append(result);
            qCDebug(m_dc()) << "Discovery: Found wallbox with firmware version:" << result.firmwareVersion << address.toString();
            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(m_dc()) << "Discovery: Unable to initialize connection on" << address.toString();
            cleanupConnection(connection);
        }
    });

    connect(connection, &EVC04ModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(m_dc()) << "Discovery: Checking reachability failed on" << address.toString();
        cleanupConnection(connection);
    });

    connection->connectDevice();
}

void EVC04Discovery::cleanupConnection(EVC04ModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void EVC04Discovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Fill in all network device infos we have
    for (int i = 0; i < m_discoveryResults.length(); i++)
        m_discoveryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_discoveryResults.at(i).address);

    // Cleanup any leftovers...we don't care any more
    foreach (EVC04ModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(m_dc()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.length()
                   << "EVC04 wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();
}
