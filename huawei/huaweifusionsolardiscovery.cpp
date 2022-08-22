/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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

#include "huaweifusionsolardiscovery.h"
#include "extern-plugininfo.h"

HuaweiFusionSolarDiscovery::HuaweiFusionSolarDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, quint16 modbusAddress, QObject *parent) :
    QObject(parent),
    m_networkDeviceDiscovery(networkDeviceDiscovery),
    m_port(port),
    m_modbusAddress(modbusAddress)
{

}


void HuaweiFusionSolarDiscovery::startDiscovery()
{
    qCInfo(dcHuawei()) << "Discovery: Start searching for Huawei FusionSolar SmartDongle in the network...";
    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();

    // Check any already discovered infos..
    foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
        checkNetworkDevice(networkDeviceInfo);
    }

    // Immedialty check any new device gets discovered
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, &HuaweiFusionSolarDiscovery::checkNetworkDevice);

    // Check what might be left on finished
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcHuawei()) << "Discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();
        qCDebug(dcHuawei()) << "Discovery: Network discovery finished. Start finishing discovery...";
        // Send a report request to nework device info not sent already...
        foreach (const NetworkDeviceInfo &networkDeviceInfo, m_networkDeviceInfos) {
            if (!m_verifiedNetworkDeviceInfos.contains(networkDeviceInfo)) {
                checkNetworkDevice(networkDeviceInfo);
            }
        }

        // Finish with some delay so the last added network device information objects still can be checked.
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcHuawei()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

NetworkDeviceInfos HuaweiFusionSolarDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void HuaweiFusionSolarDiscovery::checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo)
{
    if (m_verifiedNetworkDeviceInfos.contains(networkDeviceInfo))
        return;

    // The dongle must have a huawei registered mac address
    if (!networkDeviceInfo.macAddressManufacturer().toLower().contains("huawei"))
        return;

    HuaweiFusionSolar *connection = new HuaweiFusionSolar(networkDeviceInfo.address(), m_port, m_modbusAddress, this);
    m_connections.append(connection);
    m_verifiedNetworkDeviceInfos.append(networkDeviceInfo);

    connect(connection, &HuaweiFusionSolar::reachableChanged, this, [=](bool reachable){
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        qCDebug(dcHuawei()) << "Discovery: --> Found" << networkDeviceInfo;
        m_discoveryResults.append(networkDeviceInfo);
    });

    // If we get any error...skip this host...
    connect(connection, &HuaweiFusionSolar::connectionErrorOccurred, this, [=](QModbusDevice::Error error){
        if (error != QModbusDevice::NoError) {
            qCDebug(dcHuawei()) << "Discovery: Connection error on" << networkDeviceInfo.address().toString() << "Continue...";;
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &HuaweiFusionSolar::checkReachabilityFailed, this, [=](){
        qCDebug(dcHuawei()) << "Discovery: Check reachability failed on" << networkDeviceInfo.address().toString() << "Continue...";;
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not,
    // but retry only once to communicate with the device for reachability check...
    connection->setCheckReachableRetries(1);

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void HuaweiFusionSolarDiscovery::cleanupConnection(HuaweiFusionSolar *connection)
{
    if (m_connections.contains(connection)) {
        m_connections.removeAll(connection);
        connection->disconnectDevice();
        connection->deleteLater();
    }
}

void HuaweiFusionSolarDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    // Cleanup any leftovers...we don't care any more
    foreach (HuaweiFusionSolar *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcHuawei()) << "Discovery: Finished the discovery process. Found" << m_discoveryResults.count()
                       << "inverters in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");

    emit discoveryFinished();
}
