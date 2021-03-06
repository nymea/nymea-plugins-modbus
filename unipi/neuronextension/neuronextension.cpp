/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
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

#include "neuronextension.h"
#include "extern-plugininfo.h"

#include <QFile>
#include <QTextStream>
#include <QStandardPaths>

NeuronExtension::NeuronExtension(ExtensionTypes extensionType, NeuronExtensionBus *neuronExtensionBus, int slaveAddress, QObject *parent) :
    QObject(parent),
    m_extensionType(extensionType),
    m_slaveAddress(slaveAddress),
    m_neuronExtensionBus(neuronExtensionBus)
{
    qCDebug(dcUniPi()) << "NeuronExtension: Creating extension" << extensionType;

    //connect(m_neuronExtensionBus, &NeuronExtensionBus::readReplyReceived, this, &NeuronExtension::onReadReplyReceived);
    //connect(m_neuronExtensionBus, &NeuronExtensionBus::writeReplyReceived, this, &NeuronExtension::onWriteReplyReceived);
    connect(m_neuronExtensionBus, &NeuronExtensionBus::connectedChanged, this, [this] (bool connected) {
        if (connected) {
            getCoils();
            getRegisters();
        }
        emit connectionStateChanged(connected);
    });

    switch (extensionType) {
    case xS10: {
        m_modbusDigitalOutputRegisters = new OutputRegisters(0, 0, this);
        m_modbusRelayOutputRegisters = new OutputRegisters(0, 8, this);
        break;
    }
    case xS30: {
        m_modbusDigitalOutputRegisters = new OutputRegisters(0, 0, this);
        m_modbusRelayOutputRegisters = new OutputRegisters(0, 0, this);
        break;
    }
    case xS40: {
        m_modbusDigitalOutputRegisters = new OutputRegisters(0, 0, this);
        m_modbusRelayOutputRegisters = new OutputRegisters(0, 14, this);
        break;
    }
    case xS50: {
        m_modbusDigitalOutputRegisters = new OutputRegisters(0, 0, this);
        m_modbusRelayOutputRegisters = new OutputRegisters(0, 5, this);
        break;
    }
    case xS11: {
        m_modbusDigitalOutputRegisters = new OutputRegisters(0, 0, this);
        m_modbusRelayOutputRegisters = new OutputRegisters(0, 13, this);
        break;
    }
    case xS51: {
        m_modbusDigitalOutputRegisters = new OutputRegisters(0, 0, this);
        m_modbusRelayOutputRegisters = new OutputRegisters(0, 5, this);
        break;
    }
    }

    m_watchdog = new QTimer(this);
    m_watchdog->setInterval(m_responseTimeoutTime);
    connect(m_watchdog, &QTimer::timeout, this, [this] {
        qCWarning(dcUniPi()) << "NeuronExtension: Watchdog triggered";
        getCoils();
    });
}

NeuronExtension::~NeuronExtension()
{
    qCDebug(dcUniPi()) << "NeuronExtension: Deleting extension" << m_extensionType;
    m_watchdog->stop();
}

bool NeuronExtension::init()
{
    qCDebug(dcUniPi()) << "NeuronExtension: Init";
    if (!loadModbusMap()) {
        return false;
    }
    if (m_neuronExtensionBus->connected()) {
        getCoils();
        getRegisters();
    }
    m_watchdog->start();
    return true;
}

QString NeuronExtension::type()
{
    return stringFromType(m_extensionType);
}

QString NeuronExtension::stringFromType(NeuronExtension::ExtensionTypes extensionType)
{
    switch(extensionType) {
    case ExtensionTypes::xS10:
        return "xS10";
    case ExtensionTypes::xS30:
        return "xS30";
    case ExtensionTypes::xS40:
        return "xS40";
    case ExtensionTypes::xS50:
        return "xS50";
    case ExtensionTypes::xS11:
        return "xS11";
    case ExtensionTypes::xS51:
        return "xS51";
    default:
        return "Unknown";
    }
}

bool NeuronExtension::loadModbusMap()
{
    qCDebug(dcUniPi()) << "NeuronExtension: Load modbus map";

    QStringList fileCoilList;
    QStringList fileRegisterList;

    switch(m_extensionType) {
    case ExtensionTypes::xS10:
        fileCoilList.append(QString("/Neuron_xS10/Neuron_xS10-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS30:
        fileCoilList.append(QString("/Neuron_xS30/Neuron_xS30-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS40:
        fileCoilList.append(QString("/Neuron_xS40/Neuron_xS40-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS50:
        fileCoilList.append(QString("/Neuron_xS50/Neuron_xS50-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS11:
        fileCoilList.append(QString("/Extension_xS11/Extension_xS11-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS51:
        fileCoilList.append(QString("/Extension_xS51/Extension_xS51-Coils-group-1.csv"));
        break;
    }

    foreach (QString relativeFilePath, fileCoilList) {
        QString absoluteFilePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).last() + "/nymea/modbus" + relativeFilePath;
        qDebug(dcUniPi()) << "NeuronExtension: Open CSV File:" << absoluteFilePath;
        QFile *csvFile = new QFile(absoluteFilePath);
        if (!csvFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(dcUniPi()) << csvFile->errorString() << absoluteFilePath;
            csvFile->deleteLater();
            return false;
        }
        QTextStream *textStream = new QTextStream(csvFile);
        while (!textStream->atEnd()) {
            QString line = textStream->readLine();
            QStringList list = line.split(',');
            if (list.length() <= 4) {
                qCWarning(dcUniPi()) << "NeuronExtension: currupted CSV file:" << csvFile->fileName();
                csvFile->deleteLater();
                return false;
            }
            if (list[4] == "Basic") {
                QString circuit = list[3].split(" ").last();
                if (list[3].contains("Digital Input", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalInputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "NeuronExtension: Found input register" << circuit << list[0].toInt();
                } else if (list[3].contains("Digital Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalOutputRegisters->insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "NeuronExtension: Found output register" << circuit << list[0].toInt();
                } else if (list[3].contains("Relay Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusRelayOutputRegisters->insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "NeuronExtension: Found relay register" << circuit << list[0].toInt();
                }
            }
        }
        csvFile->close();
        csvFile->deleteLater();
    }

    switch(m_extensionType) {
    case ExtensionTypes::xS10:
        fileRegisterList.append(QString("/Neuron_xS10/Neuron_xS10-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS30:
        fileRegisterList.append(QString("/Neuron_xS30/Neuron_xS30-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS40:
        fileRegisterList.append(QString("/Neuron_xS40/Neuron_xS40-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS50:
        fileRegisterList.append(QString("/Neuron_xS50/Neuron_xS50-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS11:
        fileRegisterList.append(QString("/Extension_xS11/Extension_xS11-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS51:
        fileRegisterList.append(QString("/Extension_xS51/Extension_xS51-Registers-group-1.csv"));
        break;
    }

    foreach (QString relativeFilePath, fileRegisterList) {
        QString absoluteFilePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).last() + "/nymea/modbus" + relativeFilePath;
        qDebug(dcUniPi()) << "NeuronExtension: Open CSV File:" << absoluteFilePath;
        QFile *csvFile = new QFile(absoluteFilePath);
        if (!csvFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(dcUniPi()) << csvFile->errorString() << absoluteFilePath;
            csvFile->deleteLater();
            return false;
        }
        QTextStream *textStream = new QTextStream(csvFile);
        while (!textStream->atEnd()) {
            QString line = textStream->readLine();
            QStringList list = line.split(',');
            if (list.length() <= 5) {
                qCWarning(dcUniPi()) << "NeuronExtension: Currupted CSV file:" << csvFile->fileName();
                csvFile->deleteLater();
                return false;
            }
            if (list.last() == "Basic" && list[5].split(" ").length() > 3) {
                if (list[5].split(" ").length() <= 3) {
                    qCWarning(dcUniPi()) << "NeuronExtension: Currupted CSV file:" << csvFile->fileName();
                    csvFile->deleteLater();
                    return false;
                }
                int modbusAddress = list[0].toInt();
                if (list[5].contains("Analog Input Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogInputRegisters.insert(modbusAddress, registerDescriptorFromStringList(list));
                    qDebug(dcUniPi()) << "NeuronExtension: Found analog input register" << modbusAddress;
                } else if (list[5].contains("Analog Output Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogOutputRegisters.insert(modbusAddress, registerDescriptorFromStringList(list));
                    qDebug(dcUniPi()) << "NeuronExtension: Found analog output register" << modbusAddress;
                }
            }
        }
        csvFile->close();
        csvFile->deleteLater();
    }
    return true;
}

int NeuronExtension::slaveAddress()
{
    return m_slaveAddress;
}

void NeuronExtension::setSlaveAddress(int slaveAddress)
{
    qCDebug(dcUniPi()) << "NeuronExtension: Set slave address" << slaveAddress;
    m_slaveAddress = slaveAddress;
}

QList<QString> NeuronExtension::digitalInputs()
{
    return m_modbusDigitalInputRegisters.keys();
}

QList<QString> NeuronExtension::relayOutputs()
{
    return m_modbusRelayOutputRegisters->circuits();
}

QList<QString> NeuronExtension::digitalOutputs()
{
    return m_modbusDigitalOutputRegisters->circuits();
}

QList<QString> NeuronExtension::analogInputs()
{
    QList<QString> circuits;
    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogInputRegisters.values()) {
        circuits.append(descriptor.circuit);
    }
    return circuits;
}

QList<QString> NeuronExtension::analogOutputs()
{
    QList<QString> circuits;
    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogOutputRegisters.values()) {
        circuits.append(descriptor.circuit);
    }
    return circuits;
}

NeuronExtension::RegisterDescriptor NeuronExtension::registerDescriptorFromStringList(const QStringList &data)
{
    RegisterDescriptor descriptor;
    if (data.count() < 7) {
        return descriptor;
    }
    descriptor.address = data[0].toInt();
    descriptor.count = data[2].toInt();
    if (data[3] == "RW") {
        descriptor.readWrite = RWPermissionReadWrite;
    } else if (data[3] == "W") {
        descriptor.readWrite = RWPermissionWrite;
    } else if (data[3] == "R") {
        descriptor.readWrite = RWPermissionRead;
    }
    descriptor.circuit = data[5].split(" ").value(4);
    descriptor.category = data.last();

    if (data[5].contains("Analog Input Value", Qt::CaseSensitivity::CaseInsensitive)) {
        descriptor.registerType = NeuronExtensionRegisterType::RegisterTypeRegisters;
    } else if (data[5].contains("Analog Output Value", Qt::CaseSensitivity::CaseInsensitive)) {
        descriptor.registerType = NeuronExtensionRegisterType::RegisterTypeRegisters;
    }
    return descriptor;
}

void NeuronExtension::setRelayOutput(const QString &circuit, bool value)
{
    m_modbusRelayOutputRegisters->setValue(circuit, value?1:0);
}

void NeuronExtension::setDigitalOutput(const QString &circuit, bool value)
{
    m_modbusDigitalOutputRegisters->setValue(circuit, value?1:0);
}

void NeuronExtension::setAnalogOutput(const QString &circuit, double value)
{
    qDebug(dcUniPi()) << "NeuronExtension: Set analog output" << circuit << value;
}

void NeuronExtension::setAnalogOutputConfiguration(const QString &circuit, AnalogIOConfiguration value)
{
    qDebug(dcUniPi()) << "NeuronExtension: Set analog output configuration" << circuit << value;
}

void NeuronExtension::setAnalogInputConfiguration(const QString &circuit, AnalogIOConfiguration value)
{
    qDebug(dcUniPi()) << "NeuronExtension: Set analog input configuration" << circuit << value;
}

void NeuronExtension::getDeviceInformation()
{
    qCDebug(dcUniPi()) << "NeuronExtension: Get device information";
    NeuronExtensionRequest request(NeuronExtensionRegisterType::RegisterTypeRegisters, m_slaveAddress, 1000, 7, false);
    auto *reply = m_neuronExtensionBus->sendRequest(request);
    if (reply->isFinished()) {
        onDeviceInformationReplyFinished(reply);
        reply->deleteLater();
    } else {
        connect(reply, &NeuronExtensionReply::finished, reply, &NeuronExtensionReply::deleteLater);
        connect(reply, &NeuronExtensionReply::finished, this, [reply, this] {
            onDeviceInformationReplyFinished(reply);
        });
    }
}

void NeuronExtension::getCoils()
{
    uint8_t length = 0;

    switch (m_extensionType) {
    case xS10:
        length = 24;
        break;
    case xS30:
        length = 24;
        break;
    case xS40:
        length = 22;
        break;
    case xS50:
        length = 7;
        break;
    case xS11:
        length = 25;
        break;
    case xS51:
        length = 9;
        break;
    }
    NeuronExtensionRequest request(NeuronExtensionRegisterType::RegisterTypeCoils, m_slaveAddress, 0, length);
    auto *reply = m_neuronExtensionBus->sendRequest(request);
    if (reply->isFinished()) {
        onReplyFinished(reply);
        reply->deleteLater();
    } else {
        connect(reply, &NeuronExtensionReply::finished, reply, &NeuronExtensionReply::deleteLater);
        connect(reply, &NeuronExtensionReply::finished, this, [reply, this] {
            onReplyFinished(reply);
        });
    }
}

void NeuronExtension::getRegisters()
{
    uint8_t start, length;

    switch (m_extensionType) {
    case xS10:
    case xS30:
    case xS40:
    case xS11:
        return;
    case xS50:
    case xS51:
        start = 2;
        length = 12;
        break;
    }
    NeuronExtensionRequest request(NeuronExtensionRegisterType::RegisterTypeRegisters, m_slaveAddress, start, length);
    auto *reply = m_neuronExtensionBus->sendRequest(request);
    if (reply->isFinished()) {
        onReplyFinished(reply);
        reply->deleteLater();
    } else {
        connect(reply, &NeuronExtensionReply::finished, reply, &NeuronExtensionReply::deleteLater);
        connect(reply, &NeuronExtensionReply::finished, this, [reply, this] {
            onReplyFinished(reply);
        });
    }
}

void NeuronExtension::sendWriteRequest(const NeuronExtensionRequest &request)
{
    auto *reply = m_neuronExtensionBus->sendRequest(request);
    if (reply->isFinished()) {
        reply->deleteLater();
    } else {
        connect(reply, &NeuronExtensionReply::finished, reply, &NeuronExtensionReply::deleteLater);
        connect(reply, &NeuronExtensionReply::finished, this, [reply] {
            if (reply->error() != NeuronExtensionReply::ErrorNoError) {
                qCDebug(dcUniPi()) << "Could not send write request";
            }
            //TODO send again and add error counter
        });
    }
}

void NeuronExtension::onReplyFinished(NeuronExtensionReply *reply)
{
    if (reply->slaveAddress() != m_slaveAddress) {
        return;
    }

    QVector<uint16_t> data = reply->result();
    int startAddress = reply->startAddress();

    int modbusAddress = 0;
    bool digitalOutputNeedsUpdate = false;
    bool relayOutputNeedsUpate = false;
    for (int i = 0; i < data.length(); i++) {

        modbusAddress = startAddress + i;
        QString circuit;
        switch (reply->registerType()) {
        case RegisterTypeCoils: {
            if (m_modbusDigitalInputRegisters.values().contains(modbusAddress)) {
                circuit = m_modbusDigitalInputRegisters.key(modbusAddress);
                if (!m_previousDigitalInputValue.contains(circuit) ||
                        (m_previousDigitalInputValue.contains(circuit) && (m_previousDigitalInputValue.value(circuit) != data.value(i)))) {
                    // Only emit status change of the circuit value has changed
                    m_previousDigitalInputValue.insert(circuit, data.value(i)); //insert or update existing value
                    emit digitalInputStatusChanged(circuit, (data.value(i) != 0));
                }
            } else if (m_modbusDigitalOutputRegisters->containsAddress(modbusAddress)) {
                if (m_modbusDigitalOutputRegisters->value(modbusAddress) != data.value(i)) {
                    digitalOutputNeedsUpdate = true;
                }
                circuit = m_modbusDigitalOutputRegisters->circuit(modbusAddress);
                if (!m_previousDigitalOutputValue.contains(circuit) ||
                        (m_previousDigitalOutputValue.contains(circuit) && (m_previousDigitalOutputValue.value(circuit) != data.value(i)))) {
                    // Only emit status change of the circuit value has changed
                    m_previousDigitalOutputValue.insert(circuit, data.value(i)); //insert or update existing value
                    emit digitalOutputStatusChanged(circuit, (data.value(i) != 0));
                }
            } else if (m_modbusRelayOutputRegisters->containsAddress(modbusAddress)) {
                if (m_modbusRelayOutputRegisters->value(modbusAddress) != data.value(i)) {
                    relayOutputNeedsUpate = true;
                }
                circuit = m_modbusRelayOutputRegisters->circuit(modbusAddress);
                if (!m_previousRelayOutputValue.contains(circuit) ||
                        (m_previousRelayOutputValue.contains(circuit) && (m_previousRelayOutputValue.value(circuit) != data.value(i)))) {
                    // Only emit status change of the circuit value has changed
                    m_previousRelayOutputValue.insert(circuit, data.value(i)); //insert or update existing value
                    emit relayOutputStatusChanged(circuit, (data.value(i) != 0));
                }
            } else {
                qCWarning(dcUniPi()) << "NeuronExtension: Received unrecognised coil register" << modbusAddress;
            }
            break;
        }
        case RegisterTypeRegisters: {
            if (m_modbusAnalogOutputRegisters.keys().contains(modbusAddress)) {
                //TODO has been temporary removed
            } else if(m_modbusAnalogInputRegisters.keys().contains(modbusAddress)){
                RegisterDescriptor descriptor = m_modbusAnalogInputRegisters.value(modbusAddress);
                circuit = descriptor.circuit;
                quint32 value = 0;
                if (descriptor.count == 1) {
                    value = data.value(i);
                } else if (descriptor.count == 2) {
                    if (data.length() >= (i+1)) {
                        value = (data.value(i) << 16 | data.value(i+1));
                        i++;
                    } else {
                        qCWarning(dcUniPi()) << "NeuronExtension: Received analog input, but value count is too short";
                    }
                }
                if (!m_previousAnalogInputValue.contains(circuit) ||
                        (m_previousAnalogInputValue.contains(circuit) && (m_previousAnalogInputValue.value(circuit) != value))) {
                    m_previousAnalogInputValue.insert(circuit, value);
                    emit analogInputStatusChanged(circuit, value);
                }

            } else {
                qCWarning(dcUniPi()) << "NeuronExtension: Received unrecognised input register" << modbusAddress;
            }
            break;
        }
        }
    }

    if (reply->registerType() == RegisterTypeCoils) {
        QTimer::singleShot(180, this, [this] {
            getCoils();
        });
    } else {
        /*QTimer::singleShot(180, this, [this] {
            getRegisters();
        });*/
    }
    m_watchdog->start(); //restart watchdog timer;

    if (digitalOutputNeedsUpdate) {
        sendWriteRequest(m_modbusDigitalOutputRegisters->createWriteRequest(m_slaveAddress));
    }
    if (relayOutputNeedsUpate) {
        sendWriteRequest(m_modbusRelayOutputRegisters->createWriteRequest(m_slaveAddress));
    }
}

void NeuronExtension::onDeviceInformationReplyFinished(NeuronExtensionReply *reply)
{
    qCDebug(dcUniPi()) << "NeuronExtension: Device information reply finished";
    if (reply->slaveAddress() != m_slaveAddress) {
        qCWarning(dcUniPi()) << "NeuronExtension: Device information reply received, slave address does not fit";
        return;
    }

    QVector<uint16_t> data = reply->result();
    if (reply->startAddress() != 1000) {
        qCWarning(dcUniPi()) << "NeuronExtension: Start address does not fit";
    }

    if (data.length() < 7) {
        qCWarning(dcUniPi()) << "NeuronExtension: Device information response is too short" << data.length();
        return;
    }

    DeviceInformation info;
    info.firmwareVersion = data[0];
    info.numberOfDIs = data[1]&0xff;
    info.numberOfDOs = data[1]>>8;
    info.numberOfInternalRS485Lines = data[2]&0x0f;
    info.numberOfAOs = (data[2]>>4)&0x0f;
    info.numberOfAIs = data[2]>>8;
    info.firmwareId = data[3];
    info.hardwareId = data[4];
    info.serialNumber = (static_cast<uint32_t>(data[6])<<16) | data[5];
    emit deviceInformationReceived(info);
}

bool NeuronExtension::relayOutputValue(const QString &circuit)
{
    return m_modbusRelayOutputRegisters->value(circuit);
}

bool NeuronExtension::digitalOutputValue(const QString &circuit)
{
    return m_modbusDigitalOutputRegisters->value(circuit);
}

bool NeuronExtension::digitalInputValue(const QString &circuit)
{
    return m_previousDigitalInputValue.value(circuit);
}

double NeuronExtension::analogOutputValue(const QString &circuit)
{
    Q_UNUSED(circuit)
    return 0.f; //TODO
}

double NeuronExtension::analogInputValue(const QString &circuit)
{
    return m_previousAnalogInputValue.value(circuit);
}



OutputRegisters::OutputRegisters(int startAddress, int count, QObject *parent) :
    QObject(parent),
    m_startAddress(startAddress),
    m_registerCount(count)
{
    m_values.resize(count);
}

int OutputRegisters::address(const QString &circuit)
{
    if (!containsCircuit(circuit)) {
        qCWarning(dcUniPi()) << "OutputRegisters: Circuit not found" << circuit;
        return  -1;
    }
    return m_modbusRegisters.value(circuit);
}

bool OutputRegisters::containsAddress(int address)
{
    return m_modbusRegisters.values().contains(address);
}

QString OutputRegisters::circuit(int address)
{
    return m_modbusRegisters.key(address);
}

bool OutputRegisters::containsCircuit(const QString &circuit)
{
    return m_modbusRegisters.contains(circuit);
}

QStringList OutputRegisters::circuits()
{
    return m_modbusRegisters.keys();
}

QVector<uint16_t> OutputRegisters::values()
{
    return m_values;
}

uint16_t OutputRegisters::value(const QString &circuit)
{
    if (!containsCircuit(circuit)) {
        return 0;
    }
    return value(address(circuit));
}

uint16_t OutputRegisters::value(int address)
{
    return m_values[address];
}

void OutputRegisters::setValue(const QString &circuit, uint16_t value)
{
    if (!containsCircuit(circuit)) {
        return;
    }
    int add = address(circuit);
    setValue(add, value);
}

void OutputRegisters::setValue(int addr, uint16_t value)
{
    if (m_values[addr] == value) {
        return;
    }
    m_values[addr] = value;
    emit valuesChanged();
}

NeuronExtensionRequest OutputRegisters::createWriteRequest(int slaveAddress)
{
    NeuronExtensionRequest request(NeuronExtensionRegisterType::RegisterTypeCoils, slaveAddress, m_startAddress, m_registerCount, true);
    request.setValues(m_values);
    return request;
}

void OutputRegisters::insert(const QString &circuit, int address)
{
    m_modbusRegisters.insert(circuit, address);
}
