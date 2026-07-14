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
#include <modbustcpmaster.h>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QModbusRtuSerialClient>
#else
#include <QModbusRtuSerialMaster>
#endif

void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType, quint16 registerAddress, quint16 length, const QByteArray &writeData, QModbusClient *client);
void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType, quint16 registerAddress, quint16 length, const QByteArray &writeData, ModbusTcpMaster *client);
void printTlsInformation(ModbusTcpMaster *master, qint64 tcpConnectionTime, qint64 tlsHandshakeTime);
QString exceptionCodeToString(QModbusPdu::ExceptionCode exception);

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    application.setApplicationName("nymea-modbus-cli");
    application.setOrganizationName("nymea");
    application.setApplicationVersion("1.4.0");

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
    QCommandLineOption tlsFingerprintOption(QStringList() << "tls-fingerprint", QString("TCP: Accepted SHA-256 server certificate fingerprint."), "sha256");
    parser.addOption(tlsFingerprintOption);
    QCommandLineOption tlsServerNameOption(QStringList() << "tls-server-name", QString("TCP: TLS server name used for SNI."), "name");
    parser.addOption(tlsServerNameOption);
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

    QCommandLineOption lengthOption(QStringList() << "l" << "length", QString("The number of registers to read. Default is 1."), "length");
    lengthOption.setDefaultValue("1");
    parser.addOption(lengthOption);

    QCommandLineOption writeOption(QStringList() << "w" << "write", QString("The data to be written to the given register."), "data");
    parser.addOption(writeOption);

    QCommandLineOption debugOption(QStringList() << "d" << "debug", QString("Print more information."));
    parser.addOption(debugOption);

    QCommandLineOption broadcastOption(QStringList() << "broadcast", QString("Send the request to modbus slave ID 0 aka broadcast"));
    parser.addOption(broadcastOption);


    parser.process(application);

    const bool useTls = parser.isSet(tlsOption) || parser.isSet(tlsInfoOption);
    const bool tlsInfo = parser.isSet(tlsInfoOption);

    bool verbose = parser.isSet(debugOption);
    if (verbose) qDebug() << "Verbose debug print enabled";

    if (parser.isSet(listSerialPortsOption)) {
        foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
            qInfo().noquote() << serialPortInfo.systemLocation() << "|" << serialPortInfo.description() << "|" << serialPortInfo.serialNumber() << "|" << serialPortInfo.manufacturer();

        exit(EXIT_SUCCESS);
    }

    // Make sure we have either RTU, or TCP, not both or none
    if (parser.isSet(addressOption) && parser.isSet(serialPortOption)) {
        qCritical() << "Error: invalid paramter combination. Use either TCP connection by defining the \"address\" or RTU by defining the \"serial\" paramter, not both.";
        exit(EXIT_FAILURE);
    }

    if (tlsInfo && parser.isSet(serialPortOption)) {
        qCritical() << "Error: --tls-info can only be used with a TCP address.";
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
        qCritical() << "Error: invalid register type:" << parser.value(registerTypeOption) << "Please select on of the valid register types: input, holding, discrete, coils";
        exit(EXIT_FAILURE);
    }

    if (parser.isSet(broadcastOption) && parser.isSet(modbusServerAddressOption)) {
        qCritical() << "Error: broadcast and modbus address specified. Please use one or the other option.";
        exit(EXIT_FAILURE);
    }

    bool valueOk = false;
    quint16 modbusServerAddress = parser.value(modbusServerAddressOption).toUInt(&valueOk);

    if (parser.isSet(broadcastOption)) {
        modbusServerAddress = 0;
    } else {
        modbusServerAddress = parser.value(modbusServerAddressOption).toUInt(&valueOk);
        if (!valueOk) {
            qCritical() << "Error: invalid modbus server address (slave ID):" << parser.value(modbusServerAddressOption);
            exit(EXIT_FAILURE);
        } else if (modbusServerAddress == 0) {
            qCritical() << "Error: invalid modbus server address (slave ID):" << parser.value(modbusServerAddressOption);
            qCritical() << "Please use the broadcast parameter for sending broadcast requests.";
            exit(EXIT_FAILURE);
        }
    }

    quint16 registerAddress = parser.value(registerOption).toUInt(&valueOk);
    if (!tlsInfo && !valueOk) {
        qCritical() << "Error: invalid register number:" << parser.value(registerOption);
        exit(EXIT_FAILURE);
    }

    quint16 length = parser.value(lengthOption).toUInt(&valueOk);
    if (!valueOk) {
        qCritical() << "Error: invalid register length number:" << parser.value(lengthOption);
        exit(EXIT_FAILURE);
    }

    QByteArray writeData;
    if (parser.isSet(writeOption)) {
        writeData = parser.value(writeOption).toLocal8Bit();
        qDebug() << "Write data:" << writeData;
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
            client->setTlsConfiguration(configuration);

            QObject::connect(client, &ModbusTcpMaster::peerCertificateAvailable, &application,
                             [client](const QSslCertificate &, const QString &fingerprint) {
                qInfo().noquote() << "TLS peer certificate SHA-256:" << fingerprint;
                if (client->acceptedPeerCertificateFingerprint().isEmpty())
                    qInfo().noquote() << "Use --tls-fingerprint" << fingerprint
                                      << "to pin this certificate if it is not CA-trusted.";
            });
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
            sendRequest(modbusServerAddress, registerType, registerAddress, length, writeData, client);
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
            qCritical() << "Error: invalid parit type:" << parser.value(parityOption) << "Please select on of the valid values: [none, even, odd, space, mark].";
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
            qCritical() << "Error: invalid stop bits:" << parser.value(stopBitsOption) << "Please select on of the valid values: [1, 1.5, 2].";
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
            qCritical() << "Error: invalid data bits:" << parser.value(dataBitsOption) << "Please select on of the valid values: [5, 6, 7, 8].";
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
            sendRequest(modbusServerAddress, registerType, registerAddress, length, writeData, client);
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
void sendRequestInternal(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType, quint16 registerAddress, quint16 length, const QByteArray &writeData, Client *client)
{
    if (writeData.isEmpty()) {
        qDebug() << "Reading from modbus server address" << modbusServerAddress << registerType << "register:" << registerAddress << "Length:" << length;
        QModbusDataUnit request = QModbusDataUnit(registerType, registerAddress, length);
        QModbusReply *reply = client->sendReadRequest(request, modbusServerAddress);
        if (!reply) {
            qCritical() << "Failed to read register" << client->errorString();
            exit(EXIT_FAILURE);
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // broadcast replies return immediately
            qCritical() << "Reply finished immediatly. Something might have gone wrong:" << reply->errorString();
            exit(EXIT_FAILURE);
        }

        QObject::connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        QObject::connect(reply, &QModbusReply::finished, client, [=]() {
            if (reply->error() != QModbusDevice::NoError) {
                QModbusResponse response = reply->rawResult();
                if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                    qCritical()  << "Modbus reply finished with error" << reply->error() << reply->errorString() << exceptionCodeToString(response.exceptionCode());
                } else {
                    qCritical()  << "Modbus reply finished with error" << reply->error() << reply->errorString();
                }
                exit(EXIT_FAILURE);
            }

            const QModbusDataUnit unit = reply->result();
            // Note: we need the cast in since the valueCount() type changes with different Qt versions
            for (int i = 0; i < static_cast<int>(unit.valueCount()); i++) {
                quint16 registerValue = unit.values().at(i);
                quint16 registerNumber = unit.startAddress() + i;
                qInfo() << "-->" << registerNumber << ":" << QString("0x%1").arg(registerValue, 4, 16, QLatin1Char('0')) << registerValue;
            }

            exit(EXIT_SUCCESS);
        });

        QObject::connect(reply, &QModbusReply::errorOccurred, client, [=] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCritical()  << "Modbus reply error occurred" << error << reply->errorString() << exceptionCodeToString(response.exceptionCode());
            } else {
                qCritical()  << "Modbus reply error occurred" << error << reply->errorString();
            }
        });
    } else {
        QModbusDataUnit request = QModbusDataUnit(registerType, registerAddress, length);
        QDataStream stream(writeData);
        qDebug() << "Reading write data" << writeData;
        quint16 data = writeData.toUInt();
        request.setValues({data});

        qDebug() << "Writing" << request.values();
        QModbusReply *reply = client->sendWriteRequest(request, modbusServerAddress);
        if (!reply) {
            qCritical() << "Failed to read register" << client->errorString();
            exit(EXIT_FAILURE);
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // broadcast replies return immediately
            qCritical() << "Reply finished immediatly. Something might have gone wrong:" << reply->errorString();
            exit(EXIT_FAILURE);
        }

        QObject::connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        QObject::connect(reply, &QModbusReply::finished, client, [=]() {
            if (reply->error() != QModbusDevice::NoError) {
                QModbusResponse response = reply->rawResult();
                if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                    qCritical()  << "Modbus reply finished with error" << reply->error() << reply->errorString() << exceptionCodeToString(response.exceptionCode());
                } else {
                    qCritical()  << "Modbus reply finished with error" << reply->error() << reply->errorString();
                }
                exit(EXIT_FAILURE);
            }

            const QModbusDataUnit unit = reply->result();
            // Note: we need the cast in since the valueCount() type changes with different Qt versions
            for (int i = 0; i < static_cast<int>(unit.valueCount()); i++) {
                quint16 registerValue = unit.values().at(i);
                quint16 registerNumber = unit.startAddress() + i;
                qInfo() << "-->" << registerNumber << ":" << QString("0x%1").arg(registerValue, 4, 16, QLatin1Char('0')) << registerValue;
            }

            exit(EXIT_SUCCESS);
        });

        QObject::connect(reply, &QModbusReply::errorOccurred, client, [=] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCritical()  << "Modbus reply error occurred" << error << reply->errorString() << exceptionCodeToString(response.exceptionCode());
            } else {
                qCritical()  << "Modbus reply error occurred" << error << reply->errorString();
            }
        });
    }
}

void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType, quint16 registerAddress, quint16 length, const QByteArray &writeData, QModbusClient *client)
{
    sendRequestInternal(modbusServerAddress, registerType, registerAddress, length, writeData, client);
}

void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType, quint16 registerAddress, quint16 length, const QByteArray &writeData, ModbusTcpMaster *client)
{
    sendRequestInternal(modbusServerAddress, registerType, registerAddress, length, writeData, client);
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
