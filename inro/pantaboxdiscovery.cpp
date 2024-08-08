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
    Pantabox *connection = new Pantabox(networkDeviceInfo.address(), m_port, m_modbusAddress, this);
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
                qCDebug(dcInro()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString() << "Continue...";
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
                connect(reply, &QModbusReply::finished, this, [this, reply, connection, networkDeviceInfo](){
                    if (reply->error() != QModbusDevice::NoError) {
                        qCDebug(dcInro()) << "Discovery: Error reading product name error on" << networkDeviceInfo.address().toString() << "Continue...";
                        cleanupConnection(connection);
                        return;
                    }

                    const QModbusDataUnit unit = reply->result();
                    if (unit.values().size() == 4) {
                        QString receivedProductName = ModbusDataUtils::convertToString(unit.values(), connection->stringEndianness());
                        if (receivedProductName.toUpper().contains("PANTABOX")) {
                            addResult(connection, networkDeviceInfo);
                        } else {
                            qCDebug(dcInro()) << "Discovery: Invalid product name " << receivedProductName
                                              << "on" << networkDeviceInfo.address().toString() << "Continue...";
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
                addResult(connection, networkDeviceInfo);
            }
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
    connect(connection, &Pantabox::checkReachabilityFailed, this, [=](){
        qCDebug(dcInro()) << "Discovery: Check reachability failed on" << networkDeviceInfo.address().toString() << "Continue...";
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

    // Cleanup any leftovers...we don't care any more
    foreach (Pantabox *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcInro()) << "Discovery: Finished the discovery process. Found" << m_results.count()
                     << "PANTABOXE wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}

void PantaboxDiscovery::addResult(Pantabox *connection, const NetworkDeviceInfo &networkDeviceInfo)
{
    qCDebug(dcInro()) << "Discovery: Connection initialized successfully" << connection->serialNumber();

    Result result;
    result.serialNumber = QString::number(connection->serialNumber(), 16).toUpper();
    result.modbusTcpVersion = Pantabox::modbusVersionToString(connection->modbusTcpVersion());
    result.networkDeviceInfo = networkDeviceInfo;
    m_results.append(result);

    qCInfo(dcInro()) << "Discovery: --> Found"
                     << "Serial number:" << result.serialNumber
                     << "(" << connection->serialNumber() << ")"
                     << "ModbusTCP version:" << result.modbusTcpVersion
                     << result.networkDeviceInfo;

    // Done with this connection
    cleanupConnection(connection);
}
