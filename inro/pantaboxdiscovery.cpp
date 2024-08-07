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

#include "pantaboxdiscovery.h"
#include "extern-plugininfo.h"

PantaboxDiscovery::PantaboxDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent)
    : QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery}
{

}

QList<PantaboxDiscovery::Result> PantaboxDiscovery::results() const
{
    return m_results;
}

QString PantaboxDiscovery::modbusVersionToString(quint32 value)
{
    quint16 modbusVersionMinor = (value >> 8) & 0xffff;
    quint16 modbusVersionMajor = value & 0xffff;
    return QString("%1.%2").arg(modbusVersionMajor).arg(modbusVersionMinor);
}

void PantaboxDiscovery::startDiscovery()
{
    qCInfo(dcInro()) << "Discovery: Start searching for PANTABOX wallboxes in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &PantaboxDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        // Finish with some delay so the last added network device information objects still can be checked.
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcInro()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

void PantaboxDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    PantaboxModbusTcpConnection *connection = new PantaboxModbusTcpConnection(networkDeviceInfo.address(), m_port, m_modbusAddress, this);
    m_connections.append(connection);

    connect(connection, &PantaboxModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected...ok, let's try to initialize it!
        connect(connection, &PantaboxModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcInro()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString() << "Continue...";
                cleanupConnection(connection);
                return;
            }

            // FIXME: find a better way to discover the device besides a valid init
            qCDebug(dcInro()) << "Discovery: Connection initialized successfully" << connection->serialNumber();

            Result result;
            result.serialNumber = QString::number(connection->serialNumber(), 16).toUpper();
            result.modbusTcpVersion = modbusVersionToString(connection->modbusTcpVersion());
            result.networkDeviceInfo = networkDeviceInfo;
            m_results.append(result);

            qCInfo(dcInro()) << "Discovery: --> Found"
                             << "Serial number:" << result.serialNumber
                             << "(" << connection->serialNumber() << ")"
                             << "ModbusTCP version:" << result.modbusTcpVersion
                             << result.networkDeviceInfo;

            // Done with this connection
            cleanupConnection(connection);
        });

        // Initializing...
        if (!connection->initialize()) {
            qCDebug(dcInro()) << "Discovery: Unable to initialize connection on" << networkDeviceInfo.address().toString() << "Continue...";
            cleanupConnection(connection);
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcInro()) << "Discovery: Connection error on" << networkDeviceInfo.address().toString() << "Continue...";
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &PantaboxModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcInro()) << "Discovery: Check reachability failed on" << networkDeviceInfo.address().toString() << "Continue...";
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void PantaboxDiscovery::cleanupConnection(PantaboxModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void PantaboxDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Cleanup any leftovers...we don't care any more
    foreach (PantaboxModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcInro()) << "Discovery: Finished the discovery process. Found" << m_results.count()
                     << "PANTABOXE wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
