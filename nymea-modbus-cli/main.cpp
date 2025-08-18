/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
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

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QSerialPort>
#include <QHostAddress>
#include <QSerialPortInfo>
#include <QModbusTcpClient>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QModbusRtuSerialClient>
#else
#include <QModbusRtuSerialMaster>
#endif

void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType, quint16 registerAddress, quint16 length, const QByteArray &writeData, QModbusClient *client);
QString exceptionCodeToString(QModbusPdu::ExceptionCode exception);

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    application.setApplicationName("nymea-modbus-cli");
    application.setOrganizationName("nymea");
    application.setApplicationVersion("1.2.0");

    QString description = QString("\nTool for testing and reading Modbus TCP or RTU registers.\n\n");
    description.append(QString("Copyright %1 2016 - 2025 nymea GmbH <contact@nymea.io>\n\n").arg(QChar(0xA9)));



    description.append("TCP\n");
    description.append("-----------------------------------------\n");
    description.append("Example reading 2 holding registers from address 1000:\n");
    description.append("nymea-modbus-cli -a 192.168.0.10 -p 502 -r 1000 -l 2\n\n");


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

    parser.process(application);

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

    bool valueOk = false;
    quint16 modbusServerAddress = parser.value(modbusServerAddressOption).toUInt(&valueOk);
    if (modbusServerAddress < 1 || !valueOk) {
        qCritical() << "Error: invalid modbus server address (slave ID):" << parser.value(modbusServerAddressOption);
        exit(EXIT_FAILURE);
    }

    quint16 registerAddress = parser.value(registerOption).toUInt(&valueOk);
    if (!valueOk) {
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

        quint16 port = parser.value(portOption).toUInt();

        qInfo().noquote() << "Connecting to" << QString("%1:%2").arg(address.toString()).arg(port) << "modbus server address:" << modbusServerAddress;
        QModbusTcpClient *client = new QModbusTcpClient(nullptr);
        client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address.toString());
        client->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
        client->setTimeout(3000);
        client->setNumberOfRetries(3);

        QObject::connect(client, &QModbusTcpClient::stateChanged, &application, [=](QModbusDevice::State state){
            if (verbose) qDebug() << "Connection state changed" << state;
            if (state != QModbusDevice::ConnectedState)
                return;

            qDebug() << "Connected successfully to" << QString("%1:%2").arg(address.toString()).arg(port);
            sendRequest(modbusServerAddress, registerType, registerAddress, length, writeData, client);
        });

        QObject::connect(client, &QModbusTcpClient::errorOccurred, &application, [=](QModbusDevice::Error error){
            qWarning() << "Modbus error occurred:" << error << client->errorString();
            exit(EXIT_FAILURE);
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

void sendRequest(quint16 modbusServerAddress, QModbusDataUnit::RegisterType registerType, quint16 registerAddress, quint16 length, const QByteArray &writeData, QModbusClient *client)
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
            for (uint i = 0; i < unit.valueCount(); i++) {
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
            for (uint i = 0; i < unit.valueCount(); i++) {
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
