// SPDX-License-Identifier: GPL-3.0-or-later

#include <QDataStream>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QPointer>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTest>

#include "pcewallbox.h"

QStringList &nymeaLoggingCategories()
{
    static QStringList categories;
    return categories;
}

Q_LOGGING_CATEGORY(dcPcElectric, "PcElectricTest")

class DelayedModbusServer : public QTcpServer
{
public:
    struct Request {
        quint8 function = 0;
        quint16 address = 0;
        quint16 valueOrQuantity = 0;
        QVector<quint16> values;
        qint64 receivedAt = 0;
        qint64 repliedAt = 0;
    };

    explicit DelayedModbusServer(int responseDelay, QObject *parent = nullptr)
        : QTcpServer(parent),
          m_responseDelay(responseDelay)
    {
        m_elapsed.start();
        connect(this, &QTcpServer::newConnection, this, [this]() {
            while (QTcpSocket *socket = nextPendingConnection()) {
                connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
                    m_buffers[socket].append(socket->readAll());
                    processRequests(socket);
                });
                connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
                connect(socket, &QObject::destroyed, this, [this, socket]() {
                    m_buffers.remove(socket);
                });
            }
        });
    }

    QList<Request> requests() const { return m_requests; }
    void ignoreNextRead(quint16 address) { m_ignoredReadAddress = address; }
    void ignoreAllRequests(bool ignore) { m_ignoreAllRequests = ignore; }
    void setReadValue(quint16 address, quint16 value) { m_readValues[address] = value; }
    void setReadValues(quint16 address, const QVector<quint16> &values)
    {
        for (int offset = 0; offset < values.size(); ++offset)
            m_readValues[address + offset] = values.at(offset);
    }
    void ignoreWritesTo(quint16 address, bool ignore = true)
    {
        if (ignore)
            m_ignoredWriteAddresses.insert(address);
        else
            m_ignoredWriteAddresses.remove(address);
    }
    void failNextWrite(quint16 address) { m_failedWriteAddress = address; }

private:
    void processRequests(QTcpSocket *socket)
    {
        QByteArray &buffer = m_buffers[socket];
        while (buffer.size() >= 7) {
            QDataStream input(buffer);
            quint16 transactionId = 0;
            quint16 protocolId = 0;
            quint16 length = 0;
            quint8 unitId = 0;
            quint8 function = 0;
            quint16 address = 0;
            quint16 quantity = 0;
            input >> transactionId >> protocolId >> length;
            const int requestSize = 6 + length;
            if (buffer.size() < requestSize)
                return;
            input >> unitId >> function >> address >> quantity;
            buffer.remove(0, requestSize);

            Request request;
            request.function = function;
            request.address = address;
            request.valueOrQuantity = quantity;
            if (function == 6) {
                request.values.append(quantity);
            } else if (function == 16) {
                quint8 byteCount = 0;
                input >> byteCount;
                for (quint16 index = 0; index < quantity; ++index) {
                    quint16 value = 0;
                    input >> value;
                    request.values.append(value);
                }
            }
            request.receivedAt = m_elapsed.elapsed();
            const int requestIndex = m_requests.size();
            m_requests.append(request);

            if (m_ignoreAllRequests)
                continue;

            if (function == 3 && address == m_ignoredReadAddress) {
                m_ignoredReadAddress = -1;
                continue;
            }

            QByteArray response;
            QDataStream output(&response, QIODevice::WriteOnly);
            const bool failWrite = (function == 6 || function == 16)
                    && address == m_failedWriteAddress;
            if (failWrite) {
                m_failedWriteAddress = -1;
                output << transactionId << quint16(0) << quint16(3)
                       << unitId << quint8(function | 0x80) << quint8(4);
            } else if (function == 3) {
                output << transactionId << quint16(0) << quint16(3 + quantity * 2)
                       << unitId << function << quint8(quantity * 2);
                for (quint16 offset = 0; offset < quantity; ++offset) {
                    quint16 value = 0;
                    if (m_readValues.contains(address + offset))
                        value = m_readValues.value(address + offset);
                    else if (address + offset == 135)
                        value = 0x3030; // "00"
                    else if (address + offset == 136)
                        value = 0x3332; // "32" (firmware >= 0.25)
                    output << value;
                }
            } else {
                output << transactionId << quint16(0) << quint16(6)
                       << unitId << function << address << quantity;
                if (!m_ignoredWriteAddresses.contains(address)) {
                    for (int offset = 0; offset < request.values.size(); ++offset)
                        m_readValues[address + offset] = request.values.at(offset);
                }
            }

            const QPointer<QTcpSocket> guardedSocket(socket);
            QTimer::singleShot(m_responseDelay, this,
                               [this, guardedSocket, response, requestIndex]() {
                if (!guardedSocket)
                    return;
                guardedSocket->write(response);
                guardedSocket->flush();
                m_requests[requestIndex].repliedAt = m_elapsed.elapsed();
            });
        }
    }

    int m_responseDelay = 0;
    QElapsedTimer m_elapsed;
    QHash<QTcpSocket *, QByteArray> m_buffers;
    QList<Request> m_requests;
    QHash<quint16, quint16> m_readValues;
    QSet<quint16> m_ignoredWriteAddresses;
    int m_ignoredReadAddress = -1;
    bool m_ignoreAllRequests = false;
    int m_failedWriteAddress = -1;
};

class RfidTestWallbox : public PceWallbox
{
public:
    using PceWallbox::PceWallbox;

protected:
    bool rfidTransportAvailable() const override { return rfidEnabled(); }
};

class TestPceWallbox : public QObject
{
    Q_OBJECT

    static QVector<quint16> rfidToken(int uidLength, quint16 seed = 0x0011)
    {
        QVector<quint16> values = {
            static_cast<quint16>(0x0100 | uidLength),
            seed, 0x2233, 0x4455, 0x6677, 0x8899
        };
        return values;
    }

private slots:
    void unsentRequestStillFinishes()
    {
        PceWallbox wallbox(QHostAddress::LocalHost, 1, 1);
        QueuedModbusReply *reply = wallbox.setLedBrightnessAsync(55);
        QVERIFY(reply);
        QSignalSpy finishedSpy(reply, &QueuedModbusReply::finished);
        QModbusDevice::Error error = QModbusDevice::NoError;
        connect(reply, &QueuedModbusReply::finished, this, [reply, &error]() {
            error = reply->error();
        });

        QTRY_COMPARE_WITH_TIMEOUT(finishedSpy.size(), 1, 1000);
        QCOMPARE(error, QModbusDevice::UnknownError);
    }

    void generatedRfidRegisterContract()
    {
        const QString fileName = QFINDTESTDATA("../../../pcelectric/EV11.3-registers.json");
        QVERIFY2(!fileName.isEmpty(), "Could not locate EV11.3-registers.json");
        QFile file(fileName);
        QVERIFY(file.open(QIODevice::ReadOnly));
        const QJsonObject root = QJsonDocument::fromJson(file.readAll()).object();

        QHash<QString, QJsonObject> registers;
        for (const QJsonValue &value : root.value("registers").toArray()) {
            const QJsonObject object = value.toObject();
            registers.insert(object.value("id").toString(), object);
        }

        QCOMPARE(registers.value("rfidOperatingMode").value("address").toInt(), 209);
        QCOMPARE(registers.value("rfidLed").value("address").toInt(), 210);
        QCOMPARE(registers.value("rfidRead").value("address").toInt(), 211);
        QCOMPARE(registers.value("rfidRead").value("size").toInt(), 6);
        QCOMPARE(registers.value("rfidRead").value("access").toString(), QStringLiteral("RW"));
        QVERIFY(registers.value("rfidRead").value("sensitive").toBool());
        QCOMPARE(registers.value("rfidSession").value("address").toInt(), 217);
        QCOMPARE(registers.value("rfidSession").value("size").toInt(), 6);
        QCOMPARE(registers.value("rfidSession").value("access").toString(), QStringLiteral("RW"));
        QVERIFY(registers.value("rfidSession").value("sensitive").toBool());
        QVERIFY(!registers.contains("rfidStatus"));
        QVERIFY(!registers.contains("rfidResult"));
        QVERIFY(!registers.contains("rfidLearn"));

        QJsonArray ledValues;
        for (const QJsonValue &value : root.value("enums").toArray()) {
            const QJsonObject object = value.toObject();
            if (object.value("name").toString() == QStringLiteral("RfidLed"))
                ledValues = object.value("values").toArray();
        }
        QCOMPARE(ledValues.size(), 4);
        QCOMPARE(ledValues.at(0).toObject().value("key").toString(), QStringLiteral("None"));
        QCOMPARE(ledValues.at(0).toObject().value("value").toInt(), 0);
        QCOMPARE(ledValues.at(1).toObject().value("key").toString(), QStringLiteral("Rejected"));
        QCOMPARE(ledValues.at(1).toObject().value("value").toInt(), 1);
        QCOMPARE(ledValues.at(2).toObject().value("key").toString(), QStringLiteral("Accepted"));
        QCOMPARE(ledValues.at(2).toObject().value("value").toInt(), 2);
        QCOMPARE(ledValues.at(3).toObject().value("key").toString(), QStringLiteral("Learn"));
        QCOMPARE(ledValues.at(3).toObject().value("value").toInt(), 3);

        RfidTestWallbox wallbox(QHostAddress::LocalHost, 502, 1);
        QCOMPARE(wallbox.rfidOperatingModeDataUnit().startAddress(), 209);
        QCOMPARE(wallbox.rfidLedDataUnit().startAddress(), 210);
        QCOMPARE(wallbox.rfidReadDataUnit().startAddress(), 211);
        QCOMPARE(wallbox.rfidReadDataUnit().valueCount(), 6u);
        QCOMPARE(wallbox.rfidSessionDataUnit().startAddress(), 217);
        QCOMPARE(wallbox.rfidSessionDataUnit().valueCount(), 6u);
    }

    void parsesCanonicalRfidTokens_data()
    {
        QTest::addColumn<int>("uidLength");
        QTest::addColumn<QString>("expectedCode");
        QTest::newRow("4-byte UID") << 4 << QStringLiteral("00112233");
        QTest::newRow("7-byte UID") << 7 << QStringLiteral("00112233445566");
        QTest::newRow("10-byte UID") << 10 << QStringLiteral("00112233445566778899");
    }

    void parsesCanonicalRfidTokens()
    {
        QFETCH(int, uidLength);
        QFETCH(QString, expectedCode);
        const QVector<quint16> values = rfidToken(uidLength);
        QString code;
        QVERIFY(PceWallbox::parseRfidToken(values, &code));
        QCOMPARE(code, expectedCode);
    }

    void rejectsInvalidRfidTokens()
    {
        QVector<quint16> values = rfidToken(4);
        QVERIFY(!PceWallbox::parseRfidToken(values.mid(0, 5), nullptr));
        values.append(0);
        QVERIFY(!PceWallbox::parseRfidToken(values, nullptr));
        values.removeLast();
        values[0] = 0x0204;
        QVERIFY(!PceWallbox::parseRfidToken(values, nullptr));
        values[0] = 0x0105;
        QVERIFY(!PceWallbox::parseRfidToken(values, nullptr));
        values[0] = 0x0104;
        for (int index = 1; index < values.size(); ++index)
            values[index] = 0;
        QVERIFY(!PceWallbox::parseRfidToken(values, nullptr));
        QVERIFY(!PceWallbox::parseRfidToken(QVector<quint16>(6, 0), nullptr));
    }

    void rfidReadIsDebouncedAndModeGated()
    {
        RfidTestWallbox wallbox(QHostAddress::LocalHost, 502, 1);
        QSignalSpy tagSpy(&wallbox, &PceWallbox::rfidTagDetected);

        wallbox.processRfidOperatingModeRegisterValues({1});
        wallbox.m_rfidModeConfirmed = true;
        const QVector<quint16> first = rfidToken(4);
        wallbox.processRfidRead(first);
        wallbox.processRfidRead(first);
        QCOMPARE(tagSpy.size(), 1);
        QCOMPARE(tagSpy.first().first().toString(), QStringLiteral("00112233"));

        wallbox.processRfidRead(QVector<quint16>(6, 0));
        wallbox.processRfidRead(first);
        QCOMPARE(tagSpy.size(), 2);

        wallbox.processRfidRead(rfidToken(7, 0x1011));
        QCOMPARE(tagSpy.size(), 3);

        wallbox.processRfidOperatingModeRegisterValues({2});
        wallbox.processRfidRead(rfidToken(10, 0x2011));
        QCOMPARE(tagSpy.size(), 3);
    }

    void rfidRejectionWritesLedEdgeAndTimedReset()
    {
        DelayedModbusServer server(10);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        RfidTestWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        wallbox.setRfidEnabled(true);
        QSignalSpy decisionSpy(&wallbox, &PceWallbox::rfidDecisionFinished);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.operational(), 5000);

        wallbox.processRfidOperatingModeRegisterValues({1});
        wallbox.m_rfidModeConfirmed = true;
        wallbox.processRfidRead(rfidToken(4));
        QVERIFY(wallbox.hasPendingRfidTag());

        const auto ledWrites = [&server]() {
            QList<DelayedModbusServer::Request> writes;
            for (const DelayedModbusServer::Request &request : server.requests()) {
                if (request.function == 6 && request.address == 210)
                    writes.append(request);
            }
            return writes;
        };

        QVERIFY(wallbox.submitRfidDecision(false));
        QTRY_COMPARE_WITH_TIMEOUT(decisionSpy.size(), 1, 3000);
        QCOMPARE(decisionSpy.first().first().toBool(), true);
        QVERIFY(!wallbox.hasPendingRfidTag());
        QTRY_COMPARE_WITH_TIMEOUT(ledWrites().size(), 2, 3000);
        QCOMPARE(ledWrites().at(0).values, QVector<quint16>({1}));
        QCOMPARE(ledWrites().at(1).values, QVector<quint16>({0}));

        for (const DelayedModbusServer::Request &request : server.requests()) {
            QVERIFY(!(request.function == 16 && request.address == 211));
            QVERIFY(!(request.function == 16 && request.address == 217));
        }

        wallbox.disconnectDevice();
    }

    void rfidAcceptanceWritesSessionThenLed()
    {
        DelayedModbusServer server(10);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        RfidTestWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        wallbox.setRfidEnabled(true);
        QSignalSpy decisionSpy(&wallbox, &PceWallbox::rfidDecisionFinished);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.operational(), 5000);

        wallbox.processRfidOperatingModeRegisterValues({1});
        wallbox.m_rfidModeConfirmed = true;
        const QVector<quint16> token = rfidToken(10);
        wallbox.processRfidRead(token);
        QVERIFY(wallbox.submitRfidDecision(true));
        QTRY_COMPARE_WITH_TIMEOUT(decisionSpy.size(), 1, 4000);
        QCOMPARE(decisionSpy.first().first().toBool(), true);

        int sessionIndex = -1;
        int acceptedLedIndex = -1;
        const QList<DelayedModbusServer::Request> requests = server.requests();
        for (int index = 0; index < requests.size(); ++index) {
            const auto &request = requests.at(index);
            if (request.function == 16 && request.address == 217) {
                sessionIndex = index;
                QCOMPARE(request.values, token);
                QCOMPARE(request.valueOrQuantity, quint16(6));
            } else if (request.function == 6 && request.address == 210
                       && request.values == QVector<quint16>({2})) {
                acceptedLedIndex = index;
            }
            QVERIFY(!(request.function == 16 && request.address == 211));
            QVERIFY(!(request.function == 6 && request.address == 200));
        }
        QVERIFY(sessionIndex >= 0);
        QVERIFY(acceptedLedIndex > sessionIndex);
        QTRY_VERIFY_WITH_TIMEOUT([&server]() {
            for (const auto &request : server.requests()) {
                if (request.function == 6 && request.address == 210
                        && request.values == QVector<quint16>({0}))
                    return true;
            }
            return false;
        }(), 3000);

        wallbox.disconnectDevice();
    }

    void rfidDecisionFailureCanBeRetried()
    {
        DelayedModbusServer server(10);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        RfidTestWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        wallbox.setRfidEnabled(true);
        QSignalSpy decisionSpy(&wallbox, &PceWallbox::rfidDecisionFinished);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.operational(), 5000);

        wallbox.processRfidOperatingModeRegisterValues({1});
        wallbox.m_rfidModeConfirmed = true;
        wallbox.processRfidRead(rfidToken(7));
        server.failNextWrite(217);
        QVERIFY(wallbox.submitRfidDecision(true));
        QTRY_COMPARE_WITH_TIMEOUT(decisionSpy.size(), 1, 3000);
        QCOMPARE(decisionSpy.at(0).at(0).toBool(), false);
        QVERIFY(wallbox.hasPendingRfidTag());

        QVERIFY(wallbox.submitRfidDecision(true));
        QTRY_COMPARE_WITH_TIMEOUT(decisionSpy.size(), 2, 4000);
        QCOMPARE(decisionSpy.at(1).at(0).toBool(), true);
        QVERIFY(!wallbox.hasPendingRfidTag());

        wallbox.processRfidRead(QVector<quint16>(6, 0));
        wallbox.processRfidRead(rfidToken(10, 0x1011));
        server.failNextWrite(210);
        QVERIFY(wallbox.submitRfidDecision(true));
        QTRY_COMPARE_WITH_TIMEOUT(decisionSpy.size(), 3, 4000);
        QCOMPARE(decisionSpy.at(2).at(0).toBool(), false);
        QVERIFY(wallbox.hasPendingRfidTag());

        QVERIFY(wallbox.submitRfidDecision(true));
        QTRY_COMPARE_WITH_TIMEOUT(decisionSpy.size(), 4, 4000);
        QCOMPARE(decisionSpy.at(3).at(0).toBool(), true);
        QVERIFY(!wallbox.hasPendingRfidTag());

        int sessionWrites = 0;
        for (const auto &request : server.requests()) {
            if (request.function == 16 && request.address == 217)
                ++sessionWrites;
        }
        QCOMPARE(sessionWrites, 4);
        wallbox.disconnectDevice();
    }

    void newerLedFeedbackSupersedesOlderReset()
    {
        DelayedModbusServer server(10);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        RfidTestWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        wallbox.setRfidEnabled(true);
        QSignalSpy decisionSpy(&wallbox, &PceWallbox::rfidDecisionFinished);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.operational(), 5000);
        wallbox.processRfidOperatingModeRegisterValues({1});
        wallbox.m_rfidModeConfirmed = true;

        wallbox.processRfidRead(rfidToken(4));
        QVERIFY(wallbox.submitRfidDecision(false));
        QTRY_COMPARE_WITH_TIMEOUT(decisionSpy.size(), 1, 3000);
        wallbox.processRfidRead(QVector<quint16>(6, 0));
        wallbox.processRfidRead(rfidToken(4));
        QVERIFY(wallbox.submitRfidDecision(false));
        QTRY_COMPARE_WITH_TIMEOUT(decisionSpy.size(), 2, 3000);

        QTRY_VERIFY_WITH_TIMEOUT([&server]() {
            int count = 0;
            for (const auto &request : server.requests()) {
                if (request.function == 6 && request.address == 210)
                    ++count;
            }
            return count >= 3;
        }(), 3000);

        QVector<quint16> ledValues;
        for (const auto &request : server.requests()) {
            if (request.function == 6 && request.address == 210)
                ledValues.append(request.values.first());
        }
        QCOMPARE(ledValues, QVector<quint16>({1, 1, 0}));
        wallbox.disconnectDevice();
    }

    void rfidOperatingModeInitializationRequiresMatchingReadback()
    {
        DelayedModbusServer server(10);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        RfidTestWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        wallbox.setOperationalStartupEnabled(false);
        wallbox.setRfidEnabled(true);
        QSignalSpy initializationSpy(&wallbox, &PceWallbox::rfidInitializationFinished);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.reachable(), 3000);

        QVERIFY(wallbox.initializeRfidOperatingMode(PceWallbox::RfidOperatingModeRemote));
        QTRY_COMPARE_WITH_TIMEOUT(initializationSpy.size(), 1, 3000);
        QCOMPARE(initializationSpy.takeFirst().first().toBool(), true);

        server.ignoreWritesTo(209);
        server.setReadValue(209, 0);
        QVERIFY(wallbox.initializeRfidOperatingMode(PceWallbox::RfidOperatingModeRemote));
        QTRY_COMPARE_WITH_TIMEOUT(initializationSpy.size(), 1, 3000);
        QCOMPARE(initializationSpy.takeFirst().first().toBool(), false);
        wallbox.disconnectDevice();
    }

    void operationalRfidPollingReadsModeAndCompleteToken()
    {
        DelayedModbusServer server(10);
        server.setReadValue(209, 1);
        server.setReadValues(211, rfidToken(4));
        QVERIFY(server.listen(QHostAddress::LocalHost));

        RfidTestWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        wallbox.setRfidEnabled(true);
        QSignalSpy tagSpy(&wallbox, &PceWallbox::rfidTagDetected);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.operational(), 5000);
        QTRY_COMPARE_WITH_TIMEOUT(tagSpy.size(), 1, 6000);

        bool modeRead = false;
        bool tokenRead = false;
        for (const auto &request : server.requests()) {
            modeRead |= request.function == 3 && request.address == 209
                    && request.valueOrQuantity == 1;
            tokenRead |= request.function == 3 && request.address == 211
                    && request.valueOrQuantity == 6;
            QVERIFY(!(request.function == 16 && request.address == 211));
        }
        QVERIFY(modeRead);
        QVERIFY(tokenRead);
        wallbox.disconnectDevice();
    }

    void writePreemptsRemainingPollReads()
    {
        DelayedModbusServer server(200);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        PceWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.operational(), 5000);

        const auto findRequest = [&server](quint8 function, quint16 address, int start = 0) {
            const QList<DelayedModbusServer::Request> requests = server.requests();
            for (int index = start; index < requests.size(); ++index) {
                if (requests.at(index).function == function
                        && requests.at(index).address == address) {
                    return index;
                }
            }
            return -1;
        };

        QTRY_VERIFY_WITH_TIMEOUT(findRequest(3, 100) >= 0, 5000);
        const int statusIndex = findRequest(3, 100);
        QCOMPARE(server.requests().at(statusIndex).repliedAt, qint64(0));

        QVERIFY(wallbox.setLedBrightnessAsync(55));
        QTRY_VERIFY_WITH_TIMEOUT(findRequest(6, 204, statusIndex + 1) >= 0, 2000);
        QTRY_VERIFY_WITH_TIMEOUT(findRequest(3, 200, statusIndex + 1) >= 0, 2000);

        const QList<DelayedModbusServer::Request> requests = server.requests();
        const int writeIndex = findRequest(6, 204, statusIndex + 1);
        const int nextReadIndex = findRequest(3, 200, statusIndex + 1);
        QCOMPARE(writeIndex, statusIndex + 1);
        QCOMPARE(nextReadIndex, writeIndex + 1);
        QVERIFY(requests.at(statusIndex).repliedAt > 0);
        const qint64 writeGap = requests.at(writeIndex).receivedAt
                - requests.at(statusIndex).repliedAt;
        QVERIFY2(writeGap >= 250,
                 qPrintable(QStringLiteral("Prioritized write gap was only %1 ms").arg(writeGap)));

        QTRY_VERIFY_WITH_TIMEOUT(server.requests().at(nextReadIndex).repliedAt > 0, 1000);
        wallbox.disconnectDevice();
    }

    void timedOutPollAdvancesAndNextRoundSucceeds()
    {
        DelayedModbusServer server(25);
        server.ignoreNextRead(205);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        PceWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        wallbox.modbusTcpMaster()->setTimeout(120);
        wallbox.modbusTcpMaster()->setNumberOfRetries(0);
        QSignalSpy updateSpy(&wallbox, &PceWallbox::updateFinished);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.operational(), 5000);

        const QList<quint16> expectedAddresses = {
            100, 200, 205, 204, 144, 206, 207, 208
        };
        const auto pollAddresses = [&server]() {
            QList<quint16> addresses;
            const QList<DelayedModbusServer::Request> requests = server.requests();
            bool pollingStarted = false;
            for (const DelayedModbusServer::Request &request : requests) {
                if (request.function != 3)
                    continue;
                if (request.address == 100)
                    pollingStarted = true;
                if (pollingStarted)
                    addresses.append(request.address);
            }
            return addresses;
        };

        QTRY_VERIFY_WITH_TIMEOUT(pollAddresses().count(100) >= 2
                                 && pollAddresses().count(208) >= 2, 12000);
        const QList<quint16> addresses = pollAddresses();
        QCOMPARE(addresses.mid(0, expectedAddresses.size()), expectedAddresses);
        QCOMPARE(addresses.mid(expectedAddresses.size(), expectedAddresses.size()),
                 expectedAddresses);
        QCOMPARE(addresses.size(), expectedAddresses.size() * 2);
        QTRY_VERIFY_WITH_TIMEOUT(updateSpy.size() >= 2, 1000);

        const QList<DelayedModbusServer::Request> requests = server.requests();
        int ignoredRequestIndex = -1;
        int finalRequestIndex = -1;
        for (int index = 0; index < requests.size(); ++index) {
            if (requests.at(index).function == 3 && requests.at(index).address == 205
                    && ignoredRequestIndex < 0) {
                ignoredRequestIndex = index;
            }
            if (requests.at(index).function == 3 && requests.at(index).address == 208)
                finalRequestIndex = index;
        }
        QVERIFY(ignoredRequestIndex >= 0);
        QCOMPARE(requests.at(ignoredRequestIndex).repliedAt, qint64(0));
        QVERIFY(finalRequestIndex > ignoredRequestIndex);
        QTRY_VERIFY_WITH_TIMEOUT(server.requests().at(finalRequestIndex).repliedAt > 0, 1000);
        wallbox.disconnectDevice();
    }

    void consecutiveTimeoutsMarkHalfOpenConnectionUnreachable()
    {
        DelayedModbusServer server(0);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        PceWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        wallbox.modbusTcpMaster()->setTimeout(50);
        wallbox.modbusTcpMaster()->setNumberOfRetries(0);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.operational(), 5000);

        server.ignoreAllRequests(true);
        QTRY_VERIFY_WITH_TIMEOUT(!wallbox.reachable(), 7000);

        // The peer deliberately keeps the TCP socket open. Reachability must
        // therefore be lost based on Modbus errors, not a socket disconnect.
        QVERIFY(wallbox.modbusTcpMaster()->connected());
        wallbox.disconnectDevice();
    }

    void timedOutFinalPollStillFinishesUpdateRound()
    {
        DelayedModbusServer server(0);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        PceWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        wallbox.setOperationalStartupEnabled(false);
        wallbox.modbusTcpMaster()->setTimeout(120);
        wallbox.modbusTcpMaster()->setNumberOfRetries(0);
        QSignalSpy initializationSpy(&wallbox, &PceWallbox::initializationFinished);
        QSignalSpy updateSpy(&wallbox, &PceWallbox::updateFinished);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(!initializationSpy.isEmpty(), 5000);
        QVERIFY(initializationSpy.constFirst().constFirst().toBool());

        server.ignoreNextRead(208);
        wallbox.startOperationalMode();

        const auto finalPollReceived = [&server]() {
            const QList<DelayedModbusServer::Request> requests = server.requests();
            for (const DelayedModbusServer::Request &request : requests) {
                if (request.function == 3 && request.address == 208)
                    return true;
            }
            return false;
        };
        QTRY_VERIFY_WITH_TIMEOUT(finalPollReceived(), 5000);
        QTRY_COMPARE_WITH_TIMEOUT(updateSpy.size(), 1, 600);
        wallbox.disconnectDevice();
    }

    void updateRoundsAreSerializedAndThrottled()
    {
        DelayedModbusServer server(25);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        PceWallbox wallbox(QHostAddress::LocalHost, server.serverPort(), 1);
        QVERIFY(wallbox.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(wallbox.operational(), 5000);

        const QList<quint16> expectedAddresses = {
            100, 200, 205, 204, 144, 206, 207, 208
        };
        const auto readAddresses = [&server]() {
            QList<quint16> addresses;
            const QList<DelayedModbusServer::Request> requests = server.requests();
            for (const DelayedModbusServer::Request &request : requests) {
                if (request.function == 3)
                    addresses.append(request.address);
            }
            return addresses;
        };

        QTRY_VERIFY_WITH_TIMEOUT(readAddresses().contains(100), 5000);
        QVERIFY(wallbox.update());
        QVERIFY(wallbox.update());

        QTRY_VERIFY_WITH_TIMEOUT(readAddresses().count(100) >= 2, 10000);
        const QList<DelayedModbusServer::Request> requests = server.requests();
        QList<int> firstRoundIndexes;
        int firstStatusIndex = -1;
        int secondStatusIndex = -1;
        for (int index = 0; index < requests.size(); ++index) {
            if (requests.at(index).function != 3)
                continue;
            if (requests.at(index).address == 100) {
                if (firstStatusIndex < 0)
                    firstStatusIndex = index;
                else {
                    secondStatusIndex = index;
                    break;
                }
            }
            if (firstStatusIndex >= 0)
                firstRoundIndexes.append(index);
        }

        QVERIFY(firstStatusIndex >= 0);
        QVERIFY(secondStatusIndex > firstStatusIndex);
        QCOMPARE(firstRoundIndexes.size(), expectedAddresses.size());
        for (int offset = 0; offset < firstRoundIndexes.size(); ++offset)
            QCOMPARE(requests.at(firstRoundIndexes.at(offset)).address,
                     expectedAddresses.at(offset));

        for (int offset = 1; offset < firstRoundIndexes.size(); ++offset) {
            const qint64 gap = requests.at(firstRoundIndexes.at(offset)).receivedAt
                    - requests.at(firstRoundIndexes.at(offset - 1)).receivedAt;
            QVERIFY2(gap >= 250, qPrintable(QStringLiteral("Request gap was only %1 ms").arg(gap)));
        }

        const DelayedModbusServer::Request &lastFirstRound =
                requests.at(firstRoundIndexes.constLast());
        QVERIFY(lastFirstRound.repliedAt > 0);
        const qint64 roundGap = requests.at(secondStatusIndex).receivedAt
                - lastFirstRound.repliedAt;
        QVERIFY2(roundGap >= 900,
                 qPrintable(QStringLiteral("Next round started after only %1 ms").arg(roundGap)));

        QTRY_VERIFY_WITH_TIMEOUT(server.requests().at(secondStatusIndex).repliedAt > 0, 1000);
        wallbox.disconnectDevice();
    }
};

QTEST_GUILESS_MAIN(TestPceWallbox)

#include "tst_pcewallbox.moc"
