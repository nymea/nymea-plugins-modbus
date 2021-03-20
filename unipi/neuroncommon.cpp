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

#include "neuroncommon.h"
#include "extern-plugininfo.h"

NeuronCommon::NeuronCommon(QModbusClient *modbusInterface, int slaveAddress, QObject *parent) :
    QObject(parent),
    m_slaveAddress(slaveAddress),
    m_modbusTcpInterface(modbusInterface)
{
    m_modbusType = ModbusTypeTcp;
    initTimers();
}

NeuronCommon::NeuronCommon(ModbusRtuMaster *modbusInterface, int slaveAddress, QObject *parent) :
    QObject(parent),
    m_slaveAddress(slaveAddress),
    m_modbusRtuInterface(modbusInterface)
{
    m_modbusType = ModbusTypeRtu;
    initTimers();
}

bool NeuronCommon::init()
{
    qCDebug(dcUniPi()) << "Neuron: Init";
    if (!loadModbusMap()) {
        return false;
    }

    if (!m_modbusTcpInterface) {
        qWarning(dcUniPi()) << "Neuron: Modbus interface not available";
        return false;
    }

    if (m_modbusTcpInterface->connectDevice()) {
        qWarning(dcUniPi()) << "Neuron: Could not connect to modbus device";
        return  false;
    }
    return true;
}

int NeuronCommon::slaveAddress()
{
    return m_slaveAddress;
}

void NeuronCommon::setSlaveAddress(int slaveAddress)
{
    qCDebug(dcUniPi()) << "Neuron: Set slave address" << slaveAddress;
    m_slaveAddress = slaveAddress;
}

QList<QString> NeuronCommon::digitalInputs()
{
    return m_modbusDigitalInputRegisters.keys();
}

QList<QString> NeuronCommon::digitalOutputs()
{
    return m_modbusDigitalOutputRegisters.keys();
}

QList<QString> NeuronCommon::analogInputs()
{
    QList<QString> circuits;
    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogInputRegisters.values()) {
        circuits.append(descriptor.circuit);
    }
    return circuits;
}

QList<QString> NeuronCommon::analogOutputs()
{
    QList<QString> circuits;
    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogOutputRegisters.values()) {
        circuits.append(descriptor.circuit);
    }
    return circuits;
}

QList<QString> NeuronCommon::userLEDs()
{
    return m_modbusUserLEDRegisters.keys();
}

NeuronCommon::RegisterDescriptor NeuronCommon::registerDescriptorFromStringList(const QStringList &data)
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
        descriptor.registerType = QModbusDataUnit::RegisterType::InputRegisters;
    } else if (data[5].contains("Analog Output Value", Qt::CaseSensitivity::CaseInsensitive)) {
        descriptor.registerType = QModbusDataUnit::RegisterType::HoldingRegisters;
    }
    return descriptor;
}

void NeuronCommon::initTimers()
{
    m_inputPollingTimer = new QTimer(this);
    connect(m_inputPollingTimer, &QTimer::timeout, this, &NeuronCommon::onInputPollingTimer);
    m_inputPollingTimer->setTimerType(Qt::TimerType::PreciseTimer);
    m_inputPollingTimer->setInterval(200);

    m_outputPollingTimer = new QTimer(this);
    connect(m_outputPollingTimer, &QTimer::timeout, this, &NeuronCommon::onOutputPollingTimer);
    m_outputPollingTimer->setTimerType(Qt::TimerType::PreciseTimer);
    m_outputPollingTimer->setInterval(1000);

    if (m_modbusTcpInterface->state() == QModbusDevice::State::ConnectedState) {
        m_inputPollingTimer->start();
        m_outputPollingTimer->start();
    }

    connect(m_modbusTcpInterface, &QModbusDevice::stateChanged, this, [this] (QModbusDevice::State state) {
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

bool NeuronCommon::circuitValueChanged(const QString &circuit, quint32 value)
{
    if (m_previousCircuitValue.contains(circuit)) {
        if (m_previousCircuitValue.value(circuit) == value) {
            // Only emit status change of the circuit value has changed
            return false;
        } else  {
            m_previousCircuitValue.insert(circuit, value); //update existing value
            return true;
        }
    } else {
        m_previousCircuitValue.insert(circuit, value);
        return true;
    }
}

void NeuronCommon::getAllDigitalInputs()
{
    getCoils(m_modbusDigitalInputRegisters.values());
}

void NeuronCommon::getAllDigitalOutputs()
{
    getCoils(m_modbusDigitalOutputRegisters.values());
}

void NeuronCommon::getAllAnalogInputs()
{
    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogInputRegisters.values()) {
        getAnalogIO(descriptor);
    }
}

void NeuronCommon::getAllAnalogOutputs()
{
    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogOutputRegisters.values()) {
        getAnalogIO(descriptor);
    }
}

bool NeuronCommon::getDigitalInput(const QString &circuit)
{
    if (!m_modbusDigitalInputRegisters.contains(circuit)) {
        qCWarning(dcUniPi()) << "Neuron: Digital input circuit not found" << circuit;
        return "";
    }
    int modbusAddress = m_modbusDigitalInputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Neuron: Reading digital Input" << circuit << modbusAddress;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    return readRequest(request);
}

bool NeuronCommon::getAnalogOutput(const QString &circuit)
{
    //qDebug(dcUniPi()) << "Neuron: Get analog output" << circuit;
    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogOutputRegisters.values()) {
        if (descriptor.circuit == circuit) {
            return getAnalogIO(descriptor);
        }
    }
    qCWarning(dcUniPi()) << "Neuron: Analog output circuit not found" << circuit;
    return false;
}


QUuid NeuronCommon::setDigitalOutput(const QString &circuit, bool value)
{
    if (!m_modbusDigitalOutputRegisters.contains(circuit)) {
        qCWarning(dcUniPi()) << "Neuron: Digital output circuit not found" << circuit;
        return "";
    }
    int modbusAddress = m_modbusDigitalOutputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Neuron: Setting digital ouput" << circuit << modbusAddress << value;

    Request request;
    request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    request.data.setValue(0, static_cast<uint16_t>(value));
    request.id = QUuid::createUuid();

    if (!writeRequest(request))
        return "";
    return request.id;
}


bool NeuronCommon::getDigitalOutput(const QString &circuit)
{
    if (!m_modbusDigitalOutputRegisters.contains(circuit)) {
        qCWarning(dcUniPi()) << "Neuron: Digital output circuit not found" << circuit;
        return false;
    }
    int modbusAddress = m_modbusDigitalOutputRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Reading digital Output" << circuit << modbusAddress;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, 1);
    return readRequest(request);
}


QUuid NeuronCommon::setAnalogOutput(const QString &circuit, double value)
{
    qDebug(dcUniPi()) << "Neuron: Set analog output" << circuit << value;

    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogOutputRegisters) {
        if (descriptor.circuit == circuit) {
            Request request;
            request.id = QUuid::createUuid();
            request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, descriptor.address, descriptor.count);
            if (descriptor.count == 1) {
                request.data.setValue(0, (static_cast<uint>(value*400))); // 0 to 4000 = 0 to 10.0 V
            } else if (descriptor.count == 2) {
                request.data.setValue(0, (static_cast<uint32_t>(value) >> 16));
                request.data.setValue(1, (static_cast<uint32_t>(value) & 0xffff));
            }

            if (!writeRequest(request))
                return "";
            return request.id;
        }
    }
    qCWarning(dcUniPi()) << "Neuron: Analog output circuit not found" << circuit;
    return "";
}


QUuid NeuronCommon::setAnalogOutputConfiguration(const QString &circuit, AnalogIOConfiguration value)
{
    qDebug(dcUniPi()) << "Neuron: Set analog output configuration" << circuit << value;
    if (!m_modbusAnalogOutputConfigurationRegisters.contains(circuit)) {
        qCWarning(dcUniPi()) << "Neuron: Analog output configuration register not found" << circuit;
        return "";
    }
    int modbusAddress = m_modbusAnalogOutputConfigurationRegisters.value(circuit);
    Request request;
    request.id = QUuid::createUuid();

    request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, 1);
    request.data.setValue(0, static_cast<uint16_t>(value));
    if (!writeRequest(request))
        return "";

    return request.id;
    return "";
}

QUuid NeuronCommon::setAnalogInputConfiguration(const QString &circuit, AnalogIOConfiguration value)
{
    qDebug(dcUniPi()) << "Neuron: Set analog input configuration" << circuit << value;
    if (!m_modbusAnalogInputConfigurationRegisters.contains(circuit)) {
        qCWarning(dcUniPi()) << "Neuron: Analog input configuration register not found" << circuit;
        return "";
    }
    int modbusAddress = m_modbusAnalogInputConfigurationRegisters.value(circuit);
    Request request;
    request.id = QUuid::createUuid();

    request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, modbusAddress, 1);
    request.data.setValue(0, static_cast<uint16_t>(value));
    if (!writeRequest(request))
        return "";

    return request.id;
    return "";
}

bool NeuronCommon::getAnalogInput(const QString &circuit)
{
    //qDebug(dcUniPi()) << "Neuron: Get analog input" << circuit;

    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogOutputRegisters.values()) {
        if (descriptor.circuit == circuit) {
            return getAnalogIO(descriptor);
        }
    }
    return false;
}

QUuid NeuronCommon::setUserLED(const QString &circuit, bool value)
{
    if (!m_modbusUserLEDRegisters.contains(circuit)) {
        qCWarning(dcUniPi()) << "Neuron: User LED circuit not found" << circuit;
        return "";
    }
    int modbusAddress = m_modbusUserLEDRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Neuron: Setting user led" << circuit << modbusAddress << value;

    Request request;
    request.id = QUuid::createUuid();

    request.data = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    request.data.setValue(0, static_cast<uint16_t>(value));
    if (!writeRequest(request))
        return "";

    return request.id;
}


bool NeuronCommon::getUserLED(const QString &circuit)
{
    if (!m_modbusUserLEDRegisters.contains(circuit)) {
        qCWarning(dcUniPi()) << "Neuron: User LED circuit not found" << circuit;
        return false;
    }
    int modbusAddress = m_modbusUserLEDRegisters.value(circuit);
    //qDebug(dcUniPi()) << "Neuron: Get user LED" << circuit << modbusAddress;

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, modbusAddress, 1);
    return readRequest(request);
}

bool NeuronCommon::getAnalogIO(const RegisterDescriptor &descriptor)
{
    if (!m_modbusTcpInterface)
        return false;

    if (m_modbusTcpInterface->state() != QModbusDevice::State::ConnectedState)
        return false;

    QModbusDataUnit request = QModbusDataUnit(descriptor.registerType, descriptor.address, descriptor.count);
    return readRequest(request);
}


bool NeuronCommon::writeRequest(const Request &request)
{
    if (m_writeRequestQueue.isEmpty()) {
        return sendModbusWriteRequest(request);
    } else if (m_writeRequestQueue.length() > 200) {
        return false;
    } else {
        m_writeRequestQueue.append(request);
    }
    return true;
}

bool NeuronCommon::sendModbusWriteRequest(const Request &request)
{
    if (m_modbusType == ModbusTypeTcp) {
        if (!m_modbusTcpInterface) {
            emit requestExecuted(request.id, false);
            emit requestError(request.id, "Modbus interface not available");
            return false;
        }
        if (m_modbusTcpInterface->state() != QModbusDevice::State::ConnectedState) {
            emit requestExecuted(request.id, false);
            emit requestError(request.id, "Device not connected");
            return false;
        };

        if (QModbusReply *reply = m_modbusTcpInterface->sendWriteRequest(request.data, m_slaveAddress)) {
            if (!reply->isFinished()) {
                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                connect(reply, &QModbusReply::finished, this, [reply, request, this] {

                    if (!m_writeRequestQueue.isEmpty()) {
                        sendModbusWriteRequest(m_writeRequestQueue.takeFirst());
                    }

                    if (reply->error() == QModbusDevice::NoError) {
                        emit requestExecuted(request.id, true);
                        const QModbusDataUnit unit = reply->result();
                        writeRequestReceived(reply->result().values(), unit.startAddress(), unit.registerType());
                    } else {
                        emit requestExecuted(request.id, false);
                        qCWarning(dcUniPi()) << "Neuron: Write response error:" << reply->error();
                        emit requestError(request.id, reply->errorString());
                    }
                });
                //QTimer::singleShot(m_responseTimeoutTime, reply, &QModbusReply::deleteLater);
            } else {
                delete reply; // broadcast replies return immediately
                return false;
            }
        } else {
            qCWarning(dcUniPi()) << "Neuron: Read error: " << m_modbusTcpInterface->errorString();
            return false;
        }
    }
    if (m_modbusType == ModbusTypeRtu) {
        if (!m_modbusRtuInterface)
            return false;

        if (!m_modbusRtuInterface->connected())
            return false;

        ModbusRtuReply *reply;
        QModbusDataUnit::RegisterType registerType = request.data.registerType();
        switch (registerType) {
        case QModbusDataUnit::RegisterType::Coils: {
            reply = m_modbusRtuInterface->writeCoils(m_slaveAddress, request.data.startAddress(), request.data.values());
        } break;
        case QModbusDataUnit::RegisterType::HoldingRegisters: {
            reply = m_modbusRtuInterface->writeHoldingRegisters(m_slaveAddress, request.data.startAddress(), request.data.values());
        } break;
        default:
            break;
        }
        connect(reply, &ModbusRtuReply::finished, reply, &ModbusRtuReply::deleteLater);
        connect(reply, &ModbusRtuReply::finished, this, [reply, registerType, this] {
            if (reply->error() == ModbusRtuReply::Error::NoError) {
                writeRequestReceived(reply->result(), reply->registerAddress(), registerType);
            }
        });
    }
    return true;
}

void NeuronCommon::writeRequestReceived(const QVector<uint16_t> &data, int modbusAddress, QModbusDataUnit::RegisterType registerType)
{
    switch (registerType) {
    case QModbusDataUnit::RegisterType::Coils: {
        if(m_modbusDigitalOutputRegisters.values().contains(modbusAddress)){
            QString circuit = m_modbusDigitalOutputRegisters.key(modbusAddress);
            emit digitalOutputStatusChanged(circuit, (data.value(0) != 0));
        }
        if(m_modbusUserLEDRegisters.values().contains(modbusAddress)){
            QString circuit = m_modbusUserLEDRegisters.key(modbusAddress);
            emit userLEDStatusChanged(circuit, (data.value(0) != 0));
        }
    } break;
    case QModbusDataUnit::RegisterType::HoldingRegisters: {
        if(m_modbusAnalogOutputRegisters.contains(modbusAddress)){
            QString circuit = m_modbusAnalogOutputRegisters.value(modbusAddress).circuit;
            emit analogOutputStatusChanged(circuit, data.value(0));
        }
    } break;
    default:
        break;
    }
}

bool NeuronCommon::readRequest(const QModbusDataUnit &request)
{
    if (m_readRequestQueue.isEmpty()) {
        return sendModbusReadRequest(request);
    } else if (m_readRequestQueue.length() > 200) {
        qCWarning(dcUniPi()) << "Neuron: Too many pending read requests";
        return false;
    } else {
        m_readRequestQueue.append(request);
    }
    return true;
}

bool NeuronCommon::sendModbusReadRequest(const QModbusDataUnit &request)
{
    if (m_modbusType == ModbusTypeTcp) {
        if (!m_modbusTcpInterface) {
            return false;
        }
        if (m_modbusTcpInterface->state() != QModbusDevice::State::ConnectedState)
            return false;

        if (QModbusReply *reply = m_modbusTcpInterface->sendReadRequest(request, m_slaveAddress)) {
            if (!reply->isFinished()) {
                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                connect(reply, &QModbusReply::finished, this, [reply, this] {

                    if (reply->error() == QModbusDevice::NoError) {
                        const QModbusDataUnit unit = reply->result();
                        readRequestReceived(unit.values(), unit.startAddress(), unit.registerType());
                    } else if (reply->error() == QModbusDevice::ProtocolError) {
                        qCWarning(dcUniPi()) << "Neuron: Read response error:" << reply->errorString() << reply->rawResult().exceptionCode();
                    } else {
                        qCWarning(dcUniPi()) << "Neuron: Read response error:" << reply->error() << reply->errorString();
                    }

                });
                //QTimer::singleShot(m_responseTimeoutTime, reply, &QModbusReply::deleteLater);
            } else {
                delete reply; // broadcast replies return immediately
                return false;
            }
        } else {
            qCWarning(dcUniPi()) << "Neuron: Read error: " << m_modbusTcpInterface->errorString();
            return false;
        }
    }
    if (m_modbusType == ModbusTypeRtu) {
        if (!m_modbusRtuInterface)
            return false;

        if (!m_modbusRtuInterface->connected())
            return false;

        ModbusRtuReply *reply;
        QModbusDataUnit::RegisterType registerType = request.registerType();
        switch (registerType) {
        case QModbusDataUnit::RegisterType::Coils: {
            reply = m_modbusRtuInterface->readCoil(m_slaveAddress, request.startAddress(), request.valueCount());
        } break;
        case QModbusDataUnit::RegisterType::DiscreteInputs: {
            reply = m_modbusRtuInterface->readDiscreteInput(m_slaveAddress, request.startAddress(), request.valueCount());
        } break;
        case QModbusDataUnit::RegisterType::InputRegisters : {
            reply = m_modbusRtuInterface->readInputRegister(m_slaveAddress, request.startAddress(), request.valueCount());
        } break;
        case QModbusDataUnit::RegisterType::HoldingRegisters: {
            reply = m_modbusRtuInterface->readHoldingRegister(m_slaveAddress, request.startAddress(), request.valueCount());
        } break;
        default:
            break;
        }
        connect(reply, &ModbusRtuReply::finished, reply, &ModbusRtuReply::deleteLater);
        connect(reply, &ModbusRtuReply::finished, this, [reply, registerType, this] {
            if (reply->error() == ModbusRtuReply::Error::NoError) {
                readRequestReceived(reply->result(), reply->registerAddress(), registerType);
            }
        });
    }
    return true;
}

void NeuronCommon::readRequestReceived(const QVector<uint16_t> &data, int startAddress, QModbusDataUnit::RegisterType registerType)
{
    int modbusAddress = 0;
    for (int i = 0; i < data.length(); i++) {
        //qCDebug(dcUniPi()) << "Start Address:" << unit.startAddress() << "Register Type:" << unit.registerType() << "Value:" << unit.value(i);
        modbusAddress = startAddress + i;

        QString circuit;
        switch (registerType) {
        case QModbusDataUnit::RegisterType::Coils:
            if(m_modbusDigitalInputRegisters.values().contains(modbusAddress)){
                circuit = m_modbusDigitalInputRegisters.key(modbusAddress);
                if (circuitValueChanged(circuit, data.value(i)))
                    emit digitalInputStatusChanged(circuit, data.value(i));
            } else if(m_modbusDigitalOutputRegisters.values().contains(modbusAddress)){
                circuit = m_modbusDigitalOutputRegisters.key(modbusAddress);
                if (circuitValueChanged(circuit, data.value(i)))
                    emit digitalOutputStatusChanged(circuit, data.value(i));
            } else if(m_modbusUserLEDRegisters.values().contains(modbusAddress)){
                circuit = m_modbusUserLEDRegisters.key(modbusAddress);
                if (circuitValueChanged(circuit, data.value(i)))
                    emit userLEDStatusChanged(circuit, data.value(i));
            } else {
                qCWarning(dcUniPi()) << "Neuron: Received unrecognised coil register" << modbusAddress;
            }
            break;

        case QModbusDataUnit::RegisterType::HoldingRegisters: {
            if (m_modbusAnalogOutputRegisters.keys().contains(modbusAddress)) {
                RegisterDescriptor descriptor =  m_modbusAnalogOutputRegisters.value(modbusAddress);
                circuit = descriptor.circuit;
                quint32 value = 0;
                if (descriptor.count == 1) {
                    value = data.value(i);
                } else if (descriptor.count == 2) {
                    if (data.length() >= (i+1)) {
                        value = (data.value(i) << 16 | data.value(i+1));
                        i++;
                    } else {
                        qCWarning(dcUniPi()) << "Neuron: Received analog output, but value count is too short";
                    }
                }
                if (circuitValueChanged(circuit, value))
                    emit analogOutputStatusChanged(circuit, value);

            } else {
                qCWarning(dcUniPi()) << "Neuron: Received unrecognised holding register" << modbusAddress;
            }
        } break;
        case QModbusDataUnit::RegisterType::InputRegisters:
            if(m_modbusAnalogInputRegisters.keys().contains(modbusAddress)){
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
                        qCWarning(dcUniPi()) << "Neuron: Received analog input, but value count is too short";
                    }
                }
                if (circuitValueChanged(circuit, value))
                    emit analogInputStatusChanged(circuit, value);

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
}

void NeuronCommon::getCoils(QList<int> registerList)
{
    if (registerList.isEmpty()) {
        return;
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
        readRequest(request);
    }
}

void NeuronCommon::onOutputPollingTimer()
{
    getAllDigitalOutputs();
    getAllAnalogOutputs();
}

void NeuronCommon::onInputPollingTimer()
{
    getAllDigitalInputs();
    getAllAnalogInputs();
}
