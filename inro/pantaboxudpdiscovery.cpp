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

#include "pantaboxudpdiscovery.h"
#include "extern-plugininfo.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkDatagram>
#include "crclookuptable.h"

#define PANTABOX_DISCOVERY_PORT 52001

PantaboxUdpDiscovery::PantaboxUdpDiscovery(QObject *parent)
    : QObject{parent}
{
    m_socket = new QUdpSocket(this);
    if (!m_socket->bind(QHostAddress::Broadcast, PANTABOX_DISCOVERY_PORT, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint)) {
        qCWarning(dcInro()) << "UdpDiscovery: Failed to bind to UDP broadcast on" << PANTABOX_DISCOVERY_PORT << m_socket->errorString();
        return;
    }

    connect(m_socket, &QUdpSocket::readyRead, this, &PantaboxUdpDiscovery::readPendingDatagrams);
    m_available = true;
}

bool PantaboxUdpDiscovery::available() const
{
    return m_available;
}

QHash<QString, PantaboxUdpDiscovery::DeviceInfo> PantaboxUdpDiscovery::results() const
{
    return m_results;
}

void PantaboxUdpDiscovery::readPendingDatagrams()
{
    while(m_socket->hasPendingDatagrams()) {

        QNetworkDatagram datagram = m_socket->receiveDatagram();

        for (int i = 0; i < datagram.data().length(); i++) {

            quint8 dataByte = static_cast<quint8>(datagram.data().at(i));

            if (!m_prefixStartDiscovered[datagram.senderAddress()] && dataByte == 0xe5) {
                m_prefixStartDiscovered[datagram.senderAddress()] = true;
                continue;
            }

            if (m_prefixStartDiscovered[datagram.senderAddress()] && dataByte == 0x00) {
                m_prefixStartDiscovered[datagram.senderAddress()] = false;

                // Paket prefix discovered (0xe5 0x00), process current buffer and start collecting data
                processDataBuffer(datagram.senderAddress());
                m_buffers[datagram.senderAddress()].clear();
                continue;
            } else {
                m_prefixStartDiscovered[datagram.senderAddress()] = false;
            }

            // Adding data byte
            m_buffers[datagram.senderAddress()].append(dataByte);

            if (m_buffers[datagram.senderAddress()].length() >= 0xffff) {
                qCWarning(dcInro()) << "UdpDiscovery: Buffer overflow. Wipe data buffer...";
                m_buffers[datagram.senderAddress()].clear();
            }
        }
    }
}

quint8 PantaboxUdpDiscovery::calculateCrc8(const QByteArray &data)
{
    // CRC-8/NRSC-5 initial value
    quint8 crc = 0xFF;

    for (quint8 byte : data) {
        crc = crc8LookupTable[crc ^ byte];
    }

    return crc;
}

void PantaboxUdpDiscovery::processDataBuffer(const QHostAddress &address)
{
    if (m_buffers[address].length() < 3)
        return;

    quint8 receivedCrc = static_cast<quint8>(m_buffers[address].at(m_buffers[address].length() - 1));
    quint8 calculatedCrc = calculateCrc8(QByteArray::fromHex("e500").append(m_buffers[address].left(m_buffers[address].length() - 1)));

    if (calculatedCrc != receivedCrc) {
        qCDebug(dcInro()) << "UdpDiscovery: Crc checksum not correct. Received crc "<< receivedCrc <<", calculated crc " << calculatedCrc ;
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(m_buffers[address].mid(2, m_buffers[address].length() - 3), &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        qCDebug(dcInro()) << "UdpDiscovery: Received invalud json data" << jsonError.errorString();
        return;
    }

    //qCDebug(dcInro()) << "UdpDiscovery:" << qUtf8Printable(jsonDoc.toJson(QJsonDocument::Indented));

    /*
         {
            "deviceId": "e45749d4-8c05-44b2-9dbc-xxxxxxxxxxxx",
            "encryptBLE": 1,
            "env": "live",
            "fwVersion": "V1.19.6",
            "ipAddress": "10.10.10.111",
            "macAddress": "8C:4B:14:88:05:00",
            "name": "#1XXXXXXX",
            "productKey": "inro-test-1",
            "serialNumber": "#1XXXXXXX",
            "useTLS": 1
        }
    */

    QVariantMap dataMap = jsonDoc.toVariant().toMap();
    if (dataMap.contains("serialNumber") && dataMap.contains("ipAddress") && dataMap.contains("macAddress")) {
        DeviceInfo pantabox;
        pantabox.serialNumber = dataMap.value("serialNumber").toString().remove("#");
        pantabox.macAddress = MacAddress(dataMap.value("macAddress").toString());
        pantabox.ipAddress = QHostAddress(dataMap.value("ipAddress").toString());

        if (address != pantabox.ipAddress) {
            qCDebug(dcInro()) << "UdpDiscovery: Received UPD discovery paket from a different IP than communicated in the paket. Ignoring paket.";
            return;
        }

        qCDebug(dcInro()) << "UdpDiscovery: --> Received discovery paket from" << pantabox.serialNumber << pantabox.macAddress.toString() << pantabox.ipAddress.toString();
        m_results[pantabox.serialNumber] = pantabox;
        emit pantaboxDiscovered(pantabox);
    }
}
