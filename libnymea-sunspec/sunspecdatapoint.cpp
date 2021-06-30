#include "sunspecdatapoint.h"

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
    } else if (typString == "string") {
        dataType = DataType::String;
    } else if (typString == "sf") {
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
