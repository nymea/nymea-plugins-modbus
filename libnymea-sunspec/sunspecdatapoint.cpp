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

#include "sunspecdatapoint.h"
#include <math.h>

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
        for (int i = 0; i < m_rawData.count(); i++) {
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
    Q_ASSERT_X(m_rawData.count() == 1, "SunSpecDataPoint", "invalid raw data size for converting value to quint16");
    return m_rawData.at(0);
}

qint16 SunSpecDataPoint::toInt16() const
{
    Q_ASSERT_X(m_rawData.count() == 1, "SunSpecDataPoint", "invalid raw data size for converting value to qint16");
    return static_cast<qint16>(m_rawData.at(0));
}

quint32 SunSpecDataPoint::toUInt32() const
{
    Q_ASSERT_X(m_rawData.count() == 2, "SunSpecDataPoint", "invalid raw data size for converting value to quint32");
    return static_cast<quint32>(m_rawData.at(1) << 16 | m_rawData.at(0));
}

qint32 SunSpecDataPoint::toInt32() const
{
    Q_ASSERT_X(m_rawData.count() == 2, "SunSpecDataPoint", "invalid raw data size for converting value to quint32");
    return static_cast<qint32>(m_rawData.at(1) << 16 | m_rawData.at(0));
}

quint64 SunSpecDataPoint::toUInt64() const
{
    Q_ASSERT_X(m_rawData.count() == 4, "SunSpecDataPoint", "invalid raw data size for converting value to quint64");
    quint64 value = 0;
    value |= (static_cast<quint64>(m_rawData.at(3)) << 48);
    value |= (static_cast<quint64>(m_rawData.at(2)) << 32);
    value |= (static_cast<quint64>(m_rawData.at(1)) << 16);
    value |= static_cast<quint64>(m_rawData.at(0));
    return value;
}

qint64 SunSpecDataPoint::toInt64() const
{
    Q_ASSERT_X(m_rawData.count() == 4, "SunSpecDataPoint", "invalid raw data size for converting value to qint64");
    qint64 value = 0;
    value |= (static_cast<qint64>(m_rawData.at(3)) << 48);
    value |= (static_cast<qint64>(m_rawData.at(2)) << 32);
    value |= (static_cast<qint64>(m_rawData.at(1)) << 16);
    value |= static_cast<qint64>(m_rawData.at(0));
    return value;
}

QString SunSpecDataPoint::toString() const
{
    QByteArray bytes;
    for (int i = 0; i < m_rawData.count(); i++) {
        bytes.append(m_rawData.at(i) >> 8);
        bytes.append(m_rawData.at(i) & 0xFF);
    }

    return QString::fromUtf8(bytes).trimmed();
}

float SunSpecDataPoint::toFloat() const
{
    Q_ASSERT_X(m_dataType == Float32,  "SunSpecDataPoint", "invalid raw data size for converting value to float");
    quint32 rawValue = toUInt32();
    float value = 0;
    memcpy(&value, &rawValue, sizeof(quint32));
    return value;
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
    quint64 rawValue = toUInt64();
    double value = 0;
    memcpy(&value, &rawValue, sizeof(quint64));
    return value;
}

QString SunSpecDataPoint::registersToString(const QVector<quint16> &registers)
{
    QStringList valueStrings;
    for (int i = 0; i < registers.count(); i++) {
        QString hexString(QStringLiteral("0x%1"));
        valueStrings.append(hexString.arg(registers.at(i), 4, 16, QLatin1Char('0')));
    }

    QString registersString = "(" + valueStrings.join(", ") + ")";
    return registersString;
}

QVector<quint16> SunSpecDataPoint::convertFromUInt16(quint16 value)
{
    return QVector<quint16>() << value;
}

QVector<quint16> SunSpecDataPoint::convertFromInt16(qint16 value)
{
    return SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(value));
}

QVector<quint16> SunSpecDataPoint::convertFromUInt32(quint32 value)
{
    QVector<quint16> values;
    values << static_cast<quint16>(value | 0x000000FF);
    values << static_cast<quint16>(value >> 16);
    return values;
}

QVector<quint16> SunSpecDataPoint::convertFromInt32(qint32 value)
{
    return SunSpecDataPoint::convertFromUInt32(static_cast<quint32>(value));
}

QDebug operator<<(QDebug debug, const SunSpecDataPoint &dataPoint)
{
    debug.nospace().noquote() << "DataPoint(";
    if (dataPoint.description().isEmpty()) {
        debug.nospace().noquote() << dataPoint.name();
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

