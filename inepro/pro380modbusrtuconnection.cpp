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


#include "pro380modbusrtuconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcPro380ModbusRtuConnection, "Pro380ModbusRtuConnection")

Pro380ModbusRtuConnection::Pro380ModbusRtuConnection(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent) :
    QObject(parent),
    m_modbusRtuMaster(modbusRtuMaster),
    m_slaveId(slaveId)
{
    
}

ModbusRtuMaster *Pro380ModbusRtuConnection::modbusRtuMaster() const
{
    return m_modbusRtuMaster;
}
quint16 Pro380ModbusRtuConnection::slaveId() const
{
    return m_slaveId;
}
float Pro380ModbusRtuConnection::frequency() const
{
    return m_frequency;
}

float Pro380ModbusRtuConnection::totalEnergyConsumed() const
{
    return m_totalEnergyConsumed;
}

float Pro380ModbusRtuConnection::totalEnergyProduced() const
{
    return m_totalEnergyProduced;
}

float Pro380ModbusRtuConnection::voltagePhaseA() const
{
    return m_voltagePhaseA;
}

float Pro380ModbusRtuConnection::voltagePhaseB() const
{
    return m_voltagePhaseB;
}

float Pro380ModbusRtuConnection::voltagePhaseC() const
{
    return m_voltagePhaseC;
}

float Pro380ModbusRtuConnection::currentPhaseA() const
{
    return m_currentPhaseA;
}

float Pro380ModbusRtuConnection::currentPhaseB() const
{
    return m_currentPhaseB;
}

float Pro380ModbusRtuConnection::currentPhaseC() const
{
    return m_currentPhaseC;
}

float Pro380ModbusRtuConnection::totalCurrentPower() const
{
    return m_totalCurrentPower;
}

float Pro380ModbusRtuConnection::powerPhaseA() const
{
    return m_powerPhaseA;
}

float Pro380ModbusRtuConnection::powerPhaseB() const
{
    return m_powerPhaseB;
}

float Pro380ModbusRtuConnection::powerPhaseC() const
{
    return m_powerPhaseC;
}

float Pro380ModbusRtuConnection::energyConsumedPhaseA() const
{
    return m_energyConsumedPhaseA;
}

float Pro380ModbusRtuConnection::energyConsumedPhaseB() const
{
    return m_energyConsumedPhaseB;
}

float Pro380ModbusRtuConnection::energyConsumedPhaseC() const
{
    return m_energyConsumedPhaseC;
}

float Pro380ModbusRtuConnection::energyProducedPhaseA() const
{
    return m_energyProducedPhaseA;
}

float Pro380ModbusRtuConnection::energyProducedPhaseB() const
{
    return m_energyProducedPhaseB;
}

float Pro380ModbusRtuConnection::energyProducedPhaseC() const
{
    return m_energyProducedPhaseC;
}

void Pro380ModbusRtuConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void Pro380ModbusRtuConnection::update()
{
    updateFrequency();
    updateTotalEnergyConsumed();
    updateTotalEnergyProduced();
    updatePhasesVoltageBlock();
    updatePhasesCurrentBlock();
    updateCurrentPowerBlock();
    updatePhasesEnergyConsumedBlock();
    updatePhasesEnergyProducedBlock();
}

void Pro380ModbusRtuConnection::updateFrequency()
{
    // Update registers from Frequency
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Frequency\" register:" << 20488 << "size:" << 2;
    ModbusRtuReply *reply = readFrequency();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Frequency\" register" << 20488 << "size:" << 2 << values;
                    float receivedFrequency = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_frequency != receivedFrequency) {
                        m_frequency = receivedFrequency;
                        emit frequencyChanged(m_frequency);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Frequency\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Frequency\" registers";
    }
}

void Pro380ModbusRtuConnection::updateTotalEnergyConsumed()
{
    // Update registers from Total energy consumed (Forward active energy)
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Total energy consumed (Forward active energy)\" register:" << 24588 << "size:" << 2;
    ModbusRtuReply *reply = readTotalEnergyConsumed();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Total energy consumed (Forward active energy)\" register" << 24588 << "size:" << 2 << values;
                    float receivedTotalEnergyConsumed = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalEnergyConsumed != receivedTotalEnergyConsumed) {
                        m_totalEnergyConsumed = receivedTotalEnergyConsumed;
                        emit totalEnergyConsumedChanged(m_totalEnergyConsumed);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy consumed (Forward active energy)\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Total energy consumed (Forward active energy)\" registers";
    }
}

void Pro380ModbusRtuConnection::updateTotalEnergyProduced()
{
    // Update registers from Total energy produced (Reverse active energy)
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Total energy produced (Reverse active energy)\" register:" << 24600 << "size:" << 2;
    ModbusRtuReply *reply = readTotalEnergyProduced();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Total energy produced (Reverse active energy)\" register" << 24600 << "size:" << 2 << values;
                    float receivedTotalEnergyProduced = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalEnergyProduced != receivedTotalEnergyProduced) {
                        m_totalEnergyProduced = receivedTotalEnergyProduced;
                        emit totalEnergyProducedChanged(m_totalEnergyProduced);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy produced (Reverse active energy)\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Total energy produced (Reverse active energy)\" registers";
    }
}

void Pro380ModbusRtuConnection::updatePhasesVoltageBlock()
{
    // Update register block "phasesVoltage"
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read block \"phasesVoltage\" registers from:" << 20482 << "size:" << 6;
    ModbusRtuReply *reply = m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20482, 6);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from reading block \"phasesVoltage\" register" << 20482 << "size:" << 6 << blockValues;
                    values = blockValues.mid(0, 2);
                    float receivedVoltagePhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_voltagePhaseA != receivedVoltagePhaseA) {
                        m_voltagePhaseA = receivedVoltagePhaseA;
                        emit voltagePhaseAChanged(m_voltagePhaseA);
                    }

                    values = blockValues.mid(2, 2);
                    float receivedVoltagePhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_voltagePhaseB != receivedVoltagePhaseB) {
                        m_voltagePhaseB = receivedVoltagePhaseB;
                        emit voltagePhaseBChanged(m_voltagePhaseB);
                    }

                    values = blockValues.mid(4, 2);
                    float receivedVoltagePhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_voltagePhaseC != receivedVoltagePhaseC) {
                        m_voltagePhaseC = receivedVoltagePhaseC;
                        emit voltagePhaseCChanged(m_voltagePhaseC);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"phasesVoltage\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading block \"phasesVoltage\" registers";
    }
}

void Pro380ModbusRtuConnection::updatePhasesCurrentBlock()
{
    // Update register block "phasesCurrent"
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read block \"phasesCurrent\" registers from:" << 20492 << "size:" << 6;
    ModbusRtuReply *reply = m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20492, 6);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from reading block \"phasesCurrent\" register" << 20492 << "size:" << 6 << blockValues;
                    values = blockValues.mid(0, 2);
                    float receivedCurrentPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseA != receivedCurrentPhaseA) {
                        m_currentPhaseA = receivedCurrentPhaseA;
                        emit currentPhaseAChanged(m_currentPhaseA);
                    }

                    values = blockValues.mid(2, 2);
                    float receivedCurrentPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseB != receivedCurrentPhaseB) {
                        m_currentPhaseB = receivedCurrentPhaseB;
                        emit currentPhaseBChanged(m_currentPhaseB);
                    }

                    values = blockValues.mid(4, 2);
                    float receivedCurrentPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseC != receivedCurrentPhaseC) {
                        m_currentPhaseC = receivedCurrentPhaseC;
                        emit currentPhaseCChanged(m_currentPhaseC);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"phasesCurrent\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading block \"phasesCurrent\" registers";
    }
}

void Pro380ModbusRtuConnection::updateCurrentPowerBlock()
{
    // Update register block "currentPower"
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read block \"currentPower\" registers from:" << 20498 << "size:" << 8;
    ModbusRtuReply *reply = m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20498, 8);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from reading block \"currentPower\" register" << 20498 << "size:" << 8 << blockValues;
                    values = blockValues.mid(0, 2);
                    float receivedTotalCurrentPower = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalCurrentPower != receivedTotalCurrentPower) {
                        m_totalCurrentPower = receivedTotalCurrentPower;
                        emit totalCurrentPowerChanged(m_totalCurrentPower);
                    }

                    values = blockValues.mid(2, 2);
                    float receivedPowerPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseA != receivedPowerPhaseA) {
                        m_powerPhaseA = receivedPowerPhaseA;
                        emit powerPhaseAChanged(m_powerPhaseA);
                    }

                    values = blockValues.mid(4, 2);
                    float receivedPowerPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseB != receivedPowerPhaseB) {
                        m_powerPhaseB = receivedPowerPhaseB;
                        emit powerPhaseBChanged(m_powerPhaseB);
                    }

                    values = blockValues.mid(6, 2);
                    float receivedPowerPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseC != receivedPowerPhaseC) {
                        m_powerPhaseC = receivedPowerPhaseC;
                        emit powerPhaseCChanged(m_powerPhaseC);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"currentPower\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading block \"currentPower\" registers";
    }
}

void Pro380ModbusRtuConnection::updatePhasesEnergyConsumedBlock()
{
    // Update register block "phasesEnergyConsumed"
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read block \"phasesEnergyConsumed\" registers from:" << 24594 << "size:" << 6;
    ModbusRtuReply *reply = m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24594, 6);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from reading block \"phasesEnergyConsumed\" register" << 24594 << "size:" << 6 << blockValues;
                    values = blockValues.mid(0, 2);
                    float receivedEnergyConsumedPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseA != receivedEnergyConsumedPhaseA) {
                        m_energyConsumedPhaseA = receivedEnergyConsumedPhaseA;
                        emit energyConsumedPhaseAChanged(m_energyConsumedPhaseA);
                    }

                    values = blockValues.mid(2, 2);
                    float receivedEnergyConsumedPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseB != receivedEnergyConsumedPhaseB) {
                        m_energyConsumedPhaseB = receivedEnergyConsumedPhaseB;
                        emit energyConsumedPhaseBChanged(m_energyConsumedPhaseB);
                    }

                    values = blockValues.mid(4, 2);
                    float receivedEnergyConsumedPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseC != receivedEnergyConsumedPhaseC) {
                        m_energyConsumedPhaseC = receivedEnergyConsumedPhaseC;
                        emit energyConsumedPhaseCChanged(m_energyConsumedPhaseC);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"phasesEnergyConsumed\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading block \"phasesEnergyConsumed\" registers";
    }
}

void Pro380ModbusRtuConnection::updatePhasesEnergyProducedBlock()
{
    // Update register block "phasesEnergyProduced"
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read block \"phasesEnergyProduced\" registers from:" << 24606 << "size:" << 6;
    ModbusRtuReply *reply = m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24606, 6);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from reading block \"phasesEnergyProduced\" register" << 24606 << "size:" << 6 << blockValues;
                    values = blockValues.mid(0, 2);
                    float receivedEnergyProducedPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyProducedPhaseA != receivedEnergyProducedPhaseA) {
                        m_energyProducedPhaseA = receivedEnergyProducedPhaseA;
                        emit energyProducedPhaseAChanged(m_energyProducedPhaseA);
                    }

                    values = blockValues.mid(2, 2);
                    float receivedEnergyProducedPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyProducedPhaseB != receivedEnergyProducedPhaseB) {
                        m_energyProducedPhaseB = receivedEnergyProducedPhaseB;
                        emit energyProducedPhaseBChanged(m_energyProducedPhaseB);
                    }

                    values = blockValues.mid(4, 2);
                    float receivedEnergyProducedPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyProducedPhaseC != receivedEnergyProducedPhaseC) {
                        m_energyProducedPhaseC = receivedEnergyProducedPhaseC;
                        emit energyProducedPhaseCChanged(m_energyProducedPhaseC);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"phasesEnergyProduced\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading block \"phasesEnergyProduced\" registers";
    }
}

ModbusRtuReply *Pro380ModbusRtuConnection::readFrequency()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20488, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readTotalEnergyConsumed()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24588, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readTotalEnergyProduced()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24600, 2);
}

void Pro380ModbusRtuConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcPro380ModbusRtuConnection()) << "Initialization finished of Pro380ModbusRtuConnection";
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, Pro380ModbusRtuConnection *pro380ModbusRtuConnection)
{
    debug.nospace().noquote() << "Pro380ModbusRtuConnection(" << pro380ModbusRtuConnection->modbusRtuMaster()->modbusUuid().toString() << ", " << pro380ModbusRtuConnection->modbusRtuMaster()->serialPort() << ", slave ID:" << pro380ModbusRtuConnection->slaveId() << ")" << "\n";
    debug.nospace().noquote() << "    - Frequency:" << pro380ModbusRtuConnection->frequency() << " [Hz]" << "\n";
    debug.nospace().noquote() << "    - Total energy consumed (Forward active energy):" << pro380ModbusRtuConnection->totalEnergyConsumed() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Total energy produced (Reverse active energy):" << pro380ModbusRtuConnection->totalEnergyProduced() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L1:" << pro380ModbusRtuConnection->voltagePhaseA() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L2:" << pro380ModbusRtuConnection->voltagePhaseB() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L3:" << pro380ModbusRtuConnection->voltagePhaseC() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Current phase L1:" << pro380ModbusRtuConnection->currentPhaseA() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Current phase L2:" << pro380ModbusRtuConnection->currentPhaseB() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Current phase L3:" << pro380ModbusRtuConnection->currentPhaseC() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Total system power:" << pro380ModbusRtuConnection->totalCurrentPower() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Power phase L1:" << pro380ModbusRtuConnection->powerPhaseA() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Power phase L2:" << pro380ModbusRtuConnection->powerPhaseB() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Power phase L3:" << pro380ModbusRtuConnection->powerPhaseC() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase A:" << pro380ModbusRtuConnection->energyConsumedPhaseA() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase B:" << pro380ModbusRtuConnection->energyConsumedPhaseB() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase C:" << pro380ModbusRtuConnection->energyConsumedPhaseC() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase A:" << pro380ModbusRtuConnection->energyProducedPhaseA() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase B:" << pro380ModbusRtuConnection->energyProducedPhaseB() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase C:" << pro380ModbusRtuConnection->energyProducedPhaseC() << " [kWh]" << "\n";
    return debug.quote().space();
}

