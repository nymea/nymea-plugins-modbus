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

PantaboxDiscovery::PantaboxDiscovery(QObject *parent)
    : QObject{parent}
{

}

QList<PantaboxDiscovery::Result> PantaboxDiscovery::results() const
{
    return m_results;
}

void PantaboxDiscovery::startDiscovery()
{
    qCInfo(dcInro()) << "Discovery: Start searching for PANTABOX wallboxes in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    m_discovery = new PantaboxUdpDiscovery(this);
    connect(m_discovery, &PantaboxUdpDiscovery::pantaboxDiscovered, this, &PantaboxDiscovery::checkNetworkDevice);

    connect(&m_discoveryTimer, &QTimer::timeout, this, &PantaboxDiscovery::finishDiscovery);
    m_discoveryTimer.setSingleShot(true);
    m_discoveryTimer.start(10000);
}

void PantaboxDiscovery::checkNetworkDevice(const PantaboxUdpDiscovery::DeviceInfo &deviceInfo)
{
    if (m_alreadyCheckedHosts.contains(deviceInfo.ipAddress))
        return;

    m_alreadyCheckedHosts.append(deviceInfo.ipAddress);

    Pantabox *connection = new Pantabox(deviceInfo.ipAddress, m_port, m_modbusAddress, this);
    m_connections.append(connection);

    connect(connection, &Pantabox::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected...ok, let's try to initialize it!
        connect(connection, &Pantabox::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcInro()) << "Discovery: Initialization failed on" << deviceInfo.ipAddress.toString() << "Continue...";
                cleanupConnection(connection);
                return;
            }

            // Vendor and product name registers are available since modbus TCP version 1.1 (0x0001 0x0001) 0x10001 = 65537
            if (connection->modbusTcpVersion() >= 65537) {

                QModbusReply *reply = connection->readProductName();
                if (!reply) {
                    cleanupConnection(connection);
                    return;
                }

                if (reply->isFinished()) {
                    reply->deleteLater(); // Broadcast reply returns immediatly
                    cleanupConnection(connection);
                    return;
                }

                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                connect(reply, &QModbusReply::finished, this, [this, reply, connection, deviceInfo](){
                    if (reply->error() != QModbusDevice::NoError) {
                        qCDebug(dcInro()) << "Discovery: Error reading product name error on" << deviceInfo.ipAddress.toString() << "Continue...";
                        cleanupConnection(connection);
                        return;
                    }

                    const QModbusDataUnit unit = reply->result();
                    if (unit.values().size() == 4) {
                        QString receivedProductName = ModbusDataUtils::convertToString(unit.values(), connection->stringEndianness());
                        if (receivedProductName.toUpper().contains("PANTABOX")) {
                            addResult(connection, deviceInfo);
                        } else {
                            qCDebug(dcInro()) << "Discovery: Invalid product name " << receivedProductName
                                              << "on" << deviceInfo.ipAddress.toString() << "Continue...";
                            cleanupConnection(connection);
                        }
                    } else {
                        qCDebug(dcInro()) << "Discovery: Reading from \"Name of product\" registers" << 262 << "size:" << 4 << "returned different size than requested. Ignoring incomplete data" << unit.values();
                        cleanupConnection(connection);
                    }
                });
            } else {
                qCDebug(dcInro()) << "Discovery: Adding connection to results even tough the result is not precise due to modbus TCP protocol version:"
                                  << connection->modbusTcpVersion() << Pantabox::modbusVersionToString(connection->modbusTcpVersion());
                addResult(connection, deviceInfo);
            }
        });

        // Initializing...
        if (!connection->initialize()) {
            qCDebug(dcInro()) << "Discovery: Unable to initialize connection on" << deviceInfo.ipAddress.toString() << "Continue...";
            cleanupConnection(connection);
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcInro()) << "Discovery: Connection error on" << deviceInfo.ipAddress.toString() << "Continue...";
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &Pantabox::checkReachabilityFailed, this, [=](){
        qCDebug(dcInro()) << "Discovery: Check reachability failed on" << deviceInfo.ipAddress.toString() << "Continue...";
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void PantaboxDiscovery::cleanupConnection(Pantabox *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void PantaboxDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    m_discovery->deleteLater();
    m_discovery = nullptr;

    m_alreadyCheckedHosts.clear();

    // Cleanup any leftovers...we don't care any more
    foreach (Pantabox *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcInro()) << "Discovery: Finished the discovery process. Found" << m_results.count()
                     << "PANTABOXE wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");

    emit discoveryFinished();
}

void PantaboxDiscovery::addResult(Pantabox *connection, const PantaboxUdpDiscovery::DeviceInfo &deviceInfo)
{
    QString modbusSerialNumber = QString::number(connection->serialNumber(), 16).toUpper();
    if (deviceInfo.serialNumber != modbusSerialNumber) {
        qCWarning(dcInro()) << "Discovery: Successfully discovered PANTABOX, but the UPD serial number does not match the fetched modbus serial number. Ignoring result...";
        cleanupConnection(connection);
        return;
    }

    qCDebug(dcInro()) << "Discovery: Connection initialized successfully" << modbusSerialNumber;

    Result result;
    result.modbusTcpVersion = Pantabox::modbusVersionToString(connection->modbusTcpVersion());
    result.deviceInfo = deviceInfo;
    m_results.append(result);

    qCInfo(dcInro()) << "Discovery: --> Found"
                     << "Serial number:" << result.deviceInfo.serialNumber
                     << "(" << connection->serialNumber() << ")"
                     << "ModbusTCP version:" << result.modbusTcpVersion
                     << "on" << result.deviceInfo.ipAddress.toString() << result.deviceInfo.macAddress.toString();

    // Done with this connection
    cleanupConnection(connection);
}
