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

#include "cionmodbusrtuconnection.h"
#include "loggingcategories.h"
#include <math.h>
NYMEA_LOGGING_CATEGORY(dcCionModbusRtuConnection, "CionModbusRtuConnection")

CionModbusRtuConnection::CionModbusRtuConnection(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent) :
    QObject(parent),
    m_modbusRtuMaster(modbusRtuMaster),
    m_slaveId(slaveId)
{
    
}

ModbusRtuMaster *CionModbusRtuConnection::modbusRtuMaster() const
{
    return m_modbusRtuMaster;
}
quint16 CionModbusRtuConnection::slaveId() const
{
    return m_slaveId;
}
quint16 CionModbusRtuConnection::chargingEnabled() const
{
    return m_chargingEnabled;
}

ModbusRtuReply *CionModbusRtuConnection::setChargingEnabled(quint16 chargingEnabled)
{
    QVector<quint16> values = ModbusDataUtils::convertFromUInt16(chargingEnabled);
    qCDebug(dcCionModbusRtuConnection()) << "--> Write \"Charging enabled\" register:" << 100 << "size:" << 1 << values;
    return m_modbusRtuMaster->writeHoldingRegisters(m_slaveId, 100, values);
}

quint16 CionModbusRtuConnection::chargingCurrentSetpoint() const
{
    return m_chargingCurrentSetpoint;
}

ModbusRtuReply *CionModbusRtuConnection::setChargingCurrentSetpoint(quint16 chargingCurrentSetpoint)
{
    QVector<quint16> values = ModbusDataUtils::convertFromUInt16(chargingCurrentSetpoint);
    qCDebug(dcCionModbusRtuConnection()) << "--> Write \"Charging current setpoint\" register:" << 101 << "size:" << 1 << values;
    return m_modbusRtuMaster->writeHoldingRegisters(m_slaveId, 101, values);
}

quint16 CionModbusRtuConnection::statusBits() const
{
    return m_statusBits;
}

quint16 CionModbusRtuConnection::cpSignalState() const
{
    return m_cpSignalState;
}

float CionModbusRtuConnection::u1Voltage() const
{
    return m_u1Voltage;
}

float CionModbusRtuConnection::gridVoltage() const
{
    return m_gridVoltage;
}

quint16 CionModbusRtuConnection::minChargingCurrent() const
{
    return m_minChargingCurrent;
}

quint16 CionModbusRtuConnection::currentChargingCurrentE3() const
{
    return m_currentChargingCurrentE3;
}

quint16 CionModbusRtuConnection::maxChargingCurrentE3() const
{
    return m_maxChargingCurrentE3;
}

quint16 CionModbusRtuConnection::maxChargingCurrentCableE3() const
{
    return m_maxChargingCurrentCableE3;
}

quint32 CionModbusRtuConnection::chargingDuration() const
{
    return m_chargingDuration;
}

quint32 CionModbusRtuConnection::pluggedInDuration() const
{
    return m_pluggedInDuration;
}

void CionModbusRtuConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void CionModbusRtuConnection::update()
{
    updateChargingEnabled();
    updateChargingCurrentSetpoint();
    updateStatusBits();
    updateCpSignalState();
    updateU1Voltage();
    updateGridVoltage();
    updateMinChargingCurrent();
    updateE3Block();
    updateDurationsBlock();
}

void CionModbusRtuConnection::updateChargingEnabled()
{
    // Update registers from Charging enabled
    qCDebug(dcCionModbusRtuConnection()) << "--> Read \"Charging enabled\" register:" << 100 << "size:" << 1;
    ModbusRtuReply *reply = readChargingEnabled();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcCionModbusRtuConnection()) << "<-- Response from \"Charging enabled\" register" << 100 << "size:" << 1 << values;
                    quint16 receivedChargingEnabled = ModbusDataUtils::convertToUInt16(values);
                    if (m_chargingEnabled != receivedChargingEnabled) {
                        m_chargingEnabled = receivedChargingEnabled;
                        emit chargingEnabledChanged(m_chargingEnabled);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcCionModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Charging enabled\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcCionModbusRtuConnection()) << "Error occurred while reading \"Charging enabled\" registers";
    }
}

void CionModbusRtuConnection::updateChargingCurrentSetpoint()
{
    // Update registers from Charging current setpoint
    qCDebug(dcCionModbusRtuConnection()) << "--> Read \"Charging current setpoint\" register:" << 101 << "size:" << 1;
    ModbusRtuReply *reply = readChargingCurrentSetpoint();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcCionModbusRtuConnection()) << "<-- Response from \"Charging current setpoint\" register" << 101 << "size:" << 1 << values;
                    quint16 receivedChargingCurrentSetpoint = ModbusDataUtils::convertToUInt16(values);
                    if (m_chargingCurrentSetpoint != receivedChargingCurrentSetpoint) {
                        m_chargingCurrentSetpoint = receivedChargingCurrentSetpoint;
                        emit chargingCurrentSetpointChanged(m_chargingCurrentSetpoint);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcCionModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Charging current setpoint\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcCionModbusRtuConnection()) << "Error occurred while reading \"Charging current setpoint\" registers";
    }
}

void CionModbusRtuConnection::updateStatusBits()
{
    // Update registers from Mode3-State A, B, C, D, U
    qCDebug(dcCionModbusRtuConnection()) << "--> Read \"Mode3-State A, B, C, D, U\" register:" << 121 << "size:" << 1;
    ModbusRtuReply *reply = readStatusBits();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcCionModbusRtuConnection()) << "<-- Response from \"Mode3-State A, B, C, D, U\" register" << 121 << "size:" << 1 << values;
                    quint16 receivedStatusBits = ModbusDataUtils::convertToUInt16(values);
                    if (m_statusBits != receivedStatusBits) {
                        m_statusBits = receivedStatusBits;
                        emit statusBitsChanged(m_statusBits);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcCionModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Mode3-State A, B, C, D, U\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcCionModbusRtuConnection()) << "Error occurred while reading \"Mode3-State A, B, C, D, U\" registers";
    }
}

void CionModbusRtuConnection::updateCpSignalState()
{
    // Update registers from Status bits
    qCDebug(dcCionModbusRtuConnection()) << "--> Read \"Status bits\" register:" << 139 << "size:" << 1;
    ModbusRtuReply *reply = readCpSignalState();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcCionModbusRtuConnection()) << "<-- Response from \"Status bits\" register" << 139 << "size:" << 1 << values;
                    quint16 receivedCpSignalState = ModbusDataUtils::convertToUInt16(values);
                    if (m_cpSignalState != receivedCpSignalState) {
                        m_cpSignalState = receivedCpSignalState;
                        emit cpSignalStateChanged(m_cpSignalState);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcCionModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Status bits\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcCionModbusRtuConnection()) << "Error occurred while reading \"Status bits\" registers";
    }
}

void CionModbusRtuConnection::updateU1Voltage()
{
    // Update registers from U1 voltage
    qCDebug(dcCionModbusRtuConnection()) << "--> Read \"U1 voltage\" register:" << 167 << "size:" << 1;
    ModbusRtuReply *reply = readU1Voltage();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcCionModbusRtuConnection()) << "<-- Response from \"U1 voltage\" register" << 167 << "size:" << 1 << values;
                    float receivedU1Voltage = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -2);
                    if (m_u1Voltage != receivedU1Voltage) {
                        m_u1Voltage = receivedU1Voltage;
                        emit u1VoltageChanged(m_u1Voltage);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcCionModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"U1 voltage\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcCionModbusRtuConnection()) << "Error occurred while reading \"U1 voltage\" registers";
    }
}

void CionModbusRtuConnection::updateGridVoltage()
{
    // Update registers from Voltage of the power supply grid
    qCDebug(dcCionModbusRtuConnection()) << "--> Read \"Voltage of the power supply grid\" register:" << 302 << "size:" << 1;
    ModbusRtuReply *reply = readGridVoltage();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcCionModbusRtuConnection()) << "<-- Response from \"Voltage of the power supply grid\" register" << 302 << "size:" << 1 << values;
                    float receivedGridVoltage = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -2);
                    if (m_gridVoltage != receivedGridVoltage) {
                        m_gridVoltage = receivedGridVoltage;
                        emit gridVoltageChanged(m_gridVoltage);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcCionModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Voltage of the power supply grid\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcCionModbusRtuConnection()) << "Error occurred while reading \"Voltage of the power supply grid\" registers";
    }
}

void CionModbusRtuConnection::updateMinChargingCurrent()
{
    // Update registers from Minimum charging current
    qCDebug(dcCionModbusRtuConnection()) << "--> Read \"Minimum charging current\" register:" << 507 << "size:" << 1;
    ModbusRtuReply *reply = readMinChargingCurrent();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcCionModbusRtuConnection()) << "<-- Response from \"Minimum charging current\" register" << 507 << "size:" << 1 << values;
                    quint16 receivedMinChargingCurrent = ModbusDataUtils::convertToUInt16(values);
                    if (m_minChargingCurrent != receivedMinChargingCurrent) {
                        m_minChargingCurrent = receivedMinChargingCurrent;
                        emit minChargingCurrentChanged(m_minChargingCurrent);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcCionModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Minimum charging current\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcCionModbusRtuConnection()) << "Error occurred while reading \"Minimum charging current\" registers";
    }
}

void CionModbusRtuConnection::updateE3Block()
{
    // Update register block "e3"
    qCDebug(dcCionModbusRtuConnection()) << "--> Read block \"e3\" registers from:" << 126 << "size:" << 3;
    ModbusRtuReply *reply = m_modbusRtuMaster->readHoldingRegister(m_slaveId, 126, 3);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcCionModbusRtuConnection()) << "<-- Response from reading block \"e3\" register" << 126 << "size:" << 3 << blockValues;
                    values = blockValues.mid(0, 1);
                    quint16 receivedCurrentChargingCurrentE3 = ModbusDataUtils::convertToUInt16(values);
                    if (m_currentChargingCurrentE3 != receivedCurrentChargingCurrentE3) {
                        m_currentChargingCurrentE3 = receivedCurrentChargingCurrentE3;
                        emit currentChargingCurrentE3Changed(m_currentChargingCurrentE3);
                    }

                    values = blockValues.mid(1, 1);
                    quint16 receivedMaxChargingCurrentE3 = ModbusDataUtils::convertToUInt16(values);
                    if (m_maxChargingCurrentE3 != receivedMaxChargingCurrentE3) {
                        m_maxChargingCurrentE3 = receivedMaxChargingCurrentE3;
                        emit maxChargingCurrentE3Changed(m_maxChargingCurrentE3);
                    }

                    values = blockValues.mid(2, 1);
                    quint16 receivedMaxChargingCurrentCableE3 = ModbusDataUtils::convertToUInt16(values);
                    if (m_maxChargingCurrentCableE3 != receivedMaxChargingCurrentCableE3) {
                        m_maxChargingCurrentCableE3 = receivedMaxChargingCurrentCableE3;
                        emit maxChargingCurrentCableE3Changed(m_maxChargingCurrentCableE3);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcCionModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"e3\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcCionModbusRtuConnection()) << "Error occurred while reading block \"e3\" registers";
    }
}

void CionModbusRtuConnection::updateDurationsBlock()
{
    // Update register block "durations"
    qCDebug(dcCionModbusRtuConnection()) << "--> Read block \"durations\" registers from:" << 151 << "size:" << 4;
    ModbusRtuReply *reply = m_modbusRtuMaster->readHoldingRegister(m_slaveId, 151, 4);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> blockValues = reply->result();
                    QVector<quint16> values;
                    qCDebug(dcCionModbusRtuConnection()) << "<-- Response from reading block \"durations\" register" << 151 << "size:" << 4 << blockValues;
                    values = blockValues.mid(0, 2);
                    quint32 receivedChargingDuration = ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_chargingDuration != receivedChargingDuration) {
                        m_chargingDuration = receivedChargingDuration;
                        emit chargingDurationChanged(m_chargingDuration);
                    }

                    values = blockValues.mid(2, 2);
                    quint32 receivedPluggedInDuration = ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_pluggedInDuration != receivedPluggedInDuration) {
                        m_pluggedInDuration = receivedPluggedInDuration;
                        emit pluggedInDurationChanged(m_pluggedInDuration);
                    }

                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcCionModbusRtuConnection()) << "ModbusRtu reply error occurred while updating block \"durations\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcCionModbusRtuConnection()) << "Error occurred while reading block \"durations\" registers";
    }
}

ModbusRtuReply *CionModbusRtuConnection::readChargingEnabled()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 100, 1);
}

ModbusRtuReply *CionModbusRtuConnection::readChargingCurrentSetpoint()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 101, 1);
}

ModbusRtuReply *CionModbusRtuConnection::readStatusBits()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 121, 1);
}

ModbusRtuReply *CionModbusRtuConnection::readCpSignalState()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 139, 1);
}

ModbusRtuReply *CionModbusRtuConnection::readU1Voltage()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 167, 1);
}

ModbusRtuReply *CionModbusRtuConnection::readGridVoltage()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 302, 1);
}

ModbusRtuReply *CionModbusRtuConnection::readMinChargingCurrent()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 507, 1);
}

void CionModbusRtuConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcCionModbusRtuConnection()) << "Initialization finished of CionModbusRtuConnection";
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, CionModbusRtuConnection *cionModbusRtuConnection)
{
    debug.nospace().noquote() << "CionModbusRtuConnection(" << cionModbusRtuConnection->modbusRtuMaster()->modbusUuid().toString() << ", " << cionModbusRtuConnection->modbusRtuMaster()->serialPort() << ", slave ID:" << cionModbusRtuConnection->slaveId() << ")" << "\n";
    debug.nospace().noquote() << "    - Charging enabled:" << cionModbusRtuConnection->chargingEnabled() << "\n";
    debug.nospace().noquote() << "    - Charging current setpoint:" << cionModbusRtuConnection->chargingCurrentSetpoint() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Mode3-State A, B, C, D, U:" << cionModbusRtuConnection->statusBits() << "\n";
    debug.nospace().noquote() << "    - Status bits:" << cionModbusRtuConnection->cpSignalState() << "\n";
    debug.nospace().noquote() << "    - U1 voltage:" << cionModbusRtuConnection->u1Voltage() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Voltage of the power supply grid:" << cionModbusRtuConnection->gridVoltage() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Minimum charging current:" << cionModbusRtuConnection->minChargingCurrent() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Current charging Ampere:" << cionModbusRtuConnection->currentChargingCurrentE3() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Maximum charging current:" << cionModbusRtuConnection->maxChargingCurrentE3() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Maximum charging current of connected cable:" << cionModbusRtuConnection->maxChargingCurrentCableE3() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Charging duration:" << cionModbusRtuConnection->chargingDuration() << " [ms]" << "\n";
    debug.nospace().noquote() << "    - Plugged in duration:" << cionModbusRtuConnection->pluggedInDuration() << " [ms]" << "\n";
    return debug.quote().space();
}
