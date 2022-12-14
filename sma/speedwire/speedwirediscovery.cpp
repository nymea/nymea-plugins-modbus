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

#include <QDataStream>

SpeedwireDiscovery::SpeedwireDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent) :
    QObject(parent),
    m_networkDeviceDiscovery(networkDeviceDiscovery)
{
    // More details: https://github.com/RalfOGit/libspeedwire/

    // Request: 534d4100000402a00000000100260010 606509a0 ffffffffffff0000 7d0052be283a0000 000000000180 00020000 00000000 00000000 00000000  => command = 0x00000200, first = 0x00000000; last = 0x00000000; trailer = 0x00000000
    // Response 534d4100000402a000000001004e0010 606513a0 7d0052be283a00c0 7a01842a71b30000 000000000180 01020000 00000000 00000000 00030000 00ff0000 00000000 01007a01 842a71b3 00000a00 0c000000 00000000 00000000 01010000 00000000

    m_multicastSearchRequestTimer.setInterval(1000);
    m_multicastSearchRequestTimer.setSingleShot(false);
    connect(&m_multicastSearchRequestTimer, &QTimer::timeout, this, &SpeedwireDiscovery::sendDiscoveryRequest);
}

SpeedwireDiscovery::~SpeedwireDiscovery()
{
    if (m_initialized && m_multicastSocket) {
        if (!m_multicastSocket->leaveMulticastGroup(m_multicastAddress)) {
            qCWarning(dcSma()) << "SpeedwireDiscovery: Failed to leave multicast group" << m_multicastAddress.toString();
        }

        m_multicastSocket->close();
    }
}

bool SpeedwireDiscovery::initialize(SpeedwireInterface::DeviceType deviceType)
{
    m_deviceType = deviceType;
    switch(deviceType) {
    case SpeedwireInterface::DeviceTypeMeter:
        m_initialized = setupMulticastSocket();
        break;
    case SpeedwireInterface::DeviceTypeInverter:
        m_initialized = setupUnicastSocket();
        break;
    default:
        m_initialized = setupMulticastSocket() && setupUnicastSocket();
        break;
    }

    if (m_initialized)
        qCDebug(dcSma()) << "SpeedwireDiscovery: Interfaces initialized successfully.";

    return m_initialized;
}

bool SpeedwireDiscovery::initialized() const
{
    return m_initialized;
}

bool SpeedwireDiscovery::startDiscovery()
{
    if (discoveryRunning())
        return true;

    if (!m_initialized) {
        qCDebug(dcSma()) << "SpeedwireDiscovery: Failed to start discovery because the socket has not been initialized successfully.";
        return false;
    }

    // Start clean
    m_results.clear();
    m_networkDeviceInfos.clear();
    m_multicastRunning = false;
    m_unicastRunning = false;

    switch(m_deviceType) {
    case SpeedwireInterface::DeviceTypeMeter:
        startMulticastDiscovery();
        break;
    case SpeedwireInterface::DeviceTypeInverter:
        startUnicastDiscovery();
        break;
    default:
        startUnicastDiscovery();
        startMulticastDiscovery();
        break;
    }

    return true;
}

bool SpeedwireDiscovery::discoveryRunning() const
{
    return m_unicastRunning || m_multicastRunning;
}

QList<SpeedwireDiscovery::SpeedwireDiscoveryResult> SpeedwireDiscovery::discoveryResult() const
{
    return m_results.values();
}

bool SpeedwireDiscovery::setupMulticastSocket()
{
    if (m_multicastSocket)
        return true;

    m_multicastSocket = new QUdpSocket(this);
    connect(m_multicastSocket, &QUdpSocket::readyRead, this, &SpeedwireDiscovery::readPendingDatagramsMulticast);
    connect(m_multicastSocket, &QUdpSocket::stateChanged, this, &SpeedwireDiscovery::onSocketStateChanged);
    connect(m_multicastSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    // Setup multicast socket
    if (!m_multicastSocket->bind(QHostAddress::AnyIPv4, m_port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint)) {
        qCWarning(dcSma()) << "SpeedwireDiscovery: Initialization failed. Could not bind multicast socket to port" << m_port << m_multicastSocket->errorString();
        m_multicastSocket->deleteLater();
        m_multicastSocket = nullptr;
        return false;
    }

    if (!m_multicastSocket->joinMulticastGroup(m_multicastAddress)) {
        qCWarning(dcSma()) << "SpeedwireDiscovery: Initialization failed. Could not join multicast group" << m_multicastAddress.toString() << m_multicastSocket->errorString();
        m_multicastSocket->deleteLater();
        m_multicastSocket = nullptr;
        return false;
    }

    return true;
}

void SpeedwireDiscovery::startMulticastDiscovery()
{
    qCDebug(dcSma()) << "SpeedwireDiscovery: Start multicast discovery...";
    m_multicastRunning = true;

    // Start sending multicast messages
    sendDiscoveryRequest();

    // Give 5 seconds time, on one of the requests sent in this period the meter would have responded...
    QTimer::singleShot(5000, this, [this](){
        m_multicastRunning = false;
        evaluateDiscoveryFinished();
    });

    m_multicastSearchRequestTimer.start();
}

bool SpeedwireDiscovery::setupUnicastSocket()
{
    if (m_unicastSocket)
        return true;

    m_unicastSocket = new QUdpSocket(this);
    connect(m_unicastSocket, &QUdpSocket::readyRead, this, &SpeedwireDiscovery::readPendingDatagramsUnicast);
    connect(m_unicastSocket, &QUdpSocket::stateChanged, this, &SpeedwireDiscovery::onSocketStateChanged);
    connect(m_unicastSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    // Setup unicast socket
    if (!m_unicastSocket->bind(QHostAddress::AnyIPv4, m_port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint)) {
        qCWarning(dcSma()) << "SpeedwireDiscovery: Initialization failed. Could not bind to port" << m_port << m_multicastSocket->errorString();
        m_unicastSocket->deleteLater();
        m_unicastSocket = nullptr;
        return false;
    }

    return true;
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
            m_unicastRunning = false;
            evaluateDiscoveryFinished();
        });
    });
}

void SpeedwireDiscovery::sendUnicastDiscoveryRequest(const QHostAddress &targetHostAddress)
{
    if (m_unicastSocket->writeDatagram(Speedwire::discoveryDatagramUnicast(), targetHostAddress, m_port) < 0) {
        qCWarning(dcSma()) << "SpeedwireDiscovery: Failed to send unicast discovery datagram to address" << targetHostAddress.toString();
        return;
    }

    qCDebug(dcSma()) << "SpeedwireDiscovery: Sent successfully the discovery request to unicast address" << targetHostAddress.toString();
}

void SpeedwireDiscovery::readPendingDatagramsMulticast()
{
    QUdpSocket *socket = qobject_cast<QUdpSocket *>(sender());

    QByteArray datagram;
    QHostAddress senderAddress;
    quint16 senderPort;

    while (socket->hasPendingDatagrams()) {
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
        qCDebug(dcSma()) << "SpeedwireDiscovery: Received multicast data from" << QString("%1:%2").arg(senderAddress.toString()).arg(senderPort);
        //qCDebug(dcSma()) << "SpeedwireDiscovery: " << datagram.toHex();
        processDatagram(senderAddress, senderPort, datagram);
    }
}

void SpeedwireDiscovery::readPendingDatagramsUnicast()
{
    QUdpSocket *socket = qobject_cast<QUdpSocket *>(sender());

    QByteArray datagram;
    QHostAddress senderAddress;
    quint16 senderPort;

    while (socket->hasPendingDatagrams()) {
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
        qCDebug(dcSma()) << "SpeedwireDiscovery: Received unicast data from" << QString("%1:%2").arg(senderAddress.toString()).arg(senderPort);
        //qCDebug(dcSma()) << "SpeedwireDiscovery: " << datagram.toHex();
        processDatagram(senderAddress, senderPort, datagram);
    }
}

void SpeedwireDiscovery::onSocketError(QAbstractSocket::SocketError error)
{
    qCDebug(dcSma()) << "SpeedwireDiscovery: Socket error" << error;
}

void SpeedwireDiscovery::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    qCDebug(dcSma()) << "SpeedwireDiscovery: Socket state changed" << socketState;
}

void SpeedwireDiscovery::processDatagram(const QHostAddress &senderAddress, quint16 senderPort, const QByteArray &datagram)
{
    // Check min size of SMA datagrams
    if (datagram.size() < 18) {
        qCDebug(dcSma()) << "SpeedwireDiscovery: Received datagram is to short to be a SMA speedwire message. Ignoring data...";
        return;
    }

    // Ignore discovery requests
    if (datagram == Speedwire::discoveryDatagramMulticast() || datagram == Speedwire::discoveryDatagramUnicast())
        return;

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

        if (!m_results.contains(senderAddress)) {
            qCDebug(dcSma()) << "SpeedwireDiscovery: --> Found SMA device on" << senderAddress.toString();
            if (!m_networkDeviceInfos.hasHostAddress(senderAddress)) {
                qCDebug(dcSma()) << "SpeedwireDiscovery: Found SMA using UDP discovery but the host is not in the network discovery result list. Not adding to results" << senderAddress.toString();
            }

            SpeedwireDiscoveryResult result;
            result.address = senderAddress;
            if (m_networkDeviceInfos.hasHostAddress(senderAddress)) {
                result.networkDeviceInfo = m_networkDeviceInfos.get(senderAddress);
            }
            result.deviceType = SpeedwireInterface::DeviceTypeUnknown;
            m_results.insert(senderAddress, result);
        } else {
            if (m_networkDeviceInfos.hasHostAddress(senderAddress)) {
                m_results[senderAddress].networkDeviceInfo = m_networkDeviceInfos.get(senderAddress);
            }
        }
        return;
    }

    // We received SMA data, let's parse depending on the protocol id

    if (header.protocolId == Speedwire::ProtocolIdMeter) {
        // Example: 010e 714369ae
        quint16 modelId;
        quint32 serialNumber;
        stream >> modelId >> serialNumber;
        qCDebug(dcSma()) << "SpeedwireDiscovery: Meter identifier: Model ID:" << modelId << "Serial number:" << serialNumber;

        if (!m_results.contains(senderAddress)) {
            SpeedwireDiscoveryResult result;
            result.address = senderAddress;
            result.deviceType = SpeedwireInterface::DeviceTypeMeter;
            m_results.insert(senderAddress, result);
        }

        if (m_networkDeviceInfos.hasHostAddress(senderAddress)) {
            m_results[senderAddress].networkDeviceInfo = m_networkDeviceInfos.get(senderAddress);
        }

        m_results[senderAddress].modelId = modelId;
        m_results[senderAddress].serialNumber = serialNumber;
    } else if (header.protocolId == Speedwire::ProtocolIdInverter) {
        Speedwire::InverterPacket inverterPacket = Speedwire::parseInverterPacket(stream);
        // Response from inverter 534d4100 0004 02a0 0000 0001 004e 0010 6065 1390 7d00 52be283a 0000 b500 c2c12e12 0000 0000 00000 1800102000000000000000000000003000000ff0000ecd5ff1f0100b500c2c12e1200000a000c00000000000000030000000101000000000000
        qCDebug(dcSma()) << "SpeedwireDiscovery:" << inverterPacket;

        if (!m_results.contains(senderAddress)) {
            SpeedwireDiscoveryResult result;
            result.address = senderAddress;
            result.deviceType = SpeedwireInterface::DeviceTypeInverter;
            m_results.insert(senderAddress, result);
        }

        if (m_networkDeviceInfos.hasHostAddress(senderAddress)) {
            m_results[senderAddress].networkDeviceInfo = m_networkDeviceInfos.get(senderAddress);
        }

        m_results[senderAddress].modelId = inverterPacket.sourceModelId;
        m_results[senderAddress].serialNumber = inverterPacket.sourceSerialNumber;
    } else {
        qCWarning(dcSma()) << "SpeedwireDiscovery: Unhandled data received" << datagram.toHex();
        return;
    }
}

void SpeedwireDiscovery::sendDiscoveryRequest()
{
    if (m_multicastSocket->writeDatagram(Speedwire::discoveryDatagramMulticast(), m_multicastAddress, m_port) < 0) {
        qCWarning(dcSma()) << "SpeedwireDiscovery: Failed to send discovery datagram to multicast address" << m_multicastAddress.toString();
        return;
    }

    qCDebug(dcSma()) << "SpeedwireDiscovery: Sent successfully the discovery request to multicast address" << m_multicastAddress.toString();
}

void SpeedwireDiscovery::evaluateDiscoveryFinished()
{
    if (!m_multicastRunning && !m_unicastRunning) {
        finishDiscovery();
    }
}

void SpeedwireDiscovery::finishDiscovery()
{
    qCDebug(dcSma()) << "SpeedwireDiscovery: Discovey finished. Found" << m_results.count() << "SMA devices in the network";
    m_multicastSearchRequestTimer.stop();

    if (m_multicastSocket) {
        m_multicastSocket->close();
        m_multicastSocket->deleteLater();
        m_multicastSocket = nullptr;
    }

    if (m_unicastSocket) {
        m_unicastSocket->close();
        m_unicastSocket->deleteLater();
        m_unicastSocket = nullptr;
    }

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

