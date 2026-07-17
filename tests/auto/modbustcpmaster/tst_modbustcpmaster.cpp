// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCryptographicHash>
#include <QDataStream>
#include <QFile>
#include <QModbusReply>
#include <QProcess>
#include <QSignalSpy>
#include <QSslKey>
#include <QSslSocket>
#include <QTcpServer>
#include <QTest>

#include <modbustcpmaster.h>

namespace {
QString spkiSha256Fingerprint(const QSslCertificate &certificate)
{
    return QString::fromLatin1(QCryptographicHash::hash(certificate.publicKey().toDer(),
                                                        QCryptographicHash::Sha256).toHex());
}
}

class TlsModbusServer : public QTcpServer
{
public:
    explicit TlsModbusServer(int handshakeDelay = 0, bool requireClientCertificate = false, QObject *parent = nullptr)
        : QTcpServer(parent),
          m_handshakeDelay(handshakeDelay),
          m_requireClientCertificate(requireClientCertificate)
    {
        QFile certificateFile(QStringLiteral(":/tls/test-cert.pem"));
        QFile keyFile(QStringLiteral(":/tls/test-key.pem"));
        Q_ASSERT(certificateFile.open(QIODevice::ReadOnly));
        Q_ASSERT(keyFile.open(QIODevice::ReadOnly));
        m_certificate = QSslCertificate(certificateFile.readAll(), QSsl::Pem);
        m_key = QSslKey(keyFile.readAll(), QSsl::Rsa, QSsl::Pem);
    }

    ~TlsModbusServer() override
    {
        const QList<QSslSocket *> sockets = findChildren<QSslSocket *>();
        for (QSslSocket *socket : sockets)
            socket->disconnect(this);
    }

    QSslCertificate certificate() const { return m_certificate; }
    int modbusRequests() const { return m_modbusRequests; }
    int connectionCount() const { return m_connectionCount; }
    bool clientCertificateReceived() const { return m_clientCertificateReceived; }
    quint16 lastWriteStartAddress() const { return m_lastWriteStartAddress; }
    QVector<quint16> lastWriteValues() const { return m_lastWriteValues; }

protected:
    void incomingConnection(qintptr socketDescriptor) override
    {
        ++m_connectionCount;
        QSslSocket *socket = new QSslSocket(this);
        QVERIFY(socket->setSocketDescriptor(socketDescriptor));
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        configuration.setLocalCertificate(m_certificate);
        configuration.setPrivateKey(m_key);
        if (m_requireClientCertificate) {
            configuration.addCaCertificate(m_certificate);
            configuration.setPeerVerifyMode(QSslSocket::VerifyPeer);
        }
        socket->setSslConfiguration(configuration);
        connect(socket, &QSslSocket::encrypted, this, [this, socket]() {
            if (!socket->peerCertificate().isNull())
                m_clientCertificateReceived = true;
            connect(socket, &QSslSocket::readyRead, this, [this, socket]() {
                m_buffers[socket].append(socket->readAll());
                QByteArray &buffer = m_buffers[socket];
                while (buffer.size() >= 7) {
                    QDataStream header(buffer);
                    quint16 transactionId = 0;
                    quint16 protocolId = 0;
                    quint16 length = 0;
                    header >> transactionId >> protocolId >> length;
                    const int requestSize = 6 + length;
                    if (buffer.size() < requestSize)
                        return;

                    const QByteArray request = buffer.left(requestSize);
                    buffer.remove(0, requestSize);
                    ++m_modbusRequests;
                    socket->write(processRequest(request));
                }
            });
        });
        connect(socket, &QSslSocket::disconnected, this, [this, socket]() {
            m_buffers.remove(socket);
            socket->deleteLater();
        });
        if (m_handshakeDelay > 0) {
            QTimer::singleShot(m_handshakeDelay, socket, [socket]() {
                socket->startServerEncryption();
            });
        } else {
            socket->startServerEncryption();
        }
    }

private:
    QByteArray processRequest(const QByteArray &request)
    {
        QDataStream input(request);
        quint16 transactionId = 0;
        quint16 protocolId = 0;
        quint16 requestLength = 0;
        quint8 unitId = 0;
        quint8 function = 0;
        input >> transactionId >> protocolId >> requestLength >> unitId >> function;

        QByteArray response;
        QDataStream output(&response, QIODevice::WriteOnly);
        if (function == 3 || function == 4) {
            quint16 startAddress = 0;
            quint16 count = 0;
            input >> startAddress >> count;
            output << transactionId << quint16(0) << static_cast<quint16>(3 + count * 2)
                   << unitId << function << static_cast<quint8>(count * 2);
            for (quint16 i = 0; i < count; ++i)
                output << static_cast<quint16>(0x1234 + i);
        } else if (function == 1 || function == 2) {
            quint16 startAddress = 0;
            quint16 count = 0;
            input >> startAddress >> count;
            const quint8 byteCount = static_cast<quint8>((count + 7) / 8);
            output << transactionId << quint16(0) << static_cast<quint16>(3 + byteCount)
                   << unitId << function << byteCount;
            for (quint8 byte = 0; byte < byteCount; ++byte)
                output << static_cast<quint8>(0x55);
        } else if (function == 6 || function == 5) {
            quint16 startAddress = 0;
            quint16 value = 0;
            input >> startAddress >> value;
            m_lastWriteStartAddress = startAddress;
            m_lastWriteValues = {function == 5 ? static_cast<quint16>(value == 0xff00) : value};
            response = request;
        } else if (function == 16) {
            quint16 startAddress = 0;
            quint16 count = 0;
            quint8 byteCount = 0;
            input >> startAddress >> count >> byteCount;
            Q_UNUSED(byteCount)
            m_lastWriteStartAddress = startAddress;
            m_lastWriteValues.clear();
            for (quint16 i = 0; i < count; ++i) {
                quint16 value = 0;
                input >> value;
                m_lastWriteValues.append(value);
            }
            output << transactionId << quint16(0) << quint16(6) << unitId << function
                   << startAddress << count;
        } else if (function == 15) {
            quint16 startAddress = 0;
            quint16 count = 0;
            quint8 byteCount = 0;
            input >> startAddress >> count >> byteCount;
            m_lastWriteStartAddress = startAddress;
            m_lastWriteValues.clear();
            QByteArray packed;
            for (quint8 i = 0; i < byteCount; ++i) {
                quint8 value = 0;
                input >> value;
                packed.append(static_cast<char>(value));
            }
            for (quint16 i = 0; i < count; ++i)
                m_lastWriteValues.append((static_cast<quint8>(packed.at(i / 8)) >> (i % 8)) & 1);
            output << transactionId << quint16(0) << quint16(6) << unitId << function
                   << startAddress << count;
        } else {
            output << transactionId << quint16(0) << quint16(3) << unitId
                   << static_cast<quint8>(function | 0x80) << quint8(1);
        }
        return response;
    }

    QSslCertificate m_certificate;
    QSslKey m_key;
    QHash<QSslSocket *, QByteArray> m_buffers;
    int m_handshakeDelay = 0;
    bool m_requireClientCertificate = false;
    bool m_clientCertificateReceived = false;
    int m_connectionCount = 0;
    int m_modbusRequests = 0;
    quint16 m_lastWriteStartAddress = 0;
    QVector<quint16> m_lastWriteValues;
};

class InspectableModbusTcpMaster : public ModbusTcpMaster
{
public:
    using ModbusTcpMaster::ModbusTcpMaster;

    bool reconnectTimerActive() const { return m_reconnectTimer->isActive(); }
    int reconnectTimerInterval() const { return m_reconnectTimer->interval(); }
    void installTestSession(const QByteArray &ticket) { m_tlsSessionTicket = ticket; }
};

class TestModbusTcpMaster : public QObject
{
    Q_OBJECT

private slots:
    void sessionResumptionDefaultsAndInvalidation()
    {
        InspectableModbusTcpMaster master(QHostAddress::LocalHost, 802);
        QVERIFY(master.tlsSessionResumptionEnabled());
        QVERIFY(!master.tlsSessionAvailable());

        const auto seed = [&master]() {
            master.installTestSession(QByteArrayLiteral("ticket"));
            QVERIFY(master.tlsSessionAvailable());
        };
        seed();
        master.setHostAddress(QHostAddress(QStringLiteral("127.0.0.2")));
        QVERIFY(!master.tlsSessionAvailable());
        seed();
        master.setPort(803);
        QVERIFY(!master.tlsSessionAvailable());
        seed();
        master.setTlsServerName(QStringLiteral("charger.local"));
        QVERIFY(!master.tlsSessionAvailable());
        seed();
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        master.setTlsConfiguration(configuration);
        QVERIFY(!master.tlsSessionAvailable());
        seed();
        master.setTransport(ModbusTcpMaster::TransportTls);
        QVERIFY(!master.tlsSessionAvailable());
        seed();
        QVERIFY(master.setAcceptedPeerCertificateFingerprint(QString(64, QLatin1Char('a'))));
        QVERIFY(!master.tlsSessionAvailable());
        seed();
        master.setTlsSessionResumptionEnabled(false);
        QVERIFY(!master.tlsSessionResumptionEnabled());
        QVERIFY(!master.tlsSessionAvailable());
    }

    void unusableSessionFallsBackToFullHandshake()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        InspectableModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        master.setTransport(ModbusTcpMaster::TransportTls);
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        master.setTlsConfiguration(configuration);
        QVERIFY(master.setAcceptedPeerCertificateFingerprint(
            spkiSha256Fingerprint(server.certificate())));
        master.installTestSession(QByteArrayLiteral("not-a-valid-session-ticket"));

        QVERIFY(master.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(master.connected(), 5000);
        QCOMPARE(server.connectionCount(), 1);
        QVERIFY(master.tlsSessionAvailable());
        master.disconnectDevice();
        QVERIFY(master.tlsSessionAvailable());

        QVERIFY(master.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(master.connected(), 5000);
        QCOMPARE(server.connectionCount(), 2);
        master.disconnectDevice();
    }

    void sessionTicketIsCaptured()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        ModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        master.setTransport(ModbusTcpMaster::TransportTls);
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        master.setTlsConfiguration(configuration);
        QVERIFY(master.setAcceptedPeerCertificateFingerprint(
            spkiSha256Fingerprint(server.certificate())));

        QVERIFY(master.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(master.connected(), 5000);
        QTRY_VERIFY_WITH_TIMEOUT(master.tlsSessionAvailable(), 5000);
        master.disconnectDevice();
    }

    void disabledSessionResumptionDoesNotRetainTicket()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        ModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        master.setTransport(ModbusTcpMaster::TransportTls);
        master.setTlsSessionResumptionEnabled(false);
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        master.setTlsConfiguration(configuration);
        QVERIFY(master.setAcceptedPeerCertificateFingerprint(
            spkiSha256Fingerprint(server.certificate())));

        QVERIFY(master.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(master.connected(), 5000);
        QVERIFY(!master.tlsSessionAvailable());
        master.disconnectDevice();
    }

    void fingerprintValidation()
    {
        ModbusTcpMaster master(QHostAddress::LocalHost, 802);
        const QString fingerprint(64, QLatin1Char('a'));
        QString decorated;
        for (int i = 0; i < fingerprint.size(); i += 2) {
            if (!decorated.isEmpty())
                decorated.append(QLatin1Char(':'));
            decorated.append(fingerprint.mid(i, 2).toUpper());
        }
        QVERIFY(master.setAcceptedPeerCertificateFingerprint(decorated));
        QCOMPARE(master.acceptedPeerCertificateFingerprint(), fingerprint);
        QVERIFY(!master.setAcceptedPeerCertificateFingerprint(QStringLiteral("invalid")));
        QCOMPARE(master.acceptedPeerCertificateFingerprint(), fingerprint);
    }

    void unpinnedCertificateIsReportedWithoutModbusTraffic()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        ModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        master.setTransport(ModbusTcpMaster::TransportTls);
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        master.setTlsConfiguration(configuration);

        QSignalSpy certificateSpy(&master, &ModbusTcpMaster::peerCertificateAvailable);
        QSignalSpy verificationSpy(&master, &ModbusTcpMaster::tlsPeerVerificationFailed);
        QVERIFY(master.connectDevice());
        QVERIFY(certificateSpy.wait(5000));
        QTRY_VERIFY_WITH_TIMEOUT(!verificationSpy.isEmpty(), 5000);
        master.disconnectDevice();

        QCOMPARE(server.modbusRequests(), 0);
        QCOMPARE(master.peerCertificateFingerprint(),
                 spkiSha256Fingerprint(server.certificate()));
    }

    void caTrustedCertificateDoesNotRequirePin()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        ModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        master.setTransport(ModbusTcpMaster::TransportTls);
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        configuration.addCaCertificate(server.certificate());
        master.setTlsConfiguration(configuration);

        QSignalSpy verificationSpy(&master, &ModbusTcpMaster::tlsPeerVerificationFailed);
        QVERIFY(master.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(master.connected(), 5000);
        QCOMPARE(verificationSpy.size(), 0);
        QCOMPARE(master.acceptedPeerCertificateFingerprint(), QString());
        master.disconnectDevice();
    }

    void pinnedTls12ModbusRequest()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        ModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        master.setTransport(ModbusTcpMaster::TransportTls);
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        master.setTlsConfiguration(configuration);
        QVERIFY(master.setAcceptedPeerCertificateFingerprint(
            spkiSha256Fingerprint(server.certificate())));

        QSignalSpy tlsSpy(&master, &ModbusTcpMaster::tlsHandshakeFinished);
        QVERIFY(master.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(master.connected(), 5000);
        QCOMPARE(tlsSpy.size(), 1);

        QModbusDataUnit request(QModbusDataUnit::HoldingRegisters, 10, 1);
        QModbusReply *reply = master.sendReadRequest(request, 1);
        QVERIFY(reply);
        QSignalSpy finishedSpy(reply, &QModbusReply::finished);
        QVERIFY(finishedSpy.wait(5000));
        QCOMPARE(reply->error(), QModbusDevice::NoError);
        QCOMPARE(reply->result().value(0), quint16(0x1234));
        QCOMPARE(server.modbusRequests(), 1);
        reply->deleteLater();
        master.disconnectDevice();
    }

    void wrongPinIsRejected()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        ModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        master.setTransport(ModbusTcpMaster::TransportTls);
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        master.setTlsConfiguration(configuration);
        QVERIFY(master.setAcceptedPeerCertificateFingerprint(QString(64, QLatin1Char('0'))));

        QSignalSpy verificationSpy(&master, &ModbusTcpMaster::tlsPeerVerificationFailed);
        QVERIFY(master.connectDevice());
        QVERIFY(verificationSpy.wait(5000));
        QVERIFY(!master.connected());
        QCOMPARE(server.modbusRequests(), 0);
        master.disconnectDevice();
    }

    void tlsConnectReentryDoesNotRestartHandshake()
    {
        TlsModbusServer server(300);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        ModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        master.setTransport(ModbusTcpMaster::TransportTls);
        QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
        configuration.setProtocol(QSsl::TlsV1_2);
        master.setTlsConfiguration(configuration);
        QVERIFY(master.setAcceptedPeerCertificateFingerprint(
            spkiSha256Fingerprint(server.certificate())));

        bool handshakeSignalReentryResult = true;
        connect(&master, &ModbusTcpMaster::tlsHandshakeFinished, &master,
                [&master, &handshakeSignalReentryResult]() {
            handshakeSignalReentryResult = master.connectDevice();
        });
        QVERIFY(master.connectDevice());
        QVERIFY(!master.connectDevice());
        QTRY_VERIFY_WITH_TIMEOUT(master.connected(), 5000);
        QVERIFY(!handshakeSignalReentryResult);
        QCOMPARE(server.connectionCount(), 1);
        master.disconnectDevice();
    }

    void failedTlsConnectArmsBackoffTimer()
    {
        QTcpServer portReservation;
        QVERIFY(portReservation.listen(QHostAddress::LocalHost));
        const quint16 unusedPort = portReservation.serverPort();
        portReservation.close();

        InspectableModbusTcpMaster master(QHostAddress::LocalHost, unusedPort);
        master.setTransport(ModbusTcpMaster::TransportTls);
        QVERIFY(master.connectDevice());

        QTRY_VERIFY_WITH_TIMEOUT(master.reconnectTimerActive(), 5000);
        QCOMPARE(master.reconnectTimerInterval(), 4000);
        master.disconnectDevice();
    }

    void stalledTlsHandshakeArmsBackoffTimer()
    {
        TlsModbusServer server(30000);
        QVERIFY(server.listen(QHostAddress::LocalHost));

        InspectableModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        master.setTransport(ModbusTcpMaster::TransportTls);
        QVERIFY(master.connectDevice());

        QTRY_VERIFY_WITH_TIMEOUT(master.reconnectTimerActive(), 12000);
        QCOMPARE(master.reconnectTimerInterval(), 4000);
        QVERIFY(master.errorString().contains(QStringLiteral("timed out")));
        master.disconnectDevice();
    }

    void immediateReconnectDoesNotPoisonBackoffInterval()
    {
        QTcpServer portReservation;
        QVERIFY(portReservation.listen(QHostAddress::LocalHost));
        const quint16 unusedPort = portReservation.serverPort();
        portReservation.close();

        InspectableModbusTcpMaster master(QHostAddress::LocalHost, unusedPort);
        QVERIFY(master.reconnectDevice());

        QTRY_VERIFY_WITH_TIMEOUT(master.reconnectTimerActive()
                                 && master.reconnectTimerInterval() == 4000, 5000);
        master.disconnectDevice();
    }

    void disconnectCancelsQueuedImmediateReconnect()
    {
        QTcpServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        InspectableModbusTcpMaster master(QHostAddress::LocalHost, server.serverPort());
        QVERIFY(master.reconnectDevice());
        master.disconnectDevice();
        QTest::qWait(100);

        QVERIFY(!server.hasPendingConnections());
        QVERIFY(!master.reconnectTimerActive());
        QVERIFY(!master.connected());
    }

    void cliTlsInfoDoesNotSendModbusTraffic()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));
        const QString fingerprint = spkiSha256Fingerprint(server.certificate());

        QProcess process;
        process.setProcessChannelMode(QProcess::MergedChannels);
        process.start(QStringLiteral("../../../nymea-modbus-cli/nymea-modbus-cli"),
                      {QStringLiteral("--address"), QStringLiteral("127.0.0.1"),
                       QStringLiteral("--port"), QString::number(server.serverPort()),
                       QStringLiteral("--tls-info"), QStringLiteral("--tls-version"), QStringLiteral("1.2"),
                       QStringLiteral("--tls-fingerprint"), fingerprint});
        QVERIFY(process.waitForStarted());
        while (!process.waitForFinished(50))
            QCoreApplication::processEvents();

        const QByteArray output = process.readAll();
        QCOMPARE(process.exitCode(), 0);
        QVERIFY2(output.contains("TLS information"), output.constData());
        QVERIFY2(output.contains("TLS handshake time:"), output.constData());
        QVERIFY2(output.contains("Peer certificate chain entries:"), output.constData());
        QCOMPARE(server.modbusRequests(), 0);
    }

    void cliAcceptsAnyTlsFingerprint()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        QProcess process;
        process.setProcessChannelMode(QProcess::MergedChannels);
        process.start(QStringLiteral("../../../nymea-modbus-cli/nymea-modbus-cli"),
                      {QStringLiteral("--address"), QStringLiteral("127.0.0.1"),
                       QStringLiteral("--port"), QString::number(server.serverPort()),
                       QStringLiteral("--tls"), QStringLiteral("--tls-version"), QStringLiteral("1.2"),
                       QStringLiteral("--tls-accept-any-fingerprint"),
                       QStringLiteral("--register"), QStringLiteral("10")});
        QVERIFY(process.waitForStarted());
        while (!process.waitForFinished(50))
            QCoreApplication::processEvents();

        const QByteArray output = process.readAll();
        QCOMPARE(process.exitCode(), 0);
        QVERIFY2(output.contains("server identity verification is disabled"), output.constData());
        QVERIFY2(output.contains("TLS peer SPKI SHA-256:"), output.constData());
        QVERIFY2(output.contains("Connected successfully"), output.constData());
        QCOMPARE(server.modbusRequests(), 1);
    }

    void cliBulkRegisterReadAndWrite()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));
        const QString fingerprint = spkiSha256Fingerprint(server.certificate());

        QProcess readProcess;
        readProcess.setProcessChannelMode(QProcess::MergedChannels);
        readProcess.start(QStringLiteral("../../../nymea-modbus-cli/nymea-modbus-cli"),
                          {QStringLiteral("--address"), QStringLiteral("127.0.0.1"),
                           QStringLiteral("--port"), QString::number(server.serverPort()),
                           QStringLiteral("--tls"), QStringLiteral("--tls-version"), QStringLiteral("1.2"),
                           QStringLiteral("--tls-fingerprint"), fingerprint,
                           QStringLiteral("--register"), QStringLiteral("100"),
                           QStringLiteral("--length"), QStringLiteral("2"),
                           QStringLiteral("--decode"), QStringLiteral("uint32")});
        QVERIFY(readProcess.waitForStarted());
        while (!readProcess.waitForFinished(50))
            QCoreApplication::processEvents();
        const QByteArray readOutput = readProcess.readAll();
        QCOMPARE(readProcess.exitCode(), 0);
        QVERIFY2(readOutput.contains("Bulk raw:   0x12341235"), readOutput.constData());
        QVERIFY2(readOutput.contains("Decoded uint32"), readOutput.constData());

        QProcess writeProcess;
        writeProcess.setProcessChannelMode(QProcess::MergedChannels);
        writeProcess.start(QStringLiteral("../../../nymea-modbus-cli/nymea-modbus-cli"),
                           {QStringLiteral("--address"), QStringLiteral("127.0.0.1"),
                            QStringLiteral("--port"), QString::number(server.serverPort()),
                            QStringLiteral("--tls"), QStringLiteral("--tls-version"), QStringLiteral("1.2"),
                            QStringLiteral("--tls-fingerprint"), fingerprint,
                            QStringLiteral("--register"), QStringLiteral("100"),
                            QStringLiteral("--write"), QStringLiteral("0x00010002")});
        QVERIFY(writeProcess.waitForStarted());
        while (!writeProcess.waitForFinished(50))
            QCoreApplication::processEvents();
        const QByteArray writeOutput = writeProcess.readAll();
        QCOMPARE(writeProcess.exitCode(), 0);
        QVERIFY2(writeOutput.contains("Wrote 2 holding registers"), writeOutput.constData());
        QCOMPARE(server.lastWriteStartAddress(), quint16(100));
        QCOMPARE(server.lastWriteValues(), QVector<quint16>({1, 2}));
    }

    void cliBulkCoilWrite()
    {
        TlsModbusServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));
        const QString fingerprint = spkiSha256Fingerprint(server.certificate());

        QProcess process;
        process.setProcessChannelMode(QProcess::MergedChannels);
        process.start(QStringLiteral("../../../nymea-modbus-cli/nymea-modbus-cli"),
                      {QStringLiteral("--address"), QStringLiteral("127.0.0.1"),
                       QStringLiteral("--port"), QString::number(server.serverPort()),
                       QStringLiteral("--tls"), QStringLiteral("--tls-version"), QStringLiteral("1.2"),
                       QStringLiteral("--tls-fingerprint"), fingerprint,
                       QStringLiteral("--type"), QStringLiteral("coils"),
                       QStringLiteral("--register"), QStringLiteral("20"),
                       QStringLiteral("--write"), QStringLiteral("true,false,1,0")});
        QVERIFY(process.waitForStarted());
        while (!process.waitForFinished(50))
            QCoreApplication::processEvents();
        const QByteArray output = process.readAll();
        QCOMPARE(process.exitCode(), 0);
        QVERIFY2(output.contains("Wrote 4 coils"), output.constData());
        QCOMPARE(server.lastWriteStartAddress(), quint16(20));
        QCOMPARE(server.lastWriteValues(), QVector<quint16>({1, 0, 1, 0}));
    }

    void cliPresentsTlsClientCertificate()
    {
        TlsModbusServer server(0, true);
        QVERIFY(server.listen(QHostAddress::LocalHost));
        const QString fingerprint = spkiSha256Fingerprint(server.certificate());
        const QString certificatePath = QFINDTESTDATA("test-cert.pem");
        const QString keyPath = QFINDTESTDATA("test-key.pem");
        QVERIFY(!certificatePath.isEmpty());
        QVERIFY(!keyPath.isEmpty());

        QProcess process;
        process.setProcessChannelMode(QProcess::MergedChannels);
        process.start(QStringLiteral("../../../nymea-modbus-cli/nymea-modbus-cli"),
                      {QStringLiteral("--address"), QStringLiteral("127.0.0.1"),
                       QStringLiteral("--port"), QString::number(server.serverPort()),
                       QStringLiteral("--tls"), QStringLiteral("--tls-version"), QStringLiteral("1.2"),
                       QStringLiteral("--tls-fingerprint"), fingerprint,
                       QStringLiteral("--tls-client-certificate"), certificatePath,
                       QStringLiteral("--tls-client-key"), keyPath,
                       QStringLiteral("--register"), QStringLiteral("10")});
        QVERIFY(process.waitForStarted());
        while (!process.waitForFinished(50))
            QCoreApplication::processEvents();

        const QByteArray output = process.readAll();
        QCOMPARE(process.exitCode(), 0);
        QVERIFY2(server.clientCertificateReceived(), output.constData());
        QCOMPARE(server.modbusRequests(), 1);
        QVERIFY2(output.contains("Connected successfully"), output.constData());
    }
};

QTEST_GUILESS_MAIN(TestModbusTcpMaster)

#include "tst_modbustcpmaster.moc"
