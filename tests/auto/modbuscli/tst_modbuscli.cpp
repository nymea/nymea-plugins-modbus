// SPDX-License-Identifier: GPL-3.0-or-later

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

#include "registerdatautils.h"

class TestModbusCli : public QObject
{
    Q_OBJECT

private slots:
    void parseCompactHexWrite()
    {
        QVector<quint16> values;
        QString error;
        QVERIFY(parseHoldingWriteValues(QStringLiteral("0x00010002"), &values, &error));
        QCOMPARE(values, QVector<quint16>({0x0001, 0x0002}));

        QVERIFY(parseHoldingWriteValues(QStringLiteral("0x10002"), &values, &error));
        QCOMPARE(values, QVector<quint16>({0x0001, 0x0002}));

        QVERIFY(parseHoldingWriteValues(QStringLiteral("0x1"), &values, &error));
        QCOMPARE(values, QVector<quint16>({0x0001}));
    }

    void parseHoldingWriteLists()
    {
        QVector<quint16> values;
        QString error;
        QVERIFY(parseHoldingWriteValues(QStringLiteral("1, 0x0002,65535"), &values, &error));
        QCOMPARE(values, QVector<quint16>({1, 2, 65535}));

        QVERIFY(!parseHoldingWriteValues(QStringLiteral("65536"), &values, &error));
        QVERIFY(error.contains(QStringLiteral("16-bit")));
        QVERIFY(!parseHoldingWriteValues(QStringLiteral("0xzzzz"), &values, &error));
        QVERIFY(!parseHoldingWriteValues(QString(), &values, &error));
    }

    void parseCoilWriteLists()
    {
        QVector<quint16> values;
        QString error;
        QVERIFY(parseCoilWriteValues(QStringLiteral("true,false,1,0,TRUE"), &values, &error));
        QCOMPARE(values, QVector<quint16>({1, 0, 1, 0, 1}));
        QVERIFY(!parseCoilWriteValues(QStringLiteral("true,on"), &values, &error));
    }

    void validateRequestLimits()
    {
        QString error;
        QVERIFY(validateModbusRequest(QModbusDataUnit::HoldingRegisters, 0, 125, false, &error));
        QVERIFY(!validateModbusRequest(QModbusDataUnit::HoldingRegisters, 0, 126, false, &error));
        QVERIFY(validateModbusRequest(QModbusDataUnit::HoldingRegisters, 0, 123, true, &error));
        QVERIFY(!validateModbusRequest(QModbusDataUnit::HoldingRegisters, 0, 124, true, &error));
        QVERIFY(validateModbusRequest(QModbusDataUnit::Coils, 0, 1968, true, &error));
        QVERIFY(!validateModbusRequest(QModbusDataUnit::Coils, 0, 1969, true, &error));
        QVERIFY(!validateModbusRequest(QModbusDataUnit::InputRegisters, 0, 1, true, &error));
        QVERIFY(!validateModbusRequest(QModbusDataUnit::HoldingRegisters, 65535, 2, false, &error));
    }

    void diagnosticTableAndAscii()
    {
        RegisterOutputOptions options;
        const QString output = formatReadResult(QModbusDataUnit::HoldingRegisters, 100,
                                                {0x4142, 0x000a}, 1, options);
        QVERIFY(output.contains(QStringLiteral("Address  Raw")));
        QVERIFY(output.contains(QStringLiteral("100  0x4142")));
        QVERIFY(output.contains(QStringLiteral("AB")));
        QVERIFY(output.contains(QStringLiteral("Bulk raw:   0x4142000a")));
        QVERIFY(output.contains(QStringLiteral("Bulk ASCII: AB\\0\\n")));
    }

    void endianCombinations()
    {
        const QVector<quint16> values({0x1122, 0x3344});
        RegisterOutputOptions options;
        options.format = RegisterOutputFormat::Json;
        options.decodeTypes = {RegisterDecodeType::UInt32};

        auto decodedRaw = [&]() {
            const QJsonDocument document = QJsonDocument::fromJson(
                formatReadResult(QModbusDataUnit::HoldingRegisters, 10, values, 1, options).toUtf8());
            return document.object().value(QStringLiteral("decoded")).toArray().first()
                .toObject().value(QStringLiteral("rawHex")).toString();
        };

        QCOMPARE(decodedRaw(), QStringLiteral("0x11223344"));
        options.byteOrder = RegisterByteOrder::LittleEndian;
        QCOMPARE(decodedRaw(), QStringLiteral("0x22114433"));
        options.byteOrder = RegisterByteOrder::BigEndian;
        options.wordOrder = RegisterWordOrder::Reverse;
        QCOMPARE(decodedRaw(), QStringLiteral("0x33441122"));
        options.byteOrder = RegisterByteOrder::LittleEndian;
        QCOMPARE(decodedRaw(), QStringLiteral("0x44332211"));
    }

    void integerAndFloatDecoding()
    {
        RegisterOutputOptions options;
        options.format = RegisterOutputFormat::Json;
        options.decodeTypes = {RegisterDecodeType::Int32, RegisterDecodeType::Float32};
        const QJsonDocument document = QJsonDocument::fromJson(
            formatReadResult(QModbusDataUnit::HoldingRegisters, 20,
                             {0x3f80, 0x0000}, 1, options).toUtf8());
        const QJsonArray decoded = document.object().value(QStringLiteral("decoded")).toArray();
        QCOMPARE(decoded.at(0).toObject().value(QStringLiteral("value")).toDouble(), 1065353216.0);
        QCOMPARE(decoded.at(1).toObject().value(QStringLiteral("value")).toDouble(), 1.0);

        options.decodeTypes = {RegisterDecodeType::Int32};
        const QJsonDocument negativeDocument = QJsonDocument::fromJson(
            formatReadResult(QModbusDataUnit::HoldingRegisters, 20,
                             {0xffff, 0xfffe}, 1, options).toUtf8());
        QCOMPARE(negativeDocument.object().value(QStringLiteral("decoded")).toArray().first()
                     .toObject().value(QStringLiteral("value")).toDouble(), -2.0);
    }

    void nonFiniteFloatJson()
    {
        RegisterOutputOptions options;
        options.format = RegisterOutputFormat::Json;
        options.decodeTypes = {RegisterDecodeType::Float32};
        const QJsonDocument document = QJsonDocument::fromJson(
            formatReadResult(QModbusDataUnit::HoldingRegisters, 0,
                             {0x7f80, 0x0000}, 1, options).toUtf8());
        const QJsonObject decoded = document.object().value(QStringLiteral("decoded")).toArray().first().toObject();
        QVERIFY(decoded.value(QStringLiteral("value")).isNull());
        QCOMPARE(decoded.value(QStringLiteral("valueText")).toString(), QStringLiteral("infinity"));
    }

    void jsonAndLegacyOutput()
    {
        RegisterOutputOptions options;
        options.format = RegisterOutputFormat::Json;
        options.decodeTypes = {RegisterDecodeType::UInt64};
        const QJsonDocument document = QJsonDocument::fromJson(
            formatReadResult(QModbusDataUnit::HoldingRegisters, 1,
                             {0xffff, 0xffff, 0xffff, 0xffff}, 7, options).toUtf8());
        QVERIFY(document.isObject());
        QCOMPARE(document.object().value(QStringLiteral("unit")).toInt(), 7);
        QVERIFY(document.object().value(QStringLiteral("decoded")).toArray().first()
                    .toObject().value(QStringLiteral("value")).isString());

        options.format = RegisterOutputFormat::Legacy;
        const QString legacy = formatReadResult(QModbusDataUnit::HoldingRegisters, 10, {1}, 1, options);
        QCOMPARE(legacy, QStringLiteral("\"-->\" 10 : \"0x0001\" 1\n"));
    }

    void writeOutput()
    {
        RegisterOutputOptions options;
        QString output = formatWriteResult(QModbusDataUnit::HoldingRegisters, 100,
                                           {1, 2}, 1, false, options);
        QVERIFY(output.contains(QStringLiteral("Wrote 2 holding registers")));
        QVERIFY(output.contains(QStringLiteral("100  0x0001  1")));
        QVERIFY(output.contains(QStringLiteral("101  0x0002  2")));

        options.format = RegisterOutputFormat::Json;
        const QJsonDocument document = QJsonDocument::fromJson(
            formatWriteResult(QModbusDataUnit::Coils, 20, {1, 0}, 0, true, options).toUtf8());
        QVERIFY(document.object().value(QStringLiteral("broadcast")).toBool());
        QCOMPARE(document.object().value(QStringLiteral("values")).toArray().size(), 2);
    }
};

QTEST_GUILESS_MAIN(TestModbusCli)

#include "tst_modbuscli.moc"
