/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
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

#include "huaweismartloggerdiscovery.h"
#include "extern-plugininfo.h"

HuaweiSmartLoggerDiscovery::HuaweiSmartLoggerDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, QObject *parent)
    : QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery},
    m_port{port}
{

}

void HuaweiSmartLoggerDiscovery::startDiscovery()
{
    qCInfo(dcHuawei()) << "Discovery: Start searching for Huawei SmartLogger in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &HuaweiSmartLoggerDiscovery::checkNetworkDevice);

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [this, discoveryReply](){
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();

        // Finish with some delay so the last added network device information objects still can be checked.
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcHuawei()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

QList<HuaweiSmartLoggerDiscovery::Result> HuaweiSmartLoggerDiscovery::results() const
{
    return m_results;
}

void HuaweiSmartLoggerDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    HuaweiSmartLogger *connection = new HuaweiSmartLogger(address, m_port, 1, this);
    m_connections.append(connection);

    connect(connection, &HuaweiSmartLogger::reachableChanged, this, [this, connection](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        Result result;
        result.address = connection->modbusTcpMaster()->hostAddress();
        qCInfo(dcHuawei()) << "Discovery: --> Found reachable device on" << result.address.toString();
        m_results.append(result);
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [this, connection](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcHuawei()) << "Discovery: Connection error on" << connection->modbusTcpMaster()->hostAddress().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &HuaweiSmartLogger::checkReachabilityFailed, this, [this, connection](){
        qCDebug(dcHuawei()) << "Discovery: Check reachability failed on" << connection->modbusTcpMaster()->hostAddress().toString() << "Continue...";;
        cleanupConnection(connection);
    });

    connection->connectDevice();
}

void HuaweiSmartLoggerDiscovery::cleanupConnection(HuaweiSmartLogger *connection)
{
    if (m_connections.contains(connection)) {
        m_connections.removeAll(connection);
        connection->disconnectDevice();
        connection->deleteLater();
    }
}

void HuaweiSmartLoggerDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Fill in finished network device information
    for (int i = 0; i < m_results.count(); i++)
        m_results[i].networkDeviceInfo = m_networkDeviceInfos.get(m_results.value(i).address);

    // Cleanup any leftovers...we don't care any more
    foreach (HuaweiSmartLogger *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcHuawei()) << "Discovery: Finished the discovery process. Found" << m_results.count()
                       << "inverters in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");

    emit discoveryFinished();
}
