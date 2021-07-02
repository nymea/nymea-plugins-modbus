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
    return true;
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
    case UInt16:
        value = toUInt16() * pow(10, scaleFactor);
        break;
    case Int16:
        value = toInt16() * pow(10, scaleFactor);
        break;
    case UInt32:
        value = toUInt32() * pow(10, scaleFactor);
        break;
    case Int32:
        value = toInt32() * pow(10, scaleFactor);
        break;
    default:
        Q_ASSERT_X(false,  "SunSpecDataPoint", "invalid raw data size for converting value to float with sunspec scale factor");
        break;
    }

    return value;
}

double SunSpecDataPoint::toDouble() const
{
    Q_ASSERT_X(m_dataType == Float64,  "SunSpecDataPoint", "invalid raw data size for converting value to double");
    quint32 rawValue = toUInt64();
    double value = 0;
    memcpy(&value, &rawValue, sizeof(quint64));
    return value;
}
