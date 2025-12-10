// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "webastodiscovery.h"
#include "extern-plugininfo.h"

WebastoDiscovery::WebastoDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent)
    : QObject{parent},
      m_networkDeviceDiscovery{networkDeviceDiscovery}
{

}

void WebastoDiscovery::startDiscovery()
{

    // TODO: add parameter for searching WebastoNext or WebastoLive, for now the discovery searches only for WebastoNext
    qCInfo(dcWebasto()) << "Discovery: Starting to search for WebastoNext wallboxes in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    m_networkDeviceInfos.clear();
    m_temporaryResults.clear();
    m_results.clear();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &WebastoDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [this, discoveryReply](){
        qCDebug(dcWebasto()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "network devices";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();

        // Give the last connections added right before the network discovery finished a chance to check the device...
        QTimer::singleShot(3000, this, &WebastoDiscovery::finishDiscovery);
    });
}

QList<WebastoDiscovery::Result> WebastoDiscovery::results() const
{
    return m_results;
}

void WebastoDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    WebastoNextModbusTcpConnection *connection = new WebastoNextModbusTcpConnection(address, 502, 1, this);
    m_connections.append(connection);

    connect(connection, &WebastoNextModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Read some well known registers to verify if the register exist and make sense...
        QModbusReply *reply = connection->readCableState();
        connect(reply, &QModbusReply::finished, this, [this, reply, connection, address](){

            reply->deleteLater();

            if (reply->error() != QModbusDevice::NoError) {
                // Something went wrong...probably not the device we are searching for
                cleanupConnection(connection);
                return;
            }

            // Make sure this is a valid cable state
            const QModbusDataUnit unit = reply->result();
            quint16 rawValue = ModbusDataUtils::convertToUInt16(unit.values());
            QMetaEnum valueEnum = WebastoNextModbusTcpConnection::staticMetaObject.enumerator(WebastoNextModbusTcpConnection::staticMetaObject.indexOfEnumerator("CableState"));
            if (!valueEnum.valueToKey(rawValue)) {
                qCDebug(dcWebasto()) << "Discovery: invalid enum value for cable state on connection on" << address.toString() << "Continue...";;
                cleanupConnection(connection);
            }

            QModbusReply *reply = connection->readChargerState();
            connect(reply, &QModbusReply::finished, this, [this, reply, connection, address](){

                reply->deleteLater();

                if (reply->error() != QModbusDevice::NoError) {
                    // Something went wrong...probably not the device we are searching for
                    cleanupConnection(connection);
                    return;
                }

                // Make sure this is a valid charger state
                const QModbusDataUnit unit = reply->result();
                quint16 rawValue = ModbusDataUtils::convertToUInt16(unit.values());
                QMetaEnum valueEnum = WebastoNextModbusTcpConnection::staticMetaObject.enumerator(WebastoNextModbusTcpConnection::staticMetaObject.indexOfEnumerator("ChargerState"));
                if (!valueEnum.valueToKey(rawValue)) {
                    qCDebug(dcWebasto()) << "Discovery: invalid enum value for charger state on connection on" << address.toString() << "Continue...";;
                    cleanupConnection(connection);
                }

                // Read some registers distributed over the range...

                QModbusReply *reply = connection->readTotalActivePower();
                connect(reply, &QModbusReply::finished, this, [this, reply, connection, address](){

                    reply->deleteLater();

                    if (reply->error() != QModbusDevice::NoError) {
                        // Something went wrong...probably not the device we are searching for
                        cleanupConnection(connection);
                        return;
                    }

                    QModbusReply *reply = connection->readSessionEnergy();
                    connect(reply, &QModbusReply::finished, this, [=](){

                        reply->deleteLater();

                        if (reply->error() != QModbusDevice::NoError) {
                            // Something went wrong...probably not the device we are searching for
                            cleanupConnection(connection);
                            return;
                        }

                        // All values good so far, let's assume this is a Webasto NEXT

                        // The final check regarding the hostname will be done when all network device infos are available in the finishDiscovery slot

                        Result result;
                        result.productName = "Webasto NEXT";
                        result.type = TypeWebastoNext;
                        result.address = address;
                        m_temporaryResults.append(result);

                        // Done with this connection
                        cleanupConnection(connection);
                    });
                });
            });
        });
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcWebasto()) << "Discovery: Connection error on" << address.toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &WebastoNextModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcWebasto()) << "Discovery: Check reachability failed on" << address.toString() << "Continue...";;
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void WebastoDiscovery::cleanupConnection(WebastoNextModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void WebastoDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    for (int i = 0; i < m_temporaryResults.length(); i++) {
        // Fill in all network device infos we have
        m_temporaryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_temporaryResults.at(i).address);

        // Final check if there is a hostname available for this network device, if so it shouls contain the string "NEXT_".
        // This is neccessary since Wallboxes from Vestel EVC04 aka. Webasto Unite wallboxes would also match
        // the creteria up to here get detected as positiv Webasto NEXT.

        // Example hostname: NEXT-WS10XXXX
        QString hostName = m_temporaryResults.at(i).networkDeviceInfo.hostName();
        if (!hostName.isEmpty() && (!hostName.contains("NEXT_") || hostName.contains("VESTEL"))) {
            qCDebug(dcWebasto()) << "Discovery: network device has a hostname and it does match kriteria for Webasto next:" << hostName << "on" << m_temporaryResults.at(i).networkDeviceInfo.address().toString() << "Continue...";;
            continue;
        }

        // Hostname verification also OK, let's assume this is a Webasto NEXT

        qCDebug(dcWebasto()) << "Discovery: --> Found" << m_temporaryResults.at(i).productName << m_temporaryResults.at(i).networkDeviceInfo;
        m_results.append(m_temporaryResults.at(i));
    }

    // Cleanup any leftovers...we don't care any more
    foreach (WebastoNextModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcWebasto()) << "Discovery: Finished the discovery process. Found" << m_results.length() << "Webasto NEXT wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
