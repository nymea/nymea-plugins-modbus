/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
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

#include "idm.h"
#include "extern-plugininfo.h"
#include "../modbus/modbushelpers.h"

#include <cstring>

Idm::Idm(const QHostAddress &address, QObject *parent) :
    QObject(parent),
    m_hostAddress(address)
{
    m_modbusMaster = new ModbusTCPMaster(address, 502, this);

    if (m_modbusMaster) {
        qCDebug(dcIdm()) << "created ModbusTCPMaster";
        m_modbusMaster->connectDevice();

        connect(m_modbusMaster, &ModbusTCPMaster::receivedHoldingRegister, this, &Idm::onReceivedHoldingRegister);
        connect(m_modbusMaster, &ModbusTCPMaster::readRequestError, this, &Idm::onModbusError);
        connect(m_modbusMaster, &ModbusTCPMaster::writeRequestError, this, &Idm::onModbusError);
    }
}

Idm::~Idm()
{
    if (m_modbusMaster) {
        delete m_modbusMaster;
    }
}

void Idm::onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value)
{
    Q_UNUSED(slaveAddress);
    /* qCDebug(dcIdm()) << "receivedHoldingRegister " << modbusRegister << "(length: " << value.length() << ")"; */

    /* Introducing a delay here for testing.
     * Purposely set here, so one delay works for all branches
     * of the following switch statement. In fact, the delay
     * is used before evaluating what was just read, which
     * does not seem to make sense, but it also acts before
     * the next read command is sent. */
    QThread::msleep(200);

    switch (modbusRegister) {
    case Idm::OutsideTemperature:
        /* qCDebug(dcIdm()) << "received outside temperature"; */
        if (value.length() == 2) {
            m_info->m_outsideTemperature = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::OutsideTemperature - modbusRegister]);
        }
        m_modbusMaster->readHoldingRegister(Idm::ModbusUnitID, Idm::CurrentFaultNumber, 1);
        break;
    case Idm::CurrentFaultNumber:
        /* qCDebug(dcIdm()) << "current fault number"; */
        if (value.length() == 1) {
            if (value[0] > 0) {
                m_info->m_error = true;
            } else {
                m_info->m_error = false;
            }
        }
        m_modbusMaster->readHoldingRegister(Idm::ModbusUnitID, Idm::HeatStorageTemperature, 2);
        break;
    case Idm::HeatStorageTemperature:
        /* qCDebug(dcIdm()) << "received storage temperature"; */
        if (value.length() == 2) {
            m_info->m_waterTemperature = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::HeatStorageTemperature - modbusRegister]);
        }
        m_modbusMaster->readHoldingRegister(Idm::ModbusUnitID, Idm::TargetHotWaterTemperature, 1);
        break;
    case Idm::TargetHotWaterTemperature:
        /* qCDebug(dcIdm()) << "received target hot water temperature"; */
        if (value.length() == 1) {
            /* The hot water target temperature is stored as UCHAR (manual p. 13) */
            m_info->m_targetWaterTemperature = (double)value[RegisterList::TargetHotWaterTemperature - modbusRegister];
        }
        m_modbusMaster->readHoldingRegister(Idm::ModbusUnitID, Idm::HeatPumpOperatingMode, 1);
        break;
    case Idm::HeatPumpOperatingMode:
        /* qCDebug(dcIdm()) << "received heat pump operating mode"; */
        if (value.length() == 1) {
            m_info->m_mode = heatPumpOperationModeToString((Idm::IdmHeatPumpMode)value[RegisterList::HeatPumpOperatingMode-modbusRegister]);
        }
        m_modbusMaster->readHoldingRegister(Idm::ModbusUnitID, Idm::RoomTemperatureHKA, 2);
        break;
    case Idm::RoomTemperatureHKA:
        /* qCDebug(dcIdm()) << "received room temperature hka"; */
        if (value.length() == 2) {
            m_info->m_roomTemperature = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::RoomTemperatureHKA - modbusRegister]);
        }
        m_modbusMaster->readHoldingRegister(Idm::ModbusUnitID, Idm::RoomTemperatureTargetHeatingEcoHKA, 2);
        break;
    case Idm::RoomTemperatureTargetHeatingEcoHKA:
        /* qCDebug(dcIdm()) << "received room temprature hka eco"; */
        if (value.length() == 2) {
            m_info->m_targetRoomTemperature = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::RoomTemperatureTargetHeatingEcoHKA - modbusRegister]);
        }
        m_modbusMaster->readHoldingRegister(Idm::ModbusUnitID, Idm::CurrentPowerConsumptionHeatPump, 2);
        break;
    case Idm::CurrentPowerConsumptionHeatPump:
        /* qCDebug(dcIdm()) << "received power consumption heat pump"; */
        if (value.length() == 2) {
            m_info->m_powerConsumptionHeatPump = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::CurrentPowerConsumptionHeatPump - modbusRegister]);
        }

        /* Everything read without an error
         * -> set connected to true */
        m_info->m_connected = true;
        emit statusUpdated(m_info);
        break;
    }
}

void Idm::onModbusError()
{
    qCDebug(dcIdm()) << "Received modbus error";

    if (m_info != nullptr) {
        m_info->m_connected = false;
        emit statusUpdated(m_info);
    }
}

void Idm::onRequestStatus()
{
    m_info = new IdmInfo;

    QUuid reqId{};
    reqId = m_modbusMaster->readHoldingRegister(Idm::ModbusUnitID, Idm::OutsideTemperature, 2);

    /* qCDebug(dcIdm()) << "Request id: " << reqId; */
}

QString Idm::systemOperationModeToString(IdmSysMode mode) {
    QString result{};

    /* Operation modes according to table of manual p. 13 */
    switch (mode) {
    case IdmSysModeStandby:
        result = "Standby";
        break;
    case IdmSysModeAutomatic:
        result = "Automatik";
        break;
    case IdmSysModeAway:
        result = "Abwesend";
        break;
    case IdmSysModeOnlyHotwater:
        result = "Nur Warmwasser";
        break;
    case IdmSysModeOnlyRoomHeating:
        result = "Nur Heizung/Kühlung";
        break;
    }

    return result;
}

QString Idm::heatPumpOperationModeToString(IdmHeatPumpMode mode) {
    QString result{};

    /* Operation modes according to table of manual p. 14 */
    switch (mode) {
    case IdmHeatPumpModeOff:
        result = "Off";
        break;
    case IdmHeatPumpModeHeating:
        result = "Heating";
        break;
    case IdmHeatPumpModeCooling:
        result = "Cooling";
        break;
    case IdmHeatPumpModeHotWater:
        result = "Hot water";
        break;
    case IdmHeatPumpModeDefrost:
        result = "Defrost";
        break;
    }

    return result;
}

