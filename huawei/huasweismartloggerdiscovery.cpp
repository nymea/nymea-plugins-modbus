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

#include "huasweismartloggerdiscovery.h"
#include "extern-plugininfo.h"

HuasweiSmartLoggerDiscovery::HuasweiSmartLoggerDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, quint16 slaveId, QObject *parent)
    : QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery},
    m_port{port},
    m_slaveId{slaveId}
{

}

void HuasweiSmartLoggerDiscovery::startDiscovery()
{
    qCInfo(dcHuawei()) << "Discovery: Start searching for Huawei FusionSolar SmartDongle in the network...";
    m_startDateTime = QDateTime::currentDateTime();
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &HuasweiSmartLoggerDiscovery::checkNetworkDevice);
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

QList<HuasweiSmartLoggerDiscovery::Result> HuasweiSmartLoggerDiscovery::results() const
{
    return m_results;
}

void HuasweiSmartLoggerDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    HuaweiSmartLoggerModbusTcpConnection *connection = new HuaweiSmartLoggerModbusTcpConnection(address, m_port, m_slaveId, this);
    m_connections.append(connection);

    connect(connection, &HuaweiSmartLoggerModbusTcpConnection::reachableChanged, this, [this, connection](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Todo: initialize and check if available
        // connect(connection, &HuaweiSmartLoggerModbusTcpConnection::initializationFinished, this, [this, connection](bool success){
        //     Result result;
        //     result.address = connection->modbusTcpMaster()->hostAddress();
        //     result.slaveId = connection->slaveId();

        //     if (success) {
        //         qCDebug(dcHuawei()) << "Huawei init finished successfully:" << connection->model() << connection->serialNumber() << connection->productNumber();
        //         result.modelName = connection->model();
        //         result.serialNumber = connection->serialNumber();
        //     }

        //     qCInfo(dcHuawei()) << "Discovery: --> Found" << result.address.toString() << "slave ID:" << result.slaveId;
        //     m_results.append(result);
        // });

        connection->initialize();
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [this, connection](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcHuawei()) << "Discovery: Connection error on" << connection->modbusTcpMaster()->hostAddress().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &HuaweiSmartLoggerModbusTcpConnection::checkReachabilityFailed, this, [this, connection](){
        qCDebug(dcHuawei()) << "Discovery: Check reachability failed on" << connection->modbusTcpMaster()->hostAddress().toString() << "Continue...";;
        cleanupConnection(connection);
    });
}

void HuasweiSmartLoggerDiscovery::cleanupConnection(HuaweiSmartLoggerModbusTcpConnection *connection)
{

}

void HuasweiSmartLoggerDiscovery::finishDiscovery()
{

}
