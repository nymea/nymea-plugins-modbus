/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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


#include "huaweimodbustcpconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcHuaweiModbusTcpConnection, "HuaweiModbusTcpConnection")

HuaweiModbusTcpConnection::HuaweiModbusTcpConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    ModbusTCPMaster(hostAddress, port, parent),
    m_slaveId(slaveId)
{
    
}

float HuaweiModbusTcpConnection::inverterActivePower() const
{
    return m_inverterActivePower;
}

HuaweiModbusTcpConnection::InverterDeviceStatus HuaweiModbusTcpConnection::inverterDeviceStatus() const
{
    return m_inverterDeviceStatus;
}

float HuaweiModbusTcpConnection::inverterEnergyProduced() const
{
    return m_inverterEnergyProduced;
}

qint32 HuaweiModbusTcpConnection::powerMeterActivePower() const
{
    return m_powerMeterActivePower;
}

HuaweiModbusTcpConnection::BatteryDeviceStatus HuaweiModbusTcpConnection::lunaBattery1Status() const
{
    return m_lunaBattery1Status;
}

qint32 HuaweiModbusTcpConnection::lunaBattery1Power() const
{
    return m_lunaBattery1Power;
}

float HuaweiModbusTcpConnection::lunaBattery1Soc() const
{
    return m_lunaBattery1Soc;
}

HuaweiModbusTcpConnection::BatteryDeviceStatus HuaweiModbusTcpConnection::lunaBattery2Status() const
{
    return m_lunaBattery2Status;
}

qint32 HuaweiModbusTcpConnection::lunaBattery2Power() const
{
    return m_lunaBattery2Power;
}

float HuaweiModbusTcpConnection::lunaBattery2Soc() const
{
    return m_lunaBattery2Soc;
}

void HuaweiModbusTcpConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void HuaweiModbusTcpConnection::update()
{
    updateInverterActivePower();
    updateInverterDeviceStatus();
    updateInverterEnergyProduced();
    updatePowerMeterActivePower();
    updateLunaBattery1Status();
    updateLunaBattery1Power();
    updateLunaBattery1Soc();
    updateLunaBattery2Status();
    updateLunaBattery2Power();
    updateLunaBattery2Soc();
}

void HuaweiModbusTcpConnection::updateInverterActivePower()
{
    // Update registers from Inverter active power
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Inverter active power\" register:" << 32080 << "size:" << 2;
    QModbusReply *reply = readInverterActivePower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Inverter active power\" register" << 32080 << "size:" << 2 << values;
                    float receivedInverterActivePower = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -3);
                    if (m_inverterActivePower != receivedInverterActivePower) {
                        m_inverterActivePower = receivedInverterActivePower;
                        emit inverterActivePowerChanged(m_inverterActivePower);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Inverter active power\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Inverter active power\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterDeviceStatus()
{
    // Update registers from Inverter device status
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Inverter device status\" register:" << 32089 << "size:" << 1;
    QModbusReply *reply = readInverterDeviceStatus();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Inverter device status\" register" << 32089 << "size:" << 1 << values;
                    InverterDeviceStatus receivedInverterDeviceStatus = static_cast<InverterDeviceStatus>(ModbusDataUtils::convertToUInt16(values));
                    if (m_inverterDeviceStatus != receivedInverterDeviceStatus) {
                        m_inverterDeviceStatus = receivedInverterDeviceStatus;
                        emit inverterDeviceStatusChanged(m_inverterDeviceStatus);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Inverter device status\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Inverter device status\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterEnergyProduced()
{
    // Update registers from Inverter energy produced
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Inverter energy produced\" register:" << 32106 << "size:" << 2;
    QModbusReply *reply = readInverterEnergyProduced();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Inverter energy produced\" register" << 32106 << "size:" << 2 << values;
                    float receivedInverterEnergyProduced = ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -2);
                    if (m_inverterEnergyProduced != receivedInverterEnergyProduced) {
                        m_inverterEnergyProduced = receivedInverterEnergyProduced;
                        emit inverterEnergyProducedChanged(m_inverterEnergyProduced);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Inverter energy produced\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Inverter energy produced\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updatePowerMeterActivePower()
{
    // Update registers from Power meter active power
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Power meter active power\" register:" << 37113 << "size:" << 2;
    QModbusReply *reply = readPowerMeterActivePower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Power meter active power\" register" << 37113 << "size:" << 2 << values;
                    qint32 receivedPowerMeterActivePower = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerMeterActivePower != receivedPowerMeterActivePower) {
                        m_powerMeterActivePower = receivedPowerMeterActivePower;
                        emit powerMeterActivePowerChanged(m_powerMeterActivePower);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Power meter active power\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Power meter active power\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateLunaBattery1Status()
{
    // Update registers from Luna 2000 Battery 1 status
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Luna 2000 Battery 1 status\" register:" << 37000 << "size:" << 1;
    QModbusReply *reply = readLunaBattery1Status();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Luna 2000 Battery 1 status\" register" << 37000 << "size:" << 1 << values;
                    BatteryDeviceStatus receivedLunaBattery1Status = static_cast<BatteryDeviceStatus>(ModbusDataUtils::convertToUInt16(values));
                    m_lunaBattery1Status = receivedLunaBattery1Status;
                    emit lunaBattery1StatusChanged(m_lunaBattery1Status);
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 1 status\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Luna 2000 Battery 1 status\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateLunaBattery1Power()
{
    // Update registers from Luna 2000 Battery 1 power
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Luna 2000 Battery 1 power\" register:" << 37001 << "size:" << 2;
    QModbusReply *reply = readLunaBattery1Power();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Luna 2000 Battery 1 power\" register" << 37001 << "size:" << 2 << values;
                    qint32 receivedLunaBattery1Power = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_lunaBattery1Power != receivedLunaBattery1Power) {
                        m_lunaBattery1Power = receivedLunaBattery1Power;
                        emit lunaBattery1PowerChanged(m_lunaBattery1Power);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 1 power\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Luna 2000 Battery 1 power\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateLunaBattery1Soc()
{
    // Update registers from Luna 2000 Battery 1 state of charge
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Luna 2000 Battery 1 state of charge\" register:" << 37004 << "size:" << 1;
    QModbusReply *reply = readLunaBattery1Soc();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Luna 2000 Battery 1 state of charge\" register" << 37004 << "size:" << 1 << values;
                    float receivedLunaBattery1Soc = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_lunaBattery1Soc != receivedLunaBattery1Soc) {
                        m_lunaBattery1Soc = receivedLunaBattery1Soc;
                        emit lunaBattery1SocChanged(m_lunaBattery1Soc);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 1 state of charge\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Luna 2000 Battery 1 state of charge\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateLunaBattery2Status()
{
    // Update registers from Luna 2000 Battery 2 status
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Luna 2000 Battery 2 status\" register:" << 37741 << "size:" << 1;
    QModbusReply *reply = readLunaBattery2Status();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Luna 2000 Battery 2 status\" register" << 37741 << "size:" << 1 << values;
                    BatteryDeviceStatus receivedLunaBattery2Status = static_cast<BatteryDeviceStatus>(ModbusDataUtils::convertToUInt16(values));
                    m_lunaBattery2Status = receivedLunaBattery2Status;
                    emit lunaBattery2StatusChanged(m_lunaBattery2Status);
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 2 status\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Luna 2000 Battery 2 status\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateLunaBattery2Power()
{
    // Update registers from Luna 2000 Battery 2 power
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Luna 2000 Battery 2 power\" register:" << 37743 << "size:" << 2;
    QModbusReply *reply = readLunaBattery2Power();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Luna 2000 Battery 2 power\" register" << 37743 << "size:" << 2 << values;
                    qint32 receivedLunaBattery2Power = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_lunaBattery2Power != receivedLunaBattery2Power) {
                        m_lunaBattery2Power = receivedLunaBattery2Power;
                        emit lunaBattery2PowerChanged(m_lunaBattery2Power);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 2 power\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Luna 2000 Battery 2 power\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateLunaBattery2Soc()
{
    // Update registers from Luna 2000 Battery 2 state of charge
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Luna 2000 Battery 2 state of charge\" register:" << 37738 << "size:" << 1;
    QModbusReply *reply = readLunaBattery2Soc();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Luna 2000 Battery 2 state of charge\" register" << 37738 << "size:" << 1 << values;
                    float receivedLunaBattery2Soc = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_lunaBattery2Soc != receivedLunaBattery2Soc) {
                        m_lunaBattery2Soc = receivedLunaBattery2Soc;
                        emit lunaBattery2SocChanged(m_lunaBattery2Soc);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Luna 2000 Battery 2 state of charge\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Luna 2000 Battery 2 state of charge\" registers from" << hostAddress().toString() << errorString();
    }
}

QModbusReply *HuaweiModbusTcpConnection::readInverterActivePower()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32080, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readInverterDeviceStatus()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32089, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readInverterEnergyProduced()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32106, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readPowerMeterActivePower()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 37113, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readLunaBattery1Status()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 37000, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readLunaBattery1Power()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 37001, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readLunaBattery1Soc()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 37004, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readLunaBattery2Status()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 37741, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readLunaBattery2Power()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 37743, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readLunaBattery2Soc()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 37738, 1);
    return sendReadRequest(request, m_slaveId);
}

void HuaweiModbusTcpConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcHuaweiModbusTcpConnection()) << "Initialization finished of HuaweiModbusTcpConnection" << hostAddress().toString();
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, HuaweiModbusTcpConnection *huaweiModbusTcpConnection)
{
    debug.nospace().noquote() << "HuaweiModbusTcpConnection(" << huaweiModbusTcpConnection->hostAddress().toString() << ":" << huaweiModbusTcpConnection->port() << ")" << "\n";
    debug.nospace().noquote() << "    - Inverter active power:" << huaweiModbusTcpConnection->inverterActivePower() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Inverter device status:" << huaweiModbusTcpConnection->inverterDeviceStatus() << "\n";
    debug.nospace().noquote() << "    - Inverter energy produced:" << huaweiModbusTcpConnection->inverterEnergyProduced() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Power meter active power:" << huaweiModbusTcpConnection->powerMeterActivePower() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Luna 2000 Battery 1 status:" << huaweiModbusTcpConnection->lunaBattery1Status() << "\n";
    debug.nospace().noquote() << "    - Luna 2000 Battery 1 power:" << huaweiModbusTcpConnection->lunaBattery1Power() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Luna 2000 Battery 1 state of charge:" << huaweiModbusTcpConnection->lunaBattery1Soc() << " [%]" << "\n";
    debug.nospace().noquote() << "    - Luna 2000 Battery 2 status:" << huaweiModbusTcpConnection->lunaBattery2Status() << "\n";
    debug.nospace().noquote() << "    - Luna 2000 Battery 2 power:" << huaweiModbusTcpConnection->lunaBattery2Power() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Luna 2000 Battery 2 state of charge:" << huaweiModbusTcpConnection->lunaBattery2Soc() << " [%]" << "\n";
    return debug.quote().space();
}

