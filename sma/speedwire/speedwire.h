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

#ifndef SPEEDWIRE_H
#define SPEEDWIRE_H

#include <QObject>
#include <QDebug>
#include <QDataStream>
#include <QHostAddress>
#include <QHash>

class Speedwire
{
    Q_GADGET
public:
    enum Command {
        CommandIdentify = 0x00000201,
        CommandQueryStatus = 0x51800200,
        CommandQueryAc = 0x51000200,
        CommandQueryDc = 0x53800200,
        CommandQueryEnergy = 0x54000200,
        CommandQueryDevice = 0x58000200,
        CommandQueryDeviceResponse = 58000201,
        CommandLogin = 0xfffd040c,
        CommandLogout = 0xfffd010e,
        CommandLoginResponse = 0x0ffdf40d
    };
    Q_ENUM(Command)

    enum ProtocolId {
        ProtocolIdUnknown = 0x0000,
        ProtocolIdMeter = 0x6069,
        ProtocolIdInverter = 0x6065,
        ProtocolIdDiscoveryResponse = 0x0001,
        ProtocolIdDiscovery = 0xffff
    };
    Q_ENUM(ProtocolId)

    class Header
    {
    public:
        Header() = default;
        quint32 smaSignature = 0;
        quint16 headerLength = 0;
        quint16 tagType = 0;
        quint16 tagVersion = 0;
        quint16 group = 0;
        quint16 payloadLength = 0;
        quint16 smaNet2Version = 0;
        ProtocolId protocolId = ProtocolIdUnknown;

        inline bool isValid() const {
            return smaSignature == Speedwire::smaSignature() && protocolId != ProtocolIdUnknown;
        }
    };

    typedef struct InverterPacket {
        quint8 wordCount = 0;
        quint8 control = 0;
        quint16 destinationModelId = 0;
        quint32 destinationSerialNumber = 0;
        quint16 destinationControl = 0;
        quint16 sourceModelId = 0;
        quint32 sourceSerialNumber = 0;
        quint16 sourceControl = 0;
        quint16 errorCode = 0;
        quint16 fragmentId = 0;
        quint16 packetId = 0;
        quint32 command = 0;
    } InverterPacket;

    Speedwire() = default;

    //static QHash<quint16, QString> deviceTypes = { {0x0000, "Unknwon"} };

    static quint16 port() { return  9522; }
    static QHostAddress multicastAddress() { return QHostAddress("239.12.255.254"); }
    static quint32 smaSignature() { return  0x534d4100; }
    static quint16 tag0() { return 0x02a0; }
    static quint16 tagVersion() { return 0; }
    static quint16 smaNet2Version() { return 0x0010; }


    //  Multicast device discovery request packet, according to SMA documentation.
    //  However, this does not seem to be supported anymore with version 3.x devices
    //        0x53, 0x4d, 0x41, 0x00, 0x00, 0x04, 0x02, 0xa0,     // sma signature, tag0
    //        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x20,     // 0xffffffff group, 0x0000 length, 0x0020 "SMA Net ?", Version ?
    //        0x00, 0x00, 0x00, 0x00                              // 0x0000 protocol, 0x00 #long words, 0x00 ctrl

    // Unicast device discovery request packet, according to SMA documentation
    //        0x53, 0x4d, 0x41, 0x00, 0x00, 0x04, 0x02, 0xa0,     // sma signature, tag0
    //        0x00, 0x00, 0x00, 0x01, 0x00, 0x26, 0x00, 0x10,     // 0x26 length, 0x0010 "SMA Net 2", Version 0
    //        0x60, 0x65, 0x09, 0xa0, 0xff, 0xff, 0xff, 0xff,     // 0x6065 protocol, 0x09 #long words, 0xa0 ctrl, 0xffff dst susyID any, 0xffffffff dst serial any
    //        0xff, 0xff, 0x00, 0x00, 0x7d, 0x00, 0x52, 0xbe,     // 0x0000 dst cntrl, 0x007d src susy id, 0x3a28be52 src serial
    //        0x28, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // 0x0000 src cntrl, 0x0000 error code, 0x0000 fragment ID
    //        0x01, 0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,     // 0x8001 packet ID
    //        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //        0x00, 0x00

    static QByteArray discoveryDatagramMulticast() { return QByteArray::fromHex("534d4100000402a0ffffffff0000002000000000"); }
    static QByteArray discoveryResponseDatagram() { return QByteArray::fromHex("534d4100000402A000000001000200000001"); }
    static QByteArray discoveryDatagramUnicast() { return QByteArray::fromHex("534d4100000402a00000000100260010606509a0ffffffffffff00007d0052be283a000000000000018000020000000000000000000000000000"); }

    static Speedwire::Header parseHeader(QDataStream &stream) {
        stream.setByteOrder(QDataStream::BigEndian);
        Header header;
        quint16 protocolId;
        stream >> header.smaSignature >> header.headerLength;
        stream >> header.tagType >> header.tagVersion >> header.group;
        stream >> header.payloadLength >> header.smaNet2Version;
        stream >> protocolId;
        header.protocolId = static_cast<ProtocolId>(protocolId);
        return header;
    };

    static Speedwire::InverterPacket parseInverterPacket(QDataStream &stream) {
        // Make sure the data stream is little endian
        stream.setByteOrder(QDataStream::LittleEndian);
        InverterPacket packet;
        stream >> packet.wordCount;
        stream >> packet.control;
        stream >> packet.destinationModelId;
        stream >> packet.destinationSerialNumber;
        stream >> packet.destinationControl;
        stream >> packet.sourceModelId;
        stream >> packet.sourceSerialNumber;
        stream >> packet.sourceControl;
        stream >> packet.errorCode;
        stream >> packet.fragmentId;
        stream >> packet.packetId;
        stream >> packet.command;
        return packet;
    };
};

inline QDebug operator<<(QDebug debug, const Speedwire::Header &header)
{
    debug.nospace() << "SpeedwireHeader(" << header.protocolId << ", payload size: " << header.payloadLength << ", group: " << header.payloadLength << ")";
    return debug.maybeSpace();
}

inline QDebug operator<<(QDebug debug, const Speedwire::InverterPacket &packet)
{
    debug.nospace() << "InverterPacket(" << packet.sourceSerialNumber;
    debug.nospace() << ", Model ID: " << packet.sourceModelId;
    debug.nospace() << ", command: " << packet.command;
    debug.nospace() << ", error: " << packet.errorCode;
    debug.nospace() << ", fragment: " << packet.fragmentId;
    debug.nospace() << ", packet ID: " << packet.fragmentId;
    debug.nospace()  << ")";
    return debug.maybeSpace();
}


#endif // SPEEDWIRE_H
