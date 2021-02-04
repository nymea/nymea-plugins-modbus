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
#include <QModbusDataUnit>
#include <QStandardPaths>

NeuronExtension::NeuronExtension(ExtensionTypes extensionType, QModbusRtuSerialMaster *modbusInterface, int slaveAddress, QObject *parent) :
    QObject(parent),
    m_modbusInterface(modbusInterface),
    m_slaveAddress(slaveAddress),
    m_extensionType(extensionType)
{
    qCDebug(dcUniPi()) << "NeuronExtension: Creating extension" << extensionType;
    m_inputPollingTimer = new QTimer(this);
    connect(m_inputPollingTimer, &QTimer::timeout, this, &NeuronExtension::onInputPollingTimer);
    m_inputPollingTimer->setTimerType(Qt::TimerType::PreciseTimer);
    m_inputPollingTimer->setInterval(200);

    m_outputPollingTimer = new QTimer(this);
    connect(m_outputPollingTimer, &QTimer::timeout, this, &NeuronExtension::onOutputPollingTimer);
    m_outputPollingTimer->setTimerType(Qt::TimerType::PreciseTimer);
    m_outputPollingTimer->setInterval(1000);

    if (m_modbusInterface->state() == QModbusDevice::State::ConnectedState) {
        m_inputPollingTimer->start();
        m_outputPollingTimer->start();
    }

    connect(m_modbusInterface, &QModbusDevice::stateChanged, this, [this] (QModbusDevice::State state) {
        if (state == QModbusDevice::State::ConnectedState) {
            if (m_inputPollingTimer)
                m_inputPollingTimer->start();
            if (m_outputPollingTimer)
                m_outputPollingTimer->start();
            emit connectionStateChanged(true);
        } else {
            if (m_inputPollingTimer)
                m_inputPollingTimer->stop();
            if (m_outputPollingTimer)
                m_outputPollingTimer->stop();
            emit connectionStateChanged(false);
        }
    });
}

NeuronExtension::~NeuronExtension()
{
    qCDebug(dcUniPi()) << "Neuron Extension: Deleting extension" << m_extensionType;
}

bool NeuronExtension::init()
{
    qCDebug(dcUniPi()) << "Neuron Extension: Init";
    if (!loadModbusMap()) {
        return false;
    }

    if (!m_modbusInterface) {
        qWarning(dcUniPi()) << "Neuron Extension: Modbus RTU interface not available";
        return false;
    }

    if (m_modbusInterface->connectDevice()) {
        qWarning(dcUniPi()) << "Neuron Extension: Could not connect to RTU device";
        return  false;
    }
    return true;
}

QString NeuronExtension::type()
{
    switch(m_extensionType) {
    case ExtensionTypes::xS10:
        return "xS10";
    case ExtensionTypes::xS20:
        return "xS20";
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

int NeuronExtension::slaveAddress()
{
    return m_slaveAddress;
}

void NeuronExtension::setSlaveAddress(int slaveAddress)
{
    qCDebug(dcUniPi()) << "Neuron Extension: Set slave address" << slaveAddress;
    m_slaveAddress = slaveAddress;
}

QList<QString> NeuronExtension::digitalInputs()
{
    return m_modbusDigitalInputRegisters.keys();
}

QList<QString> NeuronExtension::digitalOutputs()
{
    return m_modbusDigitalOutputRegisters.keys();
}

QList<QString> NeuronExtension::analogInputs()
{
    return m_modbusAnalogInputRegisters.keys();
}

QList<QString> NeuronExtension::analogOutputs()
{
    return m_modbusAnalogOutputRegisters.keys();
}

QList<QString> NeuronExtension::userLEDs()
{
    return m_modbusUserLEDRegisters.keys();
}

bool NeuronExtension::loadModbusMap()
{
    qCDebug(dcUniPi()) << "Neuron Extension: Load modbus map";

    QStringList fileCoilList;
    QStringList fileRegisterList;

    switch(m_extensionType) {
    case ExtensionTypes::xS10:
        fileCoilList.append(QString("/Neuron_xS10/Neuron_xS10-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS20:
        fileCoilList.append(QString("/Neuron_xS20/Neuron_xS20-Coils-group-1.csv"));
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
        qDebug(dcUniPi()) << "Neuron Extension: Open CSV File:" << absoluteFilePath;
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
                qCWarning(dcUniPi()) << "Neuron Extension: currupted CSV file:" << csvFile->fileName();
                csvFile->deleteLater();
                return false;
            }
            if (list[4] == "Basic") {
                QString circuit = list[3].split(" ").last();
                if (list[3].contains("Digital Input", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalInputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found input register" << circuit << list[0].toInt();
                } else if (list[3].contains("Digital Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found output register" << circuit << list[0].toInt();
                } else if (list[3].contains("Relay Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found relay register" << circuit << list[0].toInt();
                }  else if (list[3].contains("User Programmable LED", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusUserLEDRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found user programmable led" << circuit << list[0].toInt();
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
    case ExtensionTypes::xS20:
        fileRegisterList.append(QString("/Neuron_xS20/Neuron_xS20-Registers-group-1.csv"));
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
        qDebug(dcUniPi()) << "Neuron Extension: Open CSV File:" << absoluteFilePath;
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
                qCWarning(dcUniPi()) << "Neuron Extension: Currupted CSV file:" << csvFile->fileName();
                csvFile->deleteLater();
                return false;
            }
            if (list.last() == "Basic" && list[5].split(" ").length() > 3) {
                if (list[5].split(" ").length() <= 3) {
                    qCWarning(dcUniPi()) << "Neuron Extension: Currupted CSV file:" << csvFile->fileName();
                    csvFile->deleteLater();
                    return false;
                }
                QString circuit = list[5].split(" ").at(3);
                if (list[5].contains("Analog Input Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogInputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found analog input register" << circuit << list[0].toInt();
                } else if (list[5].contains("Analog Output Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found analog output register" << circuit << list[0].toInt();
                }
            }
        }
        csvFile->close();
        csvFile->deleteLater();
    }
    return true;
}

bool NeuronExtension::modbusReadRequest(const QModbusDataUnit &request)
{
    if (!m_modbusInterface)
        return false;

    if (QModbusReply *reply = m_modbusInterface->sendReadRequest(request, m_slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, this] {

                int modbusAddress = 0;

                if (!m_readRequestQueue.isEmpty()) {
                    modbusReadRequest(m_readRequestQueue.takeFirst());
                }

                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();

                    for (int i = 0; i < static_cast<int>(unit.valueCount()); i++) {
                        //qCDebug(dcUniPi()) << "Neuron Extension: Start Address:" << unit.startAddress() << "Register Type:" << unit.registerType() << "Value:" << unit.value(i);
                        modbusAddress = unit.startAddress() + i;

                        if (m_previousModbusRegisterValue.contains(modbusAddress)) {
                            if (m_previousModbusRegisterValue.value(modbusAddress) == unit.value(i)) {
                                continue;
                            } else  {
                                m_previousModbusRegisterValue.insert(modbusAddress, unit.value(i)); //update existing value
                            }
                        } else {
                            m_previousModbusRegisterValue.insert(modbusAddress, unit.value(i));
                        }

                        QString circuit;
                        switch (unit.registerType()) {
                        case QModbusDataUnit::RegisterType::Coils:
                            if(m_modbusDigitalInputRegisters.values().contains(modbusAddress)){
                                circuit = m_modbusDigitalInputRegisters.key(modbusAddress);
                                emit digitalInputStatusChanged(circuit, unit.value(i));
                            } else if(m_modbusDigitalOutputRegisters.values().contains(modbusAddress)){
                                circuit = m_modbusDigitalOutputRegisters.key(modbusAddress);
                                emit digitalOutputStatusChanged(circuit, unit.value(i));
                            } else if(m_modbusUserLEDRegisters.values().contains(modbusAddress)){
                                circuit = m_modbusUserLEDRegisters.key(modbusAddress);
                                emit userLEDStatusChanged(circuit, unit.value(i));
                            } else {
                                qCWarning(dcUniPi()) << "Neuron Extension: Received unrecorgnised coil register" << modbusAddress;
                            }
                            break;

                        case QModbusDataUnit::RegisterType::InputRegisters:
                            if(m_modbusAnalogInputRegisters.values().contains(modbusAddress)){
                                circuit = m_modbusAnalogInputRegisters.key(modbusAddress);
                                emit analogInputStatusChanged(circuit, ((unit.value(i) << 16) | unit.value(i+1)));
                                i++;
                            } else {
                                qCWarning(dcUniPi()) << "Neuron Extension: Received unrecorgnised input register" << modbusAddress;
                            }
                            break;
                        case QModbusDataUnit::RegisterType::HoldingRegisters:
                            if(m_modbusAnalogOutputRegisters.values().contains(modbusAddress)){
                                circuit = m_modbusAnalogOutputRegisters.key(modbusAddress);
                                emit analogOutputStatusChanged(circuit, unit.value(i));
                            } else {
                                qCWarning(dcUniPi()) << "Neuron Extension: Received unrecorgnised holding register" << modbusAddress;
                            }
                            break;
                        case QModbusDataUnit::RegisterType::DiscreteInputs:
                        case QModbusDataUnit::RegisterType::Invalid:
                            qCWarning(dcUniPi()) << "Neuron Extension: Invalide register type";
                            break;
                        }
                    }

                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    qCWarning(dcUniPi()) << "Neuron Extension: Read response error:" << reply->errorString() << reply->rawResult().exceptionCode();
                } else {
                    qCWarning(dcUniPi()) << "Neuron Extension: Read response error:" << reply->error();
                }
            });
            QTimer::singleShot(m_responseTimeoutTime, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return false;
        }
    } else {
        qCWarning(dcUniPi()) << "Neuron Extension: Read error: " << m_modbusInterface->errorString();
        return false;
    }
    return true;
}


bool NeuronExtension::modbusWriteRequest(const Request &request)
{
    if (!m_modbusInterface)
        return false;

    if (QModbusReply *reply = m_modbusInterface->sendWriteRequest(request.data, m_slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, request, this] {

                if (!m_writeRequestQueue.isEmpty()) {
                    modbusWriteRequest(m_writeRequestQueue.takeFirst());
                }

                if (reply->error() == QModbusDevice::NoError) {
                    requestExecuted(request.id, true);
                    const QModbusDataUnit unit = reply->result();
                    int modbusAddress = unit.startAddress();
                    if(m_modbusDigitalOutputRegisters.values().contains(modbusAddress)){
                        QString circuit = m_modbusDigitalOutputRegisters.key(modbusAddress);
                        emit digitalOutputStatusChanged(circuit, unit.value(0));
                    } else if(m_modbusAnalogOutputRegisters.values().contains(modbusAddress)){
                        QString circuit = m_modbusAnalogOutputRegisters.key(modbusAddress);
                        emit analogOutputStatusChanged(circuit, unit.value(0));
                    } else if(m_modbusUserLEDRegisters.values().contains(modbusAddress)){
                        QString circuit = m_modbusUserLEDRegisters.key(modbusAddress);
                        emit userLEDStatusChanged(circuit, unit.value(0));
                    }
                } else {
                    requestExecuted(request.id, false);
                    qCWarning(dcUniPi()) << "Neuron Extension: Read response error:" << reply->error();
                    emit requestError(request.id, reply->errorString());
                }
            });
            QTimer::singleShot(m_responseTimeoutTime, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return false;
        }
    } else {
        qCWarning(dcUniPi()) << "Neuron Extension: Read error: " << m_modbusInterface->errorString();
        return false;
    }
    return true;
}


bool NeuronExtension::getDigitalInput(const QString &circuit)
{
    int modbusAddress = m_modbusDigitalInputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Reading digital input" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    if (m_readRequestQueue.isEmpty()) {
        return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        return "";
    } else {
        m_readRequestQueue.append(request);
    }
    return true;
}


QUuid NeuronExtension::setDigitalOutput(const QString &circuit, bool value)
{
    int modbusAddress = m_modbusDigitalOutputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Setting digital ouput" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return "";

    Request request;
    request.id = QUuid::createUuid();

    request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    request.data.setValue(0, static_cast<uint16_t>(value));

    if (m_writeRequestQueue.isEmpty()) {
        modbusWriteRequest(request);
    } else if (m_writeRequestQueue.length() > 100) {
        return "";
    } else {
        m_writeRequestQueue.append(request);
    }

    return request.id;
}

bool NeuronExtension::getDigitalOutput(const QString &circuit)
{
    int modbusAddress = m_modbusDigitalOutputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Reading digital output" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    if (m_readRequestQueue.isEmpty()) {
        return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        qCWarning(dcUniPi()) << "Neuron extension: Too many pending read requests";
        return false;
    } else {
        m_readRequestQueue.append(request);
    }
    return true;
}


bool NeuronExtension::getAllDigitalInputs()
{
    if (!m_modbusInterface)
        return false;

    QList<QModbusDataUnit> requests;
    QList<int> registerList = m_modbusDigitalInputRegisters.values();

    if (registerList.isEmpty()) {
        return true; //device has no digital inputs
    }

    std::sort(registerList.begin(), registerList.end());
    int previousReg = registerList.first(); //first register to read and starting point to get the following registers
    int startAddress;

    QHash<int, int> registerGroups;

    foreach (int reg, registerList) {
        //qDebug(dcUniPi()) << "Register" << reg << "previous Register" << previousReg;
        if (reg == previousReg) { //first register
            startAddress = reg;
            registerGroups.insert(startAddress, 1);
        } else if (reg == (previousReg + 1)) { //next register in block
            previousReg = reg;
            registerGroups.insert(startAddress, (registerGroups.value(startAddress) + 1)); //update block length
        } else {    // new block
            startAddress = reg;
            previousReg = reg;
            registerGroups.insert(startAddress, 1);
        }
    }

    foreach (int startAddress, registerGroups.keys()) {
        QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, startAddress, registerGroups.value(startAddress));
        if (m_readRequestQueue.isEmpty()) {
            modbusReadRequest(request);
        } else if (m_readRequestQueue.length() > 100) {
            qCWarning(dcUniPi()) << "Neuron extension: too many pending read requests";
        } else {
            m_readRequestQueue.append(request);
        }
    }
    return true;
}

bool NeuronExtension::getAllAnalogOutputs()
{
    if (!m_modbusInterface)
        return false;

    foreach (QString circuit, m_modbusAnalogOutputRegisters.keys()) {
        getAnalogOutput(circuit);
    }
    return true;
}

bool NeuronExtension::getAllAnalogInputs()
{
    if (!m_modbusInterface)
        return false;

    foreach (QString circuit, m_modbusAnalogInputRegisters.keys()) {
        getAnalogInput(circuit);
    }
    return true;
}

bool NeuronExtension::getAllDigitalOutputs()
{
    if (!m_modbusInterface)
        return false;

    QList<QModbusDataUnit> requests;
    QList<int> registerList = m_modbusDigitalOutputRegisters.values();

    if (registerList.isEmpty()) {
        return true; //device has no digital outputs
    }

    std::sort(registerList.begin(), registerList.end());
    int previousReg = registerList.first(); //first register to read and starting point to get the following registers
    int startAddress;

    QHash<int, int> registerGroups;

    foreach (int reg, registerList) {
        //qDebug(dcUniPi()) << "Register" << reg << "previous Register" << previousReg;
        if (reg == previousReg) { //first register
            startAddress = reg;
            registerGroups.insert(startAddress, 1);
        } else if (reg == (previousReg + 1)) { //next register in block
            previousReg = reg;
            registerGroups.insert(startAddress, (registerGroups.value(startAddress) + 1)); //update block length
        } else {    // new block
            startAddress = reg;
            previousReg = reg;
            registerGroups.insert(startAddress, 1);
        }
    }

    foreach (int startAddress, registerGroups.keys()) {
        QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, startAddress, registerGroups.value(startAddress));
        if (m_readRequestQueue.isEmpty()) {
            modbusReadRequest(request);
        } else if (m_readRequestQueue.length() > 100) {
            qCWarning(dcUniPi()) << "Neuron extension: too many pending read requests";
        } else {
            m_readRequestQueue.append(request);
        }
    }
    return true;
}

QUuid NeuronExtension::setAnalogOutput(const QString &circuit, double value)
{
    int modbusAddress = m_modbusAnalogOutputRegisters.value(circuit);
    if (!m_modbusInterface)
        return "";

    Request request;
    request.id = QUuid::createUuid();
    request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, 1);
    request.data.setValue(0, static_cast<uint16_t>(value));

    if (m_writeRequestQueue.isEmpty()) {
        modbusWriteRequest(request);
    } else if (m_writeRequestQueue.length() > 100) {
        return "";
    } else {
        m_writeRequestQueue.append(request);
    }

    return request.id;
}


bool NeuronExtension::getAnalogOutput(const QString &circuit)
{
    int modbusAddress = m_modbusAnalogOutputRegisters.value(circuit);

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, 1);
    if (m_readRequestQueue.isEmpty()) {
        return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        qCWarning(dcUniPi()) << "Neuron extension: too many pending read requests";
        return false;
    } else {
        m_readRequestQueue.append(request);
    }
    return true;
}


bool NeuronExtension::getAnalogInput(const QString &circuit)
{
    int modbusAddress =  m_modbusAnalogInputRegisters.value(circuit);

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, modbusAddress, 2);
    if (m_readRequestQueue.isEmpty()) {
        return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        qCWarning(dcUniPi()) << "Neuron extension: too many pending read requests";
        return false;
    } else {
        m_readRequestQueue.append(request);
    }
    return true;
}

QUuid NeuronExtension::setUserLED(const QString &circuit, bool value)
{
    int modbusAddress = m_modbusUserLEDRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Setting digital ouput" << circuit << modbusAddress << value;

    if (!m_modbusInterface)
        return "";

    Request request;
    request.id = QUuid::createUuid();

    request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    request.data.setValue(0, static_cast<uint16_t>(value));

    if (m_writeRequestQueue.isEmpty()) {
        modbusWriteRequest(request);
    } else if (m_writeRequestQueue.length() > 100) {
        return "";
    } else {
        m_writeRequestQueue.append(request);
    }

    return request.id;
}


bool NeuronExtension::getUserLED(const QString &circuit)
{
    int modbusAddress = m_modbusUserLEDRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Reading digital Output" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    if (m_readRequestQueue.isEmpty()) {
        return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        qCWarning(dcUniPi()) << "Neuron extension: too many pending read requests";
        return false;
    } else {
        m_readRequestQueue.append(request);
    }
    return true;
}


void NeuronExtension::onOutputPollingTimer()
{
    getAllDigitalOutputs();
    getAllAnalogOutputs();
}

void NeuronExtension::onInputPollingTimer()
{
    getAllDigitalInputs();
    getAllAnalogInputs();
}
