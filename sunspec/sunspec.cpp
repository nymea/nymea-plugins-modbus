/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
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

#include "sunspec.h"
#include "extern-plugininfo.h"
#include  <QtEndian>

SunSpec::SunSpec(const QHostAddress &hostAddress, uint port, QObject *parent) :
    QObject(parent),
    m_hostAddress(hostAddress),
    m_port(port)
{
    m_modbusTcpClient = new QModbusTcpClient(this);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_port);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_hostAddress.toString());
    m_modbusTcpClient->setTimeout(2000);
    m_modbusTcpClient->setNumberOfRetries(3);

    connect(m_modbusTcpClient, &QModbusTcpClient::stateChanged, this, &SunSpec::onModbusStateChanged);
}

SunSpec::~SunSpec()
{
}

bool SunSpec::connectModbus()
{
    return m_modbusTcpClient->connectDevice();;
}

QString SunSpec::manufacturer()
{
    return m_manufacturer;
}

QString SunSpec::deviceModel()
{
    return m_deviceModel;
}

QString SunSpec::serialNumber()
{
    return m_serialNumber;
}

void SunSpec::findBaseRegister()
{
    qCDebug(dcSunSpec()) << "find base register";

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 40000, 2);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, m_slaveId)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    //uint modbusAddress = unit.startAddress(); TODO
                    if ((unit.value(0) << 16 | unit.value(1)) == 0x53756e53) {
                        //Well-known value. Uniquely identifies this as a SunSpec Modbus Map
                        qCDebug(dcSunSpec()) << "Found start of modbus map";
                        //emit foundBaseRegister(modbusAddress);
                    }
                } else {
                    qCWarning(dcSunSpec()) << "Read response error:" << reply->error();
                }
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error) {
                qCWarning(dcSunSpec()) << "Modbus reply error:" << error;
                reply->finished(); // To make sure it will be deleted
            });
        } else {
            qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
            delete reply; // broadcast replies return immediately
            return;
        }
    } else {
        qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
        return;
    }
}

void SunSpec::findModbusMap(const QList<BlockId> &ids, uint modbusAddressOffset)
{
    qCDebug(dcSunSpec()) << "Find modbus map. Start register" << m_baseRegister+modbusAddressOffset;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_baseRegister+modbusAddressOffset, 2);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, m_slaveId)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [ids, reply, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    BlockId blockId    = BlockId(unit.value(0));
                    int blockLength = unit.value(1);
                    if (blockId > 800 || blockId == BlockIdEnd) {
                        qCDebug(dcSunSpec()) << "Block id not found, Id:" << ids;
                        modbusMapSearchFinished(ids, 0, "Ids not found");
                        return;
                    }
                    if (ids.contains(blockId)) {
                        qCDebug(dcSunSpec()) << "Found block" << BlockId(blockId);
                        foundModbusMap(BlockId(blockId), modbusAddress);
                    } else {
                        //read next block header
                        qCDebug(dcSunSpec()) << "Found Block" << blockId << "with block length" << blockLength;
                        findModbusMap(ids, modbusAddress+2+blockLength-m_baseRegister); //read next block
                    }
                } else {
                    qCWarning(dcSunSpec()) << "Read response error:" << reply->error();
                }
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error) {
                qCWarning(dcSunSpec()) << "Modbus replay error:" << error;
                reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // broadcast replies return immediately
            return;
        }
    } else {
        qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
        return;
    }
}

void SunSpec::readMapHeader(uint modbusAddress)
{
    qCDebug(dcSunSpec()) << "Read block header. Modbus Address:" << modbusAddress << "Slave ID" << m_slaveId;
    if (modbusAddress == 40000)
        modbusAddress += 2;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, 2);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, m_slaveId)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    BlockId mapId = BlockId(unit.value(0));
                    int mapLength = unit.value(1);
                    qCDebug(dcSunSpec()) << "Received block header response. Map ID:" << mapId << "Map length" << mapLength;
                    mapHeaderReceived(modbusAddress, mapId, mapLength);
                } else {
                    qCWarning(dcSunSpec()) << "Read response error:" << reply->error();
                }
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error) {
                qCWarning(dcSunSpec()) << "Modbus reply error:" << error;
                reply->finished(); // To make sure it will be deleted
            });
        } else {
            qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
            delete reply; // broadcast replies return immediately
            return;
        }
    } else {
        qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
        return;
    }
}

void SunSpec::readMap(uint modbusAddress, uint modelLength)
{
    qCDebug(dcSunSpec()) << "Read map. Modbus Address" << modbusAddress << ", Slave ID" << m_slaveId;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, modelLength+2);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, m_slaveId)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    BlockId mapId = BlockId(unit.value(0));
                    uint mapLength = unit.value(1);
                    qCDebug(dcSunSpec()) << "Received map. Modbus address" << modbusAddress << "Map ID" << mapId << "Map Length" << mapLength;
                    emit mapReceived(mapId, mapLength, unit.values().mid(2));
                } else {
                    qCWarning(dcSunSpec()) << "Read response error:" << reply->error();
                }
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error) {
                qCWarning(dcSunSpec()) << "Modbus reply error:" << error;
                reply->finished(); // To make sure it will be deleted
            });
        } else {
            qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
            delete reply; // broadcast replies return immediately
            return;
        }
    } else {
        qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
        return;
    }
}

void SunSpec::readCommonMap()
{
    qCDebug(dcSunSpec()) << "Read common block";

    // Base and Alternate Base Register Addresses
    // DeviceModbus maps begin at one of three well-­known Modbus base addresses.
    // Preferred Base Register: 40000
    // Alternate Base Register: 50000
    // Alternate Base Register: 00000

    // Common Model model-length is 66
    // First two registers are the SunSpec Modbus Map identifier

    qCDebug(dcSunSpec()) << "Read common block header. Modbus Address" << m_baseRegister+2 << ", Slave ID" << m_slaveId;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_baseRegister+2, 66);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, m_slaveId)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    BlockId mapId = BlockId(unit.value(0));
                    int mapLength = unit.value(1);
                    m_manufacturer = convertModbusRegisters(unit.values(), MandatoryRegistersModel1::Manufacturer, 16);
                    m_manufacturer.remove('\x00');
                    m_deviceModel  = convertModbusRegisters(unit.values(), MandatoryRegistersModel1::Model, 16);
                    m_deviceModel.remove('\x00');
                    m_serialNumber = convertModbusRegisters(unit.values(), MandatoryRegistersModel1::SerialNumber, 16);
                    m_serialNumber.remove('\x00');
                    qCDebug(dcSunSpec()) << "Received common block response. Manufacturer" << m_manufacturer << "Model" << m_deviceModel << "Serial number" << m_serialNumber;
                    mapHeaderReceived(modbusAddress, mapId, mapLength);
                } else {
                    qCWarning(dcSunSpec()) << "Read response error:" << reply->error();
                }
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error) {
                qCWarning(dcSunSpec()) << "Modbus reply error:" << error;
                reply->finished(); // To make sure it will be deleted
            });
        } else {
            qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
            delete reply; // broadcast replies return immediately
            return;
        }
    } else {
        qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
        return;
    }
}

void SunSpec::onReceivedHoldingRegister(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &data)
{
    if (modbusRegister == 00000 || modbusRegister == 40000 || modbusRegister == 50000) {
        // Common block, 66 registers long + 2 header registers

        qCDebug(dcSunSpec()) << "Sunspec Identification:" << convertModbusRegisters(data, 0, 2);

        if (convertModbusRegisters(data, 0, 2) != "SunS") {
            qCWarning(dcSunSpec()) << "Could not find SunS value at" << modbusRegister << "at Slave" << slaveAddress;
            return;
        }
        m_baseRegister = modbusRegister;

        //Mandatory SunSpec Registers
        // ID: 40003
        qCDebug(dcSunSpec()) << "Id:" << data[MandatoryRegistersModel1::Manufacturer];

        // Manufacturer: 40005 | 16
        qCDebug(dcSunSpec()) << "Manufacturer:" << QString::fromLatin1(convertModbusRegisters(data, MandatoryRegistersModel1::Manufacturer, 16));

        // Thing model: 40021 | 16
        qCDebug(dcSunSpec()) << "Thing model:" << QString::fromLatin1(convertModbusRegisters(data, MandatoryRegistersModel1::Model, 16));

        // Data manager version: 40037 | 8
        qCDebug(dcSunSpec()) << "Data manager version:" << QString::fromLatin1(convertModbusRegisters(data, 36, 8));

        // Inverter Version: 40045 | 8
        qCDebug(dcSunSpec()) << "Inverter version:" << QString::fromLatin1(convertModbusRegisters(data, 44, 8));

        // Serial Number: 40053 | 16
        qCDebug(dcSunSpec()) << "Serial number:" << QString::fromLatin1(convertModbusRegisters(data, MandatoryRegistersModel1::SerialNumber, 16));

        // Modbus thing address : 40069 | 1
        qCDebug(dcSunSpec()) << "Thing modbus address:" << data[67];
    };
}


QByteArray SunSpec::convertModbusRegister(const uint16_t &modbusData)
{
    uint8_t data[2];
    data[0] = modbusData >> 8;
    data[1] = modbusData & 0xFF;
    //qCDebug(dcSunSpec()) << (char)data[0] << (char)data[1];
    return QByteArray().append((char)data[0]).append((char)data[1]);
}

QBitArray SunSpec::convertModbusRegisterBits(const uint16_t &modbusData)
{
    QByteArray data = convertModbusRegister(modbusData);
    QBitArray bits(data.count() * 8);

    // Convert from QByteArray to QBitArray
    for(int i = 0; i < data.count(); ++i) {
        for(int b = 0; b < 8; b++) {
            bits.setBit(i * 8 + b, data.at(i) & (1 << ( 7 - b)));
        }
    }
    return bits;
}

QByteArray SunSpec::convertModbusRegisters(const QVector<quint16> &modbusData, int offset, int size)
{
    QByteArray bytes;
    for (int i = offset; i < offset + size; i++)
        bytes.append(convertModbusRegister(modbusData[i]));

    return bytes;
}

float SunSpec::convertValueWithSSF(quint16 rawValue, quint16 sunssf)
{
    float value;
    value = rawValue * (10^static_cast<qint16>(sunssf));
    return value;
}

float SunSpec::convertFloatValues(quint16 rawValue0, quint16 rawValue1)
{
    float value;
    uint32_t i = qFromLittleEndian(((uint32_t)rawValue0 << 16) + rawValue1);
    memcpy(&value, &i, sizeof(float));
    return value;
}

void SunSpec::onModbusStateChanged(QModbusDevice::State state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    if (!connected) {
        //try to reconnect in 10 seconds
        QTimer::singleShot(10000, m_modbusTcpClient, [this] {
            if (m_modbusTcpClient->connectDevice()) {
                qCDebug(dcSunSpec()) << "Could not reconnect";
            }
        });
    } else {
        readCommonMap();
    }
    emit connectionStateChanged(connected);
}

QUuid SunSpec::writeHoldingRegister(uint slaveAddress, uint registerAddress, quint16 value)
{
    return writeHoldingRegisters(slaveAddress, registerAddress, QVector<quint16>() << value);
}

QUuid SunSpec::writeHoldingRegisters(uint slaveAddress, uint registerAddress, const QVector<quint16> &values)
{
    if (!m_modbusTcpClient) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, registerAddress, values.length());
    request.setValues(values);

    if (QModbusReply *reply = m_modbusTcpClient->sendWriteRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcSunSpec()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcSunSpec()) << "Modbus reply error:" << error;
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcSunSpec()) << "Read error: " << m_modbusTcpClient->errorString();
        return "";
    }
    return requestId;
}
