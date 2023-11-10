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

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &AmtronECUDiscovery::checkNetworkDevice);

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcMennekes()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";
        m_gracePeriodTimer.start();
        discoveryReply->deleteLater();
    });
}

QList<AmtronECUDiscovery::Result> AmtronECUDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void AmtronECUDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    int port = 502;
    int slaveId = 0xff;
    qCDebug(dcMennekes()) << "Discovery: Checking network device:" << networkDeviceInfo << "Port:" << port << "Slave ID:" << slaveId;

    AmtronECU *connection = new AmtronECU(networkDeviceInfo.address(), port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &AmtronECU::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connect(connection, &AmtronECU::initializationFinished, this, [=](bool success){
            if (!success) {
                qCDebug(dcMennekes()) << "Discovery: Initialization failed on" << networkDeviceInfo.address().toString();
                cleanupConnection(connection);
                return;
            }

            Result result;
            result.detectedVersion = connection->detectedVersion();
            result.firmwareVersion = QString::fromUtf8(QByteArray::fromHex(QByteArray::number(connection->firmwareVersion(), 16)));
            result.model = connection->model();
            result.networkDeviceInfo = networkDeviceInfo;

            // Note: the model is only known in firmware >= 5.22
            // Some useres have to stay on 5.12 due to calibration law which is not available on 5.22
            switch(connection->detectedVersion()) {
            case AmtronECU::VersionOld:
                qCDebug(dcMennekes()) << "Discovery: Found wallbox with old firmware version:" << result.firmwareVersion << result.networkDeviceInfo;
                m_discoveryResults.append(result);
                break;
            case AmtronECU::VersionNew:
                if (connection->model().isEmpty()) {
                    qCDebug(dcMennekes()) << "Discovery: Firmware version is >= 5.22 but the model could not be fetched. Skipping" << networkDeviceInfo.address();
                    break;
                }

                qCDebug(dcMennekes()) << "Discovery: Found wallbox with new firmware version:" << result.model << result.firmwareVersion << result.networkDeviceInfo;
                m_discoveryResults.append(result);
                break;
            case AmtronECU::VersionUnknown:
                qCDebug(dcMennekes()) << "Discovery: Firmware version or model invalid. Skipping" << networkDeviceInfo.address();
                break;
            }

            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcMennekes()) << "Discovery: Unable to initialize connection on" << networkDeviceInfo.address().toString();
            cleanupConnection(connection);
        }
    });

    connect(connection, &AmtronECU::checkReachabilityFailed, this, [=](){
        qCDebug(dcMennekes()) << "Discovery: Checking reachability failed on" << networkDeviceInfo.address().toString();
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

    // Cleanup any leftovers...we don't care any more
    foreach (AmtronECU *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcMennekes()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.count()
                         << "AMTRON ECU wallboxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();
}
