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

#include "neuron.h"
#include "extern-plugininfo.h"

#include <QFile>
#include <QTextStream>
#include <QStandardPaths>

Neuron::Neuron(NeuronTypes neuronType, QModbusTcpClient *modbusInterface,  QObject *parent) :
    QObject(parent),
    m_modbusInterface(modbusInterface),
    m_neuronType(neuronType)
{
    qCDebug(dcUniPi()) << "Neuron: Creating Neuron connection" << neuronType;
    m_inputPollingTimer = new QTimer(this);
    connect(m_inputPollingTimer, &QTimer::timeout, this, &Neuron::onInputPollingTimer);
    m_inputPollingTimer->setTimerType(Qt::TimerType::PreciseTimer);
    m_inputPollingTimer->setInterval(200);

    m_outputPollingTimer = new QTimer(this);
    connect(m_outputPollingTimer, &QTimer::timeout, this, &Neuron::onOutputPollingTimer);
    m_outputPollingTimer->setTimerType(Qt::TimerType::PreciseTimer);
    m_outputPollingTimer->setInterval(1000);

    if (m_modbusInterface->state() == QModbusDevice::State::ConnectedState) {
        m_inputPollingTimer->start();
        m_outputPollingTimer->start();
    }

    connect(m_modbusInterface, &QModbusDevice::stateChanged, this, [this] (QModbusDevice::State state) {
        if (state == QModbusDevice::State::ConnectedState) {
            qCDebug(dcUniPi()) << "Neuron: Starting polling timer";
            if (m_inputPollingTimer)
                m_inputPollingTimer->start();
            if (m_outputPollingTimer)
                m_outputPollingTimer->start();
            emit connectionStateChanged(true);
        } else {
            qCDebug(dcUniPi()) << "Neuron: Stopping polling timer";
            if (m_inputPollingTimer)
                m_inputPollingTimer->stop();
            if (m_outputPollingTimer)
                m_outputPollingTimer->stop();
            emit connectionStateChanged(false);
        }
    });
}

Neuron::~Neuron()
{
    qCDebug(dcUniPi()) << "Neuron: Deleting Neuron connection" << m_neuronType;
}

bool Neuron::init()
{
    qCDebug(dcUniPi()) << "Neuron: Init";
    if (!loadModbusMap()) {
        return false;
    }

    if (!m_modbusInterface) {
        qWarning(dcUniPi()) << "Neuron: Modbus TCP interface not available";
        return false;
    }

    if (m_modbusInterface->connectDevice()) {
        qWarning(dcUniPi()) << "Neuron: Could not connect to modbus TCP device";
        return false;
    }
    return true;
}

QString Neuron::type()
{
    switch (m_neuronType) {
    case NeuronTypes::S103:
        return  "S103";
    case NeuronTypes::M103:
        return  "M103";
    case NeuronTypes::M203:
        return  "M203";
    case NeuronTypes::M303:
        return  "M303";
    case NeuronTypes::M403:
        return  "M403";
    case NeuronTypes::M503:
        return  "M503";
    case NeuronTypes::M523:
        return  "M523";
    case NeuronTypes::L203:
        return  "L203";
    case NeuronTypes::L303:
        return  "L303";
    case NeuronTypes::L403:
        return  "L403";
    case NeuronTypes::L503:
        return  "L503";
    case NeuronTypes::L513:
        return  "L513";
    case NeuronTypes::L523:
        return  "L523";
    case NeuronTypes::L533:
        return  "L533";
    }
    return "Unknown";
}

QList<QString> Neuron::digitalInputs()
{
    return m_modbusDigitalInputRegisters.keys();
}

QList<QString> Neuron::digitalOutputs()
{
    return m_modbusDigitalOutputRegisters.keys();
}

QList<QString> Neuron::analogInputs()
{
    return m_modbusAnalogInputRegisters.keys();
}

QList<QString> Neuron::analogOutputs()
{
    return m_modbusAnalogOutputRegisters.keys();
}

QList<QString> Neuron::userLEDs()
{
    return m_modbusUserLEDRegisters.keys();
}


bool Neuron::loadModbusMap()
{
    qCDebug(dcUniPi()) << "Neuron: Load modbus map";

    QStringList fileCoilList;
    QStringList fileRegisterList;

    switch (m_neuronType) {
    case NeuronTypes::S103:
        fileCoilList.append(QString("/Neuron_S103/Neuron_S103-Coils-group-1.csv"));
        break;
    case NeuronTypes::M103:
        fileCoilList.append(QString("/Neuron_M103/Neuron_M103-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M103/Neuron_M103-Coils-group-2.csv"));
        break;
    case NeuronTypes::M203:
        fileCoilList.append(QString("/Neuron_M203/Neuron_M203-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M203/Neuron_M203-Coils-group-2.csv"));
        break;
    case NeuronTypes::M303:
        fileCoilList.append(QString("/Neuron_M303/Neuron_M303-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M303/Neuron_M303-Coils-group-2.csv"));
        break;
    case NeuronTypes::M403:
        fileCoilList.append(QString("/Neuron_M403/Neuron_M403-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M403/Neuron_M403-Coils-group-2.csv"));
        break;
    case NeuronTypes::M503:
        fileCoilList.append(QString("/Neuron_M503/Neuron_M503-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M503/Neuron_M503-Coils-group-2.csv"));
        break;
    case NeuronTypes::M523:
        fileCoilList.append(QString("/Neuron_M523/Neuron_M523-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M523/Neuron_M523-Coils-group-2.csv"));
        break;
    case NeuronTypes::L203:
        fileCoilList.append(QString("/Neuron_L203/Neuron_L203-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L203/Neuron_L203-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L203/Neuron_L203-Coils-group-3.csv"));
        break;
    case NeuronTypes::L303:
        fileCoilList.append(QString("/Neuron_L303/Neuron_L303-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L303/Neuron_L303-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L303/Neuron_L303-Coils-group-3.csv"));
        break;
    case NeuronTypes::L403:
        fileCoilList.append(QString("/Neuron_L403/Neuron_L403-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L403/Neuron_L403-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L403/Neuron_L403-Coils-group-3.csv"));
        break;
    case NeuronTypes::L503:
        fileCoilList.append(QString("/Neuron_L503/Neuron_L503-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L503/Neuron_L503-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L503/Neuron_L503-Coils-group-3.csv"));
        break;
    case NeuronTypes::L513:
        fileCoilList.append(QString("/Neuron_L513/Neuron_L513-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L513/Neuron_L513-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L513/Neuron_L513-Coils-group-3.csv"));
        break;
    case NeuronTypes::L523:
        fileCoilList.append(QString("/Neuron_L523/Neuron_L523-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L523/Neuron_L523-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L523/Neuron_L523-Coils-group-3.csv"));
        break;
    case NeuronTypes::L533:
        fileCoilList.append(QString("/Neuron_L533/Neuron_L533-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L533/Neuron_L533-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L533/Neuron_L533-Coils-group-3.csv"));
        break;
    }

    foreach(QString relativeFilePath, fileCoilList) {
        QString absoluteFilePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).last() + "/nymea/modbus" + relativeFilePath;
        qDebug(dcUniPi()) << "Neuron: Open CSV File:" << absoluteFilePath;
        QFile *csvFile = new QFile(absoluteFilePath);
        if (!csvFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(dcUniPi()) << csvFile->errorString() << "Path:" << absoluteFilePath;
            csvFile->deleteLater();
            return false;
        }
        QTextStream *textStream = new QTextStream(csvFile);
        while (!textStream->atEnd()) {
            QString line = textStream->readLine();
            QStringList list = line.split(',');
            if (list.length() <= 4) {
                qCWarning(dcUniPi()) << "Neuron: Currupted CSV file:" << csvFile->fileName();
                csvFile->deleteLater();
                return false;
            }
            if (list[4] == "Basic") {
                QString circuit = list[3].split(" ").last();
                if (list[3].contains("Digital Input", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalInputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found input register" << circuit << list[0].toInt();
                } else if (list[3].contains("Digital Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found output register" << circuit << list[0].toInt();
                } else if (list[3].contains("Relay Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found relay register" << circuit << list[0].toInt();
                } else if (list[3].contains("User Programmable LED", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusUserLEDRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found user programmable led" << circuit << list[0].toInt();
                }
            }
        }
        csvFile->close();
        csvFile->deleteLater();
    }

    switch (m_neuronType) {
    case NeuronTypes::S103:
        fileRegisterList.append(QString("/Neuron_S103/Neuron_S103-Registers-group-1.csv"));
        break;
    case NeuronTypes::M103:
        fileRegisterList.append(QString("/Neuron_M103/Neuron_M103-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M103/Neuron_M103-Registers-group-2.csv"));
        break;
    case NeuronTypes::M203:
        fileRegisterList.append(QString("/Neuron_M203/Neuron_M203-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M203/Neuron_M203-Registers-group-2.csv"));
        break;
    case NeuronTypes::M303:
        fileRegisterList.append(QString("/Neuron_M303/Neuron_M303-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M303/Neuron_M303-Registers-group-2.csv"));
        break;
    case NeuronTypes::M403:
        fileRegisterList.append(QString("/Neuron_M403/Neuron_M403-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M403/Neuron_M403-Registers-group-2.csv"));
        break;
    case NeuronTypes::M503:
        fileRegisterList.append(QString("/Neuron_M503/Neuron_M503-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M503/Neuron_M503-Registers-group-2.csv"));
        break;
    case NeuronTypes::M523:
        fileRegisterList.append(QString("/Neuron_M523/Neuron_M523-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M523/Neuron_M523-Registers-group-2.csv"));
        break;
    case NeuronTypes::L203:
        fileRegisterList.append(QString("/Neuron_L203/Neuron_L203-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L203/Neuron_L203-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L203/Neuron_L203-Registers-group-3.csv"));
        break;
    case NeuronTypes::L303:
        fileRegisterList.append(QString("/Neuron_L303/Neuron_L303-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L303/Neuron_L303-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L303/Neuron_L303-Registers-group-3.csv"));
        break;
    case NeuronTypes::L403:
        fileRegisterList.append(QString("/Neuron_L403/Neuron_L403-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L403/Neuron_L403-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L403/Neuron_L403-Registers-group-3.csv"));
        break;
    case NeuronTypes::L503:
        fileRegisterList.append(QString("/Neuron_L503/Neuron_L503-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L503/Neuron_L503-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L503/Neuron_L503-Registers-group-3.csv"));
        break;
    case NeuronTypes::L513:
        fileRegisterList.append(QString("/Neuron_L513/Neuron_L513-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L513/Neuron_L513-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L513/Neuron_L513-Registers-group-3.csv"));
        break;
    case NeuronTypes::L523:
        fileRegisterList.append(QString("/Neuron_L523/Neuron_L523-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L523/Neuron_L523-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L523/Neuron_L523-Registers-group-3.csv"));
        break;
    case NeuronTypes::L533:
        fileRegisterList.append(QString("/Neuron_L533/Neuron_L533-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L533/Neuron_L533-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L533/Neuron_L533-Registers-group-3.csv"));
        break;
    }
    foreach (QString relativeFilePath, fileRegisterList) {
        QString absoluteFilePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).last() + "/nymea/modbus" + relativeFilePath;
        qDebug(dcUniPi()) << "Neuron: Open CSV File:" << absoluteFilePath;
        QFile *csvFile = new QFile(absoluteFilePath);
        if (!csvFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(dcUniPi()) << "Neuron:" << csvFile->errorString() << "Path:" << absoluteFilePath;
            csvFile->deleteLater();
            return false;
        }
        QTextStream *textStream = new QTextStream(csvFile);
        while (!textStream->atEnd()) {
            QString line = textStream->readLine();
            QStringList list = line.split(',');
            if (list.length() <= 5) {
                qCWarning(dcUniPi()) << "Neuron: Currupted CSV file:" << csvFile->fileName();
                csvFile->deleteLater();
                return false;
            }
            if (list.last() == "Basic") {
                QString circuit = list[5].split(" ").last();
                if (list[5].contains("Analog Input Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogInputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found analog input register" << circuit << list[0].toInt();
                } else if (list[5].contains("Analog Output Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found analog output register" << circuit << list[0].toInt();
                }
            }
        }
        csvFile->close();
        csvFile->deleteLater();
    }
    return true;
}


bool Neuron::modbusWriteRequest(const Request &request)
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
                    qCWarning(dcUniPi()) << "Neuron: Write response error:" << reply->error();
                    emit requestError(request.id, reply->errorString());
                }
            });
            QTimer::singleShot(m_responseTimeoutTime, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return false;
        }
    } else {
        qCWarning(dcUniPi()) << "Neuron: Read error: " << m_modbusInterface->errorString();
        return false;
    }
    return true;
}


bool Neuron::modbusReadRequest(const QModbusDataUnit &request)
{
    if (!m_modbusInterface)
        return false;

    if (QModbusReply *reply = m_modbusInterface->sendReadRequest(request, m_slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, this] {

                int modbusAddress = 0;

                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();

                    for (int i = 0; i < static_cast<int>(unit.valueCount()); i++) {
                        //qCDebug(dcUniPi()) << "Start Address:" << unit.startAddress() << "Register Type:" << unit.registerType() << "Value:" << unit.value(i);
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
                                qCWarning(dcUniPi()) << "Neuron: Received unrecorgnised coil register" << modbusAddress;
                            }
                            break;

                        case QModbusDataUnit::RegisterType::HoldingRegisters:
                            if(m_modbusAnalogOutputRegisters.values().contains(modbusAddress)){
                                circuit = m_modbusAnalogOutputRegisters.key(modbusAddress);
                                emit analogOutputStatusChanged(circuit, unit.value(i));
                            } else {
                                qCWarning(dcUniPi()) << "Neuron: Received unrecognised holding register" << modbusAddress;
                            }
                            break;
                        case QModbusDataUnit::RegisterType::InputRegisters:
                            if(m_modbusAnalogInputRegisters.values().contains(modbusAddress)){
                                circuit = m_modbusAnalogInputRegisters.key(modbusAddress);
                                // Analog inputs consist out of 2 registers
                                emit analogInputStatusChanged(circuit, (unit.value(i) << 16 | unit.value(i+1)));
                                i++;
                            } else {
                                qCWarning(dcUniPi()) << "Neuron: Received unrecognised input register" << modbusAddress;
                            }
                            break;
                        case QModbusDataUnit::RegisterType::DiscreteInputs:
                        case QModbusDataUnit::RegisterType::Invalid:
                            qCWarning(dcUniPi()) << "Neuron: Invalide register type";
                            break;
                        }
                    }
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    qCWarning(dcUniPi()) << "Neuron: Read response error:" << reply->errorString() << reply->rawResult().exceptionCode();
                } else {
                    qCWarning(dcUniPi()) << "Neuron: Read response error:" << reply->error() << reply->errorString();
                }
            });
            QTimer::singleShot(m_responseTimeoutTime, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return false;
        }
    } else {
        qCWarning(dcUniPi()) << "Neuron: Read error: " << m_modbusInterface->errorString();
        return false;
    }
    return true;
}


bool Neuron::getInputRegisters(QList<int> registerList)
{
    if (registerList.isEmpty()) {
        return true;
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
        QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, startAddress, registerGroups.value(startAddress));
        if (m_readRequestQueue.isEmpty()) {
            modbusReadRequest(request);
        } else if (m_readRequestQueue.length() > 100) {
            qCWarning(dcUniPi()) << "Neuron: Too many pending read requests";
        } else {
            m_readRequestQueue.append(request);
        }
    }
    return true;
}

bool Neuron::getHoldingRegisters(QList<int> registerList)
{
    if (registerList.isEmpty()) {
        return true;
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
        qDebug(dcUniPi()) << "Register" << startAddress << "length" << registerGroups.value(startAddress);
        QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, startAddress, registerGroups.value(startAddress));
        modbusReadRequest(request);
    }
    return true;
}

bool Neuron::getCoils(QList<int> registerList)
{
    if (registerList.isEmpty()) {
        return true;
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
            qCWarning(dcUniPi()) << "Neuron: Too many pending read requests";
        } else {
            m_readRequestQueue.append(request);
        }
    }
    return true;
}


bool Neuron::getAllDigitalInputs()
{
    if (!m_modbusInterface) {
        qCWarning(dcUniPi()) << "Neuron: Ḿodbus interface not initialized";
        return false;
    }
    return getCoils(m_modbusDigitalInputRegisters.values());
}

bool Neuron::getAllDigitalOutputs()
{
    if (!m_modbusInterface) {
        qCWarning(dcUniPi()) << "Neuron: Modbus interface not initialized";
        return false;
    }
    return getCoils(m_modbusDigitalOutputRegisters.values());
}

bool Neuron::getAllAnalogInputs()
{
    if (!m_modbusInterface) {
        qCWarning(dcUniPi()) << "Neuron: Modbus interface not initialized";
        return false;
    }
    foreach(QString circuit, m_modbusAnalogInputRegisters.keys()) {
        getAnalogInput(circuit);
    }
    return true;
}

bool Neuron::getAllAnalogOutputs()
{
    if (!m_modbusInterface) {
        qCWarning(dcUniPi()) << "Neuron: Modbus interface not initialized";
        return false;
    }
    foreach(QString circuit, m_modbusAnalogOutputRegisters.keys()) {
        getAnalogOutput(circuit);
    }
    return true;
}

bool Neuron::getDigitalInput(const QString &circuit)
{
    int modbusAddress = m_modbusDigitalInputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Neuron: Reading digital Input" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    if (m_readRequestQueue.isEmpty()) {
        //return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        qCWarning(dcUniPi()) << "Neuron: Too many pending read requests";
        return false;
    } else {
        //m_readRequestQueue.append(request); //FIXME
    }
    return true;
}

bool Neuron::getAnalogOutput(const QString &circuit)
{
    int modbusAddress = m_modbusAnalogInputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Neuron: Reading analog input" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, 1);
    if (m_readRequestQueue.isEmpty()) {
        //return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        qCWarning(dcUniPi()) << "Neuron extension: too many pending read requests";
        return false;
    } else {
        //m_readRequestQueue.append(request); //FIXME
    }
    return true;
}


QUuid Neuron::setDigitalOutput(const QString &circuit, bool value)
{
    int modbusAddress = m_modbusDigitalOutputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Neuron: Setting digital ouput" << circuit << modbusAddress << value;

    Request request;
    request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    request.data.setValue(0, static_cast<uint16_t>(value));
    request.id = QUuid::createUuid();

    if (m_writeRequestQueue.isEmpty()) {
        modbusWriteRequest(request);
    } else if (m_writeRequestQueue.length() > 100) {
        return "";
    } else {
        m_writeRequestQueue.append(request);
    }
    return request.id;
}


bool Neuron::getDigitalOutput(const QString &circuit)
{
    int modbusAddress = m_modbusDigitalOutputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Reading digital Output" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, 1);
    if (m_readRequestQueue.isEmpty()) {
        return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        qCWarning(dcUniPi()) << "Neuron: Too many pending read requests";
        return false;
    } else {
        m_readRequestQueue.append(request);
    }
    return true;
}


QUuid Neuron::setAnalogOutput(const QString &circuit, double value)
{
    int modbusAddress = m_modbusAnalogOutputRegisters.value(circuit);
    qDebug(dcUniPi()) << "Neuron: Writing analog Output" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return "";

    Request request;
    request.id = QUuid::createUuid();
    request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, 2);
    request.data.setValue(0, (static_cast<uint32_t>(value) >> 16));    //FIXME
    request.data.setValue(0, (static_cast<uint32_t>(value) & 0xffff)); //FIXME

    if (m_writeRequestQueue.isEmpty()) {
        modbusWriteRequest(request);
    } else if (m_writeRequestQueue.length() > 100) {
        return "";
    } else {
        m_writeRequestQueue.append(request);
    }

    return request.id;
}


bool Neuron::getAnalogInput(const QString &circuit)
{
    int modbusAddress = m_modbusAnalogInputRegisters.value(circuit);
    qDebug(dcUniPi()) << "Neuron: Reading analog Input" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, modbusAddress, 2);
    if (m_readRequestQueue.isEmpty()) {
        return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        qCWarning(dcUniPi()) << "Neuron: Too many pending read requests";
        return false;
    } else {
        m_readRequestQueue.append(request);
    }
    return true;
}

QUuid Neuron::setUserLED(const QString &circuit, bool value)
{
    int modbusAddress = m_modbusUserLEDRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Neuron: Setting digital ouput" << circuit << modbusAddress << value;

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


bool Neuron::getUserLED(const QString &circuit)
{
    int modbusAddress = m_modbusUserLEDRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Neuron: Reading digital Output" << circuit << modbusAddress;

    if (!m_modbusInterface)
        return false;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    if (m_readRequestQueue.isEmpty()) {
        //return modbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 100) {
        qCWarning(dcUniPi()) << "Neuron: Too many pending read requests";
        return false;
    } else {
        //m_readRequestQueue.append(request);
    }
    return true;
}


void Neuron::onOutputPollingTimer()
{
    getAllDigitalOutputs();
    getAllAnalogOutputs();
}

void Neuron::onInputPollingTimer()
{
    getAllDigitalInputs();
    getAllAnalogInputs();
}
