// SPDX-License-Identifier: GPL-3.0-or-later

#include <QDataStream>
#include <QElapsedTimer>
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
            if (function == 3) {
                output << transactionId << quint16(0) << quint16(3 + quantity * 2)
                       << unitId << function << quint8(quantity * 2);
                for (quint16 offset = 0; offset < quantity; ++offset) {
                    quint16 value = 0;
                    if (address + offset == 135)
                        value = 0x3030; // "00"
                    else if (address + offset == 136)
                        value = 0x3332; // "32" (firmware >= 0.25)
                    output << value;
                }
            } else {
                output << transactionId << quint16(0) << quint16(6)
                       << unitId << function << address << quantity;
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
    int m_ignoredReadAddress = -1;
    bool m_ignoreAllRequests = false;
};

class TestPceWallbox : public QObject
{
    Q_OBJECT

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
