/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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


#include "kostalconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcKostalConnection, "KostalConnection")

KostalConnection::KostalConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    ModbusTCPMaster(hostAddress, port, parent),
    m_slaveId(slaveId)
{
    
}

quint16 KostalConnection::modbusUnitId() const
{
    return m_modbusUnitId;
}

KostalConnection::ByteOrder KostalConnection::modbusByteOrder() const
{
    return m_modbusByteOrder;
}

QString KostalConnection::inverterArticleNumber() const
{
    return m_inverterArticleNumber;
}

quint16 KostalConnection::bidirectionalConverterNumber() const
{
    return m_bidirectionalConverterNumber;
}

quint16 KostalConnection::numberPvStrings() const
{
    return m_numberPvStrings;
}

quint32 KostalConnection::hardwareVersion() const
{
    return m_hardwareVersion;
}

QString KostalConnection::softwareVersionMainController() const
{
    return m_softwareVersionMainController;
}

QString KostalConnection::softwareVersionIoController() const
{
    return m_softwareVersionIoController;
}

quint16 KostalConnection::powerId() const
{
    return m_powerId;
}

KostalConnection::InverterState KostalConnection::inverterState() const
{
    return m_inverterState;
}

float KostalConnection::totalDcPower() const
{
    return m_totalDcPower;
}

KostalConnection::EnergyManagementState KostalConnection::energyManagementState() const
{
    return m_energyManagementState;
}

float KostalConnection::homeOwnConsumptionFromBattery() const
{
    return m_homeOwnConsumptionFromBattery;
}

float KostalConnection::homeOwnConsumptionFromGrid() const
{
    return m_homeOwnConsumptionFromGrid;
}

float KostalConnection::totalHomeConsumptionFromBattery() const
{
    return m_totalHomeConsumptionFromBattery;
}

float KostalConnection::totalHomeConsumptionFromGrid() const
{
    return m_totalHomeConsumptionFromGrid;
}

float KostalConnection::totalHomeConsumptionFromPv() const
{
    return m_totalHomeConsumptionFromPv;
}

float KostalConnection::homeOwnConsumptionPv() const
{
    return m_homeOwnConsumptionPv;
}

float KostalConnection::totalHomeConsumption() const
{
    return m_totalHomeConsumption;
}

float KostalConnection::gridFrequency() const
{
    return m_gridFrequency;
}

float KostalConnection::currentPhase1() const
{
    return m_currentPhase1;
}

float KostalConnection::activePowerPhase1() const
{
    return m_activePowerPhase1;
}

float KostalConnection::voltagePhase1() const
{
    return m_voltagePhase1;
}

float KostalConnection::currentPhase2() const
{
    return m_currentPhase2;
}

float KostalConnection::activePowerPhase2() const
{
    return m_activePowerPhase2;
}

float KostalConnection::voltagePhase2() const
{
    return m_voltagePhase2;
}

float KostalConnection::currentPhase3() const
{
    return m_currentPhase3;
}

float KostalConnection::activePowerPhase3() const
{
    return m_activePowerPhase3;
}

float KostalConnection::voltagePhase3() const
{
    return m_voltagePhase3;
}

float KostalConnection::totalAcPower() const
{
    return m_totalAcPower;
}

float KostalConnection::batteryChargeCurrent() const
{
    return m_batteryChargeCurrent;
}

float KostalConnection::numberOfBytteryCycles() const
{
    return m_numberOfBytteryCycles;
}

float KostalConnection::actualBatteryChargeCurrent() const
{
    return m_actualBatteryChargeCurrent;
}

float KostalConnection::batteryStateOfCharge() const
{
    return m_batteryStateOfCharge;
}

float KostalConnection::batteryTemperature() const
{
    return m_batteryTemperature;
}

float KostalConnection::batteryVoltage() const
{
    return m_batteryVoltage;
}

void KostalConnection::initialize()
{
    QModbusReply *reply = nullptr;

    if (!m_pendingInitReplies.isEmpty()) {
        qCWarning(dcKostalConnection()) << "Tried to initialize but there are still some init replies pending.";
        return;
    }

    // Read MODBUS Unit-ID
    reply = readModbusUnitId();
    if (reply) {
        if (!reply->isFinished()) {
            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    quint16 receivedModbusUnitId = ModbusDataUtils::convertToUInt16(registers);
                    if (m_modbusUnitId != receivedModbusUnitId) {
                        m_modbusUnitId = receivedModbusUnitId;
                        emit modbusUnitIdChanged(m_modbusUnitId);
                    }
                }

                m_pendingInitReplies.removeAll(reply);
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while reading \"MODBUS Unit-ID\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"MODBUS Unit-ID\" registers from" << hostAddress().toString() << errorString();
    }

    // Read MODBUS Byte Order Note
    reply = readModbusByteOrder();
    if (reply) {
        if (!reply->isFinished()) {
            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    ByteOrder receivedModbusByteOrder = static_cast<ByteOrder>(ModbusDataUtils::convertToUInt16(registers));
                    if (m_modbusByteOrder != receivedModbusByteOrder) {
                        m_modbusByteOrder = receivedModbusByteOrder;
                        emit modbusByteOrderChanged(m_modbusByteOrder);
                    }
                }

                m_pendingInitReplies.removeAll(reply);
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while reading \"MODBUS Byte Order Note\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"MODBUS Byte Order Note\" registers from" << hostAddress().toString() << errorString();
    }

    // Read Inverter article number
    reply = readInverterArticleNumber();
    if (reply) {
        if (!reply->isFinished()) {
            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    QString receivedInverterArticleNumber = ModbusDataUtils::convertToString(registers);
                    if (m_inverterArticleNumber != receivedInverterArticleNumber) {
                        m_inverterArticleNumber = receivedInverterArticleNumber;
                        emit inverterArticleNumberChanged(m_inverterArticleNumber);
                    }
                }

                m_pendingInitReplies.removeAll(reply);
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while reading \"Inverter article number\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Inverter article number\" registers from" << hostAddress().toString() << errorString();
    }

    // Read Number of bidirectional converter
    reply = readBidirectionalConverterNumber();
    if (reply) {
        if (!reply->isFinished()) {
            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    quint16 receivedBidirectionalConverterNumber = ModbusDataUtils::convertToUInt16(registers);
                    if (m_bidirectionalConverterNumber != receivedBidirectionalConverterNumber) {
                        m_bidirectionalConverterNumber = receivedBidirectionalConverterNumber;
                        emit bidirectionalConverterNumberChanged(m_bidirectionalConverterNumber);
                    }
                }

                m_pendingInitReplies.removeAll(reply);
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while reading \"Number of bidirectional converter\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Number of bidirectional converter\" registers from" << hostAddress().toString() << errorString();
    }

    // Read Number of PV strings
    reply = readNumberPvStrings();
    if (reply) {
        if (!reply->isFinished()) {
            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    quint16 receivedNumberPvStrings = ModbusDataUtils::convertToUInt16(registers);
                    if (m_numberPvStrings != receivedNumberPvStrings) {
                        m_numberPvStrings = receivedNumberPvStrings;
                        emit numberPvStringsChanged(m_numberPvStrings);
                    }
                }

                m_pendingInitReplies.removeAll(reply);
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while reading \"Number of PV strings\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Number of PV strings\" registers from" << hostAddress().toString() << errorString();
    }

    // Read Hardware-Version
    reply = readHardwareVersion();
    if (reply) {
        if (!reply->isFinished()) {
            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    quint32 receivedHardwareVersion = ModbusDataUtils::convertToUInt32(registers);
                    if (m_hardwareVersion != receivedHardwareVersion) {
                        m_hardwareVersion = receivedHardwareVersion;
                        emit hardwareVersionChanged(m_hardwareVersion);
                    }
                }

                m_pendingInitReplies.removeAll(reply);
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while reading \"Hardware-Version\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Hardware-Version\" registers from" << hostAddress().toString() << errorString();
    }

    // Read Software-Version Maincontroller (MC)
    reply = readSoftwareVersionMainController();
    if (reply) {
        if (!reply->isFinished()) {
            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    QString receivedSoftwareVersionMainController = ModbusDataUtils::convertToString(registers);
                    if (m_softwareVersionMainController != receivedSoftwareVersionMainController) {
                        m_softwareVersionMainController = receivedSoftwareVersionMainController;
                        emit softwareVersionMainControllerChanged(m_softwareVersionMainController);
                    }
                }

                m_pendingInitReplies.removeAll(reply);
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while reading \"Software-Version Maincontroller (MC)\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Software-Version Maincontroller (MC)\" registers from" << hostAddress().toString() << errorString();
    }

    // Read Software-Version IO-Controller (IOC)
    reply = readSoftwareVersionIoController();
    if (reply) {
        if (!reply->isFinished()) {
            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    QString receivedSoftwareVersionIoController = ModbusDataUtils::convertToString(registers);
                    if (m_softwareVersionIoController != receivedSoftwareVersionIoController) {
                        m_softwareVersionIoController = receivedSoftwareVersionIoController;
                        emit softwareVersionIoControllerChanged(m_softwareVersionIoController);
                    }
                }

                m_pendingInitReplies.removeAll(reply);
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while reading \"Software-Version IO-Controller (IOC)\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Software-Version IO-Controller (IOC)\" registers from" << hostAddress().toString() << errorString();
    }

    // Read Power-ID
    reply = readPowerId();
    if (reply) {
        if (!reply->isFinished()) {
            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    quint16 receivedPowerId = ModbusDataUtils::convertToUInt16(registers);
                    if (m_powerId != receivedPowerId) {
                        m_powerId = receivedPowerId;
                        emit powerIdChanged(m_powerId);
                    }
                }

                m_pendingInitReplies.removeAll(reply);
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while reading \"Power-ID\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Power-ID\" registers from" << hostAddress().toString() << errorString();
    }

    
}

void KostalConnection::update()
{
    updateInverterState();
    updateTotalDcPower();
    updateEnergyManagementState();
    updateHomeOwnConsumptionFromBattery();
    updateHomeOwnConsumptionFromGrid();
    updateTotalHomeConsumptionFromBattery();
    updateTotalHomeConsumptionFromGrid();
    updateTotalHomeConsumptionFromPv();
    updateHomeOwnConsumptionPv();
    updateTotalHomeConsumption();
    updateGridFrequency();
    updateCurrentPhase1();
    updateActivePowerPhase1();
    updateVoltagePhase1();
    updateCurrentPhase2();
    updateActivePowerPhase2();
    updateVoltagePhase2();
    updateCurrentPhase3();
    updateActivePowerPhase3();
    updateVoltagePhase3();
    updateTotalAcPower();
    updateBatteryChargeCurrent();
    updateNumberOfBytteryCycles();
    updateActualBatteryChargeCurrent();
    updateBatteryStateOfCharge();
    updateBatteryTemperature();
    updateBatteryVoltage();
}

void KostalConnection::updateInverterState()
{
    // Update registers from Inverter state
    QModbusReply *reply = readInverterState();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    InverterState receivedInverterState = static_cast<InverterState>(ModbusDataUtils::convertToUInt16(registers));
                    if (m_inverterState != receivedInverterState) {
                        m_inverterState = receivedInverterState;
                        emit inverterStateChanged(m_inverterState);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Inverter state\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Inverter state\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateTotalDcPower()
{
    // Update registers from Total DC power
    QModbusReply *reply = readTotalDcPower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedTotalDcPower = ModbusDataUtils::convertToFloat32(registers);
                    if (m_totalDcPower != receivedTotalDcPower) {
                        m_totalDcPower = receivedTotalDcPower;
                        emit totalDcPowerChanged(m_totalDcPower);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Total DC power\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Total DC power\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateEnergyManagementState()
{
    // Update registers from State of energy manager
    QModbusReply *reply = readEnergyManagementState();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    EnergyManagementState receivedEnergyManagementState = static_cast<EnergyManagementState>(ModbusDataUtils::convertToUInt32(registers));
                    if (m_energyManagementState != receivedEnergyManagementState) {
                        m_energyManagementState = receivedEnergyManagementState;
                        emit energyManagementStateChanged(m_energyManagementState);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"State of energy manager\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"State of energy manager\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateHomeOwnConsumptionFromBattery()
{
    // Update registers from Home own consumption from battery
    QModbusReply *reply = readHomeOwnConsumptionFromBattery();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedHomeOwnConsumptionFromBattery = ModbusDataUtils::convertToFloat32(registers);
                    if (m_homeOwnConsumptionFromBattery != receivedHomeOwnConsumptionFromBattery) {
                        m_homeOwnConsumptionFromBattery = receivedHomeOwnConsumptionFromBattery;
                        emit homeOwnConsumptionFromBatteryChanged(m_homeOwnConsumptionFromBattery);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Home own consumption from battery\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Home own consumption from battery\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateHomeOwnConsumptionFromGrid()
{
    // Update registers from Home own consumption from grid
    QModbusReply *reply = readHomeOwnConsumptionFromGrid();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedHomeOwnConsumptionFromGrid = ModbusDataUtils::convertToFloat32(registers);
                    if (m_homeOwnConsumptionFromGrid != receivedHomeOwnConsumptionFromGrid) {
                        m_homeOwnConsumptionFromGrid = receivedHomeOwnConsumptionFromGrid;
                        emit homeOwnConsumptionFromGridChanged(m_homeOwnConsumptionFromGrid);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Home own consumption from grid\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Home own consumption from grid\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateTotalHomeConsumptionFromBattery()
{
    // Update registers from Total home consumption Battery
    QModbusReply *reply = readTotalHomeConsumptionFromBattery();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedTotalHomeConsumptionFromBattery = ModbusDataUtils::convertToFloat32(registers);
                    if (m_totalHomeConsumptionFromBattery != receivedTotalHomeConsumptionFromBattery) {
                        m_totalHomeConsumptionFromBattery = receivedTotalHomeConsumptionFromBattery;
                        emit totalHomeConsumptionFromBatteryChanged(m_totalHomeConsumptionFromBattery);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Total home consumption Battery\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Total home consumption Battery\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateTotalHomeConsumptionFromGrid()
{
    // Update registers from Total home consumption grid
    QModbusReply *reply = readTotalHomeConsumptionFromGrid();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedTotalHomeConsumptionFromGrid = ModbusDataUtils::convertToFloat32(registers);
                    if (m_totalHomeConsumptionFromGrid != receivedTotalHomeConsumptionFromGrid) {
                        m_totalHomeConsumptionFromGrid = receivedTotalHomeConsumptionFromGrid;
                        emit totalHomeConsumptionFromGridChanged(m_totalHomeConsumptionFromGrid);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Total home consumption grid\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Total home consumption grid\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateTotalHomeConsumptionFromPv()
{
    // Update registers from Total home consumption from PV
    QModbusReply *reply = readTotalHomeConsumptionFromPv();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedTotalHomeConsumptionFromPv = ModbusDataUtils::convertToFloat32(registers);
                    if (m_totalHomeConsumptionFromPv != receivedTotalHomeConsumptionFromPv) {
                        m_totalHomeConsumptionFromPv = receivedTotalHomeConsumptionFromPv;
                        emit totalHomeConsumptionFromPvChanged(m_totalHomeConsumptionFromPv);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Total home consumption from PV\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Total home consumption from PV\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateHomeOwnConsumptionPv()
{
    // Update registers from Home own consumption from PV
    QModbusReply *reply = readHomeOwnConsumptionPv();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedHomeOwnConsumptionPv = ModbusDataUtils::convertToFloat32(registers);
                    if (m_homeOwnConsumptionPv != receivedHomeOwnConsumptionPv) {
                        m_homeOwnConsumptionPv = receivedHomeOwnConsumptionPv;
                        emit homeOwnConsumptionPvChanged(m_homeOwnConsumptionPv);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Home own consumption from PV\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Home own consumption from PV\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateTotalHomeConsumption()
{
    // Update registers from Total home consumption
    QModbusReply *reply = readTotalHomeConsumption();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedTotalHomeConsumption = ModbusDataUtils::convertToFloat32(registers);
                    if (m_totalHomeConsumption != receivedTotalHomeConsumption) {
                        m_totalHomeConsumption = receivedTotalHomeConsumption;
                        emit totalHomeConsumptionChanged(m_totalHomeConsumption);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Total home consumption\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Total home consumption\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateGridFrequency()
{
    // Update registers from Grid frequency
    QModbusReply *reply = readGridFrequency();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedGridFrequency = ModbusDataUtils::convertToFloat32(registers);
                    if (m_gridFrequency != receivedGridFrequency) {
                        m_gridFrequency = receivedGridFrequency;
                        emit gridFrequencyChanged(m_gridFrequency);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Grid frequency\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Grid frequency\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateCurrentPhase1()
{
    // Update registers from Current phase 1
    QModbusReply *reply = readCurrentPhase1();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedCurrentPhase1 = ModbusDataUtils::convertToFloat32(registers);
                    if (m_currentPhase1 != receivedCurrentPhase1) {
                        m_currentPhase1 = receivedCurrentPhase1;
                        emit currentPhase1Changed(m_currentPhase1);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Current phase 1\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Current phase 1\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateActivePowerPhase1()
{
    // Update registers from Active power phase 1
    QModbusReply *reply = readActivePowerPhase1();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedActivePowerPhase1 = ModbusDataUtils::convertToFloat32(registers);
                    if (m_activePowerPhase1 != receivedActivePowerPhase1) {
                        m_activePowerPhase1 = receivedActivePowerPhase1;
                        emit activePowerPhase1Changed(m_activePowerPhase1);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Active power phase 1\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Active power phase 1\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateVoltagePhase1()
{
    // Update registers from Voltage phase 1
    QModbusReply *reply = readVoltagePhase1();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedVoltagePhase1 = ModbusDataUtils::convertToFloat32(registers);
                    if (m_voltagePhase1 != receivedVoltagePhase1) {
                        m_voltagePhase1 = receivedVoltagePhase1;
                        emit voltagePhase1Changed(m_voltagePhase1);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Voltage phase 1\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Voltage phase 1\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateCurrentPhase2()
{
    // Update registers from Current phase 2
    QModbusReply *reply = readCurrentPhase2();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedCurrentPhase2 = ModbusDataUtils::convertToFloat32(registers);
                    if (m_currentPhase2 != receivedCurrentPhase2) {
                        m_currentPhase2 = receivedCurrentPhase2;
                        emit currentPhase2Changed(m_currentPhase2);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Current phase 2\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Current phase 2\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateActivePowerPhase2()
{
    // Update registers from Active power phase 2
    QModbusReply *reply = readActivePowerPhase2();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedActivePowerPhase2 = ModbusDataUtils::convertToFloat32(registers);
                    if (m_activePowerPhase2 != receivedActivePowerPhase2) {
                        m_activePowerPhase2 = receivedActivePowerPhase2;
                        emit activePowerPhase2Changed(m_activePowerPhase2);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Active power phase 2\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Active power phase 2\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateVoltagePhase2()
{
    // Update registers from Voltage phase 2
    QModbusReply *reply = readVoltagePhase2();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedVoltagePhase2 = ModbusDataUtils::convertToFloat32(registers);
                    if (m_voltagePhase2 != receivedVoltagePhase2) {
                        m_voltagePhase2 = receivedVoltagePhase2;
                        emit voltagePhase2Changed(m_voltagePhase2);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Voltage phase 2\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Voltage phase 2\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateCurrentPhase3()
{
    // Update registers from Current phase 3
    QModbusReply *reply = readCurrentPhase3();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedCurrentPhase3 = ModbusDataUtils::convertToFloat32(registers);
                    if (m_currentPhase3 != receivedCurrentPhase3) {
                        m_currentPhase3 = receivedCurrentPhase3;
                        emit currentPhase3Changed(m_currentPhase3);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Current phase 3\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Current phase 3\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateActivePowerPhase3()
{
    // Update registers from Active power phase 3
    QModbusReply *reply = readActivePowerPhase3();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedActivePowerPhase3 = ModbusDataUtils::convertToFloat32(registers);
                    if (m_activePowerPhase3 != receivedActivePowerPhase3) {
                        m_activePowerPhase3 = receivedActivePowerPhase3;
                        emit activePowerPhase3Changed(m_activePowerPhase3);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Active power phase 3\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Active power phase 3\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateVoltagePhase3()
{
    // Update registers from Voltage phase 3
    QModbusReply *reply = readVoltagePhase3();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedVoltagePhase3 = ModbusDataUtils::convertToFloat32(registers);
                    if (m_voltagePhase3 != receivedVoltagePhase3) {
                        m_voltagePhase3 = receivedVoltagePhase3;
                        emit voltagePhase3Changed(m_voltagePhase3);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Voltage phase 3\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Voltage phase 3\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateTotalAcPower()
{
    // Update registers from Total AC power
    QModbusReply *reply = readTotalAcPower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedTotalAcPower = ModbusDataUtils::convertToFloat32(registers);
                    if (m_totalAcPower != receivedTotalAcPower) {
                        m_totalAcPower = receivedTotalAcPower;
                        emit totalAcPowerChanged(m_totalAcPower);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Total AC power\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Total AC power\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateBatteryChargeCurrent()
{
    // Update registers from Battery charge current
    QModbusReply *reply = readBatteryChargeCurrent();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedBatteryChargeCurrent = ModbusDataUtils::convertToFloat32(registers);
                    if (m_batteryChargeCurrent != receivedBatteryChargeCurrent) {
                        m_batteryChargeCurrent = receivedBatteryChargeCurrent;
                        emit batteryChargeCurrentChanged(m_batteryChargeCurrent);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Battery charge current\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Battery charge current\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateNumberOfBytteryCycles()
{
    // Update registers from Number of battery cycles
    QModbusReply *reply = readNumberOfBytteryCycles();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedNumberOfBytteryCycles = ModbusDataUtils::convertToFloat32(registers);
                    if (m_numberOfBytteryCycles != receivedNumberOfBytteryCycles) {
                        m_numberOfBytteryCycles = receivedNumberOfBytteryCycles;
                        emit numberOfBytteryCyclesChanged(m_numberOfBytteryCycles);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Number of battery cycles\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Number of battery cycles\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateActualBatteryChargeCurrent()
{
    // Update registers from Actual battery charge (-) / discharge (+) current
    QModbusReply *reply = readActualBatteryChargeCurrent();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedActualBatteryChargeCurrent = ModbusDataUtils::convertToFloat32(registers);
                    if (m_actualBatteryChargeCurrent != receivedActualBatteryChargeCurrent) {
                        m_actualBatteryChargeCurrent = receivedActualBatteryChargeCurrent;
                        emit actualBatteryChargeCurrentChanged(m_actualBatteryChargeCurrent);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Actual battery charge (-) / discharge (+) current\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Actual battery charge (-) / discharge (+) current\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateBatteryStateOfCharge()
{
    // Update registers from Actual state of charge
    QModbusReply *reply = readBatteryStateOfCharge();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedBatteryStateOfCharge = ModbusDataUtils::convertToFloat32(registers);
                    if (m_batteryStateOfCharge != receivedBatteryStateOfCharge) {
                        m_batteryStateOfCharge = receivedBatteryStateOfCharge;
                        emit batteryStateOfChargeChanged(m_batteryStateOfCharge);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Actual state of charge\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Actual state of charge\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateBatteryTemperature()
{
    // Update registers from Battery temperature
    QModbusReply *reply = readBatteryTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedBatteryTemperature = ModbusDataUtils::convertToFloat32(registers);
                    if (m_batteryTemperature != receivedBatteryTemperature) {
                        m_batteryTemperature = receivedBatteryTemperature;
                        emit batteryTemperatureChanged(m_batteryTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Battery temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Battery temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void KostalConnection::updateBatteryVoltage()
{
    // Update registers from Battery voltage
    QModbusReply *reply = readBatteryVoltage();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> registers = unit.values();
                    float receivedBatteryVoltage = ModbusDataUtils::convertToFloat32(registers);
                    if (m_batteryVoltage != receivedBatteryVoltage) {
                        m_batteryVoltage = receivedBatteryVoltage;
                        emit batteryVoltageChanged(m_batteryVoltage);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcKostalConnection()) << "Modbus reply error occurred while updating \"Battery voltage\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcKostalConnection()) << "Error occurred while reading \"Battery voltage\" registers from" << hostAddress().toString() << errorString();
    }
}

QModbusReply *KostalConnection::readModbusUnitId()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 4, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readModbusByteOrder()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 5, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readInverterArticleNumber()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 6, 8);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readBidirectionalConverterNumber()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 30, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readNumberPvStrings()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 34, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readHardwareVersion()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 36, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readSoftwareVersionMainController()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 38, 8);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readSoftwareVersionIoController()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 46, 8);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readPowerId()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 54, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readInverterState()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 56, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readTotalDcPower()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 100, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readEnergyManagementState()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 104, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readHomeOwnConsumptionFromBattery()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 106, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readHomeOwnConsumptionFromGrid()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 108, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readTotalHomeConsumptionFromBattery()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 110, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readTotalHomeConsumptionFromGrid()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 112, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readTotalHomeConsumptionFromPv()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 114, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readHomeOwnConsumptionPv()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 116, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readTotalHomeConsumption()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 118, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readGridFrequency()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 152, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readCurrentPhase1()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 154, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readActivePowerPhase1()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 156, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readVoltagePhase1()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 158, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readCurrentPhase2()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 160, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readActivePowerPhase2()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 162, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readVoltagePhase2()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 164, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readCurrentPhase3()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 166, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readActivePowerPhase3()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 168, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readVoltagePhase3()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 170, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readTotalAcPower()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 172, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readBatteryChargeCurrent()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 190, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readNumberOfBytteryCycles()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 194, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readActualBatteryChargeCurrent()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 200, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readBatteryStateOfCharge()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 210, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readBatteryTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 214, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *KostalConnection::readBatteryVoltage()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 216, 2);
    return sendReadRequest(request, m_slaveId);
}

void KostalConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcKostalConnection()) << "Initialization finished of KostalConnection" << hostAddress().toString();
        emit initializationFinished();
    }
}

