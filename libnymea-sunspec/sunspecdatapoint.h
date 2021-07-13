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

#ifndef SUNSPECDATAPOINT_H
#define SUNSPECDATAPOINT_H

#include <QObject>
#include <QVector>
#include <QDebug>

class SunSpecDataPoint
{
    Q_GADGET
public:
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
    static quint32 convertToUInt32(const QVector<quint16> &registers);
    static qint32 convertToInt32(const QVector<quint16> &registers);
    static quint64 convertToUInt64(const QVector<quint16> &registers);
    static qint64 convertToInt64(const QVector<quint16> &registers);
    static QString convertToString(const QVector<quint16> &registers);

    // Convert from
    static QVector<quint16> convertFromUInt16(quint16 value);
    static QVector<quint16> convertFromInt16(qint16 value);
    static QVector<quint16> convertFromUInt32(quint32 value);
    static QVector<quint16> convertFromInt32(qint32 value);
    static QVector<quint16> convertFromUInt64(quint64 value);
    static QVector<quint16> convertFromInt64(qint64 value);
    static QVector<quint16> convertFromString(const QString &value, quint16 registerCount);
    static QVector<quint16> convertFromFloatWithSSF(float value, qint16 scaleFactor, DataType dataType);

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
    int m_size = 1;
    QString m_scaleFactorName;

    QVector<quint16> m_rawData;
};

QDebug operator<<(QDebug debug, const SunSpecDataPoint &dataPoint);


#endif // SUNSPECDATAPOINT_H
