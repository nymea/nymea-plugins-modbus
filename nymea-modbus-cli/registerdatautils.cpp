// SPDX-License-Identifier: GPL-3.0-or-later

#include "registerdatautils.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QRegularExpression>
#include <QTextStream>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

namespace {

QString registerTypeName(QModbusDataUnit::RegisterType type)
{
    switch (type) {
    case QModbusDataUnit::InputRegisters: return QStringLiteral("input");
    case QModbusDataUnit::HoldingRegisters: return QStringLiteral("holding");
    case QModbusDataUnit::DiscreteInputs: return QStringLiteral("discrete");
    case QModbusDataUnit::Coils: return QStringLiteral("coils");
    default: return QStringLiteral("invalid");
    }
}

QString registerItemName(QModbusDataUnit::RegisterType type, int count)
{
    const bool plural = count != 1;
    switch (type) {
    case QModbusDataUnit::InputRegisters:
        return plural ? QStringLiteral("input registers") : QStringLiteral("input register");
    case QModbusDataUnit::HoldingRegisters:
        return plural ? QStringLiteral("holding registers") : QStringLiteral("holding register");
    case QModbusDataUnit::DiscreteInputs:
        return plural ? QStringLiteral("discrete inputs") : QStringLiteral("discrete input");
    case QModbusDataUnit::Coils:
        return plural ? QStringLiteral("coils") : QStringLiteral("coil");
    default:
        return plural ? QStringLiteral("values") : QStringLiteral("value");
    }
}

QString rawWord(quint16 value)
{
    return QStringLiteral("0x%1").arg(value, 4, 16, QLatin1Char('0'));
}

quint16 swappedWord(quint16 value)
{
    return static_cast<quint16>((value >> 8) | (value << 8));
}

QString escapedAsciiByte(quint8 value)
{
    switch (value) {
    case 0: return QStringLiteral("\\0");
    case '\n': return QStringLiteral("\\n");
    case '\r': return QStringLiteral("\\r");
    case '\t': return QStringLiteral("\\t");
    default:
        if (value >= 0x20 && value <= 0x7e)
            return QString(QChar(value));
        return QStringLiteral("\\x%1").arg(value, 2, 16, QLatin1Char('0'));
    }
}

QString escapedAscii(const QByteArray &bytes)
{
    QString result;
    for (char byte : bytes)
        result.append(escapedAsciiByte(static_cast<quint8>(byte)));
    return result;
}

QByteArray wireBytes(const QVector<quint16> &values)
{
    QByteArray bytes;
    bytes.reserve(values.size() * 2);
    for (quint16 value : values) {
        bytes.append(static_cast<char>(value >> 8));
        bytes.append(static_cast<char>(value & 0xff));
    }
    return bytes;
}

QByteArray decodedBytes(const QVector<quint16> &values,
                        int offset,
                        int wordCount,
                        RegisterByteOrder byteOrder,
                        RegisterWordOrder wordOrder)
{
    QVector<quint16> words;
    words.reserve(wordCount);
    for (int i = 0; i < wordCount; ++i)
        words.append(values.at(offset + i));
    if (wordOrder == RegisterWordOrder::Reverse)
        std::reverse(words.begin(), words.end());

    QByteArray bytes;
    bytes.reserve(wordCount * 2);
    for (quint16 word : words) {
        if (byteOrder == RegisterByteOrder::BigEndian) {
            bytes.append(static_cast<char>(word >> 8));
            bytes.append(static_cast<char>(word & 0xff));
        } else {
            bytes.append(static_cast<char>(word & 0xff));
            bytes.append(static_cast<char>(word >> 8));
        }
    }
    return bytes;
}

quint64 unsignedFromBytes(const QByteArray &bytes)
{
    quint64 value = 0;
    for (char byte : bytes)
        value = (value << 8) | static_cast<quint8>(byte);
    return value;
}

QString byteOrderName(RegisterByteOrder order)
{
    return order == RegisterByteOrder::BigEndian ? QStringLiteral("big")
                                                  : QStringLiteral("little");
}

QString wordOrderName(RegisterWordOrder order)
{
    return order == RegisterWordOrder::Forward ? QStringLiteral("forward")
                                                : QStringLiteral("reverse");
}

struct DecodedValue
{
    QString display;
    QJsonValue jsonValue;
    QString valueText;
};

DecodedValue decodeValue(RegisterDecodeType type, const QByteArray &bytes)
{
    const quint64 bits = unsignedFromBytes(bytes);
    DecodedValue result;
    switch (type) {
    case RegisterDecodeType::UInt16: {
        const quint16 value = static_cast<quint16>(bits);
        result.display = QString::number(value);
        result.jsonValue = static_cast<int>(value);
        break;
    }
    case RegisterDecodeType::Int16: {
        const qint16 value = static_cast<qint16>(static_cast<quint16>(bits));
        result.display = QString::number(value);
        result.jsonValue = static_cast<int>(value);
        break;
    }
    case RegisterDecodeType::UInt32: {
        const quint32 value = static_cast<quint32>(bits);
        result.display = QString::number(value);
        result.jsonValue = static_cast<qint64>(value);
        break;
    }
    case RegisterDecodeType::Int32: {
        const qint32 value = static_cast<qint32>(static_cast<quint32>(bits));
        result.display = QString::number(value);
        result.jsonValue = value;
        break;
    }
    case RegisterDecodeType::UInt64:
        result.display = QString::number(bits);
        result.jsonValue = result.display;
        break;
    case RegisterDecodeType::Int64: {
        qint64 value = 0;
        std::memcpy(&value, &bits, sizeof(value));
        result.display = QString::number(value);
        result.jsonValue = result.display;
        break;
    }
    case RegisterDecodeType::Float32: {
        const quint32 raw = static_cast<quint32>(bits);
        float value = 0;
        std::memcpy(&value, &raw, sizeof(value));
        result.display = QString::number(value, 'g', std::numeric_limits<float>::max_digits10);
        if (std::isfinite(value)) {
            result.jsonValue = static_cast<double>(value);
        } else {
            result.jsonValue = QJsonValue(QJsonValue::Null);
            result.valueText = std::isnan(value) ? QStringLiteral("nan")
                                                 : (value < 0 ? QStringLiteral("-infinity")
                                                              : QStringLiteral("infinity"));
            result.display = result.valueText;
        }
        break;
    }
    case RegisterDecodeType::Float64: {
        double value = 0;
        std::memcpy(&value, &bits, sizeof(value));
        result.display = QString::number(value, 'g', std::numeric_limits<double>::max_digits10);
        if (std::isfinite(value)) {
            result.jsonValue = value;
        } else {
            result.jsonValue = QJsonValue(QJsonValue::Null);
            result.valueText = std::isnan(value) ? QStringLiteral("nan")
                                                 : (value < 0 ? QStringLiteral("-infinity")
                                                              : QStringLiteral("infinity"));
            result.display = result.valueText;
        }
        break;
    }
    }
    return result;
}

QJsonArray decodedJson(const QVector<quint16> &values,
                       quint16 startAddress,
                       const RegisterOutputOptions &options)
{
    QJsonArray decoded;
    for (RegisterDecodeType type : options.decodeTypes) {
        const int wordCount = registerDecodeWordCount(type);
        for (int offset = 0; offset < values.size(); offset += wordCount) {
            const QByteArray bytes = decodedBytes(values, offset, wordCount,
                                                  options.byteOrder, options.wordOrder);
            const DecodedValue decodedValue = decodeValue(type, bytes);
            QJsonObject object;
            object.insert(QStringLiteral("startAddress"), startAddress + offset);
            object.insert(QStringLiteral("type"), registerDecodeTypeName(type));
            object.insert(QStringLiteral("byteOrder"), byteOrderName(options.byteOrder));
            object.insert(QStringLiteral("wordOrder"), wordOrderName(options.wordOrder));
            object.insert(QStringLiteral("rawHex"), QStringLiteral("0x") + QString::fromLatin1(bytes.toHex()));
            object.insert(QStringLiteral("value"), decodedValue.jsonValue);
            if (!decodedValue.valueText.isEmpty())
                object.insert(QStringLiteral("valueText"), decodedValue.valueText);
            decoded.append(object);
        }
    }
    return decoded;
}

QString formatDecodedTable(const QVector<quint16> &values,
                           quint16 startAddress,
                           const RegisterOutputOptions &options)
{
    QString output;
    QTextStream stream(&output);
    for (RegisterDecodeType type : options.decodeTypes) {
        const int wordCount = registerDecodeWordCount(type);
        stream << '\n' << "Decoded " << registerDecodeTypeName(type)
               << ", byte-order=" << byteOrderName(options.byteOrder)
               << ", word-order=" << wordOrderName(options.wordOrder) << ":\n";
        for (int offset = 0; offset < values.size(); offset += wordCount) {
            const QByteArray bytes = decodedBytes(values, offset, wordCount,
                                                  options.byteOrder, options.wordOrder);
            const DecodedValue value = decodeValue(type, bytes);
            stream << startAddress + offset << '-' << startAddress + offset + wordCount - 1
                   << "  0x" << bytes.toHex() << "  " << value.display << '\n';
        }
    }
    return output;
}

QJsonObject baseJson(const QString &operation,
                     QModbusDataUnit::RegisterType type,
                     quint16 startAddress,
                     int count,
                     quint16 serverAddress)
{
    QJsonObject object;
    object.insert(QStringLiteral("operation"), operation);
    object.insert(QStringLiteral("unit"), serverAddress);
    object.insert(QStringLiteral("registerType"), registerTypeName(type));
    object.insert(QStringLiteral("startAddress"), startAddress);
    object.insert(QStringLiteral("count"), count);
    return object;
}

bool parseUnsignedWord(const QString &token, quint16 *value)
{
    QString normalized = token.trimmed();
    int base = 10;
    if (normalized.startsWith(QStringLiteral("0x"), Qt::CaseInsensitive)) {
        normalized.remove(0, 2);
        base = 16;
    }
    if (normalized.isEmpty())
        return false;
    bool ok = false;
    const quint64 parsed = normalized.toULongLong(&ok, base);
    if (!ok || parsed > std::numeric_limits<quint16>::max())
        return false;
    *value = static_cast<quint16>(parsed);
    return true;
}

}

bool parseRegisterOutputFormat(const QString &text, RegisterOutputFormat *format)
{
    const QString normalized = text.toLower();
    if (normalized == QStringLiteral("table"))
        *format = RegisterOutputFormat::Table;
    else if (normalized == QStringLiteral("json"))
        *format = RegisterOutputFormat::Json;
    else if (normalized == QStringLiteral("legacy"))
        *format = RegisterOutputFormat::Legacy;
    else
        return false;
    return true;
}

bool parseRegisterByteOrder(const QString &text, RegisterByteOrder *order)
{
    const QString normalized = text.toLower();
    if (normalized == QStringLiteral("big"))
        *order = RegisterByteOrder::BigEndian;
    else if (normalized == QStringLiteral("little"))
        *order = RegisterByteOrder::LittleEndian;
    else
        return false;
    return true;
}

bool parseRegisterWordOrder(const QString &text, RegisterWordOrder *order)
{
    const QString normalized = text.toLower();
    if (normalized == QStringLiteral("forward"))
        *order = RegisterWordOrder::Forward;
    else if (normalized == QStringLiteral("reverse"))
        *order = RegisterWordOrder::Reverse;
    else
        return false;
    return true;
}

bool parseRegisterDecodeType(const QString &text, RegisterDecodeType *type)
{
    const QString normalized = text.toLower();
    if (normalized == QStringLiteral("uint16")) *type = RegisterDecodeType::UInt16;
    else if (normalized == QStringLiteral("int16")) *type = RegisterDecodeType::Int16;
    else if (normalized == QStringLiteral("uint32")) *type = RegisterDecodeType::UInt32;
    else if (normalized == QStringLiteral("int32")) *type = RegisterDecodeType::Int32;
    else if (normalized == QStringLiteral("uint64")) *type = RegisterDecodeType::UInt64;
    else if (normalized == QStringLiteral("int64")) *type = RegisterDecodeType::Int64;
    else if (normalized == QStringLiteral("float32")) *type = RegisterDecodeType::Float32;
    else if (normalized == QStringLiteral("float64")) *type = RegisterDecodeType::Float64;
    else return false;
    return true;
}

QString registerDecodeTypeName(RegisterDecodeType type)
{
    switch (type) {
    case RegisterDecodeType::UInt16: return QStringLiteral("uint16");
    case RegisterDecodeType::Int16: return QStringLiteral("int16");
    case RegisterDecodeType::UInt32: return QStringLiteral("uint32");
    case RegisterDecodeType::Int32: return QStringLiteral("int32");
    case RegisterDecodeType::UInt64: return QStringLiteral("uint64");
    case RegisterDecodeType::Int64: return QStringLiteral("int64");
    case RegisterDecodeType::Float32: return QStringLiteral("float32");
    case RegisterDecodeType::Float64: return QStringLiteral("float64");
    }
    return QString();
}

int registerDecodeWordCount(RegisterDecodeType type)
{
    switch (type) {
    case RegisterDecodeType::UInt16:
    case RegisterDecodeType::Int16: return 1;
    case RegisterDecodeType::UInt32:
    case RegisterDecodeType::Int32:
    case RegisterDecodeType::Float32: return 2;
    case RegisterDecodeType::UInt64:
    case RegisterDecodeType::Int64:
    case RegisterDecodeType::Float64: return 4;
    }
    return 0;
}

bool parseHoldingWriteValues(const QString &text, QVector<quint16> *values, QString *errorString)
{
    values->clear();
    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        *errorString = QStringLiteral("Write data must not be empty.");
        return false;
    }

    const bool isList = trimmed.contains(QLatin1Char(','))
                        || trimmed.contains(QRegularExpression(QStringLiteral("\\s")));
    if (!isList && trimmed.startsWith(QStringLiteral("0x"), Qt::CaseInsensitive)) {
        QString hex = trimmed.mid(2);
        static const QRegularExpression hexPattern(QStringLiteral("^[0-9a-fA-F]+$"));
        if (!hexPattern.match(hex).hasMatch()) {
            *errorString = QStringLiteral("Compact hexadecimal write data contains invalid characters.");
            return false;
        }
        const int padding = (4 - (hex.size() % 4)) % 4;
        hex.prepend(QString(padding, QLatin1Char('0')));
        for (int offset = 0; offset < hex.size(); offset += 4) {
            bool ok = false;
            const quint16 word = hex.mid(offset, 4).toUShort(&ok, 16);
            if (!ok) {
                *errorString = QStringLiteral("Could not parse compact hexadecimal write data.");
                return false;
            }
            values->append(word);
        }
        return true;
    }

    const QStringList tokens = trimmed.split(QRegularExpression(QStringLiteral("[,\\s]+")), Qt::SkipEmptyParts);
    for (const QString &token : tokens) {
        quint16 value = 0;
        if (!parseUnsignedWord(token, &value)) {
            *errorString = QStringLiteral("Invalid 16-bit register value: %1").arg(token);
            values->clear();
            return false;
        }
        values->append(value);
    }
    return !values->isEmpty();
}

bool parseCoilWriteValues(const QString &text, QVector<quint16> *values, QString *errorString)
{
    values->clear();
    const QStringList tokens = text.trimmed().split(QRegularExpression(QStringLiteral("[,\\s]+")), Qt::SkipEmptyParts);
    for (const QString &token : tokens) {
        const QString normalized = token.toLower();
        if (normalized == QStringLiteral("1") || normalized == QStringLiteral("true"))
            values->append(1);
        else if (normalized == QStringLiteral("0") || normalized == QStringLiteral("false"))
            values->append(0);
        else {
            *errorString = QStringLiteral("Invalid coil value %1; use 0, 1, false, or true.").arg(token);
            values->clear();
            return false;
        }
    }
    if (values->isEmpty()) {
        *errorString = QStringLiteral("Coil write data must not be empty.");
        return false;
    }
    return true;
}

bool validateModbusRequest(QModbusDataUnit::RegisterType type,
                           quint16 startAddress,
                           quint32 count,
                           bool write,
                           QString *errorString)
{
    if (count == 0) {
        *errorString = QStringLiteral("The register count must be greater than zero.");
        return false;
    }
    if (static_cast<quint32>(startAddress) + count > 65536u) {
        *errorString = QStringLiteral("The register range exceeds address 65535.");
        return false;
    }

    quint32 maximum = 0;
    if (write) {
        if (type == QModbusDataUnit::HoldingRegisters)
            maximum = 123;
        else if (type == QModbusDataUnit::Coils)
            maximum = 1968;
        else {
            *errorString = QStringLiteral("Only holding registers and coils can be written.");
            return false;
        }
    } else {
        maximum = (type == QModbusDataUnit::HoldingRegisters
                   || type == QModbusDataUnit::InputRegisters) ? 125 : 2000;
    }
    if (count > maximum) {
        *errorString = QStringLiteral("The request count %1 exceeds the Modbus limit %2 for this operation.")
                           .arg(count).arg(maximum);
        return false;
    }
    return true;
}

QString formatReadResult(QModbusDataUnit::RegisterType type,
                         quint16 startAddress,
                         const QVector<quint16> &values,
                         quint16 serverAddress,
                         const RegisterOutputOptions &options)
{
    const bool bits = type == QModbusDataUnit::Coils || type == QModbusDataUnit::DiscreteInputs;
    if (options.format == RegisterOutputFormat::Legacy) {
        QString output;
        QTextStream stream(&output);
        for (int i = 0; i < values.size(); ++i)
            stream << "\"-->\" " << startAddress + i << " : \"" << rawWord(values.at(i))
                   << "\" " << values.at(i) << '\n';
        return output;
    }

    const QByteArray bytes = bits ? QByteArray() : wireBytes(values);
    if (options.format == RegisterOutputFormat::Json) {
        QJsonObject root = baseJson(QStringLiteral("read"), type, startAddress, values.size(), serverAddress);
        QJsonArray registers;
        for (int i = 0; i < values.size(); ++i) {
            const quint16 value = values.at(i);
            QJsonObject object;
            object.insert(QStringLiteral("address"), startAddress + i);
            object.insert(QStringLiteral("rawHex"), rawWord(value));
            if (bits) {
                object.insert(QStringLiteral("value"), value != 0);
            } else {
                const quint16 little = swappedWord(value);
                object.insert(QStringLiteral("uint16BE"), static_cast<int>(value));
                object.insert(QStringLiteral("int16BE"), static_cast<int>(static_cast<qint16>(value)));
                object.insert(QStringLiteral("uint16LE"), static_cast<int>(little));
                object.insert(QStringLiteral("int16LE"), static_cast<int>(static_cast<qint16>(little)));
                const QByteArray wordBytes = wireBytes({value});
                object.insert(QStringLiteral("ascii"), QString::fromLatin1(wordBytes));
                object.insert(QStringLiteral("asciiEscaped"), escapedAscii(wordBytes));
            }
            registers.append(object);
        }
        root.insert(QStringLiteral("registers"), registers);
        if (!bits) {
            QJsonObject bulk;
            bulk.insert(QStringLiteral("rawHex"), QStringLiteral("0x") + QString::fromLatin1(bytes.toHex()));
            bulk.insert(QStringLiteral("ascii"), QString::fromLatin1(bytes));
            bulk.insert(QStringLiteral("asciiEscaped"), escapedAscii(bytes));
            root.insert(QStringLiteral("bulk"), bulk);
            root.insert(QStringLiteral("decoded"), decodedJson(values, startAddress, options));
        }
        return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact)) + QLatin1Char('\n');
    }

    QString output;
    QTextStream stream(&output);
    stream << "Read " << values.size() << ' ' << registerItemName(type, values.size())
           << " at address " << startAddress << " (unit " << serverAddress << ")\n";
    if (bits) {
        stream << "Address  Raw     Boolean\n";
        for (int i = 0; i < values.size(); ++i)
            stream << QStringLiteral("%1  %2  %3\n")
                          .arg(startAddress + i, 7)
                          .arg(rawWord(values.at(i)), -7)
                          .arg(values.at(i) ? QStringLiteral("true") : QStringLiteral("false"));
        return output;
    }

    stream << "Address  Raw     UInt16-BE  Int16-BE  UInt16-LE  Int16-LE  ASCII\n";
    for (int i = 0; i < values.size(); ++i) {
        const quint16 value = values.at(i);
        const quint16 little = swappedWord(value);
        stream << QStringLiteral("%1  %2  %3  %4  %5  %6  %7\n")
                      .arg(startAddress + i, 7)
                      .arg(rawWord(value), -7)
                      .arg(value, 9)
                      .arg(static_cast<qint16>(value), 8)
                      .arg(little, 9)
                      .arg(static_cast<qint16>(little), 8)
                      .arg(escapedAscii(wireBytes({value})));
    }
    stream << "\nBulk raw:   0x" << bytes.toHex() << '\n';
    stream << "Bulk ASCII: " << escapedAscii(bytes) << '\n';
    stream << formatDecodedTable(values, startAddress, options);
    return output;
}

QString formatWriteResult(QModbusDataUnit::RegisterType type,
                          quint16 startAddress,
                          const QVector<quint16> &values,
                          quint16 serverAddress,
                          bool broadcast,
                          const RegisterOutputOptions &options)
{
    const bool coils = type == QModbusDataUnit::Coils;
    if (options.format == RegisterOutputFormat::Json) {
        QJsonObject root = baseJson(QStringLiteral("write"), type, startAddress, values.size(), serverAddress);
        root.insert(QStringLiteral("broadcast"), broadcast);
        root.insert(QStringLiteral("success"), true);
        QJsonArray written;
        for (int i = 0; i < values.size(); ++i) {
            QJsonObject object;
            object.insert(QStringLiteral("address"), startAddress + i);
            object.insert(QStringLiteral("rawHex"), rawWord(values.at(i)));
            object.insert(QStringLiteral("value"), coils ? QJsonValue(values.at(i) != 0)
                                                         : QJsonValue(static_cast<int>(values.at(i))));
            written.append(object);
        }
        root.insert(QStringLiteral("values"), written);
        return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact)) + QLatin1Char('\n');
    }

    QString output;
    QTextStream stream(&output);
    stream << (broadcast ? QStringLiteral("Broadcast write queued for ") : QStringLiteral("Wrote "))
           << values.size() << ' ' << registerItemName(type, values.size())
           << " at address " << startAddress << " (unit " << serverAddress << ")\n";
    for (int i = 0; i < values.size(); ++i) {
        stream << startAddress + i << "  " << rawWord(values.at(i)) << "  ";
        if (coils)
            stream << (values.at(i) ? "true" : "false");
        else
            stream << values.at(i);
        stream << '\n';
    }
    return output;
}
