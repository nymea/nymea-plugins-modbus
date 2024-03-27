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

#include "pcelectricdiscovery.h"
#include "extern-plugininfo.h"

PcElectricDiscovery::PcElectricDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, quint16 modbusAddress, QObject *parent)
    : QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery},
    m_port{port},
    m_modbusAddress{modbusAddress}
{

}

QList<PcElectricDiscovery::Result> PcElectricDiscovery::results() const
{
    return m_results;
}


void PcElectricDiscovery::startDiscovery()
{
    qCInfo(dcPcElectric()) << "Discovery: Start searching for PCE wallboxes in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &PcElectricDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        // Finish with some delay so the last added network device information objects still can be checked.
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcPcElectric()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

void PcElectricDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    EV11ModbusTcpConnection *connection = new EV11ModbusTcpConnection(networkDeviceInfo.address(), m_port, m_modbusAddress, this);
    m_connections.append(connection);

    connect(connection, &EV11ModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected...ok, let's try to initialize it!
        connect(connection, &EV11ModbusTcpConnection::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcPcElectric()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString() << "Continue...";;
                cleanupConnection(connection);
                return;
            }

            // Parse the mac address from the registers and compair with the network device info mac address.
            // If they match, we most likly found a PCE wallbox

            QByteArray macRawData;
            QDataStream stream(&macRawData, QIODevice::WriteOnly);
            for (int i = 0; i < connection->macAddress().count(); i++)
                stream << connection->macAddress().at(i);

            MacAddress registerMacAddress(macRawData);
            qCDebug(dcPcElectric()) << "Fetched mac address" << macRawData.toHex() << registerMacAddress;

            // According to PCE the HW revision must be 0
            if (registerMacAddress == MacAddress(networkDeviceInfo.macAddress()) && connection->hardwareRevision() == 0) {

                // Parse the serial number
                QByteArray serialRawData;
                QDataStream stream(&serialRawData, QIODevice::ReadWrite);
                stream << static_cast<quint16>(0);
                for (int i = 0; i < connection->serialNumber().count(); i++)
                    stream << connection->serialNumber().at(i);

                quint64 serialNumber = serialRawData.toHex().toULongLong(nullptr, 16);
                qCDebug(dcPcElectric()) << "Serial number" << serialRawData.toHex() << serialNumber;

                Result result;
                result.serialNumber = QString::number(serialNumber);
                result.firmwareRevision = connection->firmwareRevision();
                result.networkDeviceInfo = networkDeviceInfo;
                m_results.append(result);

                qCInfo(dcPcElectric()) << "Discovery: --> Found"
                                       << "Serial number:" << result.serialNumber
                                       << "Firmware revision:" << result.firmwareRevision
                                       << result.networkDeviceInfo;
            }

            // Done with this connection
            cleanupConnection(connection);
        });

        // Initializing...
        if (!connection->initialize()) {
            qCDebug(dcPcElectric()) << "Discovery: Unable to initialize connection on" << networkDeviceInfo.address().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcPcElectric()) << "Discovery: Connection error on" << networkDeviceInfo.address().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &EV11ModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcPcElectric()) << "Discovery: Check reachability failed on" << networkDeviceInfo.address().toString() << "Continue...";;
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void PcElectricDiscovery::cleanupConnection(EV11ModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void PcElectricDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Cleanup any leftovers...we don't care any more
    foreach (EV11ModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcPcElectric()) << "Discovery: Finished the discovery process. Found" << m_results.count()
                           << "PCE wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
