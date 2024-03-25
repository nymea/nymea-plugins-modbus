/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#ifndef MODBUSDATAUTILS_H
#define MODBUSDATAUTILS_H

#include <QVector>
#include <QObject>
#include <QModbusPdu>
#include <QModbusDataUnit>

class ModbusDataUtils
{
    Q_GADGET
public:
    enum Access {
        AccessReadOnly,
        AccessWriteOnly,
        AccessReadWrite
    };
    Q_ENUM(Access)

    enum ByteOrder {
        ByteOrderLittleEndian,
        ByteOrderBigEndian
    };
    Q_ENUM(ByteOrder)

    enum DataType {
        UInt8,
        UInt16,
        Uint32,
        Uint64,
        Int8,
        Int16,
        Int32,
        Int64,
        Float,
        Float64,
        String,
        Bool
    };
    Q_ENUM(DataType)

    typedef struct ModbusRegister {
        quint16 address;
        quint16 size;
        DataType dataType;
        Access access;
        QString description;
        QString unit;
        QVector<quint16> rawData;
    } ModbusRegister;

    typedef QVector<ModbusRegister> ModbusRegisters;

    explicit ModbusDataUtils();

    // Convert to
    static quint16 convertToUInt16(const QVector<quint16> &registers);
    static qint16 convertToInt16(const QVector<quint16> &registers);
    static quint32 convertToUInt32(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static qint32 convertToInt32(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static quint64 convertToUInt64(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static qint64 convertToInt64(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QString convertToString(const QVector<quint16> &registers, ByteOrder characterByteOrder = ByteOrderLittleEndian);
    static QByteArray convertToByteArray(const QVector<quint16> &registers);
    static float convertToFloat32(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);
    static double convertToFloat64(const QVector<quint16> &registers, ByteOrder byteOrder = ByteOrderLittleEndian);

    // Convert from
    static QVector<quint16> convertFromUInt16(quint16 value);
    static QVector<quint16> convertFromInt16(qint16 value);
    static QVector<quint16> convertFromUInt32(quint32 value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromInt32(qint32 value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromUInt64(quint64 value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromInt64(qint64 value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromString(const QString &value, quint16 stringLength, ByteOrder characterByteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromFloat32(float value, ByteOrder byteOrder = ByteOrderLittleEndian);
    static QVector<quint16> convertFromFloat64(double value, ByteOrder byteOrder = ByteOrderLittleEndian);

    static QString exceptionCodeToString(QModbusPdu::ExceptionCode exception);
    static QString registerTypeToString(QModbusDataUnit::RegisterType registerType);
};

#endif // MODBUSDATAUTILS_H
