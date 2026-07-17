// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef REGISTERDATAUTILS_H
#define REGISTERDATAUTILS_H

#include <QJsonObject>
#include <QModbusDataUnit>
#include <QString>
#include <QStringList>
#include <QVector>

enum class RegisterOutputFormat {
    Table,
    Json,
    Legacy
};

enum class RegisterByteOrder {
    BigEndian,
    LittleEndian
};

enum class RegisterWordOrder {
    Forward,
    Reverse
};

enum class RegisterDecodeType {
    UInt16,
    Int16,
    UInt32,
    Int32,
    UInt64,
    Int64,
    Float32,
    Float64
};

struct RegisterOutputOptions
{
    RegisterOutputFormat format = RegisterOutputFormat::Table;
    RegisterByteOrder byteOrder = RegisterByteOrder::BigEndian;
    RegisterWordOrder wordOrder = RegisterWordOrder::Forward;
    QVector<RegisterDecodeType> decodeTypes;
};

bool parseRegisterOutputFormat(const QString &text, RegisterOutputFormat *format);
bool parseRegisterByteOrder(const QString &text, RegisterByteOrder *order);
bool parseRegisterWordOrder(const QString &text, RegisterWordOrder *order);
bool parseRegisterDecodeType(const QString &text, RegisterDecodeType *type);

QString registerDecodeTypeName(RegisterDecodeType type);
int registerDecodeWordCount(RegisterDecodeType type);

bool parseHoldingWriteValues(const QString &text, QVector<quint16> *values, QString *errorString);
bool parseCoilWriteValues(const QString &text, QVector<quint16> *values, QString *errorString);

bool validateModbusRequest(QModbusDataUnit::RegisterType type,
                           quint16 startAddress,
                           quint32 count,
                           bool write,
                           QString *errorString);

QString formatReadResult(QModbusDataUnit::RegisterType type,
                         quint16 startAddress,
                         const QVector<quint16> &values,
                         quint16 serverAddress,
                         const RegisterOutputOptions &options);

QString formatWriteResult(QModbusDataUnit::RegisterType type,
                          quint16 startAddress,
                          const QVector<quint16> &values,
                          quint16 serverAddress,
                          bool broadcast,
                          const RegisterOutputOptions &options);

#endif // REGISTERDATAUTILS_H
