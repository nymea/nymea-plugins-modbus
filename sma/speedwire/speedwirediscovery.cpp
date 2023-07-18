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

#include "speedwirediscovery.h"
#include "extern-plugininfo.h"
#include "speedwire.h"

#include <QDataStream>

SpeedwireDiscovery::SpeedwireDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, SpeedwireInterface *speedwireInterface, quint32 localSerialNumber, QObject *parent) :
    QObject(parent),
    m_networkDeviceDiscovery(networkDeviceDiscovery),
    m_speedwireInterface(speedwireInterface),
    m_localSerialNumber(localSerialNumber)
{
    // More details: https://github.com/RalfOGit/libspeedwire/

    // Request: 534d4100000402a00000000100260010 606509a0 ffffffffffff0000 7d0052be283a0000 000000000180 00020000 00000000 00000000 00000000  => command = 0x00000200, first = 0x00000000; last = 0x00000000; trailer = 0x00000000
    // Response 534d4100000402a000000001004e0010 606513a0 7d0052be283a00c0 7a01842a71b30000 000000000180 01020000 00000000 00000000 00030000 00ff0000 00000000 01007a01 842a71b3 00000a00 0c000000 00000000 00000000 01010000 00000000

    m_multicastSearchRequestTimer.setInterval(1000);
    m_multicastSearchRequestTimer.setSingleShot(false);

    connect(m_speedwireInterface, &SpeedwireInterface::dataReceived, this, &SpeedwireDiscovery::processDatagram);
    connect(&m_multicastSearchRequestTimer, &QTimer::timeout, this, &SpeedwireDiscovery::sendDiscoveryRequest);
}

SpeedwireDiscovery::~SpeedwireDiscovery()
{

}

bool SpeedwireDiscovery::startDiscovery()
{
    if (discoveryRunning())
        return true;

    if (!m_speedwireInterface->available()) {
        qCDebug(dcSma()) << "SpeedwireDiscovery: Failed to start discovery because the speedwire interface is not available.";
        return false;
    }

    // Start clean
    m_results.clear();
    m_networkDeviceInfos.clear();

    startUnicastDiscovery();
    startMulticastDiscovery();
    return true;
}

bool SpeedwireDiscovery::discoveryRunning() const
{
    return m_unicastRunning || m_multicastRunning;
}

QList<SpeedwireDiscovery::SpeedwireDiscoveryResult> SpeedwireDiscovery::discoveryResult() const
{
    return m_results;
}

void SpeedwireDiscovery::startMulticastDiscovery()
{
    qCDebug(dcSma()) << "SpeedwireDiscovery: Start multicast discovery...";
    m_multicastRunning = true;
    m_multicastSearchRequestTimer.start();

    // Start sending multicast messages periodically
    sendDiscoveryRequest();
}


void SpeedwireDiscovery::startUnicastDiscovery()
{
    qCDebug(dcSma()) << "SpeedwireDiscovery: Start discovering network...";
    m_unicastRunning = true;

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::networkDeviceInfoAdded, this, [this](const NetworkDeviceInfo &networkDeviceInfo){
        m_networkDeviceInfos.append(networkDeviceInfo);
        sendUnicastDiscoveryRequest(networkDeviceInfo.address());
    });

    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        qCDebug(dcSma()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "network devices for unicast requests.";
        // Wait some extra second in otder to give the last hosts joined some time to respond.
        QTimer::singleShot(3000, this, [this](){
            m_multicastSearchRequestTimer.stop();
            m_multicastRunning = false;

            m_unicastRunning = false;
            evaluateDiscoveryFinished();
        });
    });
}

void SpeedwireDiscovery::sendUnicastDiscoveryRequest(const QHostAddress &targetHostAddress)
{
    qCDebug(dcSma()) << "SpeedwireDiscovery: Sent discovery request to unicast address" << targetHostAddress.toString();
    m_speedwireInterface->sendDataUnicast(targetHostAddress, Speedwire::pingRequest(Speedwire::sourceModelId(), m_localSerialNumber));
}

void SpeedwireDiscovery::processDatagram(const QHostAddress &senderAddress, quint16 senderPort, const QByteArray &datagram, bool multicast)
{
    Q_UNUSED(multicast)

    // Check min size of SMA datagrams
    if (datagram.size() < 18) {
        qCDebug(dcSma()) << "SpeedwireDiscovery: Received datagram is to short to be a SMA speedwire message. Ignoring data...";
        return;
    }

    QDataStream stream(datagram);
    Speedwire::Header header = Speedwire::parseHeader(stream);
    if (!header.isValid()) {
        qCWarning(dcSma()) << "SpeedwireDiscovery: Datagram header is not valid. Ignoring data...";
        return;
    }

    qCDebug(dcSma()) << "SpeedwireDiscovery:" << header;

    if (header.protocolId == Speedwire::ProtocolIdDiscoveryResponse) {
        qCDebug(dcSma()) << "SpeedwireDiscovery: Received discovery response from" << QString("%1:%2").arg(senderAddress.toString()).arg(senderPort);

        // "534d4100 0004 02a0 0000 0001 0002 0000 0001 0004 0010 0001 0003 0004 0020 0000 0001 0004 0030 c0a8 b219 0004 0040 0000 0000 0002 0070 ef0c 00000000"
        // "534d4100 0004 02a0 0000 0001 0002 0000 0001 0004 0010 0001 0001 0004 0020 0000 0001 0004 0030 c0a8 b216 0004 0040 0000 0001 00000000"

        if (!datagram.startsWith(Speedwire::discoveryResponseDatagram())) {
            qCWarning(dcSma()) << "SpeedwireDiscovery: Received discovery reply but the message start does not match the required schema. Ignoring data...";
            return;
        }

        qCDebug(dcSma()) << "SpeedwireDiscovery: --> Found SMA device on" << senderAddress.toString();
        return;
    }

    // We received SMA data, let's parse depending on the protocol id

    if (header.protocolId == Speedwire::ProtocolIdMeter) {
        // Example: 010e 714369ae
        quint16 modelId;
        quint32 serialNumber;
        stream >> modelId >> serialNumber;
        qCDebug(dcSma()) << "SpeedwireDiscovery: Meter identifier: Model ID:" << modelId << "Serial number:" << serialNumber;

        if (!m_resultMeters.contains(senderAddress)) {
            SpeedwireDiscoveryResult result;
            result.address = senderAddress;
            result.deviceType = Speedwire::DeviceTypeMeter;
            m_resultMeters.insert(senderAddress, result);
        }

        if (m_networkDeviceInfos.hasHostAddress(senderAddress)) {
            m_resultMeters[senderAddress].networkDeviceInfo = m_networkDeviceInfos.get(senderAddress);
        }

        m_resultMeters[senderAddress].modelId = modelId;
        m_resultMeters[senderAddress].serialNumber = serialNumber;
    } else if (header.protocolId == Speedwire::ProtocolIdInverter) {
        Speedwire::InverterPacket inverterPacket = Speedwire::parseInverterPacket(stream);
        // Response from inverter 534d4100 0004 02a0 0000 0001 004e 0010 6065 1390 7d00 52be283a 0000 b500 c2c12e12 0000 0000 00000 1800102000000000000000000000003000000ff0000ecd5ff1f0100b500c2c12e1200000a000c00000000000000030000000101000000000000
        qCDebug(dcSma()) << "SpeedwireDiscovery:" << inverterPacket;

        if (!m_resultInverters.contains(senderAddress)) {
            SpeedwireDiscoveryResult result;
            result.address = senderAddress;
            result.deviceType = Speedwire::DeviceTypeInverter;
            m_resultInverters.insert(senderAddress, result);
        }

        if (m_networkDeviceInfos.hasHostAddress(senderAddress)) {
            m_resultInverters[senderAddress].networkDeviceInfo = m_networkDeviceInfos.get(senderAddress);
        }

        m_resultInverters[senderAddress].modelId = inverterPacket.sourceModelId;
        m_resultInverters[senderAddress].serialNumber = inverterPacket.sourceSerialNumber;

        // Send the default login request, maybe it activates the Energy meter measurment data streaming

        SpeedwireInverter *inverter = nullptr;
        if (m_inverters.contains(senderAddress)) {
            inverter = m_inverters.value(senderAddress);
        } else {
            inverter = new SpeedwireInverter(m_speedwireInterface, senderAddress, Speedwire::sourceModelId(), m_localSerialNumber, this);
            m_inverters.insert(senderAddress, inverter);
        }

        SpeedwireInverterReply *reply = inverter->sendIdentifyRequest();
        qCDebug(dcSma()) << "SpeedwireDiscovery: Send identify request to" << senderAddress.toString();
        connect(reply, &SpeedwireInverterReply::finished, this, [/*this, inverter,*/ senderAddress, reply](){
            qCDebug(dcSma()) << "SpeedwireDiscovery: Identify request finished from" << senderAddress.toString() << reply->error();

//            SpeedwireInverterReply *loginReply = inverter->sendLoginRequest();
//            qCDebug(dcSma()) << "SpeedwireDiscovery: make login attempt using the default password.";
//            connect(loginReply, &SpeedwireInverterReply::finished, this, [loginReply, senderAddress](){
//                qCDebug(dcSma()) << "SpeedwireDiscovery: login attempt finished" << senderAddress.toString() << loginReply->error();
//            });
        });

    } else {
        qCWarning(dcSma()) << "SpeedwireDiscovery: Unhandled data received" << datagram.toHex();
        return;
    }
}

void SpeedwireDiscovery::sendDiscoveryRequest()
{
    qCDebug(dcSma()) << "SpeedwireDiscovery: Sent discovery request to multicast address" << Speedwire::multicastAddress().toString();
    m_speedwireInterface->sendDataMulticast(Speedwire::discoveryDatagramMulticast());
}

void SpeedwireDiscovery::evaluateDiscoveryFinished()
{
    if (!m_multicastRunning && !m_unicastRunning) {
        finishDiscovery();
    }
}

void SpeedwireDiscovery::finishDiscovery()
{
    m_results = m_resultMeters.values() + m_resultInverters.values();

    qCDebug(dcSma()) << "SpeedwireDiscovery: Discovey finished. Found" << m_results.count() << "SMA devices in the network";
    m_multicastSearchRequestTimer.stop();

    foreach (const SpeedwireDiscoveryResult &result, m_results) {
        qCDebug(dcSma()) << "SpeedwireDiscovery: ============================================";
        qCDebug(dcSma()) << "SpeedwireDiscovery: Device type:" << result.deviceType;
        qCDebug(dcSma()) << "SpeedwireDiscovery: Address:" << result.address.toString();
        if (result.networkDeviceInfo.isValid()) {
            qCDebug(dcSma()) << "SpeedwireDiscovery: Hostname:" << result.networkDeviceInfo.hostName();
            qCDebug(dcSma()) << "SpeedwireDiscovery: MAC:" << result.networkDeviceInfo.macAddress();
            qCDebug(dcSma()) << "SpeedwireDiscovery: MAC manufacturer:" << result.networkDeviceInfo.macAddressManufacturer();
        }
        qCDebug(dcSma()) << "SpeedwireDiscovery: Model ID:" << result.modelId;
        qCDebug(dcSma()) << "SpeedwireDiscovery: Serial number:" << result.serialNumber;
    }

    emit discoveryFinished();
}

