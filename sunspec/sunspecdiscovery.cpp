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

#include "sunspecdiscovery.h"

#include <sunspecmodel.h>
#include <models/sunspecmodelfactory.h>
#include <models/sunspeccommonmodel.h>

SunSpecDiscovery::SunSpecDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 slaveId, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent)
    : QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery},
    m_slaveId{slaveId},
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
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &SunSpecDiscovery::checkNetworkDevice);

    // Check what might be left on finished
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [this, discoveryReply](){
        qCDebug(dcSunSpec()) << "Discovery: Network discovery finished. Give some time for pending discovery checks to finish...";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();

        // Give the last connections added right before the network discovery finished a chance to check the device...
        QTimer::singleShot(3000, this, [this](){
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
        return;
    }

    /* Some connection will block the connection process and the connection will pending in the
     * connecting state for over a minute before failing.
     * This behavior blocks further connection tests for during the disdovery process and the discovery failes to detect the device.
     * This timer will make sure the connection can be established within 5 seconds, otherwise we continue with the next connection.
     */

    QTimer *connectionTimer = new QTimer(connection);
    connectionTimer->setInterval(5000);
    connectionTimer->setSingleShot(true);

    m_connectionTimers.insert(connection, connectionTimer);

    connect(connectionTimer, &QTimer::timeout, connection, [this, connection, connectionTimer](){
        qCDebug(dcSunSpec()) << "Discovery: Could not establish a connection within" << connectionTimer->interval() << "ms. Continue...";
        m_connectionTimers.remove(connection);
        connection->disconnectDevice();
        // Note: since the connection is the parent of the timer, it will be deleted with the connection
        cleanupConnection(connection);
    });

    // Once connected, the timer will be stopped.
    connectionTimer->start();
}

void SunSpecDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    // Create a connection queue for this network device

    QQueue<SunSpecConnection *> connectionQueue;

    // Check all ports for this host
    foreach (quint16 port, m_scanPorts) {


        SunSpecConnection *connection = new SunSpecConnection(address, port, m_slaveId, m_byteOrder, this);
        connection->setNumberOfRetries(1);
        connection->setTimeout(500);
        m_connections.append(connection);
        connectionQueue.enqueue(connection);

        connect(connection, &SunSpecConnection::connectedChanged, this, [this, connection, connectionQueue, address](bool connected){
            if (!connected) {
                // Disconnected ... done with this connection
                cleanupConnection(connection);
                return;
            }

            // Successfully connected, we can stop the connection timer which takes care about blocking connection attempts
            if (m_connectionTimers.contains(connection)) {
                QTimer *connectionTimer = m_connectionTimers.take(connection);
                connectionTimer->stop();
                connectionTimer->deleteLater();
            }

            // Modbus TCP connected, try to discovery sunspec models...
            connect(connection, &SunSpecConnection::discoveryFinished, this, [=](bool success){
                if (!success) {
                    qCDebug(dcSunSpec()) << "Discovery: SunSpec discovery failed on"
                                         << QString("%1:%2").arg(address.toString()).arg(connection->port())
                                         << "slave ID:" << connection->slaveId() << "Continue...";
                    cleanupConnection(connection);
                    return;
                }

                // Success, we found some sunspec models here, let's read some infomation from the models

                Result result;
                result.address = address;
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
                qCDebug(dcSunSpec()) << "Discovery: Unable to discover SunSpec data on connection"
                                     << QString("%1:%2").arg(address.toString()).arg(connection->port())
                                     << "slave ID:" << connection->slaveId() << "Continue...";
                cleanupConnection(connection);
            }
        });

        // If we get any error...skip this host...
        connect(connection->modbusTcpClient(), &QModbusTcpClient::errorOccurred, this, [=](QModbusDevice::Error error){
            if (error != QModbusDevice::NoError) {
                qCDebug(dcSunSpec()) << "Discovery: Connection error on"
                                     << QString("%1:%2").arg(address.toString()).arg(connection->port())
                                     << "slave ID:" << connection->slaveId() << "Continue...";
                cleanupConnection(connection);
            }
        });
    }

    m_pendingConnectionAttempts[address] = connectionQueue;
    testNextConnection(address);
}

void SunSpecDiscovery::cleanupConnection(SunSpecConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();

    if (m_connectionTimers.contains(connection)) {
        QTimer *connectionTimer = m_connectionTimers.take(connection);
        connectionTimer->stop();
    }

    testNextConnection(connection->hostAddress());
}

void SunSpecDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Fill in all network device infos we have
    for (int i = 0; i < m_results.length(); i++)
        m_results[i].networkDeviceInfo = m_networkDeviceInfos.get(m_results.at(i).address);

    // Cleanup any leftovers...we don't care any more
    foreach (SunSpecConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcSunSpec()) << "Discovery: Finished the discovery process. Found" << m_results.length()
                        << "SunSpec devices in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
