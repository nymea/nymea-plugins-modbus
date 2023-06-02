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

#include "modbustcpmaster.h"

Q_LOGGING_CATEGORY(dcModbusTcpMaster, "ModbusTcpMaster")

ModbusTcpMaster::ModbusTcpMaster(const QHostAddress &hostAddress, uint port, QObject *parent) :
    QObject(parent),
    m_hostAddress(hostAddress),
    m_port(port)
{
    m_modbusTcpClient = new QModbusTcpClient(this);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_port);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_hostAddress.toString());
    m_modbusTcpClient->setTimeout(m_timeout);
    m_modbusTcpClient->setNumberOfRetries(m_numberOfRetries);

    connect(m_modbusTcpClient, &QModbusTcpClient::stateChanged, this, &ModbusTcpMaster::onModbusStateChanged);
    connect(m_modbusTcpClient, &QModbusRtuSerialMaster::errorOccurred, this, &ModbusTcpMaster::onModbusErrorOccurred);

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);
    m_reconnectTimer->setInterval(4000);
    connect(m_reconnectTimer, &QTimer::timeout, this, &ModbusTcpMaster::connectDevice);
}

ModbusTcpMaster::~ModbusTcpMaster()
{
    if (m_reconnectTimer) {
        m_reconnectTimer->stop();
    }

    disconnectDevice();
}

QHostAddress ModbusTcpMaster::hostAddress() const
{
    return m_hostAddress;
}

uint ModbusTcpMaster::port() const
{
    return m_port;
}

void ModbusTcpMaster::setPort(uint port)
{
    m_port = port;
}

QString ModbusTcpMaster::connectionUrl() const
{
    return QString("%1:%2").arg(m_hostAddress.toString()).arg(m_port);
}

void ModbusTcpMaster::setHostAddress(const QHostAddress &hostAddress)
{
    m_hostAddress = hostAddress;
}

bool ModbusTcpMaster::connectDevice()
{
    // TCP connection to target device
    if (!m_modbusTcpClient)
        return false;

    // Only connect if we are in the unconnected state
    if (m_modbusTcpClient->state() == QModbusDevice::UnconnectedState) {
        qCDebug(dcModbusTcpMaster()) << "Connecting modbus TCP client to" << connectionUrl();
        m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_port);
        m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_hostAddress.toString());
        m_modbusTcpClient->setTimeout(m_timeout);
        m_modbusTcpClient->setNumberOfRetries(m_numberOfRetries);
        return m_modbusTcpClient->connectDevice();
    } else if (m_modbusTcpClient->state() != QModbusDevice::ConnectedState && m_modbusTcpClient->state() != QModbusDevice::ConnectingState) {
        // Restart the timer in case of connecting not finished yet or closing
        qCDebug(dcModbusTcpMaster()) << "Starting the re-connect mechanism timer";
        m_reconnectTimer->start();
    } else {
        qCWarning(dcModbusTcpMaster()) << "Connect modbus TCP device" << connectionUrl() << "called, but the socket is currently in the" << m_modbusTcpClient->state();
    }

    return false;
}

void ModbusTcpMaster::disconnectDevice()
{
    // Stop the reconnect timer since disconnect was explicitly called
    m_reconnectTimer->stop();
    m_modbusTcpClient->disconnectDevice();
}

bool ModbusTcpMaster::reconnectDevice()
{
    qCWarning(dcModbusTcpMaster()) << "Reconnecting modbus TCP device" << connectionUrl();
    disconnectDevice();
    return connectDevice();
}

bool ModbusTcpMaster::connected() const
{
    return m_connected;
}

int ModbusTcpMaster::numberOfRetries() const
{
    return m_modbusTcpClient->numberOfRetries();
}

void ModbusTcpMaster::setNumberOfRetries(int number)
{
    m_numberOfRetries = number;
    m_modbusTcpClient->setNumberOfRetries(number);
}

int ModbusTcpMaster::timeout() const
{
    return m_modbusTcpClient->timeout();
}

void ModbusTcpMaster::setTimeout(int timeout)
{
    m_timeout = timeout;
    m_modbusTcpClient->setTimeout(timeout);
}

QString ModbusTcpMaster::errorString() const
{
    return m_modbusTcpClient->errorString();
}

QModbusDevice::Error ModbusTcpMaster::error() const
{
    return m_modbusTcpClient->error();
}

QUuid ModbusTcpMaster::readCoil(uint slaveAddress, uint registerAddress, uint size)
{
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
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus reply error for device" << connectionUrl() << ":" << error;
                emit readRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::writeHoldingRegisters(uint slaveAddress, uint registerAddress, const QVector<quint16> &values)
{
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
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                }
                reply->deleteLater();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus replay error for device" << connectionUrl() << ":" << error;
                emit writeRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QModbusReply *ModbusTcpMaster::sendRawRequest(const QModbusRequest &request, int serverAddress)
{
    return m_modbusTcpClient->sendRawRequest(request, serverAddress);
}

QModbusReply *ModbusTcpMaster::sendReadRequest(const QModbusDataUnit &read, int serverAddress)
{
    return m_modbusTcpClient->sendReadRequest(read, serverAddress);
}

QModbusReply *ModbusTcpMaster::sendReadWriteRequest(const QModbusDataUnit &read, const QModbusDataUnit &write, int serverAddress)
{
    return m_modbusTcpClient->sendReadWriteRequest(read, write, serverAddress);
}

QModbusReply *ModbusTcpMaster::sendWriteRequest(const QModbusDataUnit &write, int serverAddress)
{
    return m_modbusTcpClient->sendWriteRequest(write, serverAddress);
}

QUuid ModbusTcpMaster::readDiscreteInput(uint slaveAddress, uint registerAddress, uint size)
{
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
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [requestId, reply, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus replay error for device" << connectionUrl() << ":" << error;
                emit readRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::readInputRegister(uint slaveAddress, uint registerAddress, uint size)
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
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus reply error for device" << connectionUrl() << ":" << error;
                emit readRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);

        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::readHoldingRegister(uint slaveAddress, uint registerAddress, uint size)
{
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
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                    emit readRequestError(requestId, reply->errorString());
                }
                reply->deleteLater();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusTcpMaster()) << "Modbus reply error for device" << connectionUrl() << ":" << error;
                emit readRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::writeCoil(uint slaveAddress, uint registerAddress, bool value)
{
    return writeCoils(slaveAddress, registerAddress, QVector<quint16>() << static_cast<quint16>(value));
}

QUuid ModbusTcpMaster::writeCoils(uint slaveAddress, uint registerAddress, const QVector<quint16> &values)
{
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
                    qCWarning(dcModbusTcpMaster()) << "Write response error for device" << connectionUrl() << ":" << reply->error();
                }
                reply->deleteLater();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus reply error for device" << connectionUrl() << ":" << error;
                emit writeRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::writeHoldingRegister(uint slaveAddress, uint registerAddress, quint16 value)
{
    return writeHoldingRegisters(slaveAddress, registerAddress, QVector<quint16>() << value);
}

void ModbusTcpMaster::onModbusErrorOccurred(QModbusDevice::Error error)
{
    qCWarning(dcModbusTcpMaster()) << "An error occurred for device" << connectionUrl() << ":" << error;
    emit connectionErrorOccurred(error);
}

void ModbusTcpMaster::onModbusStateChanged(QModbusDevice::State state)
{
    qCDebug(dcModbusTcpMaster()) << "Connection state changed for" << m_hostAddress << state;
    bool connected = (state == QModbusDevice::ConnectedState);
    if (m_connected != connected) {
        m_connected = connected;
        emit connectionStateChanged(m_connected);
    }

    // If the socket is connected, stop the reconnect timer...
    // If the socket is unconnected (not connecting and not closing), start the reconnect timer
    if (m_connected) {
        m_reconnectTimer->stop();
    } else if (state == QModbusDevice::UnconnectedState) {
        m_reconnectTimer->start();
    }
}
