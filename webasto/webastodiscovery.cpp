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

    m_startDateTime = QDateTime::currentDateTime();

    qCInfo(dcWebasto()) << "Discovery: Starting to search for WebastoNext wallboxes in the network...";
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &WebastoDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcWebasto()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";
        // Give the last connections added right before the network discovery finished a chance to check the device...
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcWebasto()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

QList<WebastoDiscovery::Result> WebastoDiscovery::results() const
{
    return m_results;
}

void WebastoDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    WebastoNextModbusTcpConnection *connection = new WebastoNextModbusTcpConnection(networkDeviceInfo.address(), 502, 1, this);
    m_connections.append(connection);

    connect(connection, &WebastoNextModbusTcpConnection::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Read some well known registers to verify if the register exist and make sense...
        QModbusReply *reply = connection->readCableState();
        connect(reply, &QModbusReply::finished, this, [=](){

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
                qCDebug(dcWebasto()) << "Discovery: invalid enum value for cable state on connection on" << networkDeviceInfo.address().toString() << "Continue...";;
                cleanupConnection(connection);
            }

            QModbusReply *reply = connection->readChargerState();
            connect(reply, &QModbusReply::finished, this, [=](){

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
                    qCDebug(dcWebasto()) << "Discovery: invalid enum value for charger state on connection on" << networkDeviceInfo.address().toString() << "Continue...";;
                    cleanupConnection(connection);
                }


                // Read some registers distributed over the range...

                QModbusReply *reply = connection->readTotalActivePower();
                connect(reply, &QModbusReply::finished, this, [=](){

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

                        // Final check if there is a hostname available for this network device, if so it shouls contain the string "NEXT_".
                        // This is neccessary since Wallboxes from Vestel EVC04 aka. Webasto Unite wallboxes would also match
                        // the creteria up to here get detected as positiv Webasto NEXT.

                        // Example hostname: NEXT-WS10XXXX

                        if (!networkDeviceInfo.hostName().isEmpty() &&
                            (!networkDeviceInfo.hostName().contains("NEXT_") || networkDeviceInfo.hostName().contains("VESTEL"))) {
                            qCDebug(dcWebasto()) << "Discovery: network device has a hostname and it does match kriteria for Webasto next:" << networkDeviceInfo.hostName() << "on" << networkDeviceInfo.address().toString() << "Continue...";;
                            cleanupConnection(connection);
                            return;
                        }

                        // Hostname verification also OK, let's assume this is a Webasto NEXT

                        Result result;
                        result.productName = "Webasto NEXT";
                        result.type = TypeWebastoNext;
                        result.networkDeviceInfo = networkDeviceInfo;
                        m_results.append(result);

                        qCDebug(dcWebasto()) << "Discovery: --> Found" << result.productName << result.networkDeviceInfo;

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
            qCDebug(dcWebasto()) << "Discovery: Connection error on" << networkDeviceInfo.address().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &WebastoNextModbusTcpConnection::checkReachabilityFailed, this, [=](){
        qCDebug(dcWebasto()) << "Discovery: Check reachability failed on" << networkDeviceInfo.address().toString() << "Continue...";;
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

    // Cleanup any leftovers...we don't care any more
    foreach (WebastoNextModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcWebasto()) << "Discovery: Finished the discovery process. Found" << m_results.count() << "Webasto NEXT wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
