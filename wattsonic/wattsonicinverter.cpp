// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "wattsonicinverter.h"
#include "extern-plugininfo.h"

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(dcWattsonicModbusRtuConnection)

WattsonicInverter::WattsonicInverter(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, InverterGenerationMode generationMode, QObject *parent)
    : WattsonicModbusRtuConnection{modbusRtuMaster, slaveId, parent},
    m_generationMode{generationMode}
{
    connect(this, &WattsonicInverter::initializationFinished, this, [this](bool success){

        // Check if we can read a gen2 register, otherwise it is a gen3 register
        if (!success) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Initialization finished with errors.";
            emit customInitializationFinished(success);
            return;
        }

        // Init was successfully

        // Verify equipment info
        WattsonicInverter::Info inverterInfo = getInverterInfo(equipmentInfo());
        if (inverterInfo.model.isEmpty() || inverterInfo.type.isEmpty()) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Invalid equipment info" << equipmentInfo();
            emit customInitializationFinished(false);
            return;
        }

        qCDebug(dcWattsonicModbusRtuConnection()) << "Internal init finished successfully" << serialNumber() << inverterInfo.type << inverterInfo.model;
        m_inverterInfo = inverterInfo;

        if (m_generationMode == InverterGenerationModeAuto) {

            // Verify which generation this is by testing one generation dependent register.
            // On Gen3 this request returns with an exception response

            // Note: The DC voltage returns in both generations valid data, don't use that one
            qCDebug(dcWattsonic()) << "Inverter generation autodetection active. Checking the inverter generation...";
            ModbusRtuReply *reply = readSOCGen2();
            if (!reply) {
                qCWarning(dcWattsonicModbusRtuConnection()) << "Unable to test the generation request on the modbus master. Something might be wrong with the connection.";
                emit customInitializationFinished(false);
                return;
            }

            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() != ModbusRtuReply::NoError) {
                    qCWarning(dcWattsonicModbusRtuConnection()) << "Reply finished with error after reading a Gen2 register. Assuming this is a Gen3 inverter.";
                    m_generation = Generation3;
                    emit customInitializationFinished(true);
                    return;
                } else {
                    qCDebug(dcWattsonicModbusRtuConnection()) << "Successfully read gen2 register. Assuming this is a Gen2 inverter.";
                    m_generation = Generation2;
                    emit customInitializationFinished(true);
                }
            });
        } else {
            if (m_generationMode == InverterGenerationModeGen2) {
                m_generation = Generation2;
            } else {
                m_generation = Generation3;
            }

            emit customInitializationFinished(true);
            qCDebug(dcWattsonic()) << "Inverter generation forced to" << m_generation;
        }

    });
}

WattsonicInverter::Info WattsonicInverter::inverterInfo() const
{
    return m_inverterInfo;
}

WattsonicInverter::Generation WattsonicInverter::generation() const
{
    return m_generation;
}

float WattsonicInverter::batteryVoltageDc() const
{
    switch (m_generation) {
    case Generation2:
        return batteryVoltageDcGen2();
    default:
        return batteryVoltageDcGen3();
    }
}

WattsonicModbusRtuConnection::BatteryMode WattsonicInverter::batteryMode() const
{
    switch (m_generation) {
    case Generation2:
        return batteryModeGen2();
    default:
        return batteryModeGen3();
    }
}

qint32 WattsonicInverter::batteryPower() const
{
    switch (m_generation) {
    case Generation2:
        return batteryPowerGen2();
    default:
        return batteryPowerGen3();
    }
}

float WattsonicInverter::totalEnergyInjectedToGrid() const
{
    switch (m_generation) {
    case Generation2:
        return totalEnergyInjectedToGridGen2();
    default:
        return totalEnergyInjectedToGridGen3();
    }
}

float WattsonicInverter::totalEnergyPurchasedFromGrid() const
{
    switch (m_generation) {
    case Generation2:
        return totalEnergyPurchasedFromGridGen2();
    default:
        return totalEnergyPurchasedFromGridGen3();
    }
}

float WattsonicInverter::SOC() const
{
    switch (m_generation) {
    case Generation2:
        return SOCGen2();
    default:
        return SOCGen3();
    }
}

float WattsonicInverter::SOH() const
{
    switch (m_generation) {
    case Generation2:
        return SOHGen2();
    default:
        return SOHGen3();
    }
}

bool WattsonicInverter::update()
{
    if (m_generation == GenerationUnknown) {
        qCDebug(dcWattsonicModbusRtuConnection()) << "Tried to update but we don't know yet if this is a gen2 or gen3 inverter. Waiting for the information.";
        return false;
    }

    if (!m_modbusRtuMaster->connected()) {
        qCDebug(dcWattsonicModbusRtuConnection()) << "Tried to update the registers but the hardware resource seems not to be connected.";
        return false;
    }

    if (!m_pendingUpdateReplies.isEmpty()) {
        qCDebug(dcWattsonicModbusRtuConnection()) << "Tried to update the registers but there are still some update replies pending. Waiting for them to be finished...";
        return true;
    }

    // Hardware resource available but communication not working.
    // Try to read the check reachability register to re-evaluatoe the communication...
    if (m_modbusRtuMaster->connected() && !m_communicationWorking) {
        testReachability();
        return false;
    }

    ModbusRtuReply *reply = nullptr;

    // Read Inverter status
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Inverter status\" register:" << 10105 << "size:" << 1;
    reply = readInverterStatus();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Inverter status\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processInverterStatusRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Inverter status\" registers" << error << reply->errorString();
    });

    // Read Phase A power
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Phase A power\" register:" << 10994 << "size:" << 2;
    reply = readPhaseAPower();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Phase A power\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processPhaseAPowerRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Phase A power\" registers" << error << reply->errorString();
    });

    // Read Phase B power
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Phase B power\" register:" << 10996 << "size:" << 2;
    reply = readPhaseBPower();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Phase B power\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processPhaseBPowerRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Phase B power\" registers" << error << reply->errorString();
    });

    // Read Phase C power
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Phase C power\" register:" << 10998 << "size:" << 2;
    reply = readPhaseCPower();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Phase C power\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processPhaseCPowerRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Phase C power\" registers" << error << reply->errorString();
    });

    // Read Total power on meter
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Total power on meter\" register:" << 11000 << "size:" << 2;
    reply = readTotalPowerOnMeter();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Total power on meter\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processTotalPowerOnMeterRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total power on meter\" registers" << error << reply->errorString();
    });

    // Read Total grid injection energy on meter
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Total grid injection energy on meter\" register:" << 11002 << "size:" << 2;
    reply = readTotalGridInjectionEnergy();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Total grid injection energy on meter\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processTotalGridInjectionEnergyRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total grid injection energy on meter\" registers" << error << reply->errorString();
    });

    // Read Total purchasing energy from grid on meter
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Total purchasing energy from grid on meter\" register:" << 11004 << "size:" << 2;
    reply = readTotalPurchasingEnergyFromGrid();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Total purchasing energy from grid on meter\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processTotalPurchasingEnergyFromGridRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total purchasing energy from grid on meter\" registers" << error << reply->errorString();
    });

    // Read Grid Phase A Voltage
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Grid Phase A Voltage\" register:" << 11009 << "size:" << 1;
    reply = readGridPhaseAVoltage();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Grid Phase A Voltage\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processGridPhaseAVoltageRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Grid Phase A Voltage\" registers" << error << reply->errorString();
    });

    // Read Grid Phase A Current
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Grid Phase A Current\" register:" << 11010 << "size:" << 1;
    reply = readGridPhaseACurrent();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Grid Phase A Current\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processGridPhaseACurrentRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Grid Phase A Current\" registers" << error << reply->errorString();
    });

    // Read Grid Phase B Voltage
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Grid Phase B Voltage\" register:" << 11011 << "size:" << 1;
    reply = readGridPhaseBVoltage();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Grid Phase B Voltage\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processGridPhaseBVoltageRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Grid Phase B Voltage\" registers" << error << reply->errorString();
    });

    // Read Grid Phase B Current
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Grid Phase B Current\" register:" << 11012 << "size:" << 1;
    reply = readGridPhaseBCurrent();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Grid Phase B Current\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processGridPhaseBCurrentRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Grid Phase B Current\" registers" << error << reply->errorString();
    });

    // Read Grid Phase C Voltage
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Grid Phase C Voltage\" register:" << 11013 << "size:" << 1;
    reply = readGridPhaseCVoltage();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Grid Phase C Voltage\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processGridPhaseCVoltageRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Grid Phase C Voltage\" registers" << error << reply->errorString();
    });

    // Read Grid Phase C Current
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Grid Phase C Current\" register:" << 11014 << "size:" << 1;
    reply = readGridPhaseCCurrent();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Grid Phase C Current\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processGridPhaseCCurrentRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Grid Phase C Current\" registers" << error << reply->errorString();
    });

    // Read P_AC
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"P_AC\" register:" << 11016 << "size:" << 2;
    reply = readPAC();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"P_AC\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processPACRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"P_AC\" registers" << error << reply->errorString();
    });

    // Read Total PV Generation from installation
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Total PV Generation from installation\" register:" << 11020 << "size:" << 2;
    reply = readTotalPVGenerationFromInstallation();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Total PV Generation from installation\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processTotalPVGenerationFromInstallationRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total PV Generation from installation\" registers" << error << reply->errorString();
    });

    // Read PV Total Input Power
    qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"PV Total Input Power\" register:" << 11028 << "size:" << 2;
    reply = readPvInputTotalPower();
    if (!reply) {
        qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"PV Total Input Power\" registers";
        return false;
    }

    if (reply->isFinished()) {
        return false; // Broadcast reply returns immediatly
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
        handleModbusError(reply->error());
        m_pendingUpdateReplies.removeAll(reply);

        if (reply->error() != ModbusRtuReply::NoError) {
            verifyUpdateFinished();
            return;
        }

        QVector<quint16> values = reply->result();
        processPvInputTotalPowerRegisterValues(values);
        verifyUpdateFinished();
    });

    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
        qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"PV Total Input Power\" registers" << error << reply->errorString();
    });


    if (m_generation == Generation2) {
        // Read Battery voltage DC (gen2)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Battery voltage DC (gen2)\" register:" << 40254 << "size:" << 1;
        reply = readBatteryVoltageDcGen2();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Battery voltage DC (gen2)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processBatteryVoltageDcGen2RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Battery voltage DC (gen2)\" registers" << error << reply->errorString();
        });

        // Read Battery mode (gen2)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Battery mode (gen2)\" register:" << 30256 << "size:" << 1;
        reply = readBatteryModeGen2();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Battery mode (gen2)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processBatteryModeGen2RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Battery mode (gen2)\" registers" << error << reply->errorString();
        });

        // Read Battery power (gen2)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Battery power (gen2)\" register:" << 30258 << "size:" << 2;
        reply = readBatteryPowerGen2();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Battery power (gen2)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processBatteryPowerGen2RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Battery power (gen2)\" registers" << error << reply->errorString();
        });

        // Read Total energy injected to grid (gen2)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Total energy injected to grid (gen2)\" register:" << 31102 << "size:" << 2;
        reply = readTotalEnergyInjectedToGridGen2();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Total energy injected to grid (gen2)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processTotalEnergyInjectedToGridGen2RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy injected to grid (gen2)\" registers" << error << reply->errorString();
        });

        // Read Total energy purchased from grid (gen2)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Total energy purchased from grid (gen2)\" register:" << 31104 << "size:" << 2;
        reply = readTotalEnergyPurchasedFromGridGen2();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Total energy purchased from grid (gen2)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processTotalEnergyPurchasedFromGridGen2RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy purchased from grid (gen2)\" registers" << error << reply->errorString();
        });

        // Read SOC (gen2)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"SOC (gen2)\" register:" << 33000 << "size:" << 1;
        reply = readSOCGen2();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"SOC (gen2)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processSOCGen2RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"SOC (gen2)\" registers" << error << reply->errorString();
        });

        // Read SOH (gen2)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"SOH (gen2)\" register:" << 33001 << "size:" << 1;
        reply = readSOHGen2();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"SOH (gen2)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processSOHGen2RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"SOH (gen2)\" registers" << error << reply->errorString();
        });

    } else {
        // Read Battery voltage DC (gen3)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Battery voltage DC (gen3)\" register:" << 30254 << "size:" << 1;
        reply = readBatteryVoltageDcGen3();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Battery voltage DC (gen3)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processBatteryVoltageDcGen3RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Battery voltage DC (gen3)\" registers" << error << reply->errorString();
        });

        // Read Battery mode (gen3)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Battery mode (gen3)\" register:" << 30256 << "size:" << 1;
        reply = readBatteryModeGen3();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Battery mode (gen3)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processBatteryModeGen3RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Battery mode (gen3)\" registers" << error << reply->errorString();
        });

        // Read Battery power (gen3)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Battery power (gen3)\" register:" << 30258 << "size:" << 2;
        reply = readBatteryPowerGen3();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Battery power (gen3)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processBatteryPowerGen3RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Battery power (gen3)\" registers" << error << reply->errorString();
        });

        // Read Total energy injected to grid (gen3)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Total energy injected to grid (gen3)\" register:" << 31102 << "size:" << 2;
        reply = readTotalEnergyInjectedToGridGen3();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Total energy injected to grid (gen3)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processTotalEnergyInjectedToGridGen3RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy injected to grid (gen3)\" registers" << error << reply->errorString();
        });

        // Read Total energy purchased from grid (gen3)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"Total energy purchased from grid (gen3)\" register:" << 31104 << "size:" << 2;
        reply = readTotalEnergyPurchasedFromGridGen3();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"Total energy purchased from grid (gen3)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processTotalEnergyPurchasedFromGridGen3RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy purchased from grid (gen3)\" registers" << error << reply->errorString();
        });

        // Read SOC (gen3)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"SOC (gen3)\" register:" << 33000 << "size:" << 1;
        reply = readSOCGen3();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"SOC (gen3)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processSOCGen3RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"SOC (gen3)\" registers" << error << reply->errorString();
        });

        // Read SOH (gen3)
        qCDebug(dcWattsonicModbusRtuConnection()) << "--> Read \"SOH (gen3)\" register:" << 33001 << "size:" << 1;
        reply = readSOHGen3();
        if (!reply) {
            qCWarning(dcWattsonicModbusRtuConnection()) << "Error occurred while reading \"SOH (gen3)\" registers";
            return false;
        }

        if (reply->isFinished()) {
            return false; // Broadcast reply returns immediatly
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
            handleModbusError(reply->error());
            m_pendingUpdateReplies.removeAll(reply);

            if (reply->error() != ModbusRtuReply::NoError) {
                verifyUpdateFinished();
                return;
            }

            QVector<quint16> values = reply->result();
            processSOHGen3RegisterValues(values);
            verifyUpdateFinished();
        });

        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
            qCWarning(dcWattsonicModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"SOH (gen3)\" registers" << error << reply->errorString();
        });
    }

    return true;
}

WattsonicInverter::Info WattsonicInverter::getInverterInfo(quint16 equipmentInfo)
{
    WattsonicInverter::Info inverterType;
    quint8 highByte = (equipmentInfo >> 8) & 0xFF;
    quint8 lowByte = equipmentInfo & 0xFF;

    switch (highByte) {
    case 30:
        inverterType.type = "Three Phase Hybrid";

        switch (lowByte) {
        case 0:
            inverterType.model = "4.0K-25A-3P";
            break;
        case 1:
            inverterType.model = "5.0K-25A-3P";
            break;
        case 2:
            inverterType.model = "6.0K-25A-3P";
            break;
        case 3:
            inverterType.model = "8.0K-25A-3P";
            break;
        case 4:
            inverterType.model = "10K-25A-3P";
            break;
        case 5:
            inverterType.model = "12K-25A-3P";
            break;
        case 6:
            inverterType.model = "10K-40A-3P";
            break;
        case 7:
            inverterType.model = "12K-40A-3P";
            break;
        case 8:
            inverterType.model = "15K-40A-3P";
            break;
        case 9:
            inverterType.model = "20K-40A-3P";
            break;
        default:
            break;
        }

        break;
    case 31:
        inverterType.type = "Single Phase Hybrid";
        switch (lowByte) {
        case 0:
            inverterType.model = "3.0K-30A-1P";
            break;
        case 1:
            inverterType.model = "3.6K-30A-1P";
            break;
        case 2:
            inverterType.model = "4.2K-30A-1P";
            break;
        case 3:
            inverterType.model = "4.6K-30A-1P";
            break;
        case 4:
            inverterType.model = "5.0K-30A-1P";
            break;
        case 5:
            inverterType.model = "6.0K-30A-1P";
            break;
        case 6:
            inverterType.model = "7.0K-30A-1P";
            break;
        case 7:
            inverterType.model = "8.0K-30A-1P";
            break;
        case 8:
            inverterType.model = "3.0K-30A-1P-S";
            break;
        case 9:
            inverterType.model = "3.6K-30A-1P-S";
            break;
        default:
            break;
        }

        break;
    case 32:
        inverterType.type = "Three Phase Hybrid (25-50K)";

        switch (lowByte) {
        case 0:
            inverterType.model = "25K-100A-3P";
            break;
        case 1:
            inverterType.model = "30K-100A-3P";
            break;
        case 2:
            inverterType.model = "36K-100A-3P";
            break;
        case 3:
            inverterType.model = "40K-100A-3P";
            break;
        case 4:
            inverterType.model = "50K-100A-3P";
            break;
        default:
            break;
        }

        break;
    default:
        break;
    }

    return inverterType;
}

