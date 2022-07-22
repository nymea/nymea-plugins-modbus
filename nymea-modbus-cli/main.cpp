/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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
#include <QHostAddress>
#include <QModbusTcpClient>

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    application.setApplicationName("nymea-modbus-cli");
    application.setOrganizationName("nymea");
    application.setApplicationVersion("1.0.0");

    QString description = QString("\nTool for testing and reading Modbus TCP registers.\n\n");
    description.append(QString("Copyright %1 2016 - 2022 nymea GmbH <contact@nymea.io>\n").arg(QChar(0xA9)));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(description);

    QCommandLineOption addressOption(QStringList() << "a" << "address", QString("The IP address of the modbus TCP server."), "address");
    parser.addOption(addressOption);

    QCommandLineOption portOption(QStringList() << "p" << "port", QString("The port of the modbus TCP server. Default is 502."), "port");
    portOption.setDefaultValue("502");
    parser.addOption(portOption);

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

    QCommandLineOption debugOption(QStringList() << "d" << "debug", QString("Print more information."));
    parser.addOption(debugOption);

    parser.process(application);

    bool verbose = parser.isSet(debugOption);
    if (verbose) qDebug() << "Verbose debug print enabled";

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
        qCritical() << "Invalid register type:" << parser.value(registerTypeOption) << "Please select on of the valid register types: input, holding, discrete, coils";
        exit(EXIT_FAILURE);
    }

    bool valueOk = false;
    quint16 modbusServerAddress = parser.value(modbusServerAddressOption).toUInt(&valueOk);
    if (modbusServerAddress < 1 || !valueOk) {
        qCritical() << "Error: Invalid modbus server address (slave ID):" << parser.value(modbusServerAddressOption);
        exit(EXIT_FAILURE);
    }

    quint16 registerAddress = parser.value(registerOption).toUInt(&valueOk);
    if (!valueOk) {
        qCritical() << "Error: Invalid register number:" << parser.value(registerOption);
        exit(EXIT_FAILURE);
    }

    quint16 length = parser.value(lengthOption).toUInt(&valueOk);
    if (!valueOk) {
        qCritical() << "Error: Invalid register length number:" << parser.value(lengthOption);
        exit(EXIT_FAILURE);
    }

    if (!parser.isSet(addressOption)) {
        qWarning() << "Error: please specify the IP address of the modbus TCP server you want connect to. Modbus RTU is not implemented yet so you need to specify it.";
        exit(EXIT_FAILURE);
    }

    // TCP connection
    QHostAddress address = QHostAddress(parser.value(addressOption));
    if (address.isNull()) {
        qCritical() << "Error: Invalid address:" << parser.value(addressOption);
        exit(EXIT_FAILURE);
    }

    quint16 port = parser.value(portOption).toUInt();

    qInfo() << "Connecting to" << QString("%1:%2").arg(address.toString()).arg(port) << "Modbus server address:" << modbusServerAddress;
    QModbusTcpClient *client = new QModbusTcpClient(nullptr);
    client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address.toString());
    client->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    client->setTimeout(3000);
    client->setNumberOfRetries(3);

    QObject::connect(client, &QModbusTcpClient::stateChanged, &application, [=](QModbusDevice::State state){
        if (verbose) qDebug() << "Connection state changed" << state;
        if (state == QModbusDevice::ConnectedState) {
            qDebug() << "Connected successfully to" << QString("%1:%2").arg(address.toString()).arg(port);

            qDebug() << "Start reading from modbus server address" << modbusServerAddress << registerTypeString << "register:" << registerAddress << "Length:" << length;
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
                    qCritical() << "Reply finished with error:" << reply->errorString();
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
                qCritical() << "Modbus reply error occurred:" << error << reply->errorString();
            });
        }
    });

    QObject::connect(client, &QModbusTcpClient::errorOccurred, &application, [=](QModbusDevice::Error error){
        qWarning() << "Modbus error occurred:" << error << client->errorString();
        exit(EXIT_FAILURE);
    });

    if (!client->connectDevice()) {
        qWarning() << "Error: could not connect to" << QString("%1:%2").arg(address.toString()).arg(port);
        exit(EXIT_FAILURE);
    }

    return application.exec();
}
