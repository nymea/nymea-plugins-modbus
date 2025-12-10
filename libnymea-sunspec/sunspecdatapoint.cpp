// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-sunspec.
*
* libnymea-sunspec is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-sunspec is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-sunspec. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "sunspecdatapoint.h"

#include <math.h>
#include <QDataStream>

SunSpecDataPoint::SunSpecDataPoint()
{

}

QString SunSpecDataPoint::name() const
{
    return m_name;
}

void SunSpecDataPoint::setName(const QString &name)
{
    m_name = name;
}

QString SunSpecDataPoint::label() const
{
    return m_label;
}

void SunSpecDataPoint::setLabel(const QString &label)
{
    m_label = label;
}

QString SunSpecDataPoint::description() const
{
    return m_description;
}

void SunSpecDataPoint::setDescription(const QString &description)
{
    m_description = description;
}

QString SunSpecDataPoint::detail() const
{
    return m_detail;
}

void SunSpecDataPoint::setDetail(const QString &detail)
{
    m_detail = detail;
}

QString SunSpecDataPoint::units() const
{
    return m_units;
}

void SunSpecDataPoint::setUnits(const QString &units)
{
    m_units = units;
}

bool SunSpecDataPoint::mandatory() const
{
    return m_mandatory;
}

void SunSpecDataPoint::setMandatory(bool mandatory)
{
    m_mandatory = mandatory;
}

SunSpecDataPoint::Access SunSpecDataPoint::access() const
{
    return m_access;
}

void SunSpecDataPoint::setAccess(Access access)
{
    m_access = access;
}

quint16 SunSpecDataPoint::addressOffset() const
{
    return m_addressOffset;
}

void SunSpecDataPoint::setAddressOffset(quint16 addressOffset)
{
    m_addressOffset = addressOffset;
}

quint16 SunSpecDataPoint::blockOffset() const
{
    return m_blockOffset;
}

void SunSpecDataPoint::setBlockOffset(quint16 blockOffset)
{
    m_blockOffset = blockOffset;
}

QString SunSpecDataPoint::sunSpecDataType() const
{
    return m_sunSpecDataType;
}

void SunSpecDataPoint::setSunSpecDataType(const QString &sunSpecDataType)
{
    m_sunSpecDataType = sunSpecDataType;
    setDataType(SunSpecDataPoint::stringToDataType(m_sunSpecDataType));
}

SunSpecDataPoint::DataType SunSpecDataPoint::dataType() const
{
    return m_dataType;
}

void SunSpecDataPoint::setDataType(DataType dataType)
{
    m_dataType = dataType;
}

SunSpecDataPoint::ByteOrder SunSpecDataPoint::byteOrder() const
{
    return m_byteOrder;
}

void SunSpecDataPoint::setByteOrder(ByteOrder byteOrder)
{
    m_byteOrder = byteOrder;
}

int SunSpecDataPoint::size() const
{
    return m_size;
}

void SunSpecDataPoint::setSize(int size)
{
    m_size = size;
}

QString SunSpecDataPoint::scaleFactorName() const
{
    return m_scaleFactorName;
}

void SunSpecDataPoint::setScaleFactorName(const QString &scaleFactorName)
{
    m_scaleFactorName = scaleFactorName;
}

QVector<quint16> SunSpecDataPoint::rawData() const
{
    return m_rawData;
}

void SunSpecDataPoint::setRawData(const QVector<quint16> &rawData)
{
    m_rawData = rawData;
}

bool SunSpecDataPoint::isValid() const
{
    // TODO: verify if value is not invalid code
    if (m_rawData.isEmpty())
        return false;

    bool valid = true;
    switch (m_dataType) {
    case DataType::Pad:
    case DataType::Int16:
    case DataType::ScaleFactor:
        if (toUInt16() == 0x8000)
            valid = false;

        break;
    case DataType::Int32:
        if (toUInt32() == 0x80000000)
            valid = false;

        break;
    case DataType::Int64:
        if (toUInt64() == 0x8000000000000000)
            valid = false;

        break;
    case DataType::Raw16:
        if (toUInt16() == 0x0000)
            valid = false;
        break;
    case DataType::Acc16:
        if (toUInt16() == 0x0000)
            valid = false;

        break;
    case DataType::IpV4Address:
    case DataType::Acc32:
        if (toUInt32() == 0x00000000)
            valid = false;

        break;
    case DataType::BitField16:
    case DataType::Enum16:
    case DataType::UInt16:
        if (toUInt16() == 0xFFFF)
            valid = false;
        break;
    case DataType::BitField32:
    case DataType::Enum32:
    case DataType::UInt32:
        if (toUInt32() == 0xFFFFFFFF)
            valid = false;
        break;
    case DataType::IpV6Address:
    case DataType::Acc64:
        if (toUInt64() == 0x0000000000000000)
            valid = false;

        break;
        break;
    case DataType::BitField64:

        break;
    case DataType::Float32:
        if (toUInt32() == 0x7FC00000)
            valid = false;
        break;
    case DataType::Float64:
        if (toUInt64() == 0x7FC0000000000000)
            valid = false;
        break;
    case DataType::String: {
        bool isNull = true;
        for (int i = 0; i < m_rawData.length(); i++) {
            if (m_rawData.at(i) != 0x0000) {
                isNull = false;
                valid = true;
                break;
            }
        }

        if (isNull)
            valid = false;

        break;
    }
    case DataType::EUI48:
    case DataType::Group:
    case DataType::Sync:
        break;
    }

    return valid;
}

SunSpecDataPoint::DataType SunSpecDataPoint::stringToDataType(const QString &typString)
{
    DataType dataType = DataType::UInt16;

    if (typString == "int16") {
        dataType = DataType::Int16;
    } else if (typString == "int32") {
        dataType = DataType::Int32;
    } else if (typString == "int64") {
        dataType = DataType::Int64;
    } else if (typString == "uint16") {
        dataType = DataType::UInt16;
    } else if (typString == "raw16") {
        dataType = DataType::Raw16;
    } else if (typString == "uint32") {
        dataType = DataType::UInt32;
    } else if (typString == "acc16") {
        dataType = DataType::Acc16;
    } else if (typString == "acc32") {
        dataType = DataType::Acc32;
    } else if (typString == "acc64") {
        dataType = DataType::Acc64;
    } else if (typString == "bitfield16") {
        dataType = DataType::BitField16;
    } else if (typString == "bitfield32") {
        dataType = DataType::BitField32;
    } else if (typString == "bitfield64") {
        dataType = DataType::BitField64;
    } else if (typString == "enum16") {
        dataType = DataType::Enum16;
    } else if (typString == "enum32") {
        dataType = DataType::Enum32;
    } else if (typString == "float32") {
        dataType = DataType::Float32;
    } else if (typString == "float64") {
        dataType = DataType::Float64;
    } else if (typString == "string") {
        dataType = DataType::String;
    } else if (typString == "sunssf") {
        dataType = DataType::ScaleFactor;
    } else if (typString == "pad") {
        dataType = DataType::Pad;
    } else if (typString == "ipaddr") {
        dataType = DataType::IpV4Address;
    } else if (typString == "ipv6addr") {
        dataType = DataType::IpV6Address;
    } else if (typString == "eui48") {
        dataType = DataType::EUI48;
    } else if (typString == "group") {
        dataType = DataType::Group;
    } else if (typString == "sync") {
        dataType = DataType::Sync;
    }

    return dataType;
}

quint16 SunSpecDataPoint::toUInt16() const
{
    return SunSpecDataPoint::convertToUInt16(m_rawData);
}

qint16 SunSpecDataPoint::toInt16() const
{
    return SunSpecDataPoint::convertToInt16(m_rawData);
}

quint32 SunSpecDataPoint::toUInt32() const
{
    return SunSpecDataPoint::convertToUInt32(m_rawData, m_byteOrder);
}

qint32 SunSpecDataPoint::toInt32() const
{
    return SunSpecDataPoint::convertToInt32(m_rawData, m_byteOrder);
}

quint64 SunSpecDataPoint::toUInt64() const
{
    return SunSpecDataPoint::convertToUInt64(m_rawData, m_byteOrder);
}

qint64 SunSpecDataPoint::toInt64() const
{
    return SunSpecDataPoint::convertToInt64(m_rawData, m_byteOrder);
}

QString SunSpecDataPoint::toString() const
{
    return SunSpecDataPoint::convertToString(m_rawData);
}

float SunSpecDataPoint::toFloat() const
{
    Q_ASSERT_X(m_dataType == Float32,  "SunSpecDataPoint", "invalid raw data size for converting value to float");
    return SunSpecDataPoint::convertToFloat32(m_rawData, m_byteOrder);
}

float SunSpecDataPoint::toFloatWithSSF(qint16 scaleFactor) const
{
    float value = 0;
    switch (m_dataType) {
    case Acc16:
    case UInt16:
        value = toUInt16() * pow(10, scaleFactor);
        break;
    case Int16:
        value = toInt16() * pow(10, scaleFactor);
        break;
    case Acc32:
    case UInt32:
        value = toUInt32() * pow(10, scaleFactor);
        break;
    case Int32:
        value = toInt32() * pow(10, scaleFactor);
        break;
    default:
        Q_ASSERT_X(false,  "SunSpecDataPoint", QString("unhandled data type for converting float with scale factor %1").arg(dataType()).toLatin1());
        break;
    }

    return value;
}

double SunSpecDataPoint::toDouble() const
{
    Q_ASSERT_X(m_dataType == Float64,  "SunSpecDataPoint", "invalid raw data size for converting value to double");
    return SunSpecDataPoint::convertToFloat64(m_rawData, m_byteOrder);
}

QString SunSpecDataPoint::registersToString(const QVector<quint16> &registers)
{
    QStringList valueStrings;
    for (int i = 0; i < registers.length(); i++) {
        QString hexString(QStringLiteral("0x%1"));
        valueStrings.append(hexString.arg(registers.at(i), 4, 16, QLatin1Char('0')));
    }

    QString registersString = "(" + valueStrings.join(", ") + ")";
    return registersString;
}

quint16 SunSpecDataPoint::convertToUInt16(const QVector<quint16> &registers)
{
    Q_ASSERT_X(registers.length() == 1,  "SunSpecDataPoint", "invalid raw data size for converting value to quint16");
    return registers.at(0);
}

qint16 SunSpecDataPoint::convertToInt16(const QVector<quint16> &registers)
{
    Q_ASSERT_X(registers.length() == 1,  "SunSpecDataPoint", "invalid raw data size for converting value to qint16");
    return static_cast<qint16>(registers.at(0));
}

quint32 SunSpecDataPoint::convertToUInt32(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 2,  "SunSpecDataPoint", "invalid raw data size for converting value to quint32");
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

qint32 SunSpecDataPoint::convertToInt32(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 2,  "SunSpecDataPoint", "invalid raw data size for converting value to quint32");
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

quint64 SunSpecDataPoint::convertToUInt64(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 4,  "SunSpecDataPoint", "invalid raw data size for converting value to quint64");
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

qint64 SunSpecDataPoint::convertToInt64(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 4,  "SunSpecDataPoint", "invalid raw data size for converting value to qint64");
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

QString SunSpecDataPoint::convertToString(const QVector<quint16> &registers)
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

    return QString::fromUtf8(bytes).trimmed();
}

float SunSpecDataPoint::convertToFloat32(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 2,  "SunSpecDataPoint", "invalid raw data size for converting value to float32");
    quint32 rawValue = SunSpecDataPoint::convertToUInt32(registers, byteOrder);
    float value = 0;
    memcpy(&value, &rawValue, sizeof(quint32));
    return value;
}

double SunSpecDataPoint::convertToFloat64(const QVector<quint16> &registers, ByteOrder byteOrder)
{
    Q_ASSERT_X(registers.length() == 4,  "SunSpecDataPoint", "invalid raw data size for converting value to float64");
    quint64 rawValue = SunSpecDataPoint::convertToUInt64(registers, byteOrder);
    double value = 0;
    memcpy(&value, &rawValue, sizeof(quint64));
    return value;
}

QVector<quint16> SunSpecDataPoint::convertFromUInt16(quint16 value)
{
    return QVector<quint16>() << value;
}

QVector<quint16> SunSpecDataPoint::convertFromInt16(qint16 value)
{
    return SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(value));
}

QVector<quint16> SunSpecDataPoint::convertFromUInt32(quint32 value, ByteOrder byteOrder)
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

QVector<quint16> SunSpecDataPoint::convertFromInt32(qint32 value, ByteOrder byteOrder)
{
    return SunSpecDataPoint::convertFromUInt32(static_cast<quint32>(value), byteOrder);
}

QVector<quint16> SunSpecDataPoint::convertFromUInt64(quint64 value, ByteOrder byteOrder)
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

QVector<quint16> SunSpecDataPoint::convertFromInt64(qint64 value, ByteOrder byteOrder)
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

QVector<quint16> SunSpecDataPoint::convertFromString(const QString &value, quint16 stringLength)
{
    Q_ASSERT_X(value.length() <= stringLength, "SunSpecDataPoint", "cannot convert a string which is bigger than the desired register vector.");
    QByteArray data = value.toLatin1() + QByteArray('\0', stringLength - value.length());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream stream(&data, QDataStream::ReadOnly);
#else
    QDataStream stream(&data, QIODevice::ReadOnly);
#endif
    QVector<quint16> values;
    for (int i = 0; i < stringLength; i++) {
        quint16 registerValue = 0;
        stream >> registerValue;
        values.append(registerValue);
    }
    return values;
}

QVector<quint16> SunSpecDataPoint::convertFromFloatWithSSF(float value, qint16 scaleFactor, DataType dataType, ByteOrder byteOrder)
{
    QVector<quint16> rawData;
    switch (dataType) {
    case Acc16:
    case UInt16: {
        quint16 rawValue = static_cast<quint16>(value * pow(10, -1 * scaleFactor));
        rawData << rawValue;
        break;
    }
    case Int16: {
        quint16 rawValue = static_cast<qint16>(value * pow(10, -1 * scaleFactor));
        rawData << rawValue;
        break;
    }
    case Acc32:
    case UInt32: {
        quint32 rawValue = static_cast<quint32>(value * pow(10, -1 * scaleFactor));
        rawData = SunSpecDataPoint::convertFromUInt32(rawValue, byteOrder);
        break;
    }
    case Int32: {
        qint32 rawValue = static_cast<qint32>(value * pow(10, -1 * scaleFactor));
        rawData = SunSpecDataPoint::convertFromInt32(rawValue, byteOrder);
        break;
    }
    default:
        Q_ASSERT_X(false,  "SunSpecDataPoint", QString("unhandled data type for converting from float with scale factor using data type %1").arg(dataType).toLatin1());
        break;
    }

    return rawData;
}

QVector<quint16> SunSpecDataPoint::convertFromFloat32(float value, ByteOrder byteOrder)
{
    quint32 rawValue = 0;
    memcpy(&rawValue, &value, sizeof(float));
    return SunSpecDataPoint::convertFromUInt32(rawValue, byteOrder);
}

QVector<quint16> SunSpecDataPoint::convertFromFloat64(double value, ByteOrder byteOrder)
{
    quint64 rawValue = 0;
    memcpy(&rawValue, &value, sizeof(double));
    return SunSpecDataPoint::convertFromUInt64(rawValue, byteOrder);
}

QDebug operator<<(QDebug debug, const SunSpecDataPoint &dataPoint)
{
    debug.nospace().noquote() << "DataPoint(";
    if (dataPoint.description().isEmpty()) {
        debug.nospace().noquote() << dataPoint.name() << ", ";
    } else {
        debug.nospace().noquote() << dataPoint.description() << ", " << dataPoint.name() << ", ";
    }

    if (dataPoint.access() == SunSpecDataPoint::AccessReadWrite) {
        debug.nospace().noquote() << "RW, ";
    } else {
        debug.nospace().noquote() << "R, ";
    }

    if (dataPoint.mandatory()) {
        debug.nospace().noquote() << "M, ";
    } else {
        debug.nospace().noquote() << "O, ";
    }

    debug.nospace().noquote() << dataPoint.sunSpecDataType();

    if (!dataPoint.units().isEmpty()) {
        debug.nospace().noquote() << ", [" << dataPoint.units() << "]";
    }

    debug.nospace().noquote() << ")";
    return debug.space().quote();
}
