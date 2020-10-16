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

#include "modbustcpmaster.h"

#include <loggingcategories.h>
NYMEA_LOGGING_CATEGORY(dcModbusTCP, "ModbusTCP")

ModbusTCPMaster::ModbusTCPMaster(const QHostAddress &hostAddress, uint port, QObject *parent) :
    QObject(parent)
{
    m_modbusTcpClient = new QModbusTcpClient(this);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, hostAddress.toString());
    m_modbusTcpClient->setTimeout(1000);
    m_modbusTcpClient->setNumberOfRetries(3);

    connect(m_modbusTcpClient, &QModbusTcpClient::stateChanged, this, &ModbusTCPMaster::onModbusStateChanged);
    connect(m_modbusTcpClient, &QModbusRtuSerialMaster::errorOccurred, this, &ModbusTCPMaster::onModbusErrorOccurred);

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &ModbusTCPMaster::onReconnectTimer);
}

ModbusTCPMaster::~ModbusTCPMaster()
{
    if (!m_modbusTcpClient) {
        m_modbusTcpClient->disconnectDevice();
        m_modbusTcpClient->deleteLater();
    }
    if (!m_reconnectTimer) {
        m_reconnectTimer->stop();
        m_reconnectTimer->deleteLater();
    }
}

bool ModbusTCPMaster::connectDevice() {
    // TCP connction to target device
    qCDebug(dcModbusTCP()) << "Setting up TCP connecion";

    if (!m_modbusTcpClient)
        return false;

    return m_modbusTcpClient->connectDevice();
}

bool ModbusTCPMaster::connected()
{
    return (m_modbusTcpClient->state() == QModbusDevice::State::ConnectedState);
}

void ModbusTCPMaster::setNumberOfRetries(int number)
{
    m_modbusTcpClient->setNumberOfRetries(number);
}

void ModbusTCPMaster::setTimeout(int timeout)
{
    m_modbusTcpClient->setTimeout(timeout);
}

uint ModbusTCPMaster::port()
{
    return m_modbusTcpClient->connectionParameter(QModbusDevice::NetworkPortParameter).toUInt();
}

bool ModbusTCPMaster::setHostAddress(const QHostAddress &hostAddress)
{
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, hostAddress.toString());
    return connectDevice();
}

bool ModbusTCPMaster::setPort(uint port)
{
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    return connectDevice();
}

void ModbusTCPMaster::onReconnectTimer()
{
    if(!m_modbusTcpClient->connectDevice()) {
        m_reconnectTimer->start(10000);
    }
}

QHostAddress ModbusTCPMaster::hostAddress()
{
    return QHostAddress(m_modbusTcpClient->connectionParameter(QModbusDevice::NetworkAddressParameter).toString());
}

QUuid ModbusTCPMaster::readCoil(uint slaveAddress, uint registerAddress, uint size)
{
    if (!m_modbusTcpClient) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedCoil(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusTCP()) << "Modbus reply error:" << error;
                emit writeRequestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusTCPMaster::writeHoldingRegisters(uint slaveAddress, uint registerAddress, const QVector<quint16> &values)
{
    if (!m_modbusTcpClient) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, registerAddress, values.length());
    request.setValues(values);

    if (QModbusReply *reply = m_modbusTcpClient->sendWriteRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedHoldingRegister(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusTCP()) << "Modbus replay error:" << error;
                emit writeRequestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusTCPMaster::readDiscreteInput(uint slaveAddress, uint registerAddress, uint size)
{
    if (!m_modbusTcpClient) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::DiscreteInputs, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedDiscreteInput(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
            });
            connect(reply, &QModbusReply::errorOccurred, this, [requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusTCP()) << "Modbus replay error:" << error;
                QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
                emit writeRequestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusTCPMaster::readInputRegister(uint slaveAddress, uint registerAddress, uint size)
{
    if (!m_modbusTcpClient) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {
                reply->deleteLater();
                if (reply->error() == QModbusDevice::NoError) {
                    writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedInputRegister(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusTCP()) << "Modbus reply error:" << error;
                emit writeRequestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusTCPMaster::readHoldingRegister(uint slaveAddress, uint registerAddress, uint size)
{
    if (!m_modbusTcpClient) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedHoldingRegister(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusTCP()) << "Modbus replay error:" << error;
                emit writeRequestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusTCPMaster::writeCoil(uint slaveAddress, uint registerAddress, bool value)
{
    return writeCoils(slaveAddress, registerAddress, QVector<quint16>() << static_cast<quint16>(value));
}

QUuid ModbusTCPMaster::writeCoils(uint slaveAddress, uint registerAddress, const QVector<quint16> &values)
    {
    if (!m_modbusTcpClient) {
        return "";
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, registerAddress, values.length());
    request.setValues(values);

    if (QModbusReply *reply = m_modbusTcpClient->sendWriteRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] () {

                if (reply->error() == QModbusDevice::NoError) {
                    writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedCoil(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });
            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusTCP()) << "Modbus reply error:" << error;
                emit writeRequestError(requestId, reply->errorString());
                reply->finished(); // To make sure it will be deleted
            });
            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return "";
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return "";
    }
    return requestId;
}

QUuid ModbusTCPMaster::writeHoldingRegister(uint slaveAddress, uint registerAddress, quint16 value)
{
    return writeHoldingRegisters(slaveAddress, registerAddress, QVector<quint16>() << value);
}


void ModbusTCPMaster::onModbusErrorOccurred(QModbusDevice::Error error)
{
    qCWarning(dcModbusTCP()) << "An error occured" << error;
}


void ModbusTCPMaster::onModbusStateChanged(QModbusDevice::State state)
{
    bool connected = (state == QModbusDevice::ConnectedState);
    if (!connected) {
        //try to reconnect in 10 seconds
        m_reconnectTimer->start(10000);
    }
    emit connectionStateChanged(connected);
}
