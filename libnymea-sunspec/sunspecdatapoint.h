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

#ifndef SUNSPECDATAPOINT_H
#define SUNSPECDATAPOINT_H

#include <QVector>
#include <QDebug>
#include <QString>
#include <QMetaObject>

class SunSpecDataPoint
{
    Q_GADGET
public:
    enum ByteOrder {
        ByteOrderLittleEndian,
        ByteOrderBigEndian
    };
    Q_ENUM(ByteOrder)

    enum Access {
        AccessReadOnly,
        AccessReadWrite
    };
    Q_ENUM(Access)

    enum DataType {
        Int16,
        Int32,
        Int64,
        UInt16,
        Raw16,
        UInt32,
        Acc16,
        Acc32,
        Acc64,
        BitField16,
        BitField32,
        BitField64,
        Enum16,
        Enum32,
        Float32,
        Float64,
        String,
        ScaleFactor,
        Pad,
        IpV4Address,
        IpV6Address,
        EUI48,
        Group,
        Sync
    };
    Q_ENUM(DataType)

    explicit SunSpecDataPoint();

    QString name() const;
    void setName(const QString &name);

    QString label() const;
    void setLabel(const QString &label);

    QString description() const;
    void setDescription(const QString &description);

    QString detail() const;
    void setDetail(const QString &detail);

    QString units() const;
    void setUnits(const QString &units);

    bool mandatory() const;
    void setMandatory(bool mandatory);

    Access access() const;
    void setAccess(Access access);

    quint16 addressOffset() const;
    void setAddressOffset(quint16 addressOffset);

    quint16 blockOffset() const;
    void setBlockOffset(quint16 blockOffset);

    QString sunSpecDataType() const;
    void setSunSpecDataType(const QString &sunSpecDataType);

    DataType dataType() const;
    void setDataType(DataType dataType);

    ByteOrder byteOrder() const;
    void setByteOrder(ByteOrder byteOrder);

    int size() const;
    void setSize(int size);

    QString scaleFactorName() const;
    void setScaleFactorName(const QString &scaleFactorName);

    QVector<quint16> rawData() const;
    void setRawData(const QVector<quint16> &rawData);

    bool isValid() const;

    static DataType stringToDataType(const QString &typString);

    // Convert to this data point type
    quint16 toUInt16() const;
    qint16 toInt16() const;
    quint32 toUInt32() const;
    qint32 toInt32() const;
    quint64 toUInt64() const;
    qint64 toInt64() const;
    QString toString() const;
    float toFloat() const;
    float toFloatWithSSF(qint16 scaleFactor) const;
    double toDouble() const;

    static QString registersToString(const QVector<quint16> &registers);

    // Convert to
    static quint16 convertToUInt16(const QVector<quint16> &registers);
    static qint16 convertToInt16(const QVector<quint16> &registers);
    static quint32 convertToUInt32(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static qint32 convertToInt32(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static quint64 convertToUInt64(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static qint64 convertToInt64(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QString convertToString(const QVector<quint16> &registers);
    static float convertToFloat32(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static double convertToFloat64(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);

    // Convert from
    static QVector<quint16> convertFromUInt16(quint16 value);
    static QVector<quint16> convertFromInt16(qint16 value);
    static QVector<quint16> convertFromUInt32(quint32 value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromInt32(qint32 value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromUInt64(quint64 value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromInt64(qint64 value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromString(const QString &value, quint16 stringLength);
    static QVector<quint16> convertFromFloatWithSSF(float value, qint16 scaleFactor, DataType dataType, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromFloat32(float value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromFloat64(double value, ByteOrder byteOrder = ByteOrderLittleEndian);

private:
    QString m_name;
    QString m_label;
    QString m_description;
    QString m_detail;
    QString m_units;
    bool m_mandatory = false;
    Access m_access = AccessReadOnly;
    quint16 m_addressOffset = 0;
    quint16 m_blockOffset = 0;
    QString m_sunSpecDataType = "uint16";
    DataType m_dataType = DataType::UInt16;
    ByteOrder m_byteOrder = ByteOrder::ByteOrderLittleEndian; // Default is little endian: 0xAABB 0xCCDD -> 0xCCDDAABB
    int m_size = 1;
    QString m_scaleFactorName;

    QVector<quint16> m_rawData;
};

QDebug operator<<(QDebug debug, const SunSpecDataPoint &dataPoint);


#endif // SUNSPECDATAPOINT_H
