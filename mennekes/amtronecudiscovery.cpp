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

#include "amtronecudiscovery.h"
#include "extern-plugininfo.h"

AmtronECUDiscovery::AmtronECUDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent) :
    QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery}
{
    m_gracePeriodTimer.setSingleShot(true);
    m_gracePeriodTimer.setInterval(3000);
    connect(&m_gracePeriodTimer, &QTimer::timeout, this, [this](){
        qCDebug(dcMennekes()) << "Discovery: Grace period timer triggered.";
        finishDiscovery();
    });
}

void AmtronECUDiscovery::startDiscovery()
{
    qCInfo(dcMennekes()) << "Discovery: Searching for AMTRON wallboxes in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &AmtronECUDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcMennekes()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "network devices";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();
        m_gracePeriodTimer.start();
    });
}

QList<AmtronECUDiscovery::Result> AmtronECUDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void AmtronECUDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    int port = 502;
    int slaveId = 0xff;
    qCDebug(dcMennekes()) << "Discovery: Checking network device:" << address.toString() << "Port:" << port << "Slave ID:" << slaveId;

    AmtronECU *connection = new AmtronECU(address, port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &AmtronECU::reachableChanged, this, [this, connection, address](bool reachable){
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connect(connection, &AmtronECU::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcMennekes()) << "Discovery: Initialization failed on" << address.toString();
                cleanupConnection(connection);
                return;
            }

            Result result;
            result.detectedVersion = connection->detectedVersion();
            result.firmwareVersion = QString::fromUtf8(QByteArray::fromHex(QByteArray::number(connection->firmwareVersion(), 16)));
            result.model = connection->model();
            result.address = address;

            // Note: the model is only known in firmware >= 5.22
            // Some users have to stay on 5.12 due to calibration law which is not available on 5.22
            switch(connection->detectedVersion()) {
            case AmtronECU::VersionOld:
                qCDebug(dcMennekes()) << "Discovery: Found wallbox with old firmware version:" << result.firmwareVersion << address.toString();
                m_discoveryResults.append(result);
                break;
            case AmtronECU::VersionNew:
                if (connection->model().isEmpty()) {
                    qCDebug(dcMennekes()) << "Discovery: Firmware version is >= 5.22 but the model could not be fetched. Skipping" << address.toString();
                    break;
                }

                qCDebug(dcMennekes()) << "Discovery: Found wallbox with new firmware version:" << result.model << result.firmwareVersion << result.networkDeviceInfo;
                m_discoveryResults.append(result);
                break;
            case AmtronECU::VersionUnknown:
                qCDebug(dcMennekes()) << "Discovery: Firmware version or model invalid. Skipping" << address.toString();
                break;
            }

            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcMennekes()) << "Discovery: Unable to initialize connection on" << address.toString();
            cleanupConnection(connection);
        }
    });

    connect(connection, &AmtronECU::checkReachabilityFailed, this, [=](){
        qCDebug(dcMennekes()) << "Discovery: Checking reachability failed on" << address.toString();
        cleanupConnection(connection);
    });

    connection->connectDevice();
}

void AmtronECUDiscovery::cleanupConnection(AmtronECU *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void AmtronECUDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Fill in all network device infos we have
    for (int i = 0; i < m_discoveryResults.length(); i++)
        m_discoveryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_discoveryResults.at(i).address);

    // Cleanup any leftovers...we don't care any more
    foreach (AmtronECU *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcMennekes()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.length()
                         << "AMTRON ECU wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();
}
