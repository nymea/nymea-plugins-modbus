﻿/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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

#include "huaweifusionsolar.h"
#include "extern-plugininfo.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcHuaweiFusionSolar, "HuaweiFusionSolar")

HuaweiFusionSolar::HuaweiFusionSolar(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    HuaweiFusionModbusTcpConnection(hostAddress, port, slaveId, parent)
{
    connect(this, &HuaweiFusionModbusTcpConnection::connectionStateChanged, this, [=](bool connected){
        if (!connected) {
            m_registersQueue.clear();
        }
    });
}

bool HuaweiFusionSolar::initialize()
{
    if (!reachable()) {
        qCWarning(dcHuaweiFusionSolar()) << "Tried to initialize but the device is not to be reachable.";
        return false;
    }

    if (m_initReply) {
        qCWarning(dcHuaweiFusionSolar()) << "Tried to initialize but the init process is already running.";
        return false;
    }

    qCDebug(dcHuaweiFusionSolar()) << "Initialize connection by reading \"Inverter active power\" register:" << 32080 << "size:" << 2;
    m_initReply = readInverterActivePower();
    if (!m_initReply) {
        qCWarning(dcHuaweiFusionSolar()) << "Error occurred while initializing connection and read \"Inverter active power\" register from" << hostAddress().toString() << errorString();
        return false;
    }

    if (m_initReply->isFinished()) {
        m_initReply->deleteLater(); // Broadcast reply returns immediatly
        m_initReply = nullptr;
        return false;
    }

    connect(m_initReply, &QModbusReply::finished, this, [this](){
        if (m_initReply->error() == QModbusDevice::NoError) {
            qCDebug(dcHuaweiFusionSolar()) << "Initialization finished of HuaweiFusionSolar" << hostAddress().toString() << "finished successfully";
            emit initializationFinished(true);
        } else {
            qCWarning(dcHuaweiFusionSolar()) << "Initialization finished of HuaweiFusionSolar" << hostAddress().toString() << "failed.";
            emit initializationFinished(false);
        }

        m_initReply->deleteLater();
        m_initReply = nullptr;
    });

    connect(m_initReply, &QModbusReply::errorOccurred, this, [this] (QModbusDevice::Error error){
        qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while initializing connection and read \"Inverter active power\" registers from" << hostAddress().toString() << error << m_initReply->errorString();
    });

    return true;
}

bool HuaweiFusionSolar::update()
{
    // Make sure there is not an update still running
    if (!m_registersQueue.isEmpty())
        return true;

    // Add the requests
    m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterInverterActivePower);
    m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterInverterDeviceStatus);
    m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterInverterEnergyProduced);
    m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterLunaBattery1Status);
    if (m_battery1Available) {
        m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterLunaBattery1Power);
        m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterLunaBattery1Soc);
    }
    m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterPowerMeterActivePower);
    m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterLunaBattery2Status);
    if (m_battery2Available) {
        m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterLunaBattery2Power);
        m_registersQueue.enqueue(HuaweiFusionModbusTcpConnection::RegisterLunaBattery2Soc);
    }

    // Note: since huawei can only process one request at the time, we need to queue the requests

    m_currentRegisterRequest = -1;
    readNextRegister();
    return true;
}

void HuaweiFusionSolar::readNextRegister()
{
    // Check if currently a reply is pending
    if (m_currentRegisterRequest >= 0)
        return;

    // Check if there is still a register to read
    if (m_registersQueue.isEmpty())
        return;

    m_currentRegisterRequest = m_registersQueue.dequeue();

    switch (m_currentRegisterRequest) {
    case HuaweiFusionModbusTcpConnection::RegisterInverterActivePower: {
        // Update registers from Inverter active power
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Inverter active power\" register:" << 32080 << "size:" << 2;
        QModbusReply *reply = readInverterActivePower();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Inverter active power\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Inverter active power\" register" << 32080 << "size:" << 2 << unit.values();
                processInverterActivePowerRegisterValues(unit.values());
            }

            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Inverter active power\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Inverter active power\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });

        break;
    }
    case HuaweiFusionModbusTcpConnection::RegisterInverterDeviceStatus: {
        // Update registers from Inverter device status
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Inverter device status\" register:" << 32089 << "size:" << 1;
        QModbusReply *reply = readInverterDeviceStatus();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Inverter device status\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Inverter device status\" register" << 32089 << "size:" << 1 << unit.values();
                processInverterDeviceStatusRegisterValues(unit.values());
            }
            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Inverter device status\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Inverter device status\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });

        break;
    }
    case HuaweiFusionModbusTcpConnection::RegisterInverterEnergyProduced: {
        // Update registers from Inverter energy produced
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Inverter energy produced\" register:" << 32106 << "size:" << 2;
        QModbusReply *reply = readInverterEnergyProduced();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Inverter energy produced\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Inverter energy produced\" register" << 32106 << "size:" << 2 << unit.values();
                processInverterEnergyProducedRegisterValues(unit.values());
            }
            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Inverter energy produced\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Inverter energy produced\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });
        break;
    }
    case HuaweiFusionModbusTcpConnection::RegisterPowerMeterActivePower: {
        // Update registers from Power meter active power
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Power meter active power\" register:" << 37113 << "size:" << 2;
        QModbusReply *reply = readPowerMeterActivePower();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Power meter active power\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Power meter active power\" register" << 37113 << "size:" << 2 << unit.values();
                processPowerMeterActivePowerRegisterValues(unit.values());
            }
            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Power meter active power\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Power meter active power\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });

        break;
    }
    case HuaweiFusionModbusTcpConnection::RegisterLunaBattery1Status: {
        // Update registers from Luna 2000 Battery 1 status
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Luna 2000 Battery 1 status\" register:" << 37000 << "size:" << 1;
        QModbusReply *reply = readLunaBattery1Status();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Luna 2000 Battery 1 status\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Luna 2000 Battery 1 status\" register" << 37000 << "size:" << 1 << unit.values();
                processLunaBattery1StatusRegisterValues(unit.values());
            }
            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 1 status\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 1 status\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });
        break;
    }
    case HuaweiFusionModbusTcpConnection::RegisterLunaBattery1Power: {
        // Update registers from Luna 2000 Battery 1 power
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Luna 2000 Battery 1 power\" register:" << 37001 << "size:" << 2;
        QModbusReply *reply = readLunaBattery1Power();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Luna 2000 Battery 1 power\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Luna 2000 Battery 1 power\" register" << 37001 << "size:" << 2 << unit.values();
                processLunaBattery1PowerRegisterValues(unit.values());
            }
            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 1 power\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 1 power\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });
        break;
    }
    case HuaweiFusionModbusTcpConnection::RegisterLunaBattery1Soc: {
        // Update registers from Luna 2000 Battery 1 state of charge
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Luna 2000 Battery 1 state of charge\" register:" << 37004 << "size:" << 1;
        QModbusReply *reply = readLunaBattery1Soc();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Luna 2000 Battery 1 state of charge\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Luna 2000 Battery 1 state of charge\" register" << 37004 << "size:" << 1 << unit.values();
                processLunaBattery1SocRegisterValues(unit.values());
            }
            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 1 state of charge\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 1 state of charge\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });
        break;
    }

    case HuaweiFusionModbusTcpConnection::RegisterLunaBattery2Status: {
        // Update registers from Luna 2000 Battery 2 status
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Luna 2000 Battery 2 status\" register:" << 37741 << "size:" << 1;
        QModbusReply *reply = readLunaBattery2Status();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Luna 2000 Battery 2 status\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Luna 2000 Battery 2 status\" register" << 37741 << "size:" << 1 << unit.values();
                processLunaBattery2StatusRegisterValues(unit.values());
            }
            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 2 status\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 2 status\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });
        break;
    }
    case HuaweiFusionModbusTcpConnection::RegisterLunaBattery2Power: {
        // Update registers from Luna 2000 Battery 2 power
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Luna 2000 Battery 2 power\" register:" << 37743 << "size:" << 2;
        QModbusReply *reply = readLunaBattery2Power();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Luna 2000 Battery 2 power\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Luna 2000 Battery 2 power\" register" << 37743 << "size:" << 2 << unit.values();
                processLunaBattery2PowerRegisterValues(unit.values());
            }
            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 2 power\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 2 power\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });
        break;
    }
    case HuaweiFusionModbusTcpConnection::RegisterLunaBattery2Soc: {
        // Update registers from Luna 2000 Battery 2 state of charge
        qCDebug(dcHuaweiFusionSolar()) << "--> Read \"Luna 2000 Battery 2 state of charge\" register:" << 37738 << "size:" << 1;
        QModbusReply *reply = readLunaBattery2Soc();
        if (!reply) {
            qCWarning(dcHuaweiFusionSolar()) << "Error occurred while reading \"Luna 2000 Battery 2 state of charge\" registers from" << hostAddress().toString() << errorString();
            finishRequest();
            return;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            finishRequest();
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            if (reply->error() == QModbusDevice::NoError) {
                const QModbusDataUnit unit = reply->result();
                qCDebug(dcHuaweiFusionSolar()) << "<-- Response from \"Luna 2000 Battery 2 state of charge\" register" << 37738 << "size:" << 1 << unit.values();
                processLunaBattery2SocRegisterValues(unit.values());
            }
            finishRequest();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            if (reply->error() == QModbusDevice::ProtocolError) {
                QModbusResponse response = reply->rawResult();
                if (response.isException()) {
                    qCDebug(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 2 state of charge\" registers from" << hostAddress().toString() << exceptionToString(response.exceptionCode());
                }
            } else {
                qCWarning(dcHuaweiFusionSolar()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 2 state of charge\" registers from" << hostAddress().toString() << error << reply->errorString();
            }
        });
        break;
    }
    }
}

void HuaweiFusionSolar::finishRequest()
{
    m_currentRegisterRequest = -1;
    QTimer::singleShot(1000, this, &HuaweiFusionSolar::readNextRegister);
}

QString HuaweiFusionSolar::exceptionToString(QModbusPdu::ExceptionCode exception)
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
