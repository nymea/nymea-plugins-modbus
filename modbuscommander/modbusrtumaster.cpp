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

#include "modbusrtumaster.h"
#include "extern-plugininfo.h"

#include <QSerialPortInfo>

ModbusRTUMaster::ModbusRTUMaster(QString serialPort, uint baudrate, QSerialPort::Parity parity, uint dataBits, uint stopBits, QObject *parent) :
    QObject(parent)
{
    m_modbusRtuSerialMaster = new QModbusRtuSerialMaster(this);
    m_modbusRtuSerialMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, serialPort);
    m_modbusRtuSerialMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudrate);
    m_modbusRtuSerialMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
    m_modbusRtuSerialMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);
    m_modbusRtuSerialMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, parity);
    //m_modbusRtuSerialMaster->setTimeout(100);
    //m_modbusRtuSerialMaster->setNumberOfRetries(1);
    connect(m_modbusRtuSerialMaster, &QModbusTcpClient::stateChanged, this, &ModbusRTUMaster::onModbusStateChanged);
    connect(m_modbusRtuSerialMaster, &QModbusRtuSerialMaster::errorOccurred, this, &ModbusRTUMaster::onModbusErrorOccurred);

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &ModbusRTUMaster::onReconnectTimer);
}


ModbusRTUMaster::~ModbusRTUMaster()
{
    if (!m_modbusRtuSerialMaster) {
        m_modbusRtuSerialMaster->disconnectDevice();
        m_modbusRtuSerialMaster->deleteLater();
    }
    if (!m_reconnectTimer) {
        m_reconnectTimer->stop();
        m_reconnectTimer->deleteLater();
    }
}

bool ModbusRTUMaster::connectDevice()
{
    qCDebug(dcModbusCommander()) << "Setting up TCP connecion";

    if (!m_modbusRtuSerialMaster)
        return false;

    return m_modbusRtuSerialMaster->connectDevice();
}

QString ModbusRTUMaster::serialPort()
{
    return m_modbusRtuSerialMaster->connectionParameter(QModbusDevice::SerialPortNameParameter).toString();
}

void ModbusRTUMaster::onReconnectTimer()
{
    if(!m_modbusRtuSerialMaster->connectDevice()) {
        m_reconnectTimer->start(10000);
    }
}

QUuid ModbusRTUMaster::readCoil(uint slaveAddress, uint registerAddress)
{
    if (!m_modbusRtuSerialMaster) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, registerAddress, 1);

    if (QModbusReply *reply = m_modbusRtuSerialMaster->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {


                if (reply->error() == QModbusDevice::NoError) {
                    requestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedCoil(reply->serverAddress(), modbusAddress, unit.value(0));

                } else {
                    requestExecuted(requestId, false);
                    qCWarning(dcModbusCommander()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusCommander()) << "Modbus replay error:" << error;

                emit requestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusCommander()) << "Read error: " << m_modbusRtuSerialMaster->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusRTUMaster::writeCoil(uint slaveAddress, uint registerAddress, bool value)
{
    if (!m_modbusRtuSerialMaster) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, registerAddress, 1);
    request.setValue(0, static_cast<uint16_t>(value));

    if (QModbusReply *reply = m_modbusRtuSerialMaster->sendWriteRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    requestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedCoil(reply->serverAddress(), modbusAddress, unit.value(0));

                } else {
                    requestExecuted(requestId, false);
                    qCWarning(dcModbusCommander()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusCommander()) << "Modbus replay error:" << error;
                emit requestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusCommander()) << "Read error: " << m_modbusRtuSerialMaster->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusRTUMaster::writeHoldingRegister(uint slaveAddress, uint registerAddress, uint value)
{
    if (!m_modbusRtuSerialMaster) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, registerAddress, 1);
    request.setValue(0, static_cast<uint16_t>(value));

    if (QModbusReply *reply = m_modbusRtuSerialMaster->sendWriteRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    requestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedHoldingRegister(reply->serverAddress(), modbusAddress, unit.value(0));

                } else {
                    requestExecuted(requestId, false);
                    qCWarning(dcModbusCommander()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusCommander()) << "Modbus replay error:" << error;
                emit requestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusCommander()) << "Read error: " << m_modbusRtuSerialMaster->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusRTUMaster::readDiscreteInput(uint slaveAddress, uint registerAddress)
{
    if (!m_modbusRtuSerialMaster) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::DiscreteInputs, registerAddress, 1);

    if (QModbusReply *reply = m_modbusRtuSerialMaster->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    requestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedDiscreteInput(reply->serverAddress(), modbusAddress, unit.value(0));

                } else {
                    requestExecuted(requestId, false);
                    qCWarning(dcModbusCommander()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusCommander()) << "Modbus replay error:" << error;

                emit requestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusCommander()) << "Read error: " << m_modbusRtuSerialMaster->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusRTUMaster::readInputRegister(uint slaveAddress, uint registerAddress)
{
    if (!m_modbusRtuSerialMaster) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, registerAddress, 1);

    if (QModbusReply *reply = m_modbusRtuSerialMaster->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {


                if (reply->error() == QModbusDevice::NoError) {
                    requestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedInputRegister(reply->serverAddress(), modbusAddress, unit.value(0));

                } else {
                    requestExecuted(requestId, false);
                    qCWarning(dcModbusCommander()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusCommander()) << "Modbus replay error:" << error;

                emit requestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusCommander()) << "Read error: " << m_modbusRtuSerialMaster->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusRTUMaster::readHoldingRegister(uint slaveAddress, uint registerAddress)
{
    if (!m_modbusRtuSerialMaster) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, registerAddress, 1);

    if (QModbusReply *reply = m_modbusRtuSerialMaster->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    requestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedHoldingRegister(reply->serverAddress(), modbusAddress, unit.value(0));

                } else {
                    requestExecuted(requestId, false);
                    qCWarning(dcModbusCommander()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusCommander()) << "Modbus replay error:" << error;

                emit requestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusCommander()) << "Read error: " << m_modbusRtuSerialMaster->errorString();
        return "";
    }
    return requestId;
}


void ModbusRTUMaster::onModbusErrorOccurred(QModbusDevice::Error error)
{
    qCWarning(dcModbusCommander()) << "An error occured" << error;
}


void ModbusRTUMaster::onModbusStateChanged(QModbusDevice::State state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    if (!connected) {
        //try to reconnect in 10 seconds
        m_reconnectTimer->start(10000);
    }
    emit connectionStateChanged(connected);
}
