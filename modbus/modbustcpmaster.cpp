/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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
    QObject(parent),
    m_hostAddress(hostAddress),
    m_port(port)
{
    m_modbusTcpClient = new QModbusTcpClient(this);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_port);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_hostAddress.toString());
    m_modbusTcpClient->setTimeout(m_timeout);
    m_modbusTcpClient->setNumberOfRetries(m_numberOfRetries);

    connect(m_modbusTcpClient, &QModbusTcpClient::stateChanged, this, &ModbusTCPMaster::onModbusStateChanged);
    connect(m_modbusTcpClient, &QModbusRtuSerialMaster::errorOccurred, this, &ModbusTCPMaster::onModbusErrorOccurred);

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &ModbusTCPMaster::onReconnectTimer);
}

ModbusTCPMaster::~ModbusTCPMaster()
{
    if (m_reconnectTimer) {
        m_reconnectTimer->stop();
        delete m_reconnectTimer;
        m_reconnectTimer = nullptr;
    }

    if (m_modbusTcpClient) {
        m_modbusTcpClient->disconnectDevice();
        delete m_modbusTcpClient;
        m_modbusTcpClient = nullptr;
    }
}

QHostAddress ModbusTCPMaster::hostAddress() const
{
    return m_hostAddress;
}

uint ModbusTCPMaster::port() const
{
    return m_port;
}

bool ModbusTCPMaster::setPort(uint port)
{
    m_port = port;
    return connectDevice();
}

bool ModbusTCPMaster::setHostAddress(const QHostAddress &hostAddress)
{
    m_hostAddress = hostAddress;
    return connectDevice();
}

bool ModbusTCPMaster::connectDevice() {
    // TCP connection to target device
    qCDebug(dcModbusTCP()) << "Setting up TCP connecion" << QString("%1:%2").arg(m_hostAddress.toString()).arg(m_port);
    if (!m_modbusTcpClient)
        return false;

    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_port);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_hostAddress.toString());
    m_modbusTcpClient->setTimeout(m_timeout);
    m_modbusTcpClient->setNumberOfRetries(m_numberOfRetries);

    return m_modbusTcpClient->connectDevice();
}

void ModbusTCPMaster::disconnectDevice()
{
    if (!m_modbusTcpClient)
        return;

    m_modbusTcpClient->disconnectDevice();
}

bool ModbusTCPMaster::connected() const
{
    return (m_modbusTcpClient->state() == QModbusDevice::State::ConnectedState);
}

int ModbusTCPMaster::numberOfRetries() const
{
    return m_modbusTcpClient->numberOfRetries();
}

void ModbusTCPMaster::setNumberOfRetries(int number)
{
    m_numberOfRetries = number;
    m_modbusTcpClient->setNumberOfRetries(number);
}

int ModbusTCPMaster::timeout() const
{
    return m_modbusTcpClient->timeout();
}

void ModbusTCPMaster::setTimeout(int timeout)
{
    m_timeout = timeout;
    m_modbusTcpClient->setTimeout(timeout);
}

QString ModbusTCPMaster::errorString() const
{
    return m_modbusTcpClient->errorString();
}

QModbusDevice::Error ModbusTCPMaster::error() const
{
    return m_modbusTcpClient->error();
}

void ModbusTCPMaster::onReconnectTimer()
{
    if (!m_modbusTcpClient->connectDevice()) {
        m_reconnectTimer->start(10000);
    }
}

QUuid ModbusTCPMaster::readCoil(uint slaveAddress, uint registerAddress, uint size)
{
    if (!m_modbusTcpClient) {
        return QUuid();
    }

    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {
                if (reply->error() == QModbusDevice::NoError) {
                    emit readRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedCoil(reply->serverAddress(), modbusAddress, unit.values());
                } else {
                    emit readRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTCP()) << "Modbus reply error:" << error;
                emit readRequestError(requestId, reply->errorString());
                emit reply->finished(); // To make sure it will be deleted
            });

            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTCPMaster::writeHoldingRegisters(uint slaveAddress, uint registerAddress, const QVector<quint16> &values)
{
    if (!m_modbusTcpClient) {
        return QUuid();
    }

    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, registerAddress, values.length());
    request.setValues(values);
    if (QModbusReply *reply = m_modbusTcpClient->sendWriteRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {
                if (reply->error() == QModbusDevice::NoError) {
                    emit writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedHoldingRegister(reply->serverAddress(), modbusAddress, unit.values());
                } else {
                    emit writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
                reply->deleteLater();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTCP()) << "Modbus replay error:" << error;
                emit writeRequestError(requestId, reply->errorString());
                emit reply->finished(); // To make sure it will be deleted
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QModbusReply *ModbusTCPMaster::sendRawRequest(const QModbusRequest &request, int serverAddress)
{
    return m_modbusTcpClient->sendRawRequest(request, serverAddress);
}

QModbusReply *ModbusTCPMaster::sendReadRequest(const QModbusDataUnit &read, int serverAddress)
{
    return m_modbusTcpClient->sendReadRequest(read, serverAddress);
}

QModbusReply *ModbusTCPMaster::sendReadWriteRequest(const QModbusDataUnit &read, const QModbusDataUnit &write, int serverAddress)
{
    return m_modbusTcpClient->sendReadWriteRequest(read, write, serverAddress);
}

QModbusReply *ModbusTCPMaster::sendWriteRequest(const QModbusDataUnit &write, int serverAddress)
{
    return m_modbusTcpClient->sendWriteRequest(write, serverAddress);
}

QUuid ModbusTCPMaster::readDiscreteInput(uint slaveAddress, uint registerAddress, uint size)
{
    if (!m_modbusTcpClient) {
        return QUuid();
    }
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::DiscreteInputs, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {
                if (reply->error() == QModbusDevice::NoError) {
                    emit readRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedDiscreteInput(reply->serverAddress(), modbusAddress, unit.values());
                } else {
                    emit readRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTCP()) << "Modbus replay error:" << error;
                QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
                emit readRequestError(requestId, reply->errorString());
                emit reply->finished(); // To make sure it will be deleted
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTCPMaster::readInputRegister(uint slaveAddress, uint registerAddress, uint size)
{
    if (!m_modbusTcpClient) {
        return QUuid();
    }

    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {
                reply->deleteLater();
                if (reply->error() == QModbusDevice::NoError) {
                    emit readRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedInputRegister(reply->serverAddress(), modbusAddress, unit.values());
                } else {
                    emit readRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTCP()) << "Modbus reply error:" << error;
                emit readRequestError(requestId, reply->errorString());
                emit reply->finished(); // To make sure it will be deleted
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);

        } else {
            delete reply; // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTCPMaster::readHoldingRegister(uint slaveAddress, uint registerAddress, uint size)
{
    if (!m_modbusTcpClient) {
        return QUuid();
    }

    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    emit writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedHoldingRegister(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    emit writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Read response error:" << reply->error();
                    emit readRequestError(requestId, reply->errorString());
                }
                reply->deleteLater();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusTCP()) << "Modbus reply error:" << error;
                emit readRequestError(requestId, reply->errorString());
                emit reply->finished(); // To make sure it will be deleted
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return QUuid();
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
        return QUuid();
    }

    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, registerAddress, values.length());
    request.setValues(values);

    if (QModbusReply *reply = m_modbusTcpClient->sendWriteRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] () {

                if (reply->error() == QModbusDevice::NoError) {
                    emit writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedCoil(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    emit writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTCP()) << "Write response error:" << reply->error();
                }
                reply->deleteLater();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTCP()) << "Modbus reply error:" << error;
                emit writeRequestError(requestId, reply->errorString());
                emit reply->finished(); // To make sure it will be deleted
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            delete reply; // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTCP()) << "Read error: " << m_modbusTcpClient->errorString();
        return QUuid();
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
    qCDebug(dcModbusTCP()) << "Connection state changed for" << m_hostAddress << state;

    bool connected = (state == QModbusDevice::ConnectedState);
    if (!connected) {
        //try to reconnect in 10 seconds
        m_reconnectTimer->start(10000);
    }

    emit connectionStateChanged(connected);
}
