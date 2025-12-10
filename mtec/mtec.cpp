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

#include "mtec.h"
#include "extern-plugininfo.h"

MTec::MTec(const QHostAddress &address, QObject *parent) :
    QObject(parent),
    m_hostAddress(address)
{
    m_modbusMaster = new ModbusTcpMaster(address, 502, this);
    m_modbusMaster->setTimeout(2000);
    m_modbusMaster->setNumberOfRetries(5);

    qCDebug(dcMTec()) << "Created ModbusTcpMaster for" << address.toString();
    connect(m_modbusMaster, &ModbusTcpMaster::connectionStateChanged, this, &MTec::connectedChanged);
    connect(m_modbusMaster, &ModbusTcpMaster::receivedHoldingRegister, this, &MTec::onReceivedHoldingRegister);
    connect(m_modbusMaster, &ModbusTcpMaster::readRequestError, this, &MTec::onModbusError);
    connect(m_modbusMaster, &ModbusTcpMaster::writeRequestError, this, &MTec::onModbusError);
}

MTec::~MTec()
{
    m_modbusMaster->disconnectDevice();
}

ModbusTcpMaster *MTec::modbusTcpMaster() const
{
    return m_modbusMaster;
}

QHostAddress MTec::hostAddress() const
{
    return m_hostAddress;
}

bool MTec::connected() const
{
    return m_modbusMaster->connected();
}

QModbusReply *MTec::setTargetRoomTemperature(double targetRoomTemperature)
{
    QVector<quint16> values;
    values << static_cast<quint16>(qRound(targetRoomTemperature * 10));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, RegisterTargetRoomTemperature, values.length());
    request.setValues(values);
    QModbusReply *reply = m_modbusMaster->sendWriteRequest(request, MTec::ModbusUnitID);
    return reply;
}

QModbusReply *MTec::setSmartHomeEnergy(quint16 smartHomeEnergy)
{
    QVector<quint16> values;
    values << smartHomeEnergy;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, RegisterActualExcessEnergySmartHome, values.length());
    request.setValues(values);
    QModbusReply *reply = m_modbusMaster->sendWriteRequest(request, MTec::ModbusUnitID);
    return reply;
}

bool MTec::connectDevice()
{
    return m_modbusMaster->connectDevice();
}

void MTec::disconnectDevice()
{
    m_modbusMaster->disconnectDevice();
}

void MTec::updateValues()
{
    if (!m_modbusMaster->connected()) {
        return;
    }

    m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterRoomTemperature, 1);
}

void MTec::onModbusError()
{
    qCWarning(dcMTec()) << "Modbus error occured" << m_modbusMaster->errorString();
}

void MTec::onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value)
{
    Q_UNUSED(slaveAddress);

    switch (modbusRegister) {
    case RegisterRoomTemperature:
        if (value.length() == 1) {
            m_roomTemperature = value[0] / 10.0;
            emit roomTemperatureChanged(m_roomTemperature);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterTargetRoomTemperature, 1);
        break;
    case RegisterTargetRoomTemperature:
        if (value.length() == 1) {
            m_targetRoomTemperature = value[0] / 10.0;
            emit targetRoomTemperatureChanged(m_targetRoomTemperature);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterHotWaterTankTemperature, 1);
        break;
    case RegisterHotWaterTankTemperature:
        if (value.length() == 1) {
            m_waterTankTopTemperature = value[0] / 10.0;
            emit waterTankTopTemperatureChanged(m_waterTankTopTemperature);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterBufferTankTemperature, 1);
        break;
    case RegisterBufferTankTemperature:
        if (value.length() == 1) {
            m_bufferTankTemperature = value[0] / 10.0;
            emit bufferTankTemperatureChanged(m_bufferTankTemperature);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterTotalAccumulatedHeatingEnergy, 1);
        break;
    case RegisterTotalAccumulatedHeatingEnergy:
        if (value.length() == 1) {
            m_totalAccumulatedElectricalEnergy = value[0];
            emit totalAccumulatedElectricalEnergyChanged(m_totalAccumulatedElectricalEnergy);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterTotalAccumulatedElectricalEnergy, 1);
        break;
    case RegisterTotalAccumulatedElectricalEnergy:
        if (value.length() == 1) {
            m_totalAccumulatedElectricalEnergy = value[0];
            emit totalAccumulatedElectricalEnergyChanged(m_totalAccumulatedElectricalEnergy);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterHeatpumpState, 1);
        break;
    case RegisterHeatpumpState:
        if (value.length() == 1) {
            m_heatPumpState = static_cast<HeatpumpState>(value[0]);
            emit heatPumpStateChanged(m_heatPumpState);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterHeatMeterPowerConsumption, 1);
        break;
    case RegisterHeatMeterPowerConsumption:
        if (value.length() == 1) {
            m_heatMeterPowerConsumption = value[0];
            emit heatMeterPowerConsumptionChanged(m_heatMeterPowerConsumption);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterEnergyMeterPowerConsumption, 1);
        break;
    case RegisterEnergyMeterPowerConsumption:
        if (value.length() == 1) {
            m_energyMeterPowerConsumption = value[0];
            emit energyMeterPowerConsumptionChanged(m_energyMeterPowerConsumption);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterActualExcessEnergySmartHome, 1);
        break;
    case RegisterActualExcessEnergySmartHome:
        if (value.length() == 1) {
            m_actualExcessEnergySmartHome = value[0];
            emit actualExcessEnergySmartHomeChanged(m_actualExcessEnergySmartHome);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterActualExcessEnergySmartHomeElectricityMeter, 1);
        break;
    case RegisterActualExcessEnergySmartHomeElectricityMeter:
        if (value.length() == 1) {
            m_actualExcessEnergySmartHomeElectricityMeter = value[0];
            emit actualExcessEnergySmartHomeChanged(m_actualExcessEnergySmartHome);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, RegisterActualOutdoorTemperature, 1);
        break;
    case RegisterActualOutdoorTemperature:
        if (value.length() == 1) {
            m_actualOutdoorTemperature = value[0] / 10.0;
            emit actualOutdoorTemperatureChanged(m_actualOutdoorTemperature);
        }

        // TODO: set initialized

        break;
    }
}

