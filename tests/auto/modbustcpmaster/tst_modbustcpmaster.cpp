// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCryptographicHash>
#include <QDataStream>
#include <QFile>
#include <QModbusReply>
#include <QProcess>
#include <QSignalSpy>
#include <QSslSocket>
#include <QTcpServer>
#include <QTest>

#include <modbustcpmaster.h>

class TlsModbusServer : public QTcpServer
{
public:
    explicit TlsModbusServer(int handshakeDelay = 0, QObject *parent = nullptr)
        : QTcpServer(parent),
          m_handshakeDelay(handshakeDelay)
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
        socket->setSslConfiguration(configuration);
        connect(socket, &QSslSocket::encrypted, this, [this, socket]() {
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

                    quint8 unitId = 0;
                    quint8 function = 0;
                    header >> unitId >> function;
                    buffer.remove(0, requestSize);
                    ++m_modbusRequests;

                    QByteArray response;
                    QDataStream output(&response, QIODevice::WriteOnly);
                    output << transactionId << quint16(0) << quint16(5) << unitId
                           << function << quint8(2) << quint16(0x1234);
                    socket->write(response);
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
    QSslCertificate m_certificate;
    QSslKey m_key;
    QHash<QSslSocket *, QByteArray> m_buffers;
    int m_handshakeDelay = 0;
    int m_connectionCount = 0;
    int m_modbusRequests = 0;
};

class InspectableModbusTcpMaster : public ModbusTcpMaster
{
public:
    using ModbusTcpMaster::ModbusTcpMaster;

    bool reconnectTimerActive() const { return m_reconnectTimer->isActive(); }
    int reconnectTimerInterval() const { return m_reconnectTimer->interval(); }
};

class TestModbusTcpMaster : public QObject
{
    Q_OBJECT

private slots:
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
                 QString::fromLatin1(server.certificate().digest(QCryptographicHash::Sha256).toHex()));
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
            QString::fromLatin1(server.certificate().digest(QCryptographicHash::Sha256).toHex())));

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
            QString::fromLatin1(server.certificate().digest(QCryptographicHash::Sha256).toHex())));

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
        const QString fingerprint = QString::fromLatin1(server.certificate().digest(QCryptographicHash::Sha256).toHex());

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
};

QTEST_GUILESS_MAIN(TestModbusTcpMaster)

#include "tst_modbustcpmaster.moc"
