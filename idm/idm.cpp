/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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
#include "modbushelpers.h"

#include <QTimer>

Idm::Idm(const QHostAddress &address, QObject *parent) :
    QObject(parent),
    m_hostAddress(address)
{
    qCDebug(dcIdm()) << "iDM: Creating iDM connection" << m_hostAddress.toString();
    m_modbusMaster = new ModbusTCPMaster(address, 502, this);
    connect(m_modbusMaster, &ModbusTCPMaster::receivedHoldingRegister, this, &Idm::onReceivedHoldingRegister);
    connect(m_modbusMaster, &ModbusTCPMaster::readRequestError, this, &Idm::onModbusError);
    connect(m_modbusMaster, &ModbusTCPMaster::writeRequestError, this, &Idm::onModbusError);
    connect(m_modbusMaster, &ModbusTCPMaster::writeRequestExecuted, this, &Idm::writeRequestExecuted);
}

Idm::~Idm()
{
    qCDebug(dcIdm()) << "iDM: Deleting iDM connection" << m_hostAddress.toString();
}

bool Idm::connectDevice()
{
    qCDebug(dcIdm()) << "iDM: Connecting device";
    return m_modbusMaster->connectDevice();
}

QHostAddress Idm::address() const
{
    return m_hostAddress;
}

void Idm::getStatus()
{
    // This request starts an update cycle
    m_modbusMaster->readHoldingRegister(Idm::modbusUnitID, Idm::OutsideTemperature, 2);
}

QUuid Idm::setTargetTemperature(double targetTemperature)
{
    QVector<uint16_t> value = ModbusHelpers::convertFloatToRegister(targetTemperature);
    return m_modbusMaster->writeHoldingRegisters(Idm::modbusUnitID, Idm::RegisterList::RoomTemperatureTargetHeatingEcoHKA, value);
}

void Idm::onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value)
{
    Q_UNUSED(slaveAddress);
    /* qCDebug(dcIdm()) << "receivedHoldingRegister " << modbusRegister << "(length: " << value.length() << ")"; */

    switch (modbusRegister) {
    case Idm::OutsideTemperature:
        /* qCDebug(dcIdm()) << "received outside temperature"; */
        if (value.length() == 2) {
            m_idmInfo.outsideTemperature = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::OutsideTemperature - modbusRegister]);
        }
        QTimer::singleShot(200, this, [this] {
            m_modbusMaster->readHoldingRegister(Idm::modbusUnitID, Idm::CurrentFaultNumber, 1);
        });
        break;
    case Idm::CurrentFaultNumber:
        /* qCDebug(dcIdm()) << "current fault number"; */
        if (value.length() == 1) {
            if (value[0] > 0) {
                m_idmInfo.error = true;
            } else {
                m_idmInfo.error = false;
            }
        }
        QTimer::singleShot(200, this, [this] {
            m_modbusMaster->readHoldingRegister(Idm::modbusUnitID, Idm::HeatStorageTemperature, 2);
        });
        break;
    case Idm::HeatStorageTemperature:
        /* qCDebug(dcIdm()) << "received storage temperature"; */
        if (value.length() == 2) {
            m_idmInfo.waterTemperature = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::HeatStorageTemperature - modbusRegister]);
        }
        QTimer::singleShot(200, this, [this] {
            m_modbusMaster->readHoldingRegister(Idm::modbusUnitID, Idm::TargetHotWaterTemperature, 1);
        });
        break;
    case Idm::TargetHotWaterTemperature:
        /* qCDebug(dcIdm()) << "received target hot water temperature"; */
        if (value.length() == 1) {
            /* The hot water target temperature is stored as UCHAR (manual p. 13) */
            m_idmInfo.targetWaterTemperature = (double)value[RegisterList::TargetHotWaterTemperature - modbusRegister];
        }
        QTimer::singleShot(200, this, [this] {
            m_modbusMaster->readHoldingRegister(Idm::modbusUnitID, Idm::HeatPumpOperatingMode, 1);
        });
        break;
    case Idm::HeatPumpOperatingMode:
        /* qCDebug(dcIdm()) << "received heat pump operating mode"; */
        if (value.length() == 1) {
            m_idmInfo.mode = heatPumpOperationModeToString((Idm::IdmHeatPumpMode)value[RegisterList::HeatPumpOperatingMode-modbusRegister]);
        }
        QTimer::singleShot(200, this, [this] {
            m_modbusMaster->readHoldingRegister(Idm::modbusUnitID, Idm::RoomTemperatureHKA, 2);
        });
        break;
    case Idm::RoomTemperatureHKA:
        /* qCDebug(dcIdm()) << "received room temperature hka"; */
        if (value.length() == 2) {
            m_idmInfo.roomTemperature = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::RoomTemperatureHKA - modbusRegister]);
        }
        QTimer::singleShot(200, this, [this] {
            m_modbusMaster->readHoldingRegister(Idm::modbusUnitID, Idm::RoomTemperatureTargetHeatingEcoHKA, 2);
        });
        break;
    case Idm::RoomTemperatureTargetHeatingEcoHKA:
        /* qCDebug(dcIdm()) << "received room temprature hka eco"; */
        if (value.length() == 2) {
            m_idmInfo.targetRoomTemperature = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::RoomTemperatureTargetHeatingEcoHKA - modbusRegister]);
        }
        QTimer::singleShot(200, this, [this] {
            m_modbusMaster->readHoldingRegister(Idm::modbusUnitID, Idm::CurrentPowerConsumptionHeatPump, 2);
        });
        break;
    case Idm::CurrentPowerConsumptionHeatPump:
        /* qCDebug(dcIdm()) << "received power consumption heat pump"; */
        if (value.length() == 2) {
            m_idmInfo.powerConsumptionHeatPump = ModbusHelpers::convertRegisterToFloat(&value[RegisterList::CurrentPowerConsumptionHeatPump - modbusRegister]);
        }

        /* Everything read without an error
         * -> set connected to true */
        m_idmInfo.connected = true;
        emit statusUpdated(m_idmInfo);
        break;
    }
}

void Idm::onModbusError()
{
    qCDebug(dcIdm()) << "iDM: Received modbus error" << m_modbusMaster->errorString();
    m_idmInfo.connected = false;
    emit statusUpdated(m_idmInfo);
}

QString Idm::heatPumpOperationModeToString(IdmHeatPumpMode mode)
{
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
