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

#include "sunspecdiscovery.h"

#include <sunspecmodel.h>
#include <models/sunspecmodelfactory.h>
#include <models/sunspeccommonmodel.h>

SunSpecDiscovery::SunSpecDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, const QList<quint16> &slaveIds, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent)
    : QObject{parent},
      m_networkDeviceDiscovery{networkDeviceDiscovery},
      m_slaveIds{slaveIds},
      m_byteOrder{byteOrder}
{
    m_scanPorts.append(502);
    m_scanPorts.append(1502);
}

QList<SunSpecDiscovery::Result> SunSpecDiscovery::results() const
{
    return m_results;
}

void SunSpecDiscovery::addCustomDiscoveryPort(quint16 port)
{
    if (m_scanPorts.contains(port))
        return;

    m_scanPorts.append(port);
}

void SunSpecDiscovery::startDiscovery()
{
    qCInfo(dcSunSpec()) << "Discovery: Start searching for SunSpec devices in the network...";
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();

    m_startDateTime = QDateTime::currentDateTime();

    // Imedialty check any new device gets discovered
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &SunSpecDiscovery::checkNetworkDevice);

    // Check what might be left on finished
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcSunSpec()) << "Discovery: Network discovery finished. Give some time for pending discovery checks to finish...";

        // Give the last connections added right before the network discovery finished a chance to check the device...
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcSunSpec()) << "Discovery: Grace period timer triggered";
            finishDiscovery();
        });
    });
}

void SunSpecDiscovery::testNextConnection(const QHostAddress &address)
{
    if (!m_pendingConnectionAttempts.contains(address))
        return;

    SunSpecConnection *connection = m_pendingConnectionAttempts[address].dequeue();
    if (m_pendingConnectionAttempts.value(address).isEmpty())
        m_pendingConnectionAttempts.remove(address);

    qCDebug(dcSunSpec()) << "Discovery: Start searching on" << QString("%1:%2").arg(address.toString()).arg(connection->port()) << "slave ID:" << connection->slaveId();
    // Try to connect, maybe it works, maybe not...
    if (!connection->connectDevice()) {
        qCDebug(dcSunSpec()) << "Discovery: Failed to connect to" << QString("%1:%2").arg(address.toString()).arg(connection->port()) << "slave ID:" << connection->slaveId() << "Continue...";;
        cleanupConnection(connection);
    }
}

void SunSpecDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    // Create a connection queue for this network device

    QQueue<SunSpecConnection *> connectionQueue;

    // Check all ports for this host
    foreach (quint16 port, m_scanPorts) {

        foreach (quint16 slaveId, m_slaveIds) {

            SunSpecConnection *connection = new SunSpecConnection(networkDeviceInfo.address(), port, slaveId, m_byteOrder, this);
            connection->setNumberOfRetries(1);
            connection->setTimeout(500);
            m_connections.append(connection);
            connectionQueue.enqueue(connection);

            connect(connection, &SunSpecConnection::connectedChanged, this, [=](bool connected){
                if (!connected) {
                    // Disconnected ... done with this connection
                    cleanupConnection(connection);
                    return;
                }

                // Modbus TCP connected, try to discovery sunspec models...
                connect(connection, &SunSpecConnection::discoveryFinished, this, [=](bool success){
                    if (!success) {
                        qCDebug(dcSunSpec()) << "Discovery: SunSpec discovery failed on" << QString("%1:%2").arg(networkDeviceInfo.address().toString()).arg(port) << "slave ID:" << slaveId << "Continue...";;
                        cleanupConnection(connection);
                        return;
                    }

                    // Success, we found some sunspec models here, let's read some infomation from the models

                    Result result;
                    result.networkDeviceInfo = networkDeviceInfo;
                    result.port = connection->port();
                    result.slaveId = connection->slaveId();

                    qCDebug(dcSunSpec()) << "Discovery: --> Found SunSpec devices on" << result.networkDeviceInfo << "port" << result.port << "slave ID:" << result.slaveId;
                    foreach (SunSpecModel *model, connection->models()) {
                        if (model->modelId() == SunSpecModelFactory::ModelIdCommon) {
                            SunSpecCommonModel *commonModel = qobject_cast<SunSpecCommonModel *>(model);
                            QString manufacturer = commonModel->manufacturer();
                            if (!manufacturer.isEmpty() && !result.modelManufacturers.contains(manufacturer)) {
                                result.modelManufacturers.append(manufacturer);
                            }
                        }
                    }

                    m_results.append(result);

                    // Done with this connection
                    cleanupConnection(connection);
                });

                // Run SunSpec discovery on connection...
                if (!connection->startDiscovery()) {
                    qCDebug(dcSunSpec()) << "Discovery: Unable to discover SunSpec data on connection" << QString("%1:%2").arg(networkDeviceInfo.address().toString()).arg(port) << "slave ID:" << slaveId << "Continue...";;
                    cleanupConnection(connection);
                }
            });

            // If we get any error...skip this host...
            connect(connection->modbusTcpClient(), &QModbusTcpClient::errorOccurred, this, [=](QModbusDevice::Error error){
                if (error != QModbusDevice::NoError) {
                    qCDebug(dcSunSpec()) << "Discovery: Connection error on" << QString("%1:%2").arg(networkDeviceInfo.address().toString()).arg(port) << "slave ID:" << slaveId << "Continue...";;
                    cleanupConnection(connection);
                }
            });
        }
    }

    m_pendingConnectionAttempts[networkDeviceInfo.address()] = connectionQueue;
    testNextConnection(networkDeviceInfo.address());
}

void SunSpecDiscovery::cleanupConnection(SunSpecConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();

    testNextConnection(connection->hostAddress());
}

void SunSpecDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Cleanup any leftovers...we don't care any more
    foreach (SunSpecConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcSunSpec()) << "Discovery: Finished the discovery process. Found" << m_results.count() << "SunSpec devices in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
