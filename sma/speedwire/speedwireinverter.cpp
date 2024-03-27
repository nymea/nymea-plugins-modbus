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

#include "speedwireinverter.h"
#include "extern-plugininfo.h"

#include <QDateTime>

SpeedwireInverter::SpeedwireInverter(SpeedwireInterface *speedwireInterface, const QHostAddress &address, quint16 modelId, quint32 serialNumber, QObject *parent) :
    QObject(parent),
    m_speedwireInterface(speedwireInterface),
    m_address(address),
    m_modelId(modelId),
    m_serialNumber(serialNumber)
{
    qCDebug(dcSma()) << "Inverter: setup interface on" << m_address.toString();
    connect(m_speedwireInterface, &SpeedwireInterface::dataReceived, this, &SpeedwireInverter::processData);
}

SpeedwireInverter::State SpeedwireInverter::state() const
{
    return m_state;
}

bool SpeedwireInverter::reachable() const
{
    return m_reachable;
}

QString SpeedwireInverter::modelName() const
{
    return m_modelName;
}

double SpeedwireInverter::totalAcPower() const
{
    return m_totalAcPower;
}

double SpeedwireInverter::gridFrequency() const
{
    return m_gridFrequency;
}

double SpeedwireInverter::totalEnergyProduced() const
{
    return m_totalEnergyProduced;
}

double SpeedwireInverter::todayEnergyProduced() const
{
    return m_todayEnergyProduced;
}

double SpeedwireInverter::voltageAcPhase1() const
{
    return m_voltageAcPhase1;
}

double SpeedwireInverter::voltageAcPhase2() const
{
    return m_voltageAcPhase2;
}

double SpeedwireInverter::voltageAcPhase3() const
{
    return m_voltageAcPhase3;
}

double SpeedwireInverter::currentAcPhase1() const
{
    return m_currentAcPhase1;
}

double SpeedwireInverter::currentAcPhase2() const
{
    return m_currentAcPhase2;
}

double SpeedwireInverter::currentAcPhase3() const
{
    return m_currentAcPhase3;
}

double SpeedwireInverter::powerAcPhase1() const
{
    return m_powerAcPhase1;
}

double SpeedwireInverter::powerAcPhase2() const
{
    return m_powerAcPhase2;
}

double SpeedwireInverter::powerAcPhase3() const
{
    return m_powerAcPhase3;
}

double SpeedwireInverter::powerDcMpp1() const
{
    return m_powerDcMpp1;
}

double SpeedwireInverter::powerDcMpp2() const
{
    return m_powerDcMpp2;
}

bool SpeedwireInverter::batteryAvailable() const
{
    return m_batteryAvailable;
}

double SpeedwireInverter::batteryCycles() const
{
    return m_batteryCycles;
}

double SpeedwireInverter::batteryCharge() const
{
    return m_batteryCharge;
}

double SpeedwireInverter::batteryTemperature() const
{
    return m_batteryTemperature;
}

double SpeedwireInverter::batteryCurrent() const
{
    return m_batteryCurrent;
}

double SpeedwireInverter::batteryVoltage() const
{
    return m_batteryVoltage;
}

SpeedwireInverterReply *SpeedwireInverter::sendIdentifyRequest()
{
    // Request  534d4100000402a000000001002600106065 09 a0 ffff ffffffff 0000 7d00 52be283a 0000 0000 0000 0180 00020000 000000000000000000000000
    // Response 534d4100000402a000000001004e00106065 13 90 7d00 52be283a 0000 b500 c2c12e12 0000 0000 0000 0180 01020000 00000000000000000003000000ff0000ecd5ff1f0100b500c2c12e1200000a000c00000000000000030000000101000000000000

    qCDebug(dcSma()) << "Inverter: Sending identify request to" << m_address.toString();
    SpeedwireInverterRequest request;
    request.setPacketId(0x8001);
    request.setCommand(Speedwire::CommandIdentify);
    request.setRequestData(Speedwire::pingRequest(Speedwire::sourceModelId(), m_serialNumber));
    return createReply(request);
}

SpeedwireInverterReply *SpeedwireInverter::sendLoginRequest(const QString &password, bool loginAsUser)
{
    qCDebug(dcSma()) << "Inverter: Sending login request as" << (loginAsUser ? "user" : "installer") << "using password" << password;

    // Request:  534d4100000402a000000001003a001060650ea0 b500 c2c12e12 0001 7d00 52be283a 0001 0000 0000 0180 0c04fdff0 7000000 84030000 3408b261 00000000 b8b8b8b8888888888888888800000000 // Login request

    // Response: 534d4100000402a000000001003a001060650ed0 7d00 52be283a 0001 b500 c2c12e12 0001 0000000001800 d04fdff0 7000000 84030000 3408b261 00000000 b8b8b8b8888888888888888800000000 // Login OK
    // Response: 534d4100000402a000000001003a001060650ed0 7d00 52be283a 0001 b500 c2c12e12 0001 0001000001800 d04fdff0 7000000 84030000 b709b261 00000000 b8b8b8b9888888888888888800000000 // Login FAILED, error 1

    // Build the header
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    buildDefaultHeader(stream, 58, 0xa0);

    quint16 packetId = static_cast<quint16>(m_packetId++) | 0x8000;
    Speedwire::Command command = Speedwire::CommandLogin;

    // The payload is little endian encoded
    buildPacket(stream, command, packetId);

    // User type: 7 = user, a = installer
    stream << (loginAsUser ? static_cast<quint32>(0x00000007) : static_cast<quint32>(0x0000000a));
    // Timeout
    stream << static_cast<quint32>(900); // 900 ms
    // Current time
    stream << static_cast<quint32>(QDateTime::currentMSecsSinceEpoch() / 1000.0);
    // Zeros
    stream << static_cast<quint32>(0);

    // Encode password
    QByteArray passwordData = password.toUtf8();
    QByteArray encodedPassword(12, loginAsUser ? 0x88 : 0xBB);
    for (int i = 0; i < password.count(); i++) {
        encodedPassword[i] = (passwordData.at(i) + (loginAsUser ? 0x88 : 0xBB) % 0xff);
    }

    // Add encoded password
    for (int i = 0; i < encodedPassword.count(); i++) {
        stream << static_cast<quint8>(encodedPassword.at(i));
    }

    // End of data
    stream << static_cast<quint32>(0);

    // Final datagram
    SpeedwireInverterRequest request;
    request.setPacketId(packetId);
    request.setCommand(command);
    request.setRequestData(datagram);
    return createReply(request);
}

SpeedwireInverterReply *SpeedwireInverter::sendLogoutRequest()
{
    // Request 534d4100000402a000000001 0022 0010 6065 08a0 ffffffffffff0003 7d0052be283a0003 00000000 0280 0e01fdff ffffffff 00000000   => logoff command = 0xfffd01e0

    //         534d4100000402a000000001 0022 0010 6065 08a0 ffffffffffff0003 7d0052be283a0003 00000000 0480 0e01fdff ffffffff 00000000

    // Request 534d4100000402a000000001 0022 0010 6065 08a0 ffffffffffff0003 7d0040be283a0003 000000000380 e001fdff ffffffff 00000000
    // Request 534d4100000402a000000001 0022 0010 6065 08a0 ffffffffffff0003 7d0042be283a0003 000000000180 e001fdff ffffffff 00000000
    //         534d4100000402a000000001 0022 0010 6065 08a0 ffffffffffff0003 7d0052be283a0003 000000000080 0e01fdff ffffffff 00000000

    // Build the header
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    buildDefaultHeader(stream, 34);

    // Reset the packet id counter, otherwise there will be no response
    quint16 packetId = static_cast<quint16>(m_packetId++) | 0x8000;
    Speedwire::Command command = Speedwire::CommandLogout;

    // The payload is little endian encoded
    stream.setByteOrder(QDataStream::LittleEndian);

    // Target
    //    stream << static_cast<quint16>(m_modelId);
    //    stream << static_cast<quint32>(m_serialNumber);
    stream << static_cast<quint16>(0xffff);
    stream << static_cast<quint32>(0xffffffff);
    stream << static_cast<quint16>(0x0300);

    // Source
    stream << Speedwire::sourceModelId();
    stream << m_speedwireInterface->sourceSerialNumber();
    stream << static_cast<quint16>(0x0300);

    stream << static_cast<quint16>(0);
    stream << static_cast<quint16>(0);
    stream << packetId;

    stream << command;
    // Only first word
    stream << static_cast<quint32>(0xffffffff);
    stream << static_cast<quint32>(0);

    SpeedwireInverterRequest request;
    request.setPacketId(packetId);
    request.setCommand(command);
    request.setRequestData(datagram);
    request.setRetries(0);
    return createReply(request);
}

SpeedwireInverterReply *SpeedwireInverter::sendSoftwareVersionRequest()
{
    qCDebug(dcSma()) << "Inverter: Sending software version request to" << m_address.toString();

    // Request  534d4100000402a00000000100260010 6065 09a0 7a01 842a71b3 0001 7d00 42be283a 0001 000000000380 00020058 00348200 ff348200 00000000 =>  query software version
    // Response 534d4100000402a000000001004e0010 6065 13a0 7d00 42be283a 00a1 7a01 842a71b3 0001 000000000380 01020058 0a000000 0a000000 01348200 2ae5e65f 00000000 00000000 feffffff feffffff 040a1003 040a1003 00000000 00000000 00000000  code = 0x00823401    3 (BCD).10 (BCD).10 (BIN) Typ R (Enum)

    // Build the header
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    buildDefaultHeader(stream, 38, 0xa0);

    // Reset the packet id counter, otherwise there will be no response
    quint16 packetId = static_cast<quint16>(m_packetId++) | 0x8000;
    Speedwire::Command command = Speedwire::CommandQueryDevice;

    // The payload is little endian encoded
    buildPacket(stream, command, packetId);

    // First and last word
    stream << static_cast<quint32>(0x00823400);
    stream << static_cast<quint32>(0x008234ff);

    // End of data
    stream << static_cast<quint32>(0);

    // Final datagram
    SpeedwireInverterRequest request;
    request.setPacketId(packetId);
    request.setCommand(command);
    request.setRequestData(datagram);
    return createReply(request);
}

SpeedwireInverterReply *SpeedwireInverter::sendDeviceTypeRequest()
{
    qCDebug(dcSma()) << "Inverter: Sending software version request to" << m_address.toString();
    // Build the header
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    buildDefaultHeader(stream, 38, 0xa0);

    // Reset the packet id counter, otherwise there will be no response
    quint16 packetId = static_cast<quint16>(m_packetId++) | 0x8000;
    Speedwire::Command command = Speedwire::CommandQueryDevice;

    // The payload is little endian encoded
    buildPacket(stream, command, packetId);

    // 2 words
    stream << static_cast<quint32>(0x00821e00);
    stream << static_cast<quint32>(0x008220ff);

    // End of data
    stream << static_cast<quint32>(0);

    // Final datagram
    SpeedwireInverterRequest request;
    request.setPacketId(packetId);
    request.setCommand(command);
    request.setRequestData(datagram);
    return createReply(request);
}

SpeedwireInverterReply *SpeedwireInverter::sendBatteryInfoRequest()
{
    qCDebug(dcSma()) << "Inverter: Sending battery info request to" << m_address.toString();
    // Build the header
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    buildDefaultHeader(stream, 38, 0xa0);

    // Reset the packet id counter, otherwise there will be no response
    quint16 packetId = static_cast<quint16>(m_packetId++) | 0x8000;
    Speedwire::Command command = Speedwire::CommandQueryAc;

    // The payload is little endian encoded
    buildPacket(stream, command, packetId);

    // 2 words
    stream << static_cast<quint32>(0x00491E00);
    stream << static_cast<quint32>(0x00495DFF);

    // End of data
    stream << static_cast<quint32>(0);

    // Final datagram
    SpeedwireInverterRequest request;
    request.setPacketId(packetId);
    request.setCommand(command);
    request.setRequestData(datagram);
    return createReply(request);
}

void SpeedwireInverter::startConnecting(const QString &password)
{
    m_password = password;
    refresh();
}

void SpeedwireInverter::refresh()
{
    // Only refresh if not already busy...
    if (m_state != StateIdle && m_state != StateDisconnected)
        return;

    // Run the state machine
    setState(StateInitializing);
}

void SpeedwireInverter::sendNextReply()
{
    // Pending reply
    if (m_currentReply)
        return;

    // No reply left
    if (m_replyQueue.isEmpty())
        return;

    // Pick the next reply and send request
    m_currentReply = m_replyQueue.dequeue();
    qCDebug(dcSma()) << "Inverter: --> Sending" << m_currentReply->request().command() << "packet ID:" << m_currentReply->request().packetId();
    m_speedwireInterface->sendDataUnicast(m_address, m_currentReply->request().requestData());
    m_currentReply->startWaiting();
}

SpeedwireInverterReply *SpeedwireInverter::createReply(const SpeedwireInverterRequest &request)
{
    SpeedwireInverterReply *reply = new SpeedwireInverterReply(request, this);
    connect(reply, &SpeedwireInverterReply::timeout, this, &SpeedwireInverter::onReplyTimeout);
    connect(reply, &SpeedwireInverterReply::finished, this, &SpeedwireInverter::onReplyFinished);
    // Make sure the reply gets deleted once finished
    connect(reply, &SpeedwireInverterReply::finished, reply, &SpeedwireInverterReply::deleteLater);

    // Schedule request
    m_replyQueue.enqueue(reply);
    sendNextReply();

    return reply;
}

void SpeedwireInverter::buildDefaultHeader(QDataStream &stream, quint16 payloadSize, quint8 control)
{
    // Header (big endian)
    // 534d4100000402a00000000100260010606509a0

    // 534d4100 : SMA\0 signature
    // 0004 : header length
    // 02a0 : Tag0 type
    // 0000 : Tag version
    // 0001 : Group
    // 0026 : payload length
    // 0010 : SMA Net 2 version
    // 6065 : inverter protocol id
    // 09 : length of long words = (payload length - 2) / 4
    // a0 : control ?

    stream.setByteOrder(QDataStream::BigEndian);
    stream << Speedwire::smaSignature();
    stream << static_cast<quint16>(4); // Header length
    stream << Speedwire::tag0();
    stream << Speedwire::tagVersion();
    stream << static_cast<quint16>(1); // Group 1 = default
    stream << payloadSize;
    stream << Speedwire::smaNet2Version();
    stream << static_cast<quint16>(Speedwire::ProtocolIdInverter);
    stream << static_cast<quint8>((payloadSize - 2) / 4); // wordCount
    stream << control;
}

void SpeedwireInverter::buildPacket(QDataStream &stream, quint32 command, quint16 packetId)
{
    // ========= packet header (little endian)
    // == 7a01842a71b30001

    // 7a01 : destination model id
    // 842a71b3 : destination serial number
    // 0001 : destination control field

    // == 7d0042be283a0001

    // 7d00 : source model id
    // 42be283a: source serial number
    // 0001 : source control field

    // == 0000 0000 0380 00020058

    // 0000 : error code
    // 0000 : fragment id
    // 0380 : packet id
    // 00020058 : command id = CommandQueryDevice

    stream.setByteOrder(QDataStream::LittleEndian);
    // Destination
    stream << m_modelId << m_serialNumber;
    // Destination Ctrl
    stream << static_cast<quint16>(0x0100);
    // Source
    stream << Speedwire::sourceModelId() << m_speedwireInterface->sourceSerialNumber();
    // Destination Ctrl
    stream << static_cast<quint16>(0x0100);

    // Packet information
    quint16 errorCode = 0;
    quint16 fragmentId = 0;
    stream << errorCode << fragmentId << packetId;

    // Command
    stream << static_cast<quint32>(command);
}

SpeedwireInverterReply *SpeedwireInverter::sendQueryRequest(Speedwire::Command command, quint32 firstWord, quint32 secondWord)
{
    qCDebug(dcSma()) << "Inverter: Sending query request to" << m_address.toString();

    // Request  534d4100000402a00000000100260010 606509a0 7a01842a71b30001 7d0042be283a0001 000000000380 00020058 00348200 ff348200 00000000 =>  query software version
    // Response 534d4100000402a000000001004e0010 606513a0 7d0042be283a00a1 7a01842a71b30001 000000000380 01020058 0a000000 0a000000 01348200 2ae5e65f 00000000 00000000 feffffff feffffff 040a1003 040a1003 00000000 00000000 00000000  code = 0x00823401    3 (BCD).10 (BCD).10 (BIN) Typ R (Enum)
    // Request  534d4100000402a00000000100260010 606509a0 7a01842a71b30001 7d0042be283a0001 000000000480 00020058 001e8200 ff208200 00000000 =>  query device type
    // Response 534d4100000402a000000001009e0010 606527a0 7d0042be283a00a1 7a01842a71b30001 000000000480 01020058 01000000 03000000 011e8210 6f89e95f 534e3a20 33303130 35333831 31360000 00000000 00000000 00000000 00000000
    //                                                                                                                              011f8208 6f89e95f 411f0001 feffff00 00000000 00000000 00000000 00000000 00000000 00000000  => 1f41 solar inverter
    //                                                                                                                              01208208 6f89e95f 96240000 80240000 81240001 82240000 feffff00 00000000 00000000 00000000 00000000
    // Request  534d4100000402a00000000100260010 606509a0 7a01842a71b30001 7d0042be283a0001 000000000480 00028053 001e2500 ff1e2500 00000000 =>  query spot dc power
    // Response 534d4100000402a000000001005e0010 606517a0 7d0042be283a00a1 7a01842a71b30001 000000000480 01028053 00000000 01000000 011e2540 61a7e95f 57000000 57000000 57000000 57000000 01000000
    //                                                                                                                              021e2540 61a7e95f 5e000000 5e000000 5e000000 5e000000 01000000 00000000
    // Request  534d4100000402a00000000100260010 606509a0 7a01842a71b30001 7d0042be283a0001 000000000580 00028053 001f4500 ff214500 00000000 =>  query spot dc voltage/current
    // Response 534d4100000402a00000000100960010 606525a0 7d0042be283a00a1 7a01842a71b30001 000000000580 01028053 02000000 05000000 011f4540 61a7e95f 05610000 05610000 05610000 05610000 01000000
    //                                                                                                                              021f4540 61a7e95f 505b0000 505b0000 505b0000 505b0000 01000000
    //                                                                                                                              01214540 61a7e95f 60010000 60010000 60010000 60010000 01000000
    //                                                                                                                              02214540 61a7e95f 95010000 95010000 95010000 95010000 01000000 00000000
    // Request  534d4100000402a00000000100260010 606509a0 7a01842a71b30001 7d0042be283a0001 000000000680 00020051 00404600 ff424600 00000000 =>  query spot ac power
    // Response 534d4100000402a000000001007a0010 60651ea0 7d0042be283a00a1 7a01842a71b30001 000000000680 01020051 09000000 0b000000 01404640 61a7e95f 38000000 38000000 38000000 38000000 01000000
    //                                                                                                                              01414640 61a7e95f 37000000 37000000 37000000 37000000 01000000
    //                                                                                                                              01424640 61a7e95f 39000000 39000000 39000000 39000000 01000000 00000000
    // Request  534d4100000402a00000000100260010 606509a0 7a01842a71b30001 7d0042be283a0001 000000000780 00020051 00484600 ff554600 00000000 =>  query spot ac voltage/current
    // Response 534d4100000402a000000001013e0010 60654fa0 7d0042be283a00a1 7a01842a71b30001 000000000780 01020051 0c000000 15000000 01484600 61a7e95f 5a590000 5a590000 5a590000 5a590000 01000000
    //                                                                                                                              01494600 61a7e95f cf590000 cf590000 cf590000 cf590000 01000000
    //                                                                                                                              014a4600 61a7e95f 7a590000 7a590000 7a590000 7a590000 01000000
    //                                                                                                                              014b4600 61a7e95f f19a0000 f19a0000 f19a0000 f19a0000 01000000
    //                                                                                                                              014c4600 61a7e95f 3c9b0000 3c9b0000 3c9b0000 3c9b0000 01000000
    //                                                                                                                              014d4600 61a7e95f 189b0000 189b0000 189b0000 189b0000 01000000
    //                                                                                                                              014e4600 51a7e95f 1d000000 1d000000 1d000000 1d000000 01000000
    //                                                                                                                              01534640 61a7e95f 24010000 24010000 24010000 24010000 01000000
    //                                                                                                                              01544640 61a7e95f 1e010000 1e010000 1e010000 1e010000 01000000
    //                                                                                                                              01554640 61a7e95f 23010000 23010000 23010000 23010000 01000000 00000000
    // Request  534d4100000402a00000000100260010 606509a0 7a01842a71b30001 7d0042be283a0001 000000000980 00028051 00482100 ff482100 00000000 =>  query device status
    // Response 534d4100000402a000000001004e0010 606513a0 7d0042be283a00a1 7a01842a71b30001 000000000980 01028051 00000000 00000000 01482108 59c5e95f 33010001 feffff00 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    // Request  534d4100000402a00000000100260010 606509a0 7a01842a71b30001 7d0042be283a0001 000000000a80 00028051 00644100 ff644100 00000000 =>  query grid relay status
    // Response 534d4100000402a000000001004e0010 606513a0 7d0042be283a00a1 7a01842a71b30001 000000000a80 01028051 07000000 07000000 01644108 59c5e95f 33000001 37010000 fdffff00 feffff00 00000000 00000000 00000000 00000000 00000000


    // Build the header
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    buildDefaultHeader(stream);

    // Reset the packet id counter, otherwise there will be no response
    quint16 packetId = static_cast<quint16>(m_packetId++) | 0x8000;

    // The payload is little endian encoded
    buildPacket(stream, command, packetId);

    // First and second word
    stream << firstWord;
    stream << secondWord;

    // End of data
    stream << static_cast<quint32>(0);

    // Final datagram
    SpeedwireInverterRequest request;
    request.setPacketId(packetId);
    request.setCommand(command);
    request.setRequestData(datagram);
    return createReply(request);
}

void SpeedwireInverter::processSoftwareVersionResponse(const QByteArray &response)
{
    // 07000000 07000000 01348200 2ff5b261 00000000 00000000 feffffff feffffff 00055302 00055302 00000000 00000000 00000000
    qCDebug(dcSma()) << "Inverter: Process software version request response" << response.toHex();
    // TODO:
    //    QDataStream stream(response);
    //    stream.setByteOrder(QDataStream::LittleEndian);

    //    // First
    //    quint32 firstWord;
    //    quint32 secondWord;
    //    stream >> firstWord >> secondWord;
    //    quint8 byte1, byte2, byte3, byte4;
    //    stream >> byte1 >> byte2 >> byte3 >> byte4;

    // BCD
    // 00 82 34 01 ??
    //    QString softwareVersion = QString("%1.%2.%3.%4").arg(byte1).arg(byte2).arg(byte3).arg(byte4);
    //    qCDebug(dcSma()) << "Inverter: Software version" << softwareVersion;

}

void SpeedwireInverter::processDeviceTypeResponse(const QByteArray &response)
{
    // Request  534d4100000402a00000000100260010 606509a0 b500 c2c12e12 0001 7d00 43be283a 0001 0000 0000 0280 0002 0058 001e8200 ff208200 00000000
    // Response 534d4100000402a00000000100c60010 60653190 7d00 43be283a 00a1 b500 c2c12e12 0001 0000 0000 0280 0102 0058 00000000 03000000
    //            011e8210 85f2b661 534e3a20333035303534313436000000 feffff00 00000000 00000000 00000000 // SN: 305054146
    //            011f8208 85f2b661 411f 0001 421f 0000 feff ff00 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 // 411f -> 1f41  -> device type soar inverter
    //            01208208 85f2b661 8a23 0000 8b23 0000 8c23 0001 8e23 0000 8f23 0000 e523 0000 e623 0000 e723 0000 // 238c -> 0001 -> STP 7000TL-20
    //            01208208 85f2b661 e923 0000 e823 0000 4124 0000 4224 0000 4324 0000 feff ff00 0000 0000 0000 0000
    //            00000000

    // Response data: 00000000 03000000 -> first, last, then data
    //                011e8210 3799b961 534e3a20 33303530 35343134 36000000 feffff00 00000000 00000000 00000000             // serialnumber as text
    //                011f8208 3799b961 411f 0001 421f 0000 feff ff00 0000000000000000000000000000000000000000              // device class
    //                01208208 3799b961 8a23 0000 8b23 0000 8c23 0001 8e23 0000 8f23 0000 e523 0000 e623 0000 e723 0000     // device model
    //                01208208 3799b961 e923 0000 e823 0000 4124 0000 4224 0000 4324 0000 feff ff00 0000000000000000        // device model
    //                00000000 // End of data


    qCDebug(dcSma()) << "Inverter: Process device type response" << response.toHex();
    // TODO:

}

void SpeedwireInverter::processAcPowerResponse(const QByteArray &response)
{
    // Request  534d4100000402a00000000100260010606509a0 b500 c2c12e12 0001 7d00 52be283a 0001 0000 0000 0180 00020051 00404600 ff424600 00000000

    // Response 534d4100000402a0000000010026001060650990 7d00 52be283a 00e1 b500 c2c12e12 0001 ffff 0000 0180 01020051 00404600 ff424600 00000000 // Error: login required

    // Response 534d4100000402a000000001007a001060651e90 7d00 52be283a 00a1 b500 c2c12e12 0001 0000 0000 0580 01020051
    // No sun
    // 07000000 09000000
    // 01 4046 40 7503ba61 00000080 00000080 00000080 00000080 01000000
    // 01 4146 40 7503ba61 00000080 00000080 00000080 00000080 01000000
    // 01 4246 40 7503ba61 00000080 00000080 00000080 00000080 01000000 00000000

    // Sun
    // 07000000 09000000
    // 01 4046 40 77fbba61 23000000 23000000 23000000 23000000 01000000
    // 01 4146 40 77fbba61 23000000 23000000 23000000 23000000 01000000
    // 01 4246 40 77fbba61 23000000 23000000 23000000 23000000 01000000
    // 00000000


    // 0a000000 0c000000
    // 09 4046 40 ec767864 be050000 be050000 be050000 be050000 01000000
    // 09 4146 40 ec767864 c6050000 c6050000 c6050000 c6050000 01000000
    // 09 4246 40 ec767864 c8050000 c8050000 c8050000 c8050000 01000000
    // 00000000


    // Note: sometimes the inverter sends negative power values, we filter those away since it does not make sense.

    // D | Sma: Inverter: Process AC power query response "0a0000000c000000094046406740b165a7feffffa7feffffa7feffffa7feffff01000000094146406740b165a7feffffa7feffffa7feffffa7feffff01000000094246406740b165a6feffffa6feffffa6feffffa6feffff0100000000000000"
    // D | Sma: Inverter: Power AC phase 1 4.29497e+06 W
    // D | Sma: Inverter: Power AC phase 2 4.29497e+06 W
    // D | Sma: Inverter: Power AC phase 3 4.29497e+06 W

    // 0a000000 0c000000
    // 09 4046 40 6740b165 a7feffff a7feffff a7feffff a7feffff 01000000
    // 09 4146 40 6740b165 a7feffff a7feffff a7feffff a7feffff 01000000
    // 09 4246 40 6740b165 a6feffff a6feffff a6feffff a6feffff 01000000
    // 00000000

    // 40464001
    qCDebug(dcSma()) << "Inverter: Process AC power query response" << response.toHex();
    QDataStream stream(response);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 firstWord, secondWord;
    stream >> firstWord >> secondWord;

    // Each line has 7 words
    quint32 measurementId;
    quint32 measurementType; // ?

    while (!stream.atEnd()) {
        // First row
        stream >> measurementId;

        // End of data, we are done
        if (measurementId == 0)
            return;

        // Unknown
        stream >> measurementType;

        measurementId = measurementId & 0x00ffff00;

        // Read measurent lines
        if (measurementId == 0x464000) {
            qint32 powerAcPhase1;
            stream >> powerAcPhase1;
            if (powerAcPhase1 >= 0) {
                m_powerAcPhase1 = readValue(powerAcPhase1, 1000.0);
                qCDebug(dcSma()) << "Inverter: Power AC phase 1" << m_powerAcPhase1 << "W";
            } else {
                qCDebug(dcSma()) << "Inverter: Power AC phase 1 is negative, ignoring value" << powerAcPhase1 << "W";
            }
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x464100) {
            qint32 powerAcPhase2;
            stream >> powerAcPhase2;
            if (powerAcPhase2 >= 0) {
                m_powerAcPhase2 = readValue(powerAcPhase2, 1000.0);
                qCDebug(dcSma()) << "Inverter: Power AC phase 2" << m_powerAcPhase2 << "W";
            } else {
                qCDebug(dcSma()) << "Inverter: Power AC phase 2 is negative, ignoring value" << powerAcPhase2 << "W";
            }
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x464200) {
            qint32 powerAcPhase3;
            stream >> powerAcPhase3;
            if (powerAcPhase3 >= 0) {
                m_powerAcPhase3 = readValue(powerAcPhase3, 1000.0);
                qCDebug(dcSma()) << "Inverter: Power AC phase 3" << m_powerAcPhase3 << "W";
            } else {
                qCDebug(dcSma()) << "Inverter: Power AC phase 3 is negative, ignoring value" << powerAcPhase3 << "W";
            }

            readUntilEndOfMeasurement(stream);
        }
    }
}


void SpeedwireInverter::processAcVoltageCurrentResponse(const QByteArray &response)
{
    // No sun
    // 0a000000 0f000000
    // 01 4846 00 7503ba61 ffffffff ffffffff ffffffff ffffffff 01000000
    // 01 4946 00 7503ba61 ffffffff ffffffff ffffffff ffffffff 01000000
    // 01 4a46 00 7503ba61 ffffffff ffffffff ffffffff ffffffff 01000000
    // 01 5046 00 7503ba61 ffffffff ffffffff ffffffff ffffffff 01000000
    // 01 5146 00 7503ba61 ffffffff ffffffff ffffffff ffffffff 01000000
    // 01 5246 00 7503ba61 ffffffff ffffffff ffffffff ffffffff 01000000
    // 00000000

    // Sun
    // 0a000000 0f000000
    // 01484600 c1f0ba61 f9580000 f9580000 f9580000 f9580000 01000000
    // 01494600 c1f0ba61 ff580000 ff580000 ff580000 ff580000 01000000
    // 014a4600 c1f0ba61 02590000 02590000 02590000 02590000 01000000
    // 01504600 c1f0ba61 00000000 00000000 00000000 00000000 01000000
    // 01514600 c1f0ba61 00000000 00000000 00000000 00000000 01000000
    // 01524600 c1f0ba61 00000000 00000000 00000000 00000000 01000000
    // 00000000

    qCDebug(dcSma()) << "Inverter: Process AC voltage/current query response" << response.toHex();
    QDataStream stream(response);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 firstWord, secondWord;
    stream >> firstWord >> secondWord;

    // Each line has 7 words
    quint32 measurementId;
    quint32 measurementType; // ?

    while (!stream.atEnd()) {
        // First row
        stream >> measurementId;

        // End of data, we are done
        if (measurementId == 0)
            return;

        // Unknown
        stream >> measurementType;

        measurementId = measurementId & 0x00ffff00;

        // Read measurent lines
        if (measurementId == 0x464800) {
            quint32 voltageAcPhase1;
            stream >> voltageAcPhase1;
            m_voltageAcPhase1 = readValue(voltageAcPhase1, 100.0);
            qCDebug(dcSma()) << "Inverter: Voltage AC phase 1" << m_voltageAcPhase1 << "V";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x464900) {
            quint32 voltageAcPhase2;
            stream >> voltageAcPhase2;
            m_voltageAcPhase2 = readValue(voltageAcPhase2, 100.0);
            qCDebug(dcSma()) << "Inverter: Voltage AC phase 2" << m_voltageAcPhase2 << "V";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x464a00) {
            quint32 voltageAcPhase3;
            stream >> voltageAcPhase3;
            m_voltageAcPhase3 = readValue(voltageAcPhase3, 100.0);
            qCDebug(dcSma()) << "Inverter: Voltage AC phase 3" << m_voltageAcPhase3 << "V";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x465000) {
            quint32 currentAcPhase1;
            stream >> currentAcPhase1;
            m_currentAcPhase1 = readValue(currentAcPhase1, 1000.0);
            qCDebug(dcSma()) << "Inverter: Current AC phase 1" << m_currentAcPhase1 << "A";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x465100) {
            quint32 currentAcPhase2;
            stream >> currentAcPhase2;
            m_currentAcPhase2 = readValue(currentAcPhase2, 1000.0);
            qCDebug(dcSma()) << "Inverter: Current AC phase 2" << m_currentAcPhase2 << "A";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x465200) {
            quint32 currentAcPhase3;
            stream >> currentAcPhase3;
            m_currentAcPhase3 = readValue(currentAcPhase3, 1000.0);
            qCDebug(dcSma()) << "Inverter: Current AC phase 3" << m_currentAcPhase3 << "A";
            readUntilEndOfMeasurement(stream);
        }
    }
}

void SpeedwireInverter::processAcTotalPowerResponse(const QByteArray &response)
{
    // 00000000 00000000
    // 013f2640 8606bb61 16010000 16010000 16010000 16010000 01000000
    // 00000000
    qCDebug(dcSma()) << "Inverter: Process AC total power query response"; // << response.toHex();
    QDataStream stream(response);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 firstWord, secondWord;
    stream >> firstWord >> secondWord;

    // Each line has 7 words
    quint32 measurementId;
    quint32 measurementType; // ?

    while (!stream.atEnd()) {
        // First row
        stream >> measurementId;

        // End of data, we are done
        if (measurementId == 0)
            return;

        // Unknown
        stream >> measurementType;

        measurementId = measurementId & 0x00ffff00;

        // Read measurent lines
        if (measurementId == 0x263f00) {
            qint32 totalAcPower;
            stream >> totalAcPower;
            if (totalAcPower >= 0) {
                m_totalAcPower = readValue(totalAcPower);
                qCDebug(dcSma()) << "Inverter: Total AC power" << m_totalAcPower << "W";
            } else {
                qCDebug(dcSma()) << "Inverter: Total AC power is negative, ignoring value" << totalAcPower << "W";
            }
            readUntilEndOfMeasurement(stream);
        }
    }
}

void SpeedwireInverter::processDcPowerResponse(const QByteArray &response)
{
    // No sun
    // 00000000 01000000
    // 011e2540 7503ba61 00000080 00000080 00000080 00000080 01000000
    // 021e2540 7503ba61 00000080 00000080 00000080 00000080 01000000
    // 00000000

    // Sun
    // 00000000 01000000
    // 011e2540 7b0dbb61 8a000000 8a000000 8a000000 8a000000 01000000
    // 021e2540 7b0dbb61 8f000000 8f000000 8f000000 8f000000 01000000
    // 00000000
    qCDebug(dcSma()) << "Inverter: Process DC power query response"; // << response.toHex();
    QDataStream stream(response);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 firstWord, secondWord;
    stream >> firstWord >> secondWord;

    // Each line has 7 words
    quint32 measurementId;
    quint32 measurementType; // ?

    while (!stream.atEnd()) {
        // First row
        stream >> measurementId;

        // End of data, we are done
        if (measurementId == 0)
            return;

        // Unknown
        stream >> measurementType;

        quint8 measurmentNumber = static_cast<quint8>(measurementId & 0xff);
        measurementId = measurementId & 0x00ffff00;

        // Read measurent lines
        if (measurementId == 0x251e00 && measurmentNumber == 0x01) {
            quint32 powerMpp1;
            stream >> powerMpp1;
            m_powerDcMpp1 = readValue(powerMpp1);
            qCDebug(dcSma()) << "Inverter: DC power MPP1" << m_powerDcMpp1 << "W";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x251e00 && measurmentNumber == 0x02) {
            quint32 powerMpp2;
            stream >> powerMpp2;
            m_powerDcMpp2 = readValue(powerMpp2);
            qCDebug(dcSma()) << "Inverter: DC power MPP2" << m_powerDcMpp2 << "W";
            readUntilEndOfMeasurement(stream);
        }
    }
}

void SpeedwireInverter::processDcVoltageCurrentResponse(const QByteArray &response)
{
    // 02000000 05000000
    // 011f4540 7503ba61 00000080 00000080 00000080 00000080 01000000
    // 021f4540 7503ba61 00000080 00000080 00000080 00000080 01000000
    // 01214540 7503ba61 00000080 00000080 00000080 00000080 01000000
    // 02214540 7503ba61 00000080 00000080 00000080 00000080 01000000
    // 00000000

    // 02000000 05000000
    // 011f4540 160ebb61 009a0000 009a0000 009a0000 009a0000 01000000
    // 021f4540 160ebb61 02a00000 02a00000 02a00000 02a00000 01000000
    // 01214540 160ebb61 76010000 76010000 76010000 76010000 01000000
    // 02214540 160ebb61 77010000 77010000 77010000 77010000 01000000
    // 00000000

    qCDebug(dcSma()) << "Inverter: Process DC voltage/current response"; // << response.toHex();
    QDataStream stream(response);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 firstWord, secondWord;
    stream >> firstWord >> secondWord;

    // Each line has 7 words
    quint32 measurementId;
    quint32 measurementType; // ?

    while (!stream.atEnd()) {
        // First row
        stream >> measurementId;

        // End of data, we are done
        if (measurementId == 0)
            return;

        // Unknown
        stream >> measurementType;

        quint8 measurmentNumber = static_cast<quint8>(measurementId & 0xff);
        measurementId = measurementId & 0x00ffff00;

        // Read measurent lines
        if (measurementId == 0x451f00 && measurmentNumber == 0x01) {
            quint32 voltageMpp1;
            stream >> voltageMpp1;
            m_voltageDcMpp1 = readValue(voltageMpp1, 100.0);
            qCDebug(dcSma()) << "Inverter: DC voltage MPP1" << m_voltageDcMpp1 << "V";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x451e00 && measurmentNumber == 0x02) {
            quint32 voltageMpp2;
            stream >> voltageMpp2;
            m_voltageDcMpp2 = readValue(voltageMpp2, 100.0);
            qCDebug(dcSma()) << "Inverter: DC voltage MPP2" << m_voltageDcMpp2 << "V";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x452100 && measurmentNumber == 0x01) {
            quint32 currentMpp1;
            stream >> currentMpp1;
            m_currentDcMpp1 = readValue(currentMpp1, 1000.0);
            qCDebug(dcSma()) << "Inverter: DC current MPP1" << m_currentDcMpp1 << "A";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x452100 && measurmentNumber == 0x02) {
            quint32 currentMpp2;
            stream >> currentMpp2;
            m_currentDcMpp2 = readValue(currentMpp2, 1000.0);
            qCDebug(dcSma()) << "Inverter: DC current MPP2" << m_currentDcMpp2 << "A";
            readUntilEndOfMeasurement(stream);
        }
    }
}

void SpeedwireInverter::processEnergyProductionResponse(const QByteArray &response)
{
    // 00000000 01000000
    // 01012600 6f03ba61 4d147402 0000000 00122260 01523ba6 17c05000 00000000
    // 000000000

    qCDebug(dcSma()) << "Inverter: Process energy production response";// << response.toHex();
    QDataStream stream(response);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 firstWord, secondWord;
    stream >> firstWord >> secondWord;
    quint32 dataId, dataType;
    stream >> dataId >> dataType;

    quint32 unknwonWord;
    quint32 totalEnergy, todayEnergy;
    stream >> totalEnergy >> unknwonWord >> unknwonWord >> unknwonWord >> todayEnergy;

    m_todayEnergyProduced = readValue(todayEnergy, 1000.0);
    m_totalEnergyProduced = readValue(totalEnergy, 1000.0);
    qCDebug(dcSma()) << "Inverter: Energy total:" << m_totalEnergyProduced << "kWh";
    qCDebug(dcSma()) << "Inverter: Energy today:" << m_todayEnergyProduced << "kWh";
}

void SpeedwireInverter::processGridFrequencyResponse(const QByteArray &response)
{
    // 10000000 10000000
    // 01574600 c20cbb61 89130000 89130000 89130000 89130000 010000000
    // 0000000
    qCDebug(dcSma()) << "Inverter: Process grid frequency response"; // << response.toHex();
    QDataStream stream(response);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 firstWord, secondWord;
    stream >> firstWord >> secondWord;

    // Each line has 7 words
    quint32 measurementId;
    quint32 measurementType; // ?

    while (!stream.atEnd()) {
        // First row
        stream >> measurementId;

        // End of data, we are done
        if (measurementId == 0)
            return;

        // Unknown
        stream >> measurementType;

        measurementId = measurementId & 0x00ffff00;

        // Read measurent lines
        if (measurementId == 0x465700) {
            quint32 frequency;
            stream >> frequency;
            m_gridFrequency = readValue(frequency, 100.0);
            qCDebug(dcSma()) << "Inverter: Grid frequency" << m_gridFrequency << "Hz";
            readUntilEndOfMeasurement(stream);
        }
    }
}

void SpeedwireInverter::processBatteryInfoResponse(const QByteArray &response)
{
    // Charging
    // 32000000 34000000
    // 095b4940 95ed5064 d2000000 d2000000 d2000000 d2000000 01000000
    // 095c4900 95ed5064 98530000 98530000 98530000 98530000 01000000
    // 095d4940 95ed5064 e1010000 e1010000 e1010000 e1010000 01000000
    // 00000000

    // Disacharging
    // 32000000 34000000
    // 095b4940 b74e5364 dc000000 dc000000 dc000000 dc000000 01000000
    // 095c4900 b74e5364 e87b0000 e87b0000 e87b0000 e87b0000 01000000
    // 095d4940 b74e5364 b6f8ffff b6f8ffff b6f8ffff b6f8ffff 01000000
    // 00000000
    qCDebug(dcSma()) << "Inverter: Process battery info response" << response.toHex();

    QDataStream stream(response);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 firstWord, secondWord;
    stream >> firstWord >> secondWord;

    // Each line has 7 words
    quint32 measurementId;
    quint32 measurementType; // ?

    while (!stream.atEnd()) {
        // First row
        stream >> measurementId;

        // End of data, we are done
        if (measurementId == 0)
            return;

        // Unknown
        stream >> measurementType;

        quint8 measurmentNumber = static_cast<quint8>(measurementId & 0xff);
        measurementId = measurementId & 0x00ffff00;

        // Read measurent lines
        if (measurementId == 0x495a00) {
            quint32 batteryCycles;
            stream >> batteryCycles;
            m_batteryCycles = readValue(batteryCycles);
            qCDebug(dcSma()) << "Battery: Cycle count" << m_batteryCycles;
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x495b00) {
            qint32 batteryTemperature;
            stream >> batteryTemperature;
            m_batteryTemperature = readValue(batteryTemperature, 10.0);
            qCDebug(dcSma()) << "Battery: Temperature" << m_batteryTemperature << "°C";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x495c00) {
            quint32 batteryVoltage;
            stream >> batteryVoltage;
            m_batteryVoltage = readValue(batteryVoltage, 100.0);
            qCDebug(dcSma()) << "Battery: Voltage" << m_batteryVoltage << "V";
            readUntilEndOfMeasurement(stream);
        } else if (measurementId == 0x495d00) {
            qint32 batteryCurrent;
            stream >> batteryCurrent;
            m_batteryCurrent = readValue(batteryCurrent, 1000.0);
            qCDebug(dcSma()) << "Battery: Current" << batteryCurrent << m_batteryCurrent << "A";
            readUntilEndOfMeasurement(stream);
        } else {
            quint32 unknwonValue;
            stream >> unknwonValue;
            qCDebug(dcSma()) << "Battery: Measurement ID:" << QString("0x%1").arg(measurementId , 0, 16) <<  "Measurement number:" << QString("0x%1").arg(measurmentNumber, 0, 16);
            qCDebug(dcSma()) << "Battery: Unknown value:" << QString("0x%1").arg(unknwonValue , 0, 16) << unknwonValue;
            readUntilEndOfMeasurement(stream);
        }
    }
}

void SpeedwireInverter::processBatteryChargeResponse(const QByteArray &response)
{
    qCDebug(dcSma()) << "Inverter: Process battery charge response" << response.toHex();

    QDataStream stream(response);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 firstWord, secondWord;
    stream >> firstWord >> secondWord;

    // Each line has 7 words
    quint32 measurementId;
    quint32 measurementType; // ?

    while (!stream.atEnd()) {
        // First row
        stream >> measurementId;

        // End of data, we are done
        if (measurementId == 0)
            return;

        // Unknown
        stream >> measurementType;

        quint8 measurmentNumber = static_cast<quint8>(measurementId & 0xff);
        measurementId = measurementId & 0x00ffff00;

        // Read measurent lines

        if (measurementId == 0x295a00) {
            quint32 batteryCharge;
            stream >> batteryCharge;
            m_batteryCharge = readValue(batteryCharge);
            qCDebug(dcSma()) << "Battery: Level" << m_batteryCharge << "%";
            readUntilEndOfMeasurement(stream);
        } else {
            quint32 unknwonValue;
            stream >> unknwonValue;
            qCDebug(dcSma()) << "Battery: Measurement ID: " << QString("0x%1").arg(measurementId , 0, 16) <<  "Measurement number:" << QString("0x%1").arg(measurmentNumber, 0, 16);
            qCDebug(dcSma()) << "Battery: Unknown value:" << QString("0x%1").arg(unknwonValue , 0, 16) << unknwonValue;
            readUntilEndOfMeasurement(stream);
        }
    }
}

void SpeedwireInverter::processInverterStatusResponse(const QByteArray &response)
{
    // 00000000 00000000
    // 01482108 b527bb61 23000000 2f010000 33010001 c7010000 feffff00 00000000 00000000 00000000
    // 00000000
    qCDebug(dcSma()) << "Inverter: Process inverter status response" << response.toHex();
    // TODO:
}


void SpeedwireInverter::readUntilEndOfMeasurement(QDataStream &stream)
{
    // Read until end of line (0x01000000)
    quint32 word;
    while (!stream.atEnd()) {
        stream >> word;
        if (word == 1) {
            return;
        }
    }
}

double SpeedwireInverter::readValue(quint32 value, double divisor)
{
    // Note: the first 2 are verifies as invalid values, the last one has been seen on some system in the field,
    // where the inverter has sent garbage data, the only consistancy was the ffff xxxx.
    if (value == 0x80000000 || value == 0xffffffff || ((value & 0xffff0000) == 0xffff0000))
        return 0;

    return value / divisor;
}

double SpeedwireInverter::readValue(qint32 value, double divisor)
{
    if (static_cast<quint32>(value) == 0x80000000 || static_cast<quint32>(value) == 0xffffffff)
        return 0;

    return value / divisor;
}

void SpeedwireInverter::setReachable(bool reachable)
{
    if (m_reachable == reachable)
        return;

    m_reachable = reachable;
    emit reachableChanged(m_reachable);
}

void SpeedwireInverter::setBatteryAvailable(bool available)
{
    if (m_batteryAvailable == available)
        return;

    m_batteryAvailable = available;
    emit batteryAvailableChanged(m_batteryAvailable);
}

void SpeedwireInverter::processData(const QHostAddress &senderAddress, quint16 senderPort, const QByteArray &data)
{
    Q_UNUSED(senderPort)

    // Process only data coming from our target address if there is any
    if (!m_address.isNull() && senderAddress != m_address)
        return;

    if (data.size() < 18) {
        qCDebug(dcSma()) << "Inverter: The received datagram is to short to be a SMA speedwire message. Ignoring data...";
        return;
    }

    QDataStream stream(data);
    Speedwire::Header header = Speedwire::parseHeader(stream);
    if (!header.isValid()) {
        qCWarning(dcSma()) << "Inverter: Datagram header is not valid. Ignoring data...";
        return;
    }

    if (header.protocolId != Speedwire::ProtocolIdInverter) {
        //qCWarning(dcSma()) << "Inverter: Received datagram from different protocol" << header.protocolId << "Ignoring data...";
        return;
    }

    Speedwire::InverterPacket packet = Speedwire::parseInverterPacket(stream);
    if (packet.sourceModelId != m_modelId || packet.sourceSerialNumber != m_serialNumber) {
        qCWarning(dcSma()) << "Inverter: Received datagram from different inverter" << packet.sourceSerialNumber << "Ignoring data...";
        return;
    }

    qCDebug(dcSma()) << "Inverter: <-- Received" << static_cast<Speedwire::Command>(packet.command) << "Packet ID:" << packet.packetId;
    //qCDebug(dcSma()) << "Inverter:" << data.toHex();
    if (m_currentReply && m_currentReply->request().packetId() == packet.packetId) {
        qCDebug(dcSma()) << "Inverter: Received response for current reply" << static_cast<Speedwire::Command>(m_currentReply->request().command()) << "Packet ID:" << m_currentReply->request().packetId();
        m_currentReply->m_responseData = data;
        m_currentReply->m_responseHeader = header;
        m_currentReply->m_responsePacket = packet;
        // Set the payload
        while (!stream.atEnd()) {
            quint8 byte;
            stream >> byte;
            m_currentReply->m_responsePayload.append(byte);
        }

        if (packet.errorCode != 0) {
            m_currentReply->finishReply(SpeedwireInverterReply::ErrorInverterError);
        } else {
            m_currentReply->finishReply(SpeedwireInverterReply::ErrorNoError);
        }
    } else {
        if (m_currentReply) {
            qCWarning(dcSma()) << "Inverter: Received unexpected data: waiting for" << static_cast<Speedwire::Command>(m_currentReply->request().command()) << "Packet ID:" << m_currentReply->request().packetId();
        } else {
            qCWarning(dcSma()) << "Inverter: Received unexpected data: not waiting for any response.";
        }
        qCWarning(dcSma()) << "Inverter:" << header;
        qCWarning(dcSma()) << "Inverter:" << packet;
        qCWarning(dcSma()) << "Inverter:" << data.toHex();
    }
}

void SpeedwireInverter::onReplyTimeout()
{
    SpeedwireInverterReply *reply = qobject_cast<SpeedwireInverterReply *>(sender());
    qCDebug(dcSma()) << "Inverter: Reply timeout" << reply->request().packetId() << reply->request().command();
    reply->m_retries += 1;
    if (reply->m_retries <= reply->m_maxRetries) {
        qCDebug(dcSma()) << "Inverter: Resend request" << reply->m_retries << "/" << reply->m_maxRetries;
        m_replyQueue.prepend(reply);
        m_currentReply = nullptr;
        sendNextReply();
    } else {
        if (reply->m_maxRetries == 0) {
            qCWarning(dcSma()) << "Inverter: No response received for request. Finish reply with" << SpeedwireInverterReply::ErrorTimeout;
        } else {
            qCWarning(dcSma()) << "Inverter: No response received for request after" << reply->m_maxRetries << "attempts. Finish reply with" << SpeedwireInverterReply::ErrorTimeout;
        }
        // Finish with timeout error
        reply->finishReply(SpeedwireInverterReply::ErrorTimeout);
    }
}

void SpeedwireInverter::onReplyFinished()
{
    SpeedwireInverterReply *reply = qobject_cast<SpeedwireInverterReply *>(sender());
    if (m_currentReply == reply) {
        // Note: the reply is self deleting on finished
        m_currentReply = nullptr;
        sendNextReply();
    }
}

void SpeedwireInverter::setState(State state)
{
    if (m_state == state)
        return;

    qCDebug(dcSma()) << "Inverter: State changed" << state;
    m_state = state;
    emit stateChanged(m_state);

    switch (m_state) {
    case StateIdle:
        break;
    case StateDisconnected:
        setReachable(false);
        break;
    case StateInitializing: {
        // Try to fetch ac power
        qCDebug(dcSma()) << "Inverter: Request AC power...";
        SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryAc, 0x00464000, 0x004642ff);
        connect(reply, &SpeedwireInverterReply::finished, this, [=](){
            if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                if (reply->error() == SpeedwireInverterReply::ErrorTimeout) {
                    qCWarning(dcSma()) << "Inverter: Failed to query data from inverter:" << reply->request().command() << reply->error();

                    // TODO: try to send identify request and retry 3 times before giving up,
                    // still need to figure out why the inverter stops responding sometimes and how we can
                    // make it communicative again, a reconfugre always fixes this issue...somehow...

                    setState(StateDisconnected);
                    return;
                }

                // Reachable, but received an inverter error, probably not logged
                if (reply->error() == SpeedwireInverterReply::ErrorInverterError) {
                    qCDebug(dcSma()) << "Inverter: Query data request finished with inverter error. Try to login...";
                    setState(StateLogin);
                    return;
                }
            }

            // We where able to read data...emit the signal for the setup just incase
            emit loginFinished(true);

            qCDebug(dcSma()) << "Inverter: Query request finished successfully" << reply->request().command();
            processAcPowerResponse(reply->responsePayload());


            if (m_deviceInformationFetched) {
                setState(StateQueryData);
            } else {
                setState(StateGetInformation);
            }
        });
        break;
    }
    case StateLogin: {
        SpeedwireInverterReply *loginReply = sendLoginRequest(m_password);
        connect(loginReply, &SpeedwireInverterReply::finished, this, [=](){
            if (loginReply->error() != SpeedwireInverterReply::ErrorNoError) {
                qCWarning(dcSma()) << "Inverter: Failed to login to inverter:" << loginReply->error();
                emit loginFinished(false);
                setState(StateDisconnected);
                return;
            }

            qCDebug(dcSma()) << "Inverter: Login request finished successfully.";
            emit loginFinished(true);
            setReachable(true);

            // Logged in successfully, reinit the data fetch process
            setState(StateInitializing);
        });
        break;
    }
    case StateGetInformation: {
        SpeedwireInverterReply *softwareVersionReply = sendSoftwareVersionRequest();
        connect(softwareVersionReply, &SpeedwireInverterReply::finished, this, [=](){
            if (softwareVersionReply->error() != SpeedwireInverterReply::ErrorNoError) {
                qCWarning(dcSma()) << "Inverter: Failed to get software version from inverter:" << softwareVersionReply->error();
                setState(StateDisconnected);
                return;
            }

            qCDebug(dcSma()) << "Inverter: Software version request finished successfully.";
            processSoftwareVersionResponse(softwareVersionReply->responsePayload());

            SpeedwireInverterReply *deviceTypeReply = sendDeviceTypeRequest();
            connect(deviceTypeReply, &SpeedwireInverterReply::finished, this, [=](){
                if (deviceTypeReply->error() != SpeedwireInverterReply::ErrorNoError) {
                    qCWarning(dcSma()) << "Inverter: Failed to get device information from inverter:" << deviceTypeReply->error();
                    setState(StateDisconnected);
                    return;
                }

                qCDebug(dcSma()) << "Inverter: Get device information finished successfully.";
                processDeviceTypeResponse(deviceTypeReply->responsePayload());
                m_deviceInformationFetched = true;
                setState(StateQueryData);
            });
        });
        break;
    }
    case StateQueryData: {
        // Query inverter status
        qCDebug(dcSma()) << "Inverter: Request inverter status...";
        SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryStatus, 0x00214800, 0x002148FF);
        connect(reply, &SpeedwireInverterReply::finished, this, [=](){
            if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                qCWarning(dcSma()) << "Inverter: Failed to get status from inverter:" << reply->request().command() << reply->error();
                setState(StateDisconnected);
                return;
            }

            qCDebug(dcSma()) << "Inverter: Get inverter status request finished successfully" << reply->request().command();
            processInverterStatusResponse(reply->responsePayload());


            // Query AC voltage / current
            qCDebug(dcSma()) << "Inverter: Request AC voltage and current...";
            SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryAc, 0x00464800, 0x004655ff);
            connect(reply, &SpeedwireInverterReply::finished, this, [=](){
                if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                    qCWarning(dcSma()) << "Inverter: Failed to query data from inverter:" << reply->request().command() << reply->error();
                    setState(StateDisconnected);
                    return;
                }

                qCDebug(dcSma()) << "Inverter: Query request finished successfully" << reply->request().command();
                processAcVoltageCurrentResponse(reply->responsePayload());


                // Query DC power
                qCDebug(dcSma()) << "Inverter: Request DC power...";
                SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryDc, 0x00251e00, 0x00251eff);
                connect(reply, &SpeedwireInverterReply::finished, this, [=](){
                    if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                        qCWarning(dcSma()) << "Inverter: Failed to query data from inverter:" << reply->request().command() << reply->error();
                        setState(StateDisconnected);
                        return;
                    }

                    qCDebug(dcSma()) << "Inverter: Query request finished successfully" << reply->request().command();
                    processDcPowerResponse(reply->responsePayload());


                    // Query DC voltage/current
                    qCDebug(dcSma()) << "Inverter: Request DC voltage and current...";
                    SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryDc, 0x00451f00, 0x004521ff);
                    connect(reply, &SpeedwireInverterReply::finished, this, [=](){
                        if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                            qCWarning(dcSma()) << "Inverter: Failed to query data from inverter:" << reply->request().command() << reply->error();
                            setState(StateDisconnected);
                            return;
                        }

                        qCDebug(dcSma()) << "Inverter: Query request finished successfully" << reply->request().command();
                        processDcVoltageCurrentResponse(reply->responsePayload());


                        // Query energy production
                        qCDebug(dcSma()) << "Inverter: Request energy production...";
                        SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryEnergy, 0x00260100, 0x002622ff);
                        connect(reply, &SpeedwireInverterReply::finished, this, [=](){
                            if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                                qCWarning(dcSma()) << "Inverter: Failed to query data from inverter:" << reply->request().command() << reply->error();
                                setState(StateDisconnected);
                                return;
                            }

                            qCDebug(dcSma()) << "Inverter: Query request finished successfully" << reply->request().command();
                            processEnergyProductionResponse(reply->responsePayload());


                            // Query total AC power
                            qCDebug(dcSma()) << "Inverter: Request total AC power...";
                            SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryAc, 0x00263f00, 0x00263fff);
                            connect(reply, &SpeedwireInverterReply::finished, this, [=](){
                                if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                                    qCWarning(dcSma()) << "Inverter: Failed to query data from inverter:" << reply->request().command() << reply->error();
                                    setState(StateDisconnected);
                                    return;
                                }

                                qCDebug(dcSma()) << "Inverter: Query request finished successfully" << reply->request().command();
                                processAcTotalPowerResponse(reply->responsePayload());


                                // Query grid frequency
                                qCDebug(dcSma()) << "Inverter: Request grid frequency...";
                                SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryAc, 0x00465700, 0x004657ff);
                                connect(reply, &SpeedwireInverterReply::finished, this, [=](){
                                    if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                                        qCWarning(dcSma()) << "Inverter: Failed to query data from inverter:" << reply->request().command() << reply->error();
                                        setState(StateDisconnected);
                                        return;
                                    }

                                    qCDebug(dcSma()) << "Inverter: Query request finished successfully" << reply->request().command();
                                    processGridFrequencyResponse(reply->responsePayload());

                                    setReachable(true);
                                    emit valuesUpdated();

                                    // ############# Optional ##########

                                    // Query battery info
                                    qCDebug(dcSma()) << "Inverter: Request battery info...";
                                    SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryAc, 0x00491e00, 0x00495dff); // Battery infos
                                    connect(reply, &SpeedwireInverterReply::finished, this, [=](){
                                        if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                                            qCDebug(dcSma()) << "Inverter: Failed to query battery info from inverter:" << reply->request().command() << reply->error();
                                            setBatteryAvailable(false);
                                            setState(StateIdle);
                                            return;
                                        } else {
                                            qCDebug(dcSma()) << "Inverter: Process battery info response" << reply->responsePayload().toHex();
                                            processBatteryInfoResponse(reply->responsePayload());
                                        }

                                        qCDebug(dcSma()) << "Inverter: Request battery charge status...";
                                        SpeedwireInverterReply *reply = sendQueryRequest(Speedwire::CommandQueryAc, 0x00295A00, 0x00295AFF); // Battery SoC
                                        connect(reply, &SpeedwireInverterReply::finished, this, [=](){
                                            if (reply->error() != SpeedwireInverterReply::ErrorNoError) {
                                                qCWarning(dcSma()) << "Inverter: Failed to query battery charge status from inverter:" << reply->request().command() << reply->error();
                                                setBatteryAvailable(false);
                                                setState(StateIdle);
                                                return;
                                            } else {
                                                qCDebug(dcSma()) << "Inverter: Process battery charge status response" << reply->responsePayload().toHex();
                                                processBatteryChargeResponse(reply->responsePayload());
                                            }

                                            setBatteryAvailable(true);
                                            emit batteryValuesUpdated();
                                            setState(StateIdle);
                                        });
                                    });
                                });
                            });
                        });
                    });
                });
            });
        });
        break;
    }
    }
}
