// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-modbus.
*
* libnymea-modbus is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "modbusdatautils.h"

#include <QDataStream>

ModbusDataUtils::ModbusDataUtils()
{

}

quint16 ModbusDataUtils::convertToUInt16(const QVector<quint16> &registers)
{
    Q_ASSERT_X(registers.length() == 1,  "ModbusDataUtils", "invalid raw data size for converting value to quint16");
    return registers.at(0);
}

qint16 ModbusDataUtils::convertToInt16(const QVector<quint16> &registers)
{
    Q_ASSERT_X(registers.length() == 1,  "ModbusDataUtils", "invalid raw data size for converting value to qint16");
    return static_cast<qint16>(registers.at(0));
}

quint32 ModbusDataUtils::convertToUInt32(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 2,  "ModbusDataUtils", "invalid raw data size for converting value to quint32");
    QByteArray data;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream inputStream(&data, QDataStream::WriteOnly);
#else
    QDataStream inputStream(&data, QIODevice::WriteOnly);
#endif
    if (byteOrder == ByteOrderBigEndian) {
        inputStream << registers.at(0);
        inputStream << registers.at(1);
    } else {
        inputStream << registers.at(1);
        inputStream << registers.at(0);
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream outputStream(&data, QDataStream::ReadOnly);
#else
    QDataStream outputStream(&data, QIODevice::ReadOnly);
#endif
    quint32 result = 0;
    outputStream >> result;
    return result;
}

qint32 ModbusDataUtils::convertToInt32(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 2,  "ModbusDataUtils", "invalid raw data size for converting value to quint32");
    QByteArray data;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream inputStream(&data, QDataStream::WriteOnly);
#else
    QDataStream inputStream(&data, QIODevice::WriteOnly);
#endif
    if (byteOrder == ByteOrderBigEndian) {
        inputStream << registers.at(0);
        inputStream << registers.at(1);
    } else {
        inputStream << registers.at(1);
        inputStream << registers.at(0);
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream outputStream(&data, QDataStream::ReadOnly);
#else
    QDataStream outputStream(&data, QIODevice::ReadOnly);
#endif
    qint32 result = 0;
    outputStream >> result;
    return result;
}

quint64 ModbusDataUtils::convertToUInt64(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 4,  "ModbusDataUtils", "invalid raw data size for converting value to quint64");
    QByteArray data;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream inputStream(&data, QDataStream::WriteOnly);
#else
    QDataStream inputStream(&data, QIODevice::WriteOnly);
#endif
    if (byteOrder == ByteOrderBigEndian) {
        inputStream << registers.at(0);
        inputStream << registers.at(1);
        inputStream << registers.at(2);
        inputStream << registers.at(3);
    } else {
        inputStream << registers.at(3);
        inputStream << registers.at(2);
        inputStream << registers.at(1);
        inputStream << registers.at(0);
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream outputStream(&data, QDataStream::ReadOnly);
#else
    QDataStream outputStream(&data, QIODevice::ReadOnly);
#endif
    quint64 result = 0;
    outputStream >> result;
    return result;
}

qint64 ModbusDataUtils::convertToInt64(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 4,  "ModbusDataUtils", "invalid raw data size for converting value to qint64");
    QByteArray data;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream inputStream(&data, QDataStream::WriteOnly);
#else
    QDataStream inputStream(&data, QIODevice::WriteOnly);
#endif
    if (byteOrder == ByteOrderBigEndian) {
        inputStream << registers.at(0);
        inputStream << registers.at(1);
        inputStream << registers.at(2);
        inputStream << registers.at(3);
    } else {
        inputStream << registers.at(3);
        inputStream << registers.at(2);
        inputStream << registers.at(1);
        inputStream << registers.at(0);
    }


#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream outputStream(&data, QDataStream::ReadOnly);
#else
    QDataStream outputStream(&data, QIODevice::ReadOnly);
#endif
    qint64 result = 0;
    outputStream >> result;
    return result;
}

QString ModbusDataUtils::convertToString(const QVector<quint16> &registers, ByteOrder characterByteOrder)
{
    QByteArray bytes;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream stream(&bytes, QDataStream::WriteOnly);
#else
    QDataStream stream(&bytes, QIODevice::WriteOnly);
#endif
    // Note: some devices use little endian within the register uint16 representation of the 2 characters.
    stream.setByteOrder(characterByteOrder == ByteOrderBigEndian ? QDataStream::BigEndian : QDataStream::LittleEndian);
    for (int i = 0; i < registers.length(); i++) {
        stream << registers.at(i);
    }

    return QString::fromUtf8(bytes).trimmed();
}

QByteArray ModbusDataUtils::convertToByteArray(const QVector<quint16> &registers)
{
    QByteArray bytes;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream stream(&bytes, QDataStream::WriteOnly);
#else
    QDataStream stream(&bytes, QIODevice::WriteOnly);
#endif
    for (int i = 0; i < registers.length(); i++) {
        stream << registers.at(i);
    }
    return bytes;
}

float ModbusDataUtils::convertToFloat32(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 2,  "ModbusDataUtils", "invalid raw data size for converting value to float32");
    quint32 rawValue = ModbusDataUtils::convertToUInt32(registers, byteOrder);
    float value = 0;
    memcpy(&value, &rawValue, sizeof(quint32));
    return value;
}

double ModbusDataUtils::convertToFloat64(const QVector<quint16> &registers, ByteOrder byteOrder)
{

    Q_ASSERT_X(registers.length() == 4,  "ModbusDataUtils", "invalid raw data size for converting value to float64");
    quint64 rawValue = ModbusDataUtils::convertToUInt64(registers, byteOrder);
    double value = 0;
    memcpy(&value, &rawValue, sizeof(quint64));
    return value;
}

QVector<quint16> ModbusDataUtils::convertFromUInt16(quint16 value)
{
    return QVector<quint16>() << value;
}

QVector<quint16> ModbusDataUtils::convertFromInt16(qint16 value)
{
    return ModbusDataUtils::convertFromUInt16(static_cast<quint16>(value));
}

QVector<quint16> ModbusDataUtils::convertFromUInt32(quint32 value, ByteOrder byteOrder)
{
    QByteArray data;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream inputStream(&data, QDataStream::WriteOnly);
#else
    QDataStream inputStream(&data, QIODevice::WriteOnly);
#endif
    inputStream << value;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream outputStream(&data, QDataStream::ReadOnly);
#else
    QDataStream outputStream(&data, QIODevice::ReadOnly);
#endif
    QVector<quint16> values;
    for (int i = 0; i < 2; i++) {
        quint16 registerValue = 0;
        outputStream >> registerValue;
        if (byteOrder == ByteOrderBigEndian) {
            values.append(registerValue);
        } else {
            values.prepend(registerValue);
        }
    }

    return values;
}

QVector<quint16> ModbusDataUtils::convertFromInt32(qint32 value, ByteOrder byteOrder)
{
    return ModbusDataUtils::convertFromUInt32(static_cast<quint32>(value), byteOrder);
}

QVector<quint16> ModbusDataUtils::convertFromUInt64(quint64 value, ByteOrder byteOrder)
{
    QByteArray data;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream inputStream(&data, QDataStream::WriteOnly);
#else
    QDataStream inputStream(&data, QIODevice::WriteOnly);
#endif
    inputStream << value;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream outputStream(&data, QDataStream::ReadOnly);
#else
    QDataStream outputStream(&data, QIODevice::ReadOnly);
#endif
    QVector<quint16> values;
    for (int i = 0; i < 4; i++) {
        quint16 registerValue = 0;
        outputStream >> registerValue;
        if (byteOrder == ByteOrderBigEndian) {
            values.append(registerValue);
        } else {
            values.prepend(registerValue);
        }
    }
    return values;
}

QVector<quint16> ModbusDataUtils::convertFromInt64(qint64 value, ByteOrder byteOrder)
{
    QByteArray data;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream inputStream(&data, QDataStream::WriteOnly);
#else
    QDataStream inputStream(&data, QIODevice::WriteOnly);
#endif
    inputStream << value;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream outputStream(&data, QDataStream::ReadOnly);
#else
    QDataStream outputStream(&data, QIODevice::ReadOnly);
#endif
    QVector<quint16> values;
    for (int i = 0; i < 4; i++) {
        quint16 registerValue = 0;
        outputStream >> registerValue;
        if (byteOrder == ByteOrderBigEndian) {
            values.append(registerValue);
        } else {
            values.prepend(registerValue);
        }
    }
    return values;
}

QVector<quint16> ModbusDataUtils::convertFromString(const QString &value, quint16 stringLength, ByteOrder characterByteOrder)
{
    Q_ASSERT_X(value.length() <= stringLength, "ModbusDataUtils", "cannot convert a string which is bigger than the desired register vector.");
    QByteArray data = value.toLatin1() + QByteArray('\0', stringLength - value.length());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream stream(&data, QDataStream::ReadOnly);
#else
    QDataStream stream(&data, QIODevice::ReadOnly);
#endif
    // Note: some devices use little endian within the register uint16 representation of the 2 characters.
    stream.setByteOrder(characterByteOrder == ByteOrderBigEndian ? QDataStream::BigEndian : QDataStream::LittleEndian);
    QVector<quint16> values;
    for (int i = 0; i < stringLength; i++) {
        quint16 registerValue = 0;
        stream >> registerValue;
        values.append(registerValue);
    }
    return values;
}

QVector<quint16> ModbusDataUtils::convertFromFloat32(float value, ByteOrder byteOrder)
{
    quint32 rawValue = 0;
    memcpy(&rawValue, &value, sizeof(float));
    return ModbusDataUtils::convertFromUInt32(rawValue, byteOrder);
}

QVector<quint16> ModbusDataUtils::convertFromFloat64(double value, ByteOrder byteOrder)
{
    quint64 rawValue = 0;
    memcpy(&rawValue, &value, sizeof(double));
    return ModbusDataUtils::convertFromUInt64(rawValue, byteOrder);
}

QString ModbusDataUtils::exceptionCodeToString(QModbusPdu::ExceptionCode exception)
{
    QString exceptionString;
    switch (exception) {
    case QModbusPdu::IllegalFunction:
        exceptionString = "Illegal function";
        break;
    case QModbusPdu::IllegalDataAddress:
        exceptionString = "Illegal data address";
        break;
    case QModbusPdu::IllegalDataValue:
        exceptionString = "Illegal data value";
        break;
    case QModbusPdu::ServerDeviceFailure:
        exceptionString = "Server device failure";
        break;
    case QModbusPdu::Acknowledge:
        exceptionString = "Acknowledge";
        break;
    case QModbusPdu::ServerDeviceBusy:
        exceptionString = "Server device busy";
        break;
    case QModbusPdu::NegativeAcknowledge:
        exceptionString = "Negative acknowledge";
        break;
    case QModbusPdu::MemoryParityError:
        exceptionString = "Memory parity error";
        break;
    case QModbusPdu::GatewayPathUnavailable:
        exceptionString = "Gateway path unavailable";
        break;
    case QModbusPdu::GatewayTargetDeviceFailedToRespond:
        exceptionString = "Gateway target device failed to respond";
        break;
    case QModbusPdu::ExtendedException:
        exceptionString = "Extended exception";
        break;
    }

    return exceptionString;
}

QString ModbusDataUtils::registerTypeToString(QModbusDataUnit::RegisterType registerType)
{
    QString registerTypeString;
    switch (registerType) {
    case QModbusDataUnit::DiscreteInputs:
        registerTypeString = "DiscreteInputs";
        break;
    case QModbusDataUnit::Coils:
        registerTypeString = "Coils";
        break;
    case QModbusDataUnit::InputRegisters:
        registerTypeString = "InputRegisters";
        break;
    case QModbusDataUnit::HoldingRegisters:
        registerTypeString = "HoldingRegisters";
        break;
    default:
        registerTypeString = "Invalid";
        break;
    }

    return registerTypeString;
}
