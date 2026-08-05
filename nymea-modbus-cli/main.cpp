// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCryptographicHash>
#include <QElapsedTimer>
#include <QFile>
#include <QSharedPointer>
#include <QSslCertificateExtension>
#include <QSslCipher>
#include <QSslKey>
#include <QSslSocket>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QSerialPort>
#include <QHostAddress>
#include <QSerialPortInfo>
#include <QModbusTcpClient>
#include <QTextStream>
#include <modbustcpmaster.h>
#include "registerdatautils.h"

#include <limits>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QModbusRtuSerialClient>
#else
#include <QModbusRtuSerialMaster>
#endif

void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType,
                 quint16 registerAddress, quint16 length, const QVector<quint16> &writeValues,
                 bool writeRequest, const RegisterOutputOptions &outputOptions, QModbusClient *client);
void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType,
                 quint16 registerAddress, quint16 length, const QVector<quint16> &writeValues,
                 bool writeRequest, const RegisterOutputOptions &outputOptions, ModbusTcpMaster *client);
void printTlsInformation(ModbusTcpMaster *master, qint64 tcpConnectionTime, qint64 tlsHandshakeTime);
QString exceptionCodeToString(QModbusPdu::ExceptionCode exception);

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    application.setApplicationName("nymea-modbus-cli");
    application.setOrganizationName("nymea");
    application.setApplicationVersion("1.5.0");

    QString description = QString("\nTool for testing and reading Modbus TCP or RTU registers.\n\n");
    description.append(QString("Copyright %1 2016 - 2025 nymea GmbH <contact@nymea.io>\n\n").arg(QChar(0xA9)));



    description.append("TCP\n");
    description.append("-----------------------------------------\n");
    description.append("Example reading 2 holding registers from address 1000:\n");
    description.append("nymea-modbus-cli -a 192.168.0.10 -p 502 -r 1000 -l 2\n\n");
    description.append("Example inspecting a TLS 1.2 endpoint without sending Modbus traffic:\n");
    description.append("nymea-modbus-cli -a 192.168.0.10 --tls-info --tls-version 1.2\n\n");


    description.append("RTU\n");
    description.append("-----------------------------------------\n\n");

    description.append("Typical baudrates:\n");
    description.append("- 1200\n");
    description.append("- 2400\n");
    description.append("- 4800\n");
    description.append("- 9600\n");
    description.append("- 19200\n");
    description.append("- 38400\n");
    description.append("- 57600\n");
    description.append("- 115200\n\n");

    description.append("Example reading 2 holding registers from address 1000:\n");
    description.append("nymea-modbus-cli --serial /dev/ttyUSB0 --baudrate 9600 -r 1000 -l 2\n\n");


    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(description);

    // TCP
    QCommandLineOption addressOption(QStringList() << "a" << "address", QString("TCP: The IP address of the modbus TCP server."), "address");
    parser.addOption(addressOption);

    QCommandLineOption portOption(QStringList() << "p" << "port", QString("TCP: The port of the modbus TCP server. Default is 502."), "port");
    portOption.setDefaultValue("502");
    parser.addOption(portOption);

    QCommandLineOption tlsOption(QStringList() << "tls", QString("TCP: Use Modbus over TLS. The default port is 802."));
    parser.addOption(tlsOption);
    QCommandLineOption tlsVersionOption(QStringList() << "tls-version", QString("TCP: TLS protocol to use: auto or 1.2. Default is auto."), "version", "auto");
    parser.addOption(tlsVersionOption);
    QCommandLineOption tlsFingerprintOption(QStringList() << "tls-fingerprint", QString("TCP: Accepted SHA-256 server SPKI fingerprint."), "sha256");
    parser.addOption(tlsFingerprintOption);
    QCommandLineOption tlsAcceptAnyFingerprintOption(
        QStringList() << "tls-accept-any-fingerprint",
        QString("TCP: Accept any TLS server SPKI fingerprint. Disables server identity verification."));
    parser.addOption(tlsAcceptAnyFingerprintOption);
    QCommandLineOption tlsServerNameOption(QStringList() << "tls-server-name", QString("TCP: TLS server name used for SNI."), "name");
    parser.addOption(tlsServerNameOption);
    QCommandLineOption tlsClientCertificateOption(QStringList() << "tls-client-certificate", QString("TCP: PEM client certificate (optionally followed by intermediate certificates)."), "file");
    parser.addOption(tlsClientCertificateOption);
    QCommandLineOption tlsClientKeyOption(QStringList() << "tls-client-key", QString("TCP: PEM EC or RSA client private key."), "file");
    parser.addOption(tlsClientKeyOption);
    QCommandLineOption tlsClientKeyPassphraseFileOption(QStringList() << "tls-client-key-passphrase-file", QString("TCP: File containing the client private-key passphrase."), "file");
    parser.addOption(tlsClientKeyPassphraseFileOption);
    QCommandLineOption tlsInfoOption(QStringList() << "tls-info", QString("TCP: Print TLS handshake and server certificate information without sending Modbus traffic."));
    parser.addOption(tlsInfoOption);

    // RTU
    QCommandLineOption serialPortOption(QStringList() << "serial", QString("RTU: The serial port to use for the RTU communication."), "port");
    parser.addOption(serialPortOption);

    QCommandLineOption baudrateOption(QStringList() << "baudrate", QString("RTU: The baudrate for the RTU communication. Default is 19200."), "baudrate");
    baudrateOption.setDefaultValue("19200");
    parser.addOption(baudrateOption);

    QCommandLineOption parityOption(QStringList() << "parity", QString("RTU: The parity for the RTU communication. Allowed values are [none, even, odd, space, mark]. Default is none."), "parity");
    parityOption.setDefaultValue("none");
    parser.addOption(parityOption);

    QCommandLineOption dataBitsOption(QStringList() << "databits", QString("RTU: The amount of data bits for the RTU communication. Allowed values are [5, 6, 7, 8]. Default is 8."), "databits");
    dataBitsOption.setDefaultValue("8");
    parser.addOption(dataBitsOption);

    QCommandLineOption stopBitsOption(QStringList() << "stopbits", QString("RTU: The amount of stop bits for the RTU communication. Allowed values are [1, 1.5, 2]. Default is 1."), "stopbits");
    stopBitsOption.setDefaultValue("1");
    parser.addOption(stopBitsOption);


    QCommandLineOption listSerialPortsOption(QStringList() << "list-serials", QString("List the available serial ports on this host."));
    parser.addOption(listSerialPortsOption);

    // General

    QCommandLineOption modbusServerAddressOption(QStringList() << "m" << "modbus-address", QString("The modbus server address on the bus (slave ID). Default is 1."), "id");
    modbusServerAddressOption.setDefaultValue("1");
    parser.addOption(modbusServerAddressOption);

    QCommandLineOption registerTypeOption(QStringList() << "t" << "type", QString("The type of the modbus register. Default is holding."), "input, holding, discrete, coils");
    registerTypeOption.setDefaultValue("holding");
    parser.addOption(registerTypeOption);

    QCommandLineOption registerOption(QStringList() << "r" << "register", QString("The number of the modbus register."), "register");
    parser.addOption(registerOption);

    QCommandLineOption lengthOption(QStringList() << "l" << "length", QString("The number of values to read, or the expected count for a write. Default read count is 1."), "length");
    lengthOption.setDefaultValue("1");
    parser.addOption(lengthOption);

    QCommandLineOption writeOption(QStringList() << "w" << "write", QString("Write holding-register words or coil values. Accepts decimal/hex lists and compact hex register data."), "data");
    parser.addOption(writeOption);

    QCommandLineOption outputOption(QStringList() << "output", QString("Output format: table, json, or legacy. Default is table."), "format", "table");
    parser.addOption(outputOption);

    QCommandLineOption decodeOption(QStringList() << "decode", QString("Decode register blocks as uint16, int16, uint32, int32, uint64, int64, float32, or float64. May be repeated."), "type");
    parser.addOption(decodeOption);

    QCommandLineOption byteOrderOption(QStringList() << "byte-order", QString("Byte order inside each register for --decode: big or little. Default is big."), "order", "big");
    parser.addOption(byteOrderOption);

    QCommandLineOption wordOrderOption(QStringList() << "word-order", QString("Word order inside each decoded value: forward or reverse. Default is forward."), "order", "forward");
    parser.addOption(wordOrderOption);

    QCommandLineOption debugOption(QStringList() << "d" << "debug", QString("Print more information."));
    parser.addOption(debugOption);

    QCommandLineOption broadcastOption(QStringList() << "broadcast", QString("Send the request to modbus slave ID 0 aka broadcast"));
    parser.addOption(broadcastOption);


    parser.process(application);

    const bool useTls = parser.isSet(tlsOption) || parser.isSet(tlsInfoOption);
    const bool tlsInfo = parser.isSet(tlsInfoOption);
    const bool tlsAcceptAnyFingerprint = parser.isSet(tlsAcceptAnyFingerprintOption);
    const bool hasTlsClientCertificate = parser.isSet(tlsClientCertificateOption);
    const bool hasTlsClientKey = parser.isSet(tlsClientKeyOption);
    const bool hasTlsClientKeyPassphraseFile = parser.isSet(tlsClientKeyPassphraseFileOption);

    bool verbose = parser.isSet(debugOption);
    if (verbose) qDebug() << "Verbose debug print enabled";

    if (parser.isSet(listSerialPortsOption)) {
        foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
            qInfo().noquote() << serialPortInfo.systemLocation() << "|" << serialPortInfo.description() << "|" << serialPortInfo.serialNumber() << "|" << serialPortInfo.manufacturer();

        exit(EXIT_SUCCESS);
    }

    // Make sure we have either RTU, or TCP, not both or none
    if (parser.isSet(addressOption) && parser.isSet(serialPortOption)) {
        qCritical() << "Error: invalid parameter combination. Use either a TCP address or an RTU serial port, not both.";
        exit(EXIT_FAILURE);
    }

    if (tlsInfo && parser.isSet(serialPortOption)) {
        qCritical() << "Error: --tls-info can only be used with a TCP address.";
        exit(EXIT_FAILURE);
    }

    if ((hasTlsClientCertificate || hasTlsClientKey || hasTlsClientKeyPassphraseFile) && !useTls) {
        qCritical() << "Error: TLS client credentials require --tls or --tls-info.";
        exit(EXIT_FAILURE);
    }

    if (tlsAcceptAnyFingerprint && !useTls) {
        qCritical() << "Error: --tls-accept-any-fingerprint requires --tls or --tls-info.";
        exit(EXIT_FAILURE);
    }

    if (tlsAcceptAnyFingerprint && parser.isSet(tlsFingerprintOption)) {
        qCritical() << "Error: --tls-accept-any-fingerprint and --tls-fingerprint cannot be used together.";
        exit(EXIT_FAILURE);
    }

    if (hasTlsClientCertificate != hasTlsClientKey) {
        qCritical() << "Error: --tls-client-certificate and --tls-client-key must be specified together.";
        exit(EXIT_FAILURE);
    }

    if (hasTlsClientKeyPassphraseFile && !hasTlsClientKey) {
        qCritical() << "Error: --tls-client-key-passphrase-file requires --tls-client-key.";
        exit(EXIT_FAILURE);
    }

    if (!parser.isSet(addressOption) && !parser.isSet(serialPortOption)) {
        qCritical() << "Error: unknown protocol. Use either TCP connection by specifying the \"address\" or RTU by specifying the \"serial\" port.";
        exit(EXIT_FAILURE);
    }

    QModbusDataUnit::RegisterType registerType = QModbusDataUnit::RegisterType::Invalid;
    QString registerTypeString = parser.value(registerTypeOption);
    if (registerTypeString.toLower() == "input") {
        registerType = QModbusDataUnit::RegisterType::InputRegisters;
    } else if (registerTypeString.toLower() == "holding") {
        registerType = QModbusDataUnit::RegisterType::HoldingRegisters;
    } else if (registerTypeString.toLower() == "discrete") {
        registerType = QModbusDataUnit::RegisterType::DiscreteInputs;
    } else if (registerTypeString.toLower() == "coils") {
        registerType = QModbusDataUnit::RegisterType::Coils;
    } else {
        qCritical() << "Error: invalid register type:" << parser.value(registerTypeOption)
                    << "Use input, holding, discrete, or coils.";
        exit(EXIT_FAILURE);
    }

    if (parser.isSet(broadcastOption) && parser.isSet(modbusServerAddressOption)) {
        qCritical() << "Error: broadcast and modbus address specified. Please use one or the other option.";
        exit(EXIT_FAILURE);
    }

    bool valueOk = false;
    quint16 modbusServerAddress = 0;

    if (parser.isSet(broadcastOption)) {
        modbusServerAddress = 0;
    } else {
        const uint parsedServerAddress = parser.value(modbusServerAddressOption).toUInt(&valueOk);
        if (!valueOk || parsedServerAddress > 255) {
            qCritical() << "Error: invalid modbus server address (slave ID):" << parser.value(modbusServerAddressOption);
            exit(EXIT_FAILURE);
        } else if (parsedServerAddress == 0) {
            qCritical() << "Error: invalid modbus server address (slave ID):" << parser.value(modbusServerAddressOption);
            qCritical() << "Please use the broadcast parameter for sending broadcast requests.";
            exit(EXIT_FAILURE);
        }
        modbusServerAddress = static_cast<quint16>(parsedServerAddress);
    }

    const uint parsedRegisterAddress = parser.value(registerOption).toUInt(&valueOk);
    if (!tlsInfo && (!valueOk || parsedRegisterAddress > std::numeric_limits<quint16>::max())) {
        qCritical() << "Error: invalid register number:" << parser.value(registerOption);
        exit(EXIT_FAILURE);
    }
    const quint16 registerAddress = static_cast<quint16>(parsedRegisterAddress);

    uint parsedLength = parser.value(lengthOption).toUInt(&valueOk);
    if (!valueOk || parsedLength > std::numeric_limits<quint16>::max()) {
        qCritical() << "Error: invalid register length number:" << parser.value(lengthOption);
        exit(EXIT_FAILURE);
    }
    quint16 length = static_cast<quint16>(parsedLength);

    RegisterOutputOptions outputOptions;
    if (!parseRegisterOutputFormat(parser.value(outputOption), &outputOptions.format)) {
        qCritical() << "Error: invalid output format. Use table, json, or legacy.";
        exit(EXIT_FAILURE);
    }
    if (!parseRegisterByteOrder(parser.value(byteOrderOption), &outputOptions.byteOrder)) {
        qCritical() << "Error: invalid byte order. Use big or little.";
        exit(EXIT_FAILURE);
    }
    if (!parseRegisterWordOrder(parser.value(wordOrderOption), &outputOptions.wordOrder)) {
        qCritical() << "Error: invalid word order. Use forward or reverse.";
        exit(EXIT_FAILURE);
    }
    for (const QString &decodeName : parser.values(decodeOption)) {
        RegisterDecodeType decodeType;
        if (!parseRegisterDecodeType(decodeName, &decodeType)) {
            qCritical() << "Error: invalid decode type:" << decodeName;
            exit(EXIT_FAILURE);
        }
        if (!outputOptions.decodeTypes.contains(decodeType))
            outputOptions.decodeTypes.append(decodeType);
    }

    const bool writeRequest = parser.isSet(writeOption);
    QVector<quint16> writeValues;
    QString requestError;
    if (writeRequest) {
        bool parsed = false;
        if (registerType == QModbusDataUnit::HoldingRegisters)
            parsed = parseHoldingWriteValues(parser.value(writeOption), &writeValues, &requestError);
        else if (registerType == QModbusDataUnit::Coils)
            parsed = parseCoilWriteValues(parser.value(writeOption), &writeValues, &requestError);
        else
            requestError = QStringLiteral("Only holding registers and coils can be written.");
        if (!parsed) {
            qCritical().noquote() << "Error:" << requestError;
            exit(EXIT_FAILURE);
        }
        if (!outputOptions.decodeTypes.isEmpty()) {
            qCritical() << "Error: --decode can only be used for register reads.";
            exit(EXIT_FAILURE);
        }
        if (parser.isSet(lengthOption) && length != writeValues.size()) {
            qCritical() << "Error: explicit length" << length << "does not match"
                        << writeValues.size() << "write values.";
            exit(EXIT_FAILURE);
        }
        if (writeValues.size() > std::numeric_limits<quint16>::max()) {
            qCritical() << "Error: too many write values.";
            exit(EXIT_FAILURE);
        }
        length = static_cast<quint16>(writeValues.size());
        qDebug() << "Write values:" << writeValues;
    }

    if (!tlsInfo) {
        if (parser.isSet(broadcastOption) && !writeRequest) {
            qCritical() << "Error: broadcast reads are not supported.";
            exit(EXIT_FAILURE);
        }
        if (!validateModbusRequest(registerType, registerAddress, length, writeRequest, &requestError)) {
            qCritical().noquote() << "Error:" << requestError;
            exit(EXIT_FAILURE);
        }
        if (!writeRequest && (registerType == QModbusDataUnit::Coils
                              || registerType == QModbusDataUnit::DiscreteInputs)
            && !outputOptions.decodeTypes.isEmpty()) {
            qCritical() << "Error: --decode is only available for holding and input registers.";
            exit(EXIT_FAILURE);
        }
        for (RegisterDecodeType decodeType : outputOptions.decodeTypes) {
            const int wordCount = registerDecodeWordCount(decodeType);
            if (length % wordCount != 0) {
                qCritical() << "Error: read length" << length << "is not divisible by"
                            << wordCount << "registers required for"
                            << registerDecodeTypeName(decodeType) << "decoding.";
                exit(EXIT_FAILURE);
            }
        }
    }

    // TCP
    if (parser.isSet(addressOption)) {
        // TCP connection
        QHostAddress address = QHostAddress(parser.value(addressOption));
        if (address.isNull()) {
            qCritical() << "Error: invalid address:" << parser.value(addressOption);
            exit(EXIT_FAILURE);
        }

        quint16 port = parser.isSet(portOption) ? parser.value(portOption).toUInt() : (useTls ? 802 : 502);

        qInfo().noquote() << "Connecting to" << QString("%1:%2").arg(address.toString()).arg(port) << "modbus server address:" << modbusServerAddress;
        ModbusTcpMaster *client = new ModbusTcpMaster(address, port, &application);
        client->setTimeout(3000);
        client->setNumberOfRetries(3);

        const QSharedPointer<QElapsedTimer> connectionTimer(new QElapsedTimer);
        const QSharedPointer<QElapsedTimer> handshakeTimer(new QElapsedTimer);
        const QSharedPointer<qint64> tcpConnectionTime(new qint64(-1));
        connectionTimer->start();

        if (useTls) {
            client->setTransport(ModbusTcpMaster::TransportTls);
            client->setTlsServerName(parser.value(tlsServerNameOption));
            if (tlsAcceptAnyFingerprint) {
                qWarning().noquote()
                    << "WARNING: TLS server identity verification is disabled; any server fingerprint will be accepted.";
            }
            if (!parser.value(tlsFingerprintOption).isEmpty()
                && !client->setAcceptedPeerCertificateFingerprint(parser.value(tlsFingerprintOption))) {
                qCritical() << "Error: invalid SHA-256 TLS fingerprint.";
                exit(EXIT_FAILURE);
            }

            QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
            const QString tlsVersion = parser.value(tlsVersionOption).toLower();
            if (tlsVersion == "1.2") {
                configuration.setProtocol(QSsl::TlsV1_2);
            } else if (tlsVersion != "auto") {
                qCritical() << "Error: invalid TLS version. Use auto or 1.2.";
                exit(EXIT_FAILURE);
            }
            if (tlsInfo)
                configuration.setSslOption(QSsl::SslOptionDisableSessionPersistence, false);

            if (hasTlsClientCertificate) {
                QFile certificateFile(parser.value(tlsClientCertificateOption));
                if (!certificateFile.open(QIODevice::ReadOnly)) {
                    qCritical().noquote() << "Error: could not read TLS client certificate:"
                                          << certificateFile.errorString();
                    exit(EXIT_FAILURE);
                }
                const QList<QSslCertificate> certificateChain = QSslCertificate::fromData(certificateFile.readAll(), QSsl::Pem);
                if (certificateChain.isEmpty()) {
                    qCritical() << "Error: the TLS client certificate file does not contain a valid PEM certificate.";
                    exit(EXIT_FAILURE);
                }

                QByteArray keyPassphrase;
                if (hasTlsClientKeyPassphraseFile) {
                    QFile passphraseFile(parser.value(tlsClientKeyPassphraseFileOption));
                    if (!passphraseFile.open(QIODevice::ReadOnly)) {
                        qCritical().noquote() << "Error: could not read TLS client key passphrase file:"
                                              << passphraseFile.errorString();
                        exit(EXIT_FAILURE);
                    }
                    keyPassphrase = passphraseFile.readAll();
                    while (keyPassphrase.endsWith('\n') || keyPassphrase.endsWith('\r'))
                        keyPassphrase.chop(1);
                }

                QFile keyFile(parser.value(tlsClientKeyOption));
                if (!keyFile.open(QIODevice::ReadOnly)) {
                    qCritical().noquote() << "Error: could not read TLS client private key:"
                                          << keyFile.errorString();
                    exit(EXIT_FAILURE);
                }
                const QByteArray encodedKey = keyFile.readAll();
                QSslKey privateKey(encodedKey, QSsl::Ec, QSsl::Pem, QSsl::PrivateKey, keyPassphrase);
                if (privateKey.isNull())
                    privateKey = QSslKey(encodedKey, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, keyPassphrase);
                if (privateKey.isNull()) {
                    qCritical() << "Error: the TLS client key is not a valid PEM EC or RSA private key, or its passphrase is incorrect.";
                    exit(EXIT_FAILURE);
                }
                if (certificateChain.first().publicKey().algorithm() != privateKey.algorithm()) {
                    qCritical() << "Error: the TLS client certificate and private key use different algorithms.";
                    exit(EXIT_FAILURE);
                }

                configuration.setLocalCertificateChain(certificateChain);
                configuration.setPrivateKey(privateKey);
            }
            client->setTlsConfiguration(configuration);

            QObject::connect(client, &ModbusTcpMaster::peerCertificateAvailable, &application,
                             [client, tlsAcceptAnyFingerprint](const QSslCertificate &,
                                                               const QString &fingerprint) {
                qInfo().noquote() << "TLS peer SPKI SHA-256:" << fingerprint;
                if (tlsAcceptAnyFingerprint) {
                    // The TLS tunnel requires an accepted fingerprint to override
                    // CA and hostname errors. Set the currently presented SPKI
                    // synchronously, but do not retain it beyond this CLI process.
                    client->setAcceptedPeerCertificateFingerprint(fingerprint);
                    return;
                }
                if (client->acceptedPeerCertificateFingerprint().isEmpty())
                    qInfo().noquote() << "Use --tls-fingerprint" << fingerprint
                                      << "to pin this certificate if it is not CA-trusted.";
            }, Qt::DirectConnection);
            QObject::connect(client, &ModbusTcpMaster::tlsErrors, &application, [](const QList<QSslError> &errors) {
                for (const QSslError &error : errors)
                    qWarning().noquote() << "TLS certificate error:" << error.errorString();
            });
            QObject::connect(client, &ModbusTcpMaster::tcpConnectionEstablished, &application, [=]() {
                *tcpConnectionTime = connectionTimer->elapsed();
                handshakeTimer->start();
            });
            QObject::connect(client, &ModbusTcpMaster::tlsHandshakeFinished, &application,
                             [=](const QSslConfiguration &) {
                const qint64 handshakeTime = handshakeTimer->isValid() ? handshakeTimer->elapsed() : -1;
                if (tlsInfo) {
                    printTlsInformation(client, *tcpConnectionTime, handshakeTime);
                    QCoreApplication::exit(EXIT_SUCCESS);
                }
            });
        }

        QObject::connect(client, &ModbusTcpMaster::connectionStateChanged, &application, [=](bool connected){
            if (verbose) qDebug() << "Connection state changed" << connected;
            if (!connected || tlsInfo)
                return;

            qDebug() << "Connected successfully to" << QString("%1:%2").arg(address.toString()).arg(port);
            sendRequest(modbusServerAddress, registerType, registerAddress, length, writeValues,
                        writeRequest, outputOptions, client);
        });

        QObject::connect(client, &ModbusTcpMaster::connectionErrorOccurred, &application, [=](QModbusDevice::Error error){
            qWarning() << "Modbus connection error occurred:" << error << client->errorString();
            QCoreApplication::exit(EXIT_FAILURE);
        });

        if (!client->connectDevice()) {
            qWarning() << "Error: could not connect to" << QString("%1:%2").arg(address.toString()).arg(port);
            exit(EXIT_FAILURE);
        }
    }

    if (parser.isSet(serialPortOption)) {

        QString serialPortName = parser.value(serialPortOption);

        quint32 baudrate = parser.value(baudrateOption).toUInt();

        QSerialPort::Parity parity = QSerialPort::NoParity;
        QString parityString = parser.value(parityOption);
        if (parityString.toLower() == "none") {
            parity = QSerialPort::NoParity;
        } else if (parityString.toLower() == "even") {
            parity = QSerialPort::EvenParity;
        } else if (parityString.toLower() == "odd") {
            parity = QSerialPort::OddParity;
        } else if (parityString.toLower() == "space") {
            parity = QSerialPort::SpaceParity;
        } else if (parityString.toLower() == "mark") {
            parity = QSerialPort::MarkParity;
        } else {
            qCritical() << "Error: invalid parity:" << parser.value(parityOption)
                        << "Use none, even, odd, space, or mark.";
            exit(EXIT_FAILURE);
        }

        QSerialPort::StopBits stopBits = QSerialPort::OneStop;
        QString stopBitsString = parser.value(stopBitsOption);
        if (stopBitsString == "1") {
            stopBits = QSerialPort::OneStop;
        } else if (stopBitsString == "1.5") {
            stopBits = QSerialPort::OneAndHalfStop;
        } else if (stopBitsString == "2") {
            stopBits = QSerialPort::TwoStop;
        } else {
            qCritical() << "Error: invalid stop bits:" << parser.value(stopBitsOption)
                        << "Use 1, 1.5, or 2.";
            exit(EXIT_FAILURE);
        }

        QSerialPort::DataBits dataBits = QSerialPort::Data8;
        QString dataBitsString = parser.value(dataBitsOption);
        if (dataBitsString == "5") {
            dataBits = QSerialPort::Data5;
        } else if (dataBitsString == "6") {
            dataBits = QSerialPort::Data6;
        } else if (dataBitsString == "7") {
            dataBits = QSerialPort::Data7;
        } else if (dataBitsString == "8") {
            dataBits = QSerialPort::Data8;
        } else {
            qCritical() << "Error: invalid data bits:" << parser.value(dataBitsOption)
                        << "Use 5, 6, 7, or 8.";
            exit(EXIT_FAILURE);
        }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QModbusRtuSerialClient *client = new QModbusRtuSerialClient(nullptr);
#else
        QModbusRtuSerialMaster *client = new QModbusRtuSerialMaster(nullptr);
#endif
        client->setConnectionParameter(QModbusDevice::SerialPortNameParameter, serialPortName);
        client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudrate);
        client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
        client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);
        client->setConnectionParameter(QModbusDevice::SerialParityParameter, parity);
        client->setNumberOfRetries(3);
        client->setTimeout(500);

        QObject::connect(client, &QModbusTcpClient::stateChanged, &application, [=](QModbusDevice::State state){
            qDebug() << "Connection state changed" << state;
            if (state != QModbusDevice::ConnectedState)
                return;

            qDebug() << "Connected successfully to" << serialPortName << baudrate << dataBits << stopBits << parity << "modbus server address:" << modbusServerAddress;
            sendRequest(modbusServerAddress, registerType, registerAddress, length, writeValues,
                        writeRequest, outputOptions, client);
        });

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QObject::connect(client, &QModbusRtuSerialClient::errorOccurred, &application, [=](QModbusDevice::Error error){
#else
        QObject::connect(client, &QModbusRtuSerialMaster::errorOccurred, &application, [=](QModbusDevice::Error error){
#endif
            if (error != QModbusDevice::NoError) {
                exit(EXIT_FAILURE);
            }
        });

        if (!client->connectDevice()) {
            qWarning() << "Error: failed not connect to" << serialPortName << client->errorString();
            exit(EXIT_FAILURE);
        }
    }

    return application.exec();
}

template <typename Client>
void sendRequestInternal(quint16 modbusServerAddress,
                         QModbusDataUnit::RegisterType registerType,
                         quint16 registerAddress,
                         quint16 length,
                         const QVector<quint16> &writeValues,
                         bool writeRequest,
                         const RegisterOutputOptions &outputOptions,
                         Client *client)
{
    if (!writeRequest) {
        qDebug() << "Reading from modbus server address" << modbusServerAddress << registerType
                 << "register:" << registerAddress << "Length:" << length;
        const QModbusDataUnit request(registerType, registerAddress, length);
        QModbusReply *reply = client->sendReadRequest(request, modbusServerAddress);
        if (!reply) {
            qCritical() << "Failed to send read request:" << client->errorString();
            exit(EXIT_FAILURE);
        }
        if (reply->isFinished()) {
            qCritical() << "Read reply finished immediately:" << reply->errorString();
            reply->deleteLater();
            exit(EXIT_FAILURE);
        }

        QObject::connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        QObject::connect(reply, &QModbusReply::finished, client, [=]() {
            if (reply->error() != QModbusDevice::NoError) {
                const QModbusResponse response = reply->rawResult();
                if (reply->error() == QModbusDevice::ProtocolError && response.isException())
                    qCritical() << "Modbus read failed:" << reply->errorString()
                                << exceptionCodeToString(response.exceptionCode());
                else
                    qCritical() << "Modbus read failed:" << reply->errorString();
                exit(EXIT_FAILURE);
            }

            const QModbusDataUnit unit = reply->result();
            QTextStream output(stdout);
            output << formatReadResult(registerType, unit.startAddress(), unit.values(),
                                       modbusServerAddress, outputOptions);
            output.flush();
            exit(EXIT_SUCCESS);
        });
        return;
    }

    QModbusDataUnit request(registerType, registerAddress, writeValues.size());
    request.setValues(writeValues);
    qDebug() << "Writing" << request.values();
    QModbusReply *reply = client->sendWriteRequest(request, modbusServerAddress);
    if (!reply) {
        qCritical() << "Failed to send write request:" << client->errorString();
        exit(EXIT_FAILURE);
    }

    const auto printWriteSuccess = [=]() {
        QTextStream output(stdout);
        output << formatWriteResult(registerType, registerAddress, writeValues,
                                    modbusServerAddress, modbusServerAddress == 0, outputOptions);
        output.flush();
    };

    if (reply->isFinished()) {
        if (reply->error() == QModbusDevice::NoError && modbusServerAddress == 0) {
            printWriteSuccess();
            reply->deleteLater();
            exit(EXIT_SUCCESS);
        }
        qCritical() << "Write reply finished immediately:" << reply->errorString();
        reply->deleteLater();
        exit(EXIT_FAILURE);
    }

    QObject::connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    QObject::connect(reply, &QModbusReply::finished, client, [=]() {
        if (reply->error() != QModbusDevice::NoError) {
            const QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException())
                qCritical() << "Modbus write failed:" << reply->errorString()
                            << exceptionCodeToString(response.exceptionCode());
            else
                qCritical() << "Modbus write failed:" << reply->errorString();
            exit(EXIT_FAILURE);
        }
        printWriteSuccess();
        exit(EXIT_SUCCESS);
    });
}

void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType,
                 quint16 registerAddress, quint16 length, const QVector<quint16> &writeValues,
                 bool writeRequest, const RegisterOutputOptions &outputOptions, QModbusClient *client)
{
    sendRequestInternal(modbusServerAddress, registerType, registerAddress, length, writeValues,
                        writeRequest, outputOptions, client);
}

void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType,
                 quint16 registerAddress, quint16 length, const QVector<quint16> &writeValues,
                 bool writeRequest, const RegisterOutputOptions &outputOptions, ModbusTcpMaster *client)
{
    sendRequestInternal(modbusServerAddress, registerType, registerAddress, length, writeValues,
                        writeRequest, outputOptions, client);
}

static QString keyAlgorithmName(QSsl::KeyAlgorithm algorithm)
{
    switch (algorithm) {
    case QSsl::Rsa: return QStringLiteral("RSA");
    case QSsl::Dsa: return QStringLiteral("DSA");
    case QSsl::Ec: return QStringLiteral("EC");
    case QSsl::Dh: return QStringLiteral("DH");
    case QSsl::Opaque: return QStringLiteral("opaque");
    default: break;
    }
    return QStringLiteral("unknown");
}

void printTlsInformation(ModbusTcpMaster *master, qint64 tcpConnectionTime, qint64 tlsHandshakeTime)
{
    const QSslConfiguration configuration = master->negotiatedTlsConfiguration();
    const QSslCipher cipher = configuration.sessionCipher();

    qInfo().noquote() << "TLS information";
    qInfo().noquote() << "  Endpoint:" << master->connectionUrl();
    qInfo().noquote() << "  Server name (SNI):" << (master->tlsServerName().isEmpty() ? QStringLiteral("<none>") : master->tlsServerName());
    qInfo().noquote() << "  TCP connection time:" << tcpConnectionTime << "ms";
    qInfo().noquote() << "  TLS handshake time:" << tlsHandshakeTime << "ms";
    qInfo().noquote() << "  TLS protocol:" << QSslSocket::sslLibraryVersionString() << "/ negotiated" << cipher.protocolString();
    qInfo().noquote() << "  Cipher:" << cipher.name();
    qInfo().noquote() << "  Cipher authentication:" << cipher.authenticationMethod();
    qInfo().noquote() << "  Cipher key exchange:" << cipher.keyExchangeMethod();
    qInfo().noquote() << "  Cipher encryption:" << cipher.encryptionMethod();
    qInfo().noquote() << "  Cipher bits:" << cipher.usedBits() << "/" << cipher.supportedBits();
    qInfo().noquote() << "  ALPN:" << (configuration.nextNegotiatedProtocol().isEmpty() ? QByteArray("<none>") : configuration.nextNegotiatedProtocol());
    qInfo().noquote() << "  Session ticket received:" << !configuration.sessionTicket().isEmpty();
    qInfo().noquote() << "  Session ticket lifetime hint:" << configuration.sessionTicketLifeTimeHint();
    qInfo().noquote() << "  Qt TLS build library:" << QSslSocket::sslLibraryBuildVersionString();
    qInfo().noquote() << "  Qt TLS runtime library:" << QSslSocket::sslLibraryVersionString();

    const QSslKey ephemeralKey = configuration.ephemeralServerKey();
    qInfo().noquote() << "  Ephemeral server key:"
                      << (ephemeralKey.isNull() ? QStringLiteral("<not exposed>")
                                                : QStringLiteral("%1 %2 bits").arg(keyAlgorithmName(ephemeralKey.algorithm())).arg(ephemeralKey.length()));

    const QList<QSslCertificate> chain = configuration.peerCertificateChain();
    qInfo().noquote() << "  Peer certificate chain entries:" << chain.size();
    for (int i = 0; i < chain.size(); ++i) {
        const QSslCertificate certificate = chain.at(i);
        const QSslKey publicKey = certificate.publicKey();
        qInfo().noquote() << QStringLiteral("  Certificate %1:").arg(i);
        qInfo().noquote() << "    Subject CN:" << certificate.subjectInfo(QSslCertificate::CommonName).join(", ");
        qInfo().noquote() << "    Subject organization:" << certificate.subjectInfo(QSslCertificate::Organization).join(", ");
        qInfo().noquote() << "    Issuer CN:" << certificate.issuerInfo(QSslCertificate::CommonName).join(", ");
        qInfo().noquote() << "    Serial:" << certificate.serialNumber();
        qInfo().noquote() << "    Effective:" << certificate.effectiveDate().toString(Qt::ISODate);
        qInfo().noquote() << "    Expires:" << certificate.expiryDate().toString(Qt::ISODate);
        qInfo().noquote() << "    SHA-256:" << certificate.digest(QCryptographicHash::Sha256).toHex();
        qInfo().noquote() << "    Public key:" << keyAlgorithmName(publicKey.algorithm()) << publicKey.length() << "bits";
        const auto alternatives = certificate.subjectAlternativeNames();
        for (auto it = alternatives.cbegin(); it != alternatives.cend(); ++it)
            qInfo().noquote() << "    Subject alternative name:" << it.value();
        for (const QSslCertificateExtension &extension : certificate.extensions())
            qInfo().noquote() << "    Extension:" << extension.oid() << extension.name() << "critical:" << extension.isCritical();
    }
}

QString exceptionCodeToString(QModbusPdu::ExceptionCode exception)
{
    QString exceptionString;
    switch (exception) {
    case QModbusPdu::IllegalFunction:
        exceptionString = "Illegal function";
        break;
    case QModbusPdu::IllegalDataAddress:
        exceptionString = "Illegal data address";
        break;
    case QModbusPdu::IllegalDataValue:
        exceptionString = "Illegal data value";
        break;
    case QModbusPdu::ServerDeviceFailure:
        exceptionString = "Server device failure";
        break;
    case QModbusPdu::Acknowledge:
        exceptionString = "Acknowledge";
        break;
    case QModbusPdu::ServerDeviceBusy:
        exceptionString = "Server device busy";
        break;
    case QModbusPdu::NegativeAcknowledge:
        exceptionString = "Negative acknowledge";
        break;
    case QModbusPdu::MemoryParityError:
        exceptionString = "Memory parity error";
        break;
    case QModbusPdu::GatewayPathUnavailable:
        exceptionString = "Gateway path unavailable";
        break;
    case QModbusPdu::GatewayTargetDeviceFailedToRespond:
        exceptionString = "Gateway target device failed to respond";
        break;
    case QModbusPdu::ExtendedException:
        exceptionString = "Extended exception";
        break;
    }

    return exceptionString;
}
