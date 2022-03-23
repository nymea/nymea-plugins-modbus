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


#include "sdm630modbusrtuconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcSdm630ModbusRtuConnection, "Sdm630ModbusRtuConnection")

Sdm630ModbusRtuConnection::Sdm630ModbusRtuConnection(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent) :
    QObject(parent),
    m_modbusRtuMaster(modbusRtuMaster),
    m_slaveId(slaveId)
{
    
}

ModbusRtuMaster *Sdm630ModbusRtuConnection::modbusRtuMaster() const
{
    return m_modbusRtuMaster;
}
quint16 Sdm630ModbusRtuConnection::slaveId() const
{
    return m_slaveId;
}
float Sdm630ModbusRtuConnection::totalCurrentPower() const
{
    return m_totalCurrentPower;
}

float Sdm630ModbusRtuConnection::voltagePhaseA() const
{
    return m_voltagePhaseA;
}

float Sdm630ModbusRtuConnection::voltagePhaseB() const
{
    return m_voltagePhaseB;
}

float Sdm630ModbusRtuConnection::voltagePhaseC() const
{
    return m_voltagePhaseC;
}

float Sdm630ModbusRtuConnection::currentPhaseA() const
{
    return m_currentPhaseA;
}

float Sdm630ModbusRtuConnection::currentPhaseB() const
{
    return m_currentPhaseB;
}

float Sdm630ModbusRtuConnection::currentPhaseC() const
{
    return m_currentPhaseC;
}

float Sdm630ModbusRtuConnection::powerPhaseA() const
{
    return m_powerPhaseA;
}

float Sdm630ModbusRtuConnection::powerPhaseB() const
{
    return m_powerPhaseB;
}

float Sdm630ModbusRtuConnection::powerPhaseC() const
{
    return m_powerPhaseC;
}

float Sdm630ModbusRtuConnection::frequency() const
{
    return m_frequency;
}

float Sdm630ModbusRtuConnection::totalEnergyConsumed() const
{
    return m_totalEnergyConsumed;
}

float Sdm630ModbusRtuConnection::totalEnergyProduced() const
{
    return m_totalEnergyProduced;
}

float Sdm630ModbusRtuConnection::energyProducedPhaseA() const
{
    return m_energyProducedPhaseA;
}

float Sdm630ModbusRtuConnection::energyProducedPhaseB() const
{
    return m_energyProducedPhaseB;
}

float Sdm630ModbusRtuConnection::energyProducedPhaseC() const
{
    return m_energyProducedPhaseC;
}

float Sdm630ModbusRtuConnection::energyConsumedPhaseA() const
{
    return m_energyConsumedPhaseA;
}

float Sdm630ModbusRtuConnection::energyConsumedPhaseB() const
{
    return m_energyConsumedPhaseB;
}

float Sdm630ModbusRtuConnection::energyConsumedPhaseC() const
{
    return m_energyConsumedPhaseC;
}

void Sdm630ModbusRtuConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void Sdm630ModbusRtuConnection::update()
{
    updateTotalCurrentPower();
    updatePhaseVoltageAndCurrentBlock();
    updatePhasePowerBlock();
    updateFrequencyAndTotalEnergyBlock();
    updatePhaseEnergyEnergyBlock();
}

void Sdm630ModbusRtuConnection::updateTotalCurrentPower()
{
    // Update registers from Total system power
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Total system power\" register:" << 52 << "size:" << 2;
    ModbusRtuReply *reply = readTotalCurrentPower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Total system power\" register" << 52 << "size:" << 2 << values;
                    float receivedTotalCurrentPower = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalCurrentPower != receivedTotalCurrentPower) {
                        m_totalCurrentPower = receivedTotalCurrentPower;
                        emit totalCurrentPowerChanged(m_totalCurrentPower);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total system power\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Total system power\" registers";
    }
}

void Sdm630ModbusRtuConnection::updatePhaseVoltageAndCurrentBlock()
{
    // Update register block "phaseVoltageAndCurrent"
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read block \"phaseVoltageAndCurrent\" registers from:" << 0 << "size:" << 12;
    ModbusRtuReply *reply = m_modbusRtuMaster->readInputRegister(m_slaveId, 0, 12);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from reading block \"phaseVoltageAndCurrent\" register" << 0 << "size:" << 12 << blockValues;
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

                    values = blockValues.mid(6, 2);
                    float receivedCurrentPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseA != receivedCurrentPhaseA) {
                        m_currentPhaseA = receivedCurrentPhaseA;
                        emit currentPhaseAChanged(m_currentPhaseA);
                    }

                    values = blockValues.mid(8, 2);
                    float receivedCurrentPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseB != receivedCurrentPhaseB) {
                        m_currentPhaseB = receivedCurrentPhaseB;
                        emit currentPhaseBChanged(m_currentPhaseB);
                    }

                    values = blockValues.mid(10, 2);
                    float receivedCurrentPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseC != receivedCurrentPhaseC) {
                        m_currentPhaseC = receivedCurrentPhaseC;
                        emit currentPhaseCChanged(m_currentPhaseC);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"phaseVoltageAndCurrent\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading block \"phaseVoltageAndCurrent\" registers";
    }
}

void Sdm630ModbusRtuConnection::updatePhasePowerBlock()
{
    // Update register block "phasePower"
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read block \"phasePower\" registers from:" << 12 << "size:" << 6;
    ModbusRtuReply *reply = m_modbusRtuMaster->readInputRegister(m_slaveId, 12, 6);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from reading block \"phasePower\" register" << 12 << "size:" << 6 << blockValues;
                    values = blockValues.mid(0, 2);
                    float receivedPowerPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseA != receivedPowerPhaseA) {
                        m_powerPhaseA = receivedPowerPhaseA;
                        emit powerPhaseAChanged(m_powerPhaseA);
                    }

                    values = blockValues.mid(2, 2);
                    float receivedPowerPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseB != receivedPowerPhaseB) {
                        m_powerPhaseB = receivedPowerPhaseB;
                        emit powerPhaseBChanged(m_powerPhaseB);
                    }

                    values = blockValues.mid(4, 2);
                    float receivedPowerPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseC != receivedPowerPhaseC) {
                        m_powerPhaseC = receivedPowerPhaseC;
                        emit powerPhaseCChanged(m_powerPhaseC);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"phasePower\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading block \"phasePower\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateFrequencyAndTotalEnergyBlock()
{
    // Update register block "frequencyAndTotalEnergy"
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read block \"frequencyAndTotalEnergy\" registers from:" << 70 << "size:" << 6;
    ModbusRtuReply *reply = m_modbusRtuMaster->readInputRegister(m_slaveId, 70, 6);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from reading block \"frequencyAndTotalEnergy\" register" << 70 << "size:" << 6 << blockValues;
                    values = blockValues.mid(0, 2);
                    float receivedFrequency = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_frequency != receivedFrequency) {
                        m_frequency = receivedFrequency;
                        emit frequencyChanged(m_frequency);
                    }

                    values = blockValues.mid(2, 2);
                    float receivedTotalEnergyConsumed = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalEnergyConsumed != receivedTotalEnergyConsumed) {
                        m_totalEnergyConsumed = receivedTotalEnergyConsumed;
                        emit totalEnergyConsumedChanged(m_totalEnergyConsumed);
                    }

                    values = blockValues.mid(4, 2);
                    float receivedTotalEnergyProduced = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalEnergyProduced != receivedTotalEnergyProduced) {
                        m_totalEnergyProduced = receivedTotalEnergyProduced;
                        emit totalEnergyProducedChanged(m_totalEnergyProduced);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"frequencyAndTotalEnergy\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading block \"frequencyAndTotalEnergy\" registers";
    }
}

void Sdm630ModbusRtuConnection::updatePhaseEnergyEnergyBlock()
{
    // Update register block "phaseEnergyEnergy"
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read block \"phaseEnergyEnergy\" registers from:" << 346 << "size:" << 12;
    ModbusRtuReply *reply = m_modbusRtuMaster->readInputRegister(m_slaveId, 346, 12);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from reading block \"phaseEnergyEnergy\" register" << 346 << "size:" << 12 << blockValues;
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

                    values = blockValues.mid(6, 2);
                    float receivedEnergyConsumedPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseA != receivedEnergyConsumedPhaseA) {
                        m_energyConsumedPhaseA = receivedEnergyConsumedPhaseA;
                        emit energyConsumedPhaseAChanged(m_energyConsumedPhaseA);
                    }

                    values = blockValues.mid(8, 2);
                    float receivedEnergyConsumedPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseB != receivedEnergyConsumedPhaseB) {
                        m_energyConsumedPhaseB = receivedEnergyConsumedPhaseB;
                        emit energyConsumedPhaseBChanged(m_energyConsumedPhaseB);
                    }

                    values = blockValues.mid(10, 2);
                    float receivedEnergyConsumedPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseC != receivedEnergyConsumedPhaseC) {
                        m_energyConsumedPhaseC = receivedEnergyConsumedPhaseC;
                        emit energyConsumedPhaseCChanged(m_energyConsumedPhaseC);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"phaseEnergyEnergy\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading block \"phaseEnergyEnergy\" registers";
    }
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readTotalCurrentPower()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 52, 2);
}

void Sdm630ModbusRtuConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcSdm630ModbusRtuConnection()) << "Initialization finished of Sdm630ModbusRtuConnection";
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, Sdm630ModbusRtuConnection *sdm630ModbusRtuConnection)
{
    debug.nospace().noquote() << "Sdm630ModbusRtuConnection(" << sdm630ModbusRtuConnection->modbusRtuMaster()->modbusUuid().toString() << ", " << sdm630ModbusRtuConnection->modbusRtuMaster()->serialPort() << ", slave ID:" << sdm630ModbusRtuConnection->slaveId() << ")" << "\n";
    debug.nospace().noquote() << "    - Total system power:" << sdm630ModbusRtuConnection->totalCurrentPower() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L1:" << sdm630ModbusRtuConnection->voltagePhaseA() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L2:" << sdm630ModbusRtuConnection->voltagePhaseB() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L3:" << sdm630ModbusRtuConnection->voltagePhaseC() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Current phase L1:" << sdm630ModbusRtuConnection->currentPhaseA() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Current phase L2:" << sdm630ModbusRtuConnection->currentPhaseB() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Current phase L3:" << sdm630ModbusRtuConnection->currentPhaseC() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Power phase L1:" << sdm630ModbusRtuConnection->powerPhaseA() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Power phase L2:" << sdm630ModbusRtuConnection->powerPhaseB() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Power phase L3:" << sdm630ModbusRtuConnection->powerPhaseC() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Frequency:" << sdm630ModbusRtuConnection->frequency() << " [Hz]" << "\n";
    debug.nospace().noquote() << "    - Total energy consumed:" << sdm630ModbusRtuConnection->totalEnergyConsumed() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Total energy produced:" << sdm630ModbusRtuConnection->totalEnergyProduced() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase A:" << sdm630ModbusRtuConnection->energyProducedPhaseA() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase B:" << sdm630ModbusRtuConnection->energyProducedPhaseB() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase C:" << sdm630ModbusRtuConnection->energyProducedPhaseC() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase A:" << sdm630ModbusRtuConnection->energyConsumedPhaseA() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase B:" << sdm630ModbusRtuConnection->energyConsumedPhaseB() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase C:" << sdm630ModbusRtuConnection->energyConsumedPhaseC() << " [kWh]" << "\n";
    return debug.quote().space();
}

