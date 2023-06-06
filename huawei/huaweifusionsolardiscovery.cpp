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

#include "huaweifusionsolardiscovery.h"
#include "extern-plugininfo.h"

HuaweiFusionSolarDiscovery::HuaweiFusionSolarDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, const QList<quint16> &slaveIds, QObject *parent)
    : QObject{parent},
      m_networkDeviceDiscovery{networkDeviceDiscovery},
      m_port{port},
      m_slaveIds{slaveIds}
{

}

void HuaweiFusionSolarDiscovery::startDiscovery()
{
    qCInfo(dcHuawei()) << "Discovery: Start searching for Huawei FusionSolar SmartDongle in the network...";
    m_startDateTime = QDateTime::currentDateTime();
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &HuaweiFusionSolarDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        // Finish with some delay so the last added network device information objects still can be checked.
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcHuawei()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

QList<HuaweiFusionSolarDiscovery::Result> HuaweiFusionSolarDiscovery::results() const
{
    return m_results;
}

void HuaweiFusionSolarDiscovery::testNextConnection(const QHostAddress &address)
{
    if (!m_pendingConnectionAttempts.contains(address))
        return;

    HuaweiFusionSolar *connection = m_pendingConnectionAttempts[address].dequeue();
    if (m_pendingConnectionAttempts.value(address).isEmpty())
        m_pendingConnectionAttempts.remove(address);

    // Try to connect, maybe it works, maybe not,
    // but retry only once to communicate with the device for reachability check...
    connection->setCheckReachableRetries(1);

    qCDebug(dcHuawei()) << "Discovery: Start searching on" << QString("%1:%2").arg(address.toString()).arg(connection->port()) << "slave ID:" << connection->slaveId();
    // Try to connect, maybe it works, maybe not...
    if (!connection->connectDevice()) {
        qCDebug(dcHuawei()) << "Discovery: Failed to connect to" << QString("%1:%2").arg(address.toString()).arg(connection->port()) << "slave ID:" << connection->slaveId() << "Continue...";;
        cleanupConnection(connection);
    }
}

void HuaweiFusionSolarDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    QQueue<HuaweiFusionSolar *> connectionQueue;
    foreach (quint16 slaveId, m_slaveIds) {
        HuaweiFusionSolar *connection = new HuaweiFusionSolar(networkDeviceInfo.address(), m_port, slaveId, this);
        m_connections.append(connection);
        connectionQueue.enqueue(connection);

        connect(connection, &HuaweiFusionSolar::reachableChanged, this, [=](bool reachable){
            if (!reachable) {
                // Disconnected ... done with this connection
                cleanupConnection(connection);
                return;
            }

            // Todo: initialize and check if available
            connect(connection, &HuaweiFusionSolar::initializationFinished, this, [=](bool success){
                Result result;
                result.networkDeviceInfo = networkDeviceInfo;
                result.slaveId = slaveId;

                if (success) {
                    qCDebug(dcHuawei()) << "Huawei init finished successfully:" << connection->model() << connection->serialNumber() << connection->productNumber();
                    result.modelName = connection->model();
                    result.serialNumber = connection->serialNumber();
                }

                qCInfo(dcHuawei()) << "Discovery: --> Found" << networkDeviceInfo << "slave ID:" << slaveId;
                m_results.append(result);
            });

            connection->initialize();
        });

        // If we get any error...skip this host...
        connect(connection, &HuaweiFusionSolar::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
            if (error != QModbusDevice::NoError) {
                qCDebug(dcHuawei()) << "Discovery: Connection error on" << networkDeviceInfo.address().toString() << "Continue...";;
                cleanupConnection(connection);
            }
        });

        // If check reachability failed...skip this host...
        connect(connection, &HuaweiFusionSolar::checkReachabilityFailed, this, [=](){
            qCDebug(dcHuawei()) << "Discovery: Check reachability failed on" << networkDeviceInfo.address().toString() << "Continue...";;
            cleanupConnection(connection);
        });

    }

    m_pendingConnectionAttempts[networkDeviceInfo.address()] = connectionQueue;
    testNextConnection(networkDeviceInfo.address());
}

void HuaweiFusionSolarDiscovery::cleanupConnection(HuaweiFusionSolar *connection)
{
    if (m_connections.contains(connection)) {
        m_connections.removeAll(connection);
        connection->disconnectDevice();
        connection->deleteLater();
    }

    testNextConnection(connection->hostAddress());
}

void HuaweiFusionSolarDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Cleanup any leftovers...we don't care any more
    foreach (HuaweiFusionSolar *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcHuawei()) << "Discovery: Finished the discovery process. Found" << m_results.count()
                       << "inverters in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");

    emit discoveryFinished();
}
