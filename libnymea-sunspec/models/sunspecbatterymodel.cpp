/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
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

#include "sunspecbatterymodel.h"
#include "sunspecconnection.h"

SunSpecBatteryModel::SunSpecBatteryModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 802, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecBatteryModel::~SunSpecBatteryModel()
{

}

QString SunSpecBatteryModel::name() const
{
    return "battery";
}

QString SunSpecBatteryModel::description() const
{
    return QString();
}

QString SunSpecBatteryModel::label() const
{
    return "Battery Base Model";
}

float SunSpecBatteryModel::nameplateChargeCapacity() const
{
    return m_nameplateChargeCapacity;
}
float SunSpecBatteryModel::nameplateEnergyCapacity() const
{
    return m_nameplateEnergyCapacity;
}
float SunSpecBatteryModel::nameplateMaxChargeRate() const
{
    return m_nameplateMaxChargeRate;
}
float SunSpecBatteryModel::nameplateMaxDischargeRate() const
{
    return m_nameplateMaxDischargeRate;
}
float SunSpecBatteryModel::selfDischargeRate() const
{
    return m_selfDischargeRate;
}
float SunSpecBatteryModel::nameplateMaxSoC() const
{
    return m_nameplateMaxSoC;
}
float SunSpecBatteryModel::nameplateMinSoC() const
{
    return m_nameplateMinSoC;
}
float SunSpecBatteryModel::maxReservePercent() const
{
    return m_maxReservePercent;
}

QModbusReply *SunSpecBatteryModel::setMaxReservePercent(float maxReservePercent)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("SocRsvMax");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(maxReservePercent, m_soC_SF, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecBatteryModel::minReservePercent() const
{
    return m_minReservePercent;
}

QModbusReply *SunSpecBatteryModel::setMinReservePercent(float minReservePercent)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("SoCRsvMin");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(minReservePercent, m_soC_SF, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecBatteryModel::stateOfCharge() const
{
    return m_stateOfCharge;
}
float SunSpecBatteryModel::depthOfDischarge() const
{
    return m_depthOfDischarge;
}
float SunSpecBatteryModel::stateOfHealth() const
{
    return m_stateOfHealth;
}
quint32 SunSpecBatteryModel::cycleCount() const
{
    return m_cycleCount;
}
SunSpecBatteryModel::Chast SunSpecBatteryModel::chargeStatus() const
{
    return m_chargeStatus;
}
SunSpecBatteryModel::Locremctl SunSpecBatteryModel::controlMode() const
{
    return m_controlMode;
}
quint16 SunSpecBatteryModel::batteryHeartbeat() const
{
    return m_batteryHeartbeat;
}
quint16 SunSpecBatteryModel::controllerHeartbeat() const
{
    return m_controllerHeartbeat;
}

QModbusReply *SunSpecBatteryModel::setControllerHeartbeat(quint16 controllerHeartbeat)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("CtrlHb");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(controllerHeartbeat);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecBatteryModel::alarmReset() const
{
    return m_alarmReset;
}

QModbusReply *SunSpecBatteryModel::setAlarmReset(quint16 alarmReset)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("AlmRst");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(alarmReset);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecBatteryModel::Typ SunSpecBatteryModel::batteryType() const
{
    return m_batteryType;
}
SunSpecBatteryModel::State SunSpecBatteryModel::stateOfTheBatteryBank() const
{
    return m_stateOfTheBatteryBank;
}
quint16 SunSpecBatteryModel::vendorBatteryBankState() const
{
    return m_vendorBatteryBankState;
}
quint32 SunSpecBatteryModel::warrantyDate() const
{
    return m_warrantyDate;
}
SunSpecBatteryModel::Evt1Flags SunSpecBatteryModel::batteryEvent1Bitfield() const
{
    return m_batteryEvent1Bitfield;
}
quint32 SunSpecBatteryModel::batteryEvent2Bitfield() const
{
    return m_batteryEvent2Bitfield;
}
quint32 SunSpecBatteryModel::vendorEventBitfield1() const
{
    return m_vendorEventBitfield1;
}
quint32 SunSpecBatteryModel::vendorEventBitfield2() const
{
    return m_vendorEventBitfield2;
}
float SunSpecBatteryModel::externalBatteryVoltage() const
{
    return m_externalBatteryVoltage;
}
float SunSpecBatteryModel::maxBatteryVoltage() const
{
    return m_maxBatteryVoltage;
}
float SunSpecBatteryModel::minBatteryVoltage() const
{
    return m_minBatteryVoltage;
}
float SunSpecBatteryModel::maxCellVoltage() const
{
    return m_maxCellVoltage;
}
quint16 SunSpecBatteryModel::maxCellVoltageString() const
{
    return m_maxCellVoltageString;
}
quint16 SunSpecBatteryModel::maxCellVoltageModule() const
{
    return m_maxCellVoltageModule;
}
float SunSpecBatteryModel::minCellVoltage() const
{
    return m_minCellVoltage;
}
quint16 SunSpecBatteryModel::minCellVoltageString() const
{
    return m_minCellVoltageString;
}
quint16 SunSpecBatteryModel::minCellVoltageModule() const
{
    return m_minCellVoltageModule;
}
float SunSpecBatteryModel::averageCellVoltage() const
{
    return m_averageCellVoltage;
}
float SunSpecBatteryModel::totalDcCurrent() const
{
    return m_totalDcCurrent;
}
float SunSpecBatteryModel::maxChargeCurrent() const
{
    return m_maxChargeCurrent;
}
float SunSpecBatteryModel::maxDischargeCurrent() const
{
    return m_maxDischargeCurrent;
}
float SunSpecBatteryModel::totalPower() const
{
    return m_totalPower;
}
SunSpecBatteryModel::Reqinvstate SunSpecBatteryModel::inverterStateRequest() const
{
    return m_inverterStateRequest;
}
float SunSpecBatteryModel::batteryPowerRequest() const
{
    return m_batteryPowerRequest;
}
SunSpecBatteryModel::Setop SunSpecBatteryModel::setOperation() const
{
    return m_setOperation;
}

QModbusReply *SunSpecBatteryModel::setSetOperation(Setop setOperation)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("SetOp");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(setOperation));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecBatteryModel::Setinvstate SunSpecBatteryModel::setInverterState() const
{
    return m_setInverterState;
}

QModbusReply *SunSpecBatteryModel::setSetInverterState(Setinvstate setInverterState)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("SetInvState");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(setInverterState));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
qint16 SunSpecBatteryModel::aHRtg_SF() const
{
    return m_aHRtg_SF;
}
qint16 SunSpecBatteryModel::wHRtg_SF() const
{
    return m_wHRtg_SF;
}
qint16 SunSpecBatteryModel::wChaDisChaMax_SF() const
{
    return m_wChaDisChaMax_SF;
}
qint16 SunSpecBatteryModel::disChaRte_SF() const
{
    return m_disChaRte_SF;
}
qint16 SunSpecBatteryModel::soC_SF() const
{
    return m_soC_SF;
}
qint16 SunSpecBatteryModel::doD_SF() const
{
    return m_doD_SF;
}
qint16 SunSpecBatteryModel::soH_SF() const
{
    return m_soH_SF;
}
qint16 SunSpecBatteryModel::v_SF() const
{
    return m_v_SF;
}
qint16 SunSpecBatteryModel::cellV_SF() const
{
    return m_cellV_SF;
}
qint16 SunSpecBatteryModel::a_SF() const
{
    return m_a_SF;
}
qint16 SunSpecBatteryModel::aMax_SF() const
{
    return m_aMax_SF;
}
qint16 SunSpecBatteryModel::w_SF() const
{
    return m_w_SF;
}
void SunSpecBatteryModel::initDataPoints()
{
    SunSpecDataPoint modelIdDataPoint;
    modelIdDataPoint.setName("ID");
    modelIdDataPoint.setLabel("Model ID");
    modelIdDataPoint.setDescription("Model identifier");
    modelIdDataPoint.setMandatory(true);
    modelIdDataPoint.setSize(1);
    modelIdDataPoint.setAddressOffset(0);
    modelIdDataPoint.setSunSpecDataType("uint16");
    modelIdDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setSunSpecDataType("uint16");
    modelLengthDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

    SunSpecDataPoint nameplateChargeCapacityDataPoint;
    nameplateChargeCapacityDataPoint.setName("AHRtg");
    nameplateChargeCapacityDataPoint.setLabel("Nameplate Charge Capacity");
    nameplateChargeCapacityDataPoint.setDescription("Nameplate charge capacity in amp-hours.");
    nameplateChargeCapacityDataPoint.setUnits("Ah");
    nameplateChargeCapacityDataPoint.setMandatory(true);
    nameplateChargeCapacityDataPoint.setSize(1);
    nameplateChargeCapacityDataPoint.setAddressOffset(2);
    nameplateChargeCapacityDataPoint.setBlockOffset(0);
    nameplateChargeCapacityDataPoint.setScaleFactorName("AHRtg_SF");
    nameplateChargeCapacityDataPoint.setSunSpecDataType("uint16");
    nameplateChargeCapacityDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nameplateChargeCapacityDataPoint.name(), nameplateChargeCapacityDataPoint);

    SunSpecDataPoint nameplateEnergyCapacityDataPoint;
    nameplateEnergyCapacityDataPoint.setName("WHRtg");
    nameplateEnergyCapacityDataPoint.setLabel("Nameplate Energy Capacity");
    nameplateEnergyCapacityDataPoint.setDescription("Nameplate energy capacity in DC watt-hours.");
    nameplateEnergyCapacityDataPoint.setUnits("Wh");
    nameplateEnergyCapacityDataPoint.setMandatory(true);
    nameplateEnergyCapacityDataPoint.setSize(1);
    nameplateEnergyCapacityDataPoint.setAddressOffset(3);
    nameplateEnergyCapacityDataPoint.setBlockOffset(1);
    nameplateEnergyCapacityDataPoint.setScaleFactorName("WHRtg_SF");
    nameplateEnergyCapacityDataPoint.setSunSpecDataType("uint16");
    nameplateEnergyCapacityDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nameplateEnergyCapacityDataPoint.name(), nameplateEnergyCapacityDataPoint);

    SunSpecDataPoint nameplateMaxChargeRateDataPoint;
    nameplateMaxChargeRateDataPoint.setName("WChaRteMax");
    nameplateMaxChargeRateDataPoint.setLabel("Nameplate Max Charge Rate");
    nameplateMaxChargeRateDataPoint.setDescription("Maximum rate of energy transfer into the storage device in DC watts.");
    nameplateMaxChargeRateDataPoint.setUnits("W");
    nameplateMaxChargeRateDataPoint.setMandatory(true);
    nameplateMaxChargeRateDataPoint.setSize(1);
    nameplateMaxChargeRateDataPoint.setAddressOffset(4);
    nameplateMaxChargeRateDataPoint.setBlockOffset(2);
    nameplateMaxChargeRateDataPoint.setScaleFactorName("WChaDisChaMax_SF");
    nameplateMaxChargeRateDataPoint.setSunSpecDataType("uint16");
    nameplateMaxChargeRateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nameplateMaxChargeRateDataPoint.name(), nameplateMaxChargeRateDataPoint);

    SunSpecDataPoint nameplateMaxDischargeRateDataPoint;
    nameplateMaxDischargeRateDataPoint.setName("WDisChaRteMax");
    nameplateMaxDischargeRateDataPoint.setLabel("Nameplate Max Discharge Rate");
    nameplateMaxDischargeRateDataPoint.setDescription("Maximum rate of energy transfer out of the storage device in DC watts.");
    nameplateMaxDischargeRateDataPoint.setUnits("W");
    nameplateMaxDischargeRateDataPoint.setMandatory(true);
    nameplateMaxDischargeRateDataPoint.setSize(1);
    nameplateMaxDischargeRateDataPoint.setAddressOffset(5);
    nameplateMaxDischargeRateDataPoint.setBlockOffset(3);
    nameplateMaxDischargeRateDataPoint.setScaleFactorName("WChaDisChaMax_SF");
    nameplateMaxDischargeRateDataPoint.setSunSpecDataType("uint16");
    nameplateMaxDischargeRateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nameplateMaxDischargeRateDataPoint.name(), nameplateMaxDischargeRateDataPoint);

    SunSpecDataPoint selfDischargeRateDataPoint;
    selfDischargeRateDataPoint.setName("DisChaRte");
    selfDischargeRateDataPoint.setLabel("Self Discharge Rate");
    selfDischargeRateDataPoint.setDescription("Self discharge rate.  Percentage of capacity (WHRtg) discharged per day.");
    selfDischargeRateDataPoint.setUnits("%WHRtg");
    selfDischargeRateDataPoint.setSize(1);
    selfDischargeRateDataPoint.setAddressOffset(6);
    selfDischargeRateDataPoint.setBlockOffset(4);
    selfDischargeRateDataPoint.setScaleFactorName("DisChaRte_SF");
    selfDischargeRateDataPoint.setSunSpecDataType("uint16");
    selfDischargeRateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(selfDischargeRateDataPoint.name(), selfDischargeRateDataPoint);

    SunSpecDataPoint nameplateMaxSoCDataPoint;
    nameplateMaxSoCDataPoint.setName("SoCMax");
    nameplateMaxSoCDataPoint.setLabel("Nameplate Max SoC");
    nameplateMaxSoCDataPoint.setDescription("Manufacturer maximum state of charge, expressed as a percentage.");
    nameplateMaxSoCDataPoint.setUnits("%WHRtg");
    nameplateMaxSoCDataPoint.setSize(1);
    nameplateMaxSoCDataPoint.setAddressOffset(7);
    nameplateMaxSoCDataPoint.setBlockOffset(5);
    nameplateMaxSoCDataPoint.setScaleFactorName("SoC_SF");
    nameplateMaxSoCDataPoint.setSunSpecDataType("uint16");
    nameplateMaxSoCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nameplateMaxSoCDataPoint.name(), nameplateMaxSoCDataPoint);

    SunSpecDataPoint nameplateMinSoCDataPoint;
    nameplateMinSoCDataPoint.setName("SoCMin");
    nameplateMinSoCDataPoint.setLabel("Nameplate Min SoC");
    nameplateMinSoCDataPoint.setDescription("Manufacturer minimum state of charge, expressed as a percentage.");
    nameplateMinSoCDataPoint.setUnits("%WHRtg");
    nameplateMinSoCDataPoint.setSize(1);
    nameplateMinSoCDataPoint.setAddressOffset(8);
    nameplateMinSoCDataPoint.setBlockOffset(6);
    nameplateMinSoCDataPoint.setScaleFactorName("SoC_SF");
    nameplateMinSoCDataPoint.setSunSpecDataType("uint16");
    nameplateMinSoCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nameplateMinSoCDataPoint.name(), nameplateMinSoCDataPoint);

    SunSpecDataPoint maxReservePercentDataPoint;
    maxReservePercentDataPoint.setName("SocRsvMax");
    maxReservePercentDataPoint.setLabel("Max Reserve Percent");
    maxReservePercentDataPoint.setDescription("Setpoint for maximum reserve for storage as a percentage of the nominal maximum storage.");
    maxReservePercentDataPoint.setUnits("%WHRtg");
    maxReservePercentDataPoint.setSize(1);
    maxReservePercentDataPoint.setAddressOffset(9);
    maxReservePercentDataPoint.setBlockOffset(7);
    maxReservePercentDataPoint.setScaleFactorName("SoC_SF");
    maxReservePercentDataPoint.setSunSpecDataType("uint16");
    maxReservePercentDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    maxReservePercentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxReservePercentDataPoint.name(), maxReservePercentDataPoint);

    SunSpecDataPoint minReservePercentDataPoint;
    minReservePercentDataPoint.setName("SoCRsvMin");
    minReservePercentDataPoint.setLabel("Min Reserve Percent");
    minReservePercentDataPoint.setDescription("Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage.");
    minReservePercentDataPoint.setUnits("%WHRtg");
    minReservePercentDataPoint.setSize(1);
    minReservePercentDataPoint.setAddressOffset(10);
    minReservePercentDataPoint.setBlockOffset(8);
    minReservePercentDataPoint.setScaleFactorName("SoC_SF");
    minReservePercentDataPoint.setSunSpecDataType("uint16");
    minReservePercentDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    minReservePercentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minReservePercentDataPoint.name(), minReservePercentDataPoint);

    SunSpecDataPoint stateOfChargeDataPoint;
    stateOfChargeDataPoint.setName("SoC");
    stateOfChargeDataPoint.setLabel("State of Charge");
    stateOfChargeDataPoint.setDescription("State of charge, expressed as a percentage.");
    stateOfChargeDataPoint.setUnits("%WHRtg");
    stateOfChargeDataPoint.setMandatory(true);
    stateOfChargeDataPoint.setSize(1);
    stateOfChargeDataPoint.setAddressOffset(11);
    stateOfChargeDataPoint.setBlockOffset(9);
    stateOfChargeDataPoint.setScaleFactorName("SoC_SF");
    stateOfChargeDataPoint.setSunSpecDataType("uint16");
    stateOfChargeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stateOfChargeDataPoint.name(), stateOfChargeDataPoint);

    SunSpecDataPoint depthOfDischargeDataPoint;
    depthOfDischargeDataPoint.setName("DoD");
    depthOfDischargeDataPoint.setLabel("Depth of Discharge");
    depthOfDischargeDataPoint.setDescription("Depth of discharge, expressed as a percentage.");
    depthOfDischargeDataPoint.setUnits("%");
    depthOfDischargeDataPoint.setSize(1);
    depthOfDischargeDataPoint.setAddressOffset(12);
    depthOfDischargeDataPoint.setBlockOffset(10);
    depthOfDischargeDataPoint.setScaleFactorName("DoD_SF");
    depthOfDischargeDataPoint.setSunSpecDataType("uint16");
    depthOfDischargeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(depthOfDischargeDataPoint.name(), depthOfDischargeDataPoint);

    SunSpecDataPoint stateOfHealthDataPoint;
    stateOfHealthDataPoint.setName("SoH");
    stateOfHealthDataPoint.setLabel("State of Health");
    stateOfHealthDataPoint.setDescription("Percentage of battery life remaining.");
    stateOfHealthDataPoint.setUnits("%");
    stateOfHealthDataPoint.setSize(1);
    stateOfHealthDataPoint.setAddressOffset(13);
    stateOfHealthDataPoint.setBlockOffset(11);
    stateOfHealthDataPoint.setScaleFactorName("SoH_SF");
    stateOfHealthDataPoint.setSunSpecDataType("uint16");
    stateOfHealthDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stateOfHealthDataPoint.name(), stateOfHealthDataPoint);

    SunSpecDataPoint cycleCountDataPoint;
    cycleCountDataPoint.setName("NCyc");
    cycleCountDataPoint.setLabel("Cycle Count");
    cycleCountDataPoint.setDescription("Number of cycles executed in the battery.");
    cycleCountDataPoint.setSize(2);
    cycleCountDataPoint.setAddressOffset(14);
    cycleCountDataPoint.setBlockOffset(12);
    cycleCountDataPoint.setSunSpecDataType("uint32");
    cycleCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cycleCountDataPoint.name(), cycleCountDataPoint);

    SunSpecDataPoint chargeStatusDataPoint;
    chargeStatusDataPoint.setName("ChaSt");
    chargeStatusDataPoint.setLabel("Charge Status");
    chargeStatusDataPoint.setDescription("Charge status of storage device. Enumeration.");
    chargeStatusDataPoint.setSize(1);
    chargeStatusDataPoint.setAddressOffset(16);
    chargeStatusDataPoint.setBlockOffset(14);
    chargeStatusDataPoint.setSunSpecDataType("enum16");
    chargeStatusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(chargeStatusDataPoint.name(), chargeStatusDataPoint);

    SunSpecDataPoint controlModeDataPoint;
    controlModeDataPoint.setName("LocRemCtl");
    controlModeDataPoint.setLabel("Control Mode");
    controlModeDataPoint.setDescription("Battery control mode. Enumeration.");
    controlModeDataPoint.setMandatory(true);
    controlModeDataPoint.setSize(1);
    controlModeDataPoint.setAddressOffset(17);
    controlModeDataPoint.setBlockOffset(15);
    controlModeDataPoint.setSunSpecDataType("enum16");
    controlModeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(controlModeDataPoint.name(), controlModeDataPoint);

    SunSpecDataPoint batteryHeartbeatDataPoint;
    batteryHeartbeatDataPoint.setName("Hb");
    batteryHeartbeatDataPoint.setLabel("Battery Heartbeat");
    batteryHeartbeatDataPoint.setDescription("Value is incremented every second with periodic resets to zero.");
    batteryHeartbeatDataPoint.setSize(1);
    batteryHeartbeatDataPoint.setAddressOffset(18);
    batteryHeartbeatDataPoint.setBlockOffset(16);
    batteryHeartbeatDataPoint.setSunSpecDataType("uint16");
    batteryHeartbeatDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(batteryHeartbeatDataPoint.name(), batteryHeartbeatDataPoint);

    SunSpecDataPoint controllerHeartbeatDataPoint;
    controllerHeartbeatDataPoint.setName("CtrlHb");
    controllerHeartbeatDataPoint.setLabel("Controller Heartbeat");
    controllerHeartbeatDataPoint.setDescription("Value is incremented every second with periodic resets to zero.");
    controllerHeartbeatDataPoint.setSize(1);
    controllerHeartbeatDataPoint.setAddressOffset(19);
    controllerHeartbeatDataPoint.setBlockOffset(17);
    controllerHeartbeatDataPoint.setSunSpecDataType("uint16");
    controllerHeartbeatDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    controllerHeartbeatDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(controllerHeartbeatDataPoint.name(), controllerHeartbeatDataPoint);

    SunSpecDataPoint alarmResetDataPoint;
    alarmResetDataPoint.setName("AlmRst");
    alarmResetDataPoint.setLabel("Alarm Reset");
    alarmResetDataPoint.setDescription("Used to reset any latched alarms.  1 = Reset.");
    alarmResetDataPoint.setMandatory(true);
    alarmResetDataPoint.setSize(1);
    alarmResetDataPoint.setAddressOffset(20);
    alarmResetDataPoint.setBlockOffset(18);
    alarmResetDataPoint.setSunSpecDataType("uint16");
    alarmResetDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    alarmResetDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(alarmResetDataPoint.name(), alarmResetDataPoint);

    SunSpecDataPoint batteryTypeDataPoint;
    batteryTypeDataPoint.setName("Typ");
    batteryTypeDataPoint.setLabel("Battery Type");
    batteryTypeDataPoint.setDescription("Type of battery. Enumeration.");
    batteryTypeDataPoint.setMandatory(true);
    batteryTypeDataPoint.setSize(1);
    batteryTypeDataPoint.setAddressOffset(21);
    batteryTypeDataPoint.setBlockOffset(19);
    batteryTypeDataPoint.setSunSpecDataType("enum16");
    batteryTypeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(batteryTypeDataPoint.name(), batteryTypeDataPoint);

    SunSpecDataPoint stateOfTheBatteryBankDataPoint;
    stateOfTheBatteryBankDataPoint.setName("State");
    stateOfTheBatteryBankDataPoint.setLabel("State of the Battery Bank");
    stateOfTheBatteryBankDataPoint.setDescription("State of the battery bank.  Enumeration.");
    stateOfTheBatteryBankDataPoint.setMandatory(true);
    stateOfTheBatteryBankDataPoint.setSize(1);
    stateOfTheBatteryBankDataPoint.setAddressOffset(22);
    stateOfTheBatteryBankDataPoint.setBlockOffset(20);
    stateOfTheBatteryBankDataPoint.setSunSpecDataType("enum16");
    stateOfTheBatteryBankDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stateOfTheBatteryBankDataPoint.name(), stateOfTheBatteryBankDataPoint);

    SunSpecDataPoint vendorBatteryBankStateDataPoint;
    vendorBatteryBankStateDataPoint.setName("StateVnd");
    vendorBatteryBankStateDataPoint.setLabel("Vendor Battery Bank State");
    vendorBatteryBankStateDataPoint.setDescription("Vendor specific battery bank state.  Enumeration.");
    vendorBatteryBankStateDataPoint.setSize(1);
    vendorBatteryBankStateDataPoint.setAddressOffset(23);
    vendorBatteryBankStateDataPoint.setBlockOffset(21);
    vendorBatteryBankStateDataPoint.setSunSpecDataType("enum16");
    vendorBatteryBankStateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorBatteryBankStateDataPoint.name(), vendorBatteryBankStateDataPoint);

    SunSpecDataPoint warrantyDateDataPoint;
    warrantyDateDataPoint.setName("WarrDt");
    warrantyDateDataPoint.setLabel("Warranty Date");
    warrantyDateDataPoint.setDescription("Date the device warranty expires.");
    warrantyDateDataPoint.setSize(2);
    warrantyDateDataPoint.setAddressOffset(24);
    warrantyDateDataPoint.setBlockOffset(22);
    warrantyDateDataPoint.setSunSpecDataType("uint32");
    warrantyDateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(warrantyDateDataPoint.name(), warrantyDateDataPoint);

    SunSpecDataPoint batteryEvent1BitfieldDataPoint;
    batteryEvent1BitfieldDataPoint.setName("Evt1");
    batteryEvent1BitfieldDataPoint.setLabel("Battery Event 1 Bitfield");
    batteryEvent1BitfieldDataPoint.setDescription("Alarms and warnings.  Bit flags.");
    batteryEvent1BitfieldDataPoint.setMandatory(true);
    batteryEvent1BitfieldDataPoint.setSize(2);
    batteryEvent1BitfieldDataPoint.setAddressOffset(26);
    batteryEvent1BitfieldDataPoint.setBlockOffset(24);
    batteryEvent1BitfieldDataPoint.setSunSpecDataType("bitfield32");
    batteryEvent1BitfieldDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(batteryEvent1BitfieldDataPoint.name(), batteryEvent1BitfieldDataPoint);

    SunSpecDataPoint batteryEvent2BitfieldDataPoint;
    batteryEvent2BitfieldDataPoint.setName("Evt2");
    batteryEvent2BitfieldDataPoint.setLabel("Battery Event 2 Bitfield");
    batteryEvent2BitfieldDataPoint.setDescription("Alarms and warnings.  Bit flags.");
    batteryEvent2BitfieldDataPoint.setMandatory(true);
    batteryEvent2BitfieldDataPoint.setSize(2);
    batteryEvent2BitfieldDataPoint.setAddressOffset(28);
    batteryEvent2BitfieldDataPoint.setBlockOffset(26);
    batteryEvent2BitfieldDataPoint.setSunSpecDataType("bitfield32");
    batteryEvent2BitfieldDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(batteryEvent2BitfieldDataPoint.name(), batteryEvent2BitfieldDataPoint);

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield1DataPoint.setMandatory(true);
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setAddressOffset(30);
    vendorEventBitfield1DataPoint.setBlockOffset(28);
    vendorEventBitfield1DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield2DataPoint.setMandatory(true);
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setAddressOffset(32);
    vendorEventBitfield2DataPoint.setBlockOffset(30);
    vendorEventBitfield2DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

    SunSpecDataPoint externalBatteryVoltageDataPoint;
    externalBatteryVoltageDataPoint.setName("V");
    externalBatteryVoltageDataPoint.setLabel("External Battery Voltage");
    externalBatteryVoltageDataPoint.setDescription("DC Bus Voltage.");
    externalBatteryVoltageDataPoint.setUnits("V");
    externalBatteryVoltageDataPoint.setMandatory(true);
    externalBatteryVoltageDataPoint.setSize(1);
    externalBatteryVoltageDataPoint.setAddressOffset(34);
    externalBatteryVoltageDataPoint.setBlockOffset(32);
    externalBatteryVoltageDataPoint.setScaleFactorName("V_SF");
    externalBatteryVoltageDataPoint.setSunSpecDataType("uint16");
    externalBatteryVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(externalBatteryVoltageDataPoint.name(), externalBatteryVoltageDataPoint);

    SunSpecDataPoint maxBatteryVoltageDataPoint;
    maxBatteryVoltageDataPoint.setName("VMax");
    maxBatteryVoltageDataPoint.setLabel("Max Battery Voltage");
    maxBatteryVoltageDataPoint.setDescription("Instantaneous maximum battery voltage.");
    maxBatteryVoltageDataPoint.setUnits("V");
    maxBatteryVoltageDataPoint.setSize(1);
    maxBatteryVoltageDataPoint.setAddressOffset(35);
    maxBatteryVoltageDataPoint.setBlockOffset(33);
    maxBatteryVoltageDataPoint.setScaleFactorName("V_SF");
    maxBatteryVoltageDataPoint.setSunSpecDataType("uint16");
    maxBatteryVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxBatteryVoltageDataPoint.name(), maxBatteryVoltageDataPoint);

    SunSpecDataPoint minBatteryVoltageDataPoint;
    minBatteryVoltageDataPoint.setName("VMin");
    minBatteryVoltageDataPoint.setLabel("Min Battery Voltage");
    minBatteryVoltageDataPoint.setDescription("Instantaneous minimum battery voltage.");
    minBatteryVoltageDataPoint.setUnits("V");
    minBatteryVoltageDataPoint.setSize(1);
    minBatteryVoltageDataPoint.setAddressOffset(36);
    minBatteryVoltageDataPoint.setBlockOffset(34);
    minBatteryVoltageDataPoint.setScaleFactorName("V_SF");
    minBatteryVoltageDataPoint.setSunSpecDataType("uint16");
    minBatteryVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minBatteryVoltageDataPoint.name(), minBatteryVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("CellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the bank.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setAddressOffset(37);
    maxCellVoltageDataPoint.setBlockOffset(35);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageStringDataPoint;
    maxCellVoltageStringDataPoint.setName("CellVMaxStr");
    maxCellVoltageStringDataPoint.setLabel("Max Cell Voltage String");
    maxCellVoltageStringDataPoint.setDescription("String containing the cell with maximum voltage.");
    maxCellVoltageStringDataPoint.setSize(1);
    maxCellVoltageStringDataPoint.setAddressOffset(38);
    maxCellVoltageStringDataPoint.setBlockOffset(36);
    maxCellVoltageStringDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageStringDataPoint.name(), maxCellVoltageStringDataPoint);

    SunSpecDataPoint maxCellVoltageModuleDataPoint;
    maxCellVoltageModuleDataPoint.setName("CellVMaxMod");
    maxCellVoltageModuleDataPoint.setLabel("Max Cell Voltage Module");
    maxCellVoltageModuleDataPoint.setDescription("Module containing the cell with maximum voltage.");
    maxCellVoltageModuleDataPoint.setSize(1);
    maxCellVoltageModuleDataPoint.setAddressOffset(39);
    maxCellVoltageModuleDataPoint.setBlockOffset(37);
    maxCellVoltageModuleDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageModuleDataPoint.name(), maxCellVoltageModuleDataPoint);

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("CellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the bank.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setAddressOffset(40);
    minCellVoltageDataPoint.setBlockOffset(38);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setSunSpecDataType("uint16");
    minCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageStringDataPoint;
    minCellVoltageStringDataPoint.setName("CellVMinStr");
    minCellVoltageStringDataPoint.setLabel("Min Cell Voltage String");
    minCellVoltageStringDataPoint.setDescription("String containing the cell with minimum voltage.");
    minCellVoltageStringDataPoint.setSize(1);
    minCellVoltageStringDataPoint.setAddressOffset(41);
    minCellVoltageStringDataPoint.setBlockOffset(39);
    minCellVoltageStringDataPoint.setSunSpecDataType("uint16");
    minCellVoltageStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageStringDataPoint.name(), minCellVoltageStringDataPoint);

    SunSpecDataPoint minCellVoltageModuleDataPoint;
    minCellVoltageModuleDataPoint.setName("CellVMinMod");
    minCellVoltageModuleDataPoint.setLabel("Min Cell Voltage Module");
    minCellVoltageModuleDataPoint.setDescription("Module containing the cell with minimum voltage.");
    minCellVoltageModuleDataPoint.setSize(1);
    minCellVoltageModuleDataPoint.setAddressOffset(42);
    minCellVoltageModuleDataPoint.setBlockOffset(40);
    minCellVoltageModuleDataPoint.setSunSpecDataType("uint16");
    minCellVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageModuleDataPoint.name(), minCellVoltageModuleDataPoint);

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("CellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average cell voltage for all cells in the bank.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setAddressOffset(43);
    averageCellVoltageDataPoint.setBlockOffset(41);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setSunSpecDataType("uint16");
    averageCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageCellVoltageDataPoint.name(), averageCellVoltageDataPoint);

    SunSpecDataPoint totalDcCurrentDataPoint;
    totalDcCurrentDataPoint.setName("A");
    totalDcCurrentDataPoint.setLabel("Total DC Current");
    totalDcCurrentDataPoint.setDescription("Total DC current flowing to/from the battery bank.");
    totalDcCurrentDataPoint.setUnits("A");
    totalDcCurrentDataPoint.setMandatory(true);
    totalDcCurrentDataPoint.setSize(1);
    totalDcCurrentDataPoint.setAddressOffset(44);
    totalDcCurrentDataPoint.setBlockOffset(42);
    totalDcCurrentDataPoint.setScaleFactorName("A_SF");
    totalDcCurrentDataPoint.setSunSpecDataType("int16");
    totalDcCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalDcCurrentDataPoint.name(), totalDcCurrentDataPoint);

    SunSpecDataPoint maxChargeCurrentDataPoint;
    maxChargeCurrentDataPoint.setName("AChaMax");
    maxChargeCurrentDataPoint.setLabel("Max Charge Current");
    maxChargeCurrentDataPoint.setDescription("Instantaneous maximum DC charge current.");
    maxChargeCurrentDataPoint.setUnits("A");
    maxChargeCurrentDataPoint.setSize(1);
    maxChargeCurrentDataPoint.setAddressOffset(45);
    maxChargeCurrentDataPoint.setBlockOffset(43);
    maxChargeCurrentDataPoint.setScaleFactorName("AMax_SF");
    maxChargeCurrentDataPoint.setSunSpecDataType("uint16");
    maxChargeCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxChargeCurrentDataPoint.name(), maxChargeCurrentDataPoint);

    SunSpecDataPoint maxDischargeCurrentDataPoint;
    maxDischargeCurrentDataPoint.setName("ADisChaMax");
    maxDischargeCurrentDataPoint.setLabel("Max Discharge Current");
    maxDischargeCurrentDataPoint.setDescription("Instantaneous maximum DC discharge current.");
    maxDischargeCurrentDataPoint.setUnits("A");
    maxDischargeCurrentDataPoint.setSize(1);
    maxDischargeCurrentDataPoint.setAddressOffset(46);
    maxDischargeCurrentDataPoint.setBlockOffset(44);
    maxDischargeCurrentDataPoint.setScaleFactorName("AMax_SF");
    maxDischargeCurrentDataPoint.setSunSpecDataType("uint16");
    maxDischargeCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxDischargeCurrentDataPoint.name(), maxDischargeCurrentDataPoint);

    SunSpecDataPoint totalPowerDataPoint;
    totalPowerDataPoint.setName("W");
    totalPowerDataPoint.setLabel("Total Power");
    totalPowerDataPoint.setDescription("Total power flowing to/from the battery bank.");
    totalPowerDataPoint.setUnits("W");
    totalPowerDataPoint.setMandatory(true);
    totalPowerDataPoint.setSize(1);
    totalPowerDataPoint.setAddressOffset(47);
    totalPowerDataPoint.setBlockOffset(45);
    totalPowerDataPoint.setScaleFactorName("W_SF");
    totalPowerDataPoint.setSunSpecDataType("int16");
    totalPowerDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalPowerDataPoint.name(), totalPowerDataPoint);

    SunSpecDataPoint inverterStateRequestDataPoint;
    inverterStateRequestDataPoint.setName("ReqInvState");
    inverterStateRequestDataPoint.setLabel("Inverter State Request");
    inverterStateRequestDataPoint.setDescription("Request from battery to start or stop the inverter.  Enumeration.");
    inverterStateRequestDataPoint.setSize(1);
    inverterStateRequestDataPoint.setAddressOffset(48);
    inverterStateRequestDataPoint.setBlockOffset(46);
    inverterStateRequestDataPoint.setSunSpecDataType("enum16");
    inverterStateRequestDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inverterStateRequestDataPoint.name(), inverterStateRequestDataPoint);

    SunSpecDataPoint batteryPowerRequestDataPoint;
    batteryPowerRequestDataPoint.setName("ReqW");
    batteryPowerRequestDataPoint.setLabel("Battery Power Request");
    batteryPowerRequestDataPoint.setDescription("AC Power requested by battery.");
    batteryPowerRequestDataPoint.setUnits("W");
    batteryPowerRequestDataPoint.setSize(1);
    batteryPowerRequestDataPoint.setAddressOffset(49);
    batteryPowerRequestDataPoint.setBlockOffset(47);
    batteryPowerRequestDataPoint.setScaleFactorName("W_SF");
    batteryPowerRequestDataPoint.setSunSpecDataType("int16");
    batteryPowerRequestDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(batteryPowerRequestDataPoint.name(), batteryPowerRequestDataPoint);

    SunSpecDataPoint setOperationDataPoint;
    setOperationDataPoint.setName("SetOp");
    setOperationDataPoint.setLabel("Set Operation");
    setOperationDataPoint.setDescription("Instruct the battery bank to perform an operation such as connecting.  Enumeration.");
    setOperationDataPoint.setMandatory(true);
    setOperationDataPoint.setSize(1);
    setOperationDataPoint.setAddressOffset(50);
    setOperationDataPoint.setBlockOffset(48);
    setOperationDataPoint.setSunSpecDataType("enum16");
    setOperationDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    setOperationDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(setOperationDataPoint.name(), setOperationDataPoint);

    SunSpecDataPoint setInverterStateDataPoint;
    setInverterStateDataPoint.setName("SetInvState");
    setInverterStateDataPoint.setLabel("Set Inverter State");
    setInverterStateDataPoint.setDescription("Set the current state of the inverter.");
    setInverterStateDataPoint.setMandatory(true);
    setInverterStateDataPoint.setSize(1);
    setInverterStateDataPoint.setAddressOffset(51);
    setInverterStateDataPoint.setBlockOffset(49);
    setInverterStateDataPoint.setSunSpecDataType("enum16");
    setInverterStateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    setInverterStateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(setInverterStateDataPoint.name(), setInverterStateDataPoint);

    SunSpecDataPoint aHRtg_SFDataPoint;
    aHRtg_SFDataPoint.setName("AHRtg_SF");
    aHRtg_SFDataPoint.setDescription("Scale factor for charge capacity.");
    aHRtg_SFDataPoint.setMandatory(true);
    aHRtg_SFDataPoint.setSize(1);
    aHRtg_SFDataPoint.setAddressOffset(52);
    aHRtg_SFDataPoint.setBlockOffset(50);
    aHRtg_SFDataPoint.setSunSpecDataType("sunssf");
    aHRtg_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(aHRtg_SFDataPoint.name(), aHRtg_SFDataPoint);

    SunSpecDataPoint wHRtg_SFDataPoint;
    wHRtg_SFDataPoint.setName("WHRtg_SF");
    wHRtg_SFDataPoint.setDescription("Scale factor for energy capacity.");
    wHRtg_SFDataPoint.setMandatory(true);
    wHRtg_SFDataPoint.setSize(1);
    wHRtg_SFDataPoint.setAddressOffset(53);
    wHRtg_SFDataPoint.setBlockOffset(51);
    wHRtg_SFDataPoint.setSunSpecDataType("sunssf");
    wHRtg_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wHRtg_SFDataPoint.name(), wHRtg_SFDataPoint);

    SunSpecDataPoint wChaDisChaMax_SFDataPoint;
    wChaDisChaMax_SFDataPoint.setName("WChaDisChaMax_SF");
    wChaDisChaMax_SFDataPoint.setDescription("Scale factor for maximum charge and discharge rate.");
    wChaDisChaMax_SFDataPoint.setMandatory(true);
    wChaDisChaMax_SFDataPoint.setSize(1);
    wChaDisChaMax_SFDataPoint.setAddressOffset(54);
    wChaDisChaMax_SFDataPoint.setBlockOffset(52);
    wChaDisChaMax_SFDataPoint.setSunSpecDataType("sunssf");
    wChaDisChaMax_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wChaDisChaMax_SFDataPoint.name(), wChaDisChaMax_SFDataPoint);

    SunSpecDataPoint disChaRte_SFDataPoint;
    disChaRte_SFDataPoint.setName("DisChaRte_SF");
    disChaRte_SFDataPoint.setDescription("Scale factor for self discharge rate.");
    disChaRte_SFDataPoint.setSize(1);
    disChaRte_SFDataPoint.setAddressOffset(55);
    disChaRte_SFDataPoint.setBlockOffset(53);
    disChaRte_SFDataPoint.setSunSpecDataType("sunssf");
    disChaRte_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(disChaRte_SFDataPoint.name(), disChaRte_SFDataPoint);

    SunSpecDataPoint soC_SFDataPoint;
    soC_SFDataPoint.setName("SoC_SF");
    soC_SFDataPoint.setDescription("Scale factor for state of charge values.");
    soC_SFDataPoint.setMandatory(true);
    soC_SFDataPoint.setSize(1);
    soC_SFDataPoint.setAddressOffset(56);
    soC_SFDataPoint.setBlockOffset(54);
    soC_SFDataPoint.setSunSpecDataType("sunssf");
    soC_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soC_SFDataPoint.name(), soC_SFDataPoint);

    SunSpecDataPoint doD_SFDataPoint;
    doD_SFDataPoint.setName("DoD_SF");
    doD_SFDataPoint.setDescription("Scale factor for depth of discharge.");
    doD_SFDataPoint.setSize(1);
    doD_SFDataPoint.setAddressOffset(57);
    doD_SFDataPoint.setBlockOffset(55);
    doD_SFDataPoint.setSunSpecDataType("sunssf");
    doD_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(doD_SFDataPoint.name(), doD_SFDataPoint);

    SunSpecDataPoint soH_SFDataPoint;
    soH_SFDataPoint.setName("SoH_SF");
    soH_SFDataPoint.setDescription("Scale factor for state of health.");
    soH_SFDataPoint.setSize(1);
    soH_SFDataPoint.setAddressOffset(58);
    soH_SFDataPoint.setBlockOffset(56);
    soH_SFDataPoint.setSunSpecDataType("sunssf");
    soH_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soH_SFDataPoint.name(), soH_SFDataPoint);

    SunSpecDataPoint v_SFDataPoint;
    v_SFDataPoint.setName("V_SF");
    v_SFDataPoint.setDescription("Scale factor for DC bus voltage.");
    v_SFDataPoint.setMandatory(true);
    v_SFDataPoint.setSize(1);
    v_SFDataPoint.setAddressOffset(59);
    v_SFDataPoint.setBlockOffset(57);
    v_SFDataPoint.setSunSpecDataType("sunssf");
    v_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v_SFDataPoint.name(), v_SFDataPoint);

    SunSpecDataPoint cellV_SFDataPoint;
    cellV_SFDataPoint.setName("CellV_SF");
    cellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    cellV_SFDataPoint.setMandatory(true);
    cellV_SFDataPoint.setSize(1);
    cellV_SFDataPoint.setAddressOffset(60);
    cellV_SFDataPoint.setBlockOffset(58);
    cellV_SFDataPoint.setSunSpecDataType("sunssf");
    cellV_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cellV_SFDataPoint.name(), cellV_SFDataPoint);

    SunSpecDataPoint a_SFDataPoint;
    a_SFDataPoint.setName("A_SF");
    a_SFDataPoint.setDescription("Scale factor for DC current.");
    a_SFDataPoint.setMandatory(true);
    a_SFDataPoint.setSize(1);
    a_SFDataPoint.setAddressOffset(61);
    a_SFDataPoint.setBlockOffset(59);
    a_SFDataPoint.setSunSpecDataType("sunssf");
    a_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(a_SFDataPoint.name(), a_SFDataPoint);

    SunSpecDataPoint aMax_SFDataPoint;
    aMax_SFDataPoint.setName("AMax_SF");
    aMax_SFDataPoint.setDescription("Scale factor for instantaneous DC charge/discharge current.");
    aMax_SFDataPoint.setMandatory(true);
    aMax_SFDataPoint.setSize(1);
    aMax_SFDataPoint.setAddressOffset(62);
    aMax_SFDataPoint.setBlockOffset(60);
    aMax_SFDataPoint.setSunSpecDataType("sunssf");
    aMax_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(aMax_SFDataPoint.name(), aMax_SFDataPoint);

    SunSpecDataPoint w_SFDataPoint;
    w_SFDataPoint.setName("W_SF");
    w_SFDataPoint.setDescription("Scale factor for AC power request.");
    w_SFDataPoint.setSize(1);
    w_SFDataPoint.setAddressOffset(63);
    w_SFDataPoint.setBlockOffset(61);
    w_SFDataPoint.setSunSpecDataType("sunssf");
    w_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w_SFDataPoint.name(), w_SFDataPoint);

}

void SunSpecBatteryModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("AHRtg_SF").isValid())
        m_aHRtg_SF = m_dataPoints.value("AHRtg_SF").toInt16();

    if (m_dataPoints.value("WHRtg_SF").isValid())
        m_wHRtg_SF = m_dataPoints.value("WHRtg_SF").toInt16();

    if (m_dataPoints.value("WChaDisChaMax_SF").isValid())
        m_wChaDisChaMax_SF = m_dataPoints.value("WChaDisChaMax_SF").toInt16();

    if (m_dataPoints.value("DisChaRte_SF").isValid())
        m_disChaRte_SF = m_dataPoints.value("DisChaRte_SF").toInt16();

    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("DoD_SF").isValid())
        m_doD_SF = m_dataPoints.value("DoD_SF").toInt16();

    if (m_dataPoints.value("SoH_SF").isValid())
        m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("A_SF").isValid())
        m_a_SF = m_dataPoints.value("A_SF").toInt16();

    if (m_dataPoints.value("AMax_SF").isValid())
        m_aMax_SF = m_dataPoints.value("AMax_SF").toInt16();

    if (m_dataPoints.value("W_SF").isValid())
        m_w_SF = m_dataPoints.value("W_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("AHRtg").isValid())
        m_nameplateChargeCapacity = m_dataPoints.value("AHRtg").toFloatWithSSF(m_aHRtg_SF);

    if (m_dataPoints.value("WHRtg").isValid())
        m_nameplateEnergyCapacity = m_dataPoints.value("WHRtg").toFloatWithSSF(m_wHRtg_SF);

    if (m_dataPoints.value("WChaRteMax").isValid())
        m_nameplateMaxChargeRate = m_dataPoints.value("WChaRteMax").toFloatWithSSF(m_wChaDisChaMax_SF);

    if (m_dataPoints.value("WDisChaRteMax").isValid())
        m_nameplateMaxDischargeRate = m_dataPoints.value("WDisChaRteMax").toFloatWithSSF(m_wChaDisChaMax_SF);

    if (m_dataPoints.value("DisChaRte").isValid())
        m_selfDischargeRate = m_dataPoints.value("DisChaRte").toFloatWithSSF(m_disChaRte_SF);

    if (m_dataPoints.value("SoCMax").isValid())
        m_nameplateMaxSoC = m_dataPoints.value("SoCMax").toFloatWithSSF(m_soC_SF);

    if (m_dataPoints.value("SoCMin").isValid())
        m_nameplateMinSoC = m_dataPoints.value("SoCMin").toFloatWithSSF(m_soC_SF);

    if (m_dataPoints.value("SocRsvMax").isValid())
        m_maxReservePercent = m_dataPoints.value("SocRsvMax").toFloatWithSSF(m_soC_SF);

    if (m_dataPoints.value("SoCRsvMin").isValid())
        m_minReservePercent = m_dataPoints.value("SoCRsvMin").toFloatWithSSF(m_soC_SF);

    if (m_dataPoints.value("SoC").isValid())
        m_stateOfCharge = m_dataPoints.value("SoC").toFloatWithSSF(m_soC_SF);

    if (m_dataPoints.value("DoD").isValid())
        m_depthOfDischarge = m_dataPoints.value("DoD").toFloatWithSSF(m_doD_SF);

    if (m_dataPoints.value("SoH").isValid())
        m_stateOfHealth = m_dataPoints.value("SoH").toFloatWithSSF(m_soH_SF);

    if (m_dataPoints.value("NCyc").isValid())
        m_cycleCount = m_dataPoints.value("NCyc").toUInt32();

    if (m_dataPoints.value("ChaSt").isValid())
        m_chargeStatus = static_cast<Chast>(m_dataPoints.value("ChaSt").toUInt16());

    if (m_dataPoints.value("LocRemCtl").isValid())
        m_controlMode = static_cast<Locremctl>(m_dataPoints.value("LocRemCtl").toUInt16());

    if (m_dataPoints.value("Hb").isValid())
        m_batteryHeartbeat = m_dataPoints.value("Hb").toUInt16();

    if (m_dataPoints.value("CtrlHb").isValid())
        m_controllerHeartbeat = m_dataPoints.value("CtrlHb").toUInt16();

    if (m_dataPoints.value("AlmRst").isValid())
        m_alarmReset = m_dataPoints.value("AlmRst").toUInt16();

    if (m_dataPoints.value("Typ").isValid())
        m_batteryType = static_cast<Typ>(m_dataPoints.value("Typ").toUInt16());

    if (m_dataPoints.value("State").isValid())
        m_stateOfTheBatteryBank = static_cast<State>(m_dataPoints.value("State").toUInt16());

    if (m_dataPoints.value("StateVnd").isValid())
        m_vendorBatteryBankState = m_dataPoints.value("StateVnd").toUInt16();

    if (m_dataPoints.value("WarrDt").isValid())
        m_warrantyDate = m_dataPoints.value("WarrDt").toUInt32();

    if (m_dataPoints.value("Evt1").isValid())
        m_batteryEvent1Bitfield = static_cast<Evt1Flags>(m_dataPoints.value("Evt1").toUInt32());

    if (m_dataPoints.value("Evt2").isValid())
        m_batteryEvent2Bitfield = m_dataPoints.value("Evt2").toUInt32();

    if (m_dataPoints.value("EvtVnd1").isValid())
        m_vendorEventBitfield1 = m_dataPoints.value("EvtVnd1").toUInt32();

    if (m_dataPoints.value("EvtVnd2").isValid())
        m_vendorEventBitfield2 = m_dataPoints.value("EvtVnd2").toUInt32();

    if (m_dataPoints.value("V").isValid())
        m_externalBatteryVoltage = m_dataPoints.value("V").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("VMax").isValid())
        m_maxBatteryVoltage = m_dataPoints.value("VMax").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("VMin").isValid())
        m_minBatteryVoltage = m_dataPoints.value("VMin").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("CellVMax").isValid())
        m_maxCellVoltage = m_dataPoints.value("CellVMax").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("CellVMaxStr").isValid())
        m_maxCellVoltageString = m_dataPoints.value("CellVMaxStr").toUInt16();

    if (m_dataPoints.value("CellVMaxMod").isValid())
        m_maxCellVoltageModule = m_dataPoints.value("CellVMaxMod").toUInt16();

    if (m_dataPoints.value("CellVMin").isValid())
        m_minCellVoltage = m_dataPoints.value("CellVMin").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("CellVMinStr").isValid())
        m_minCellVoltageString = m_dataPoints.value("CellVMinStr").toUInt16();

    if (m_dataPoints.value("CellVMinMod").isValid())
        m_minCellVoltageModule = m_dataPoints.value("CellVMinMod").toUInt16();

    if (m_dataPoints.value("CellVAvg").isValid())
        m_averageCellVoltage = m_dataPoints.value("CellVAvg").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("A").isValid())
        m_totalDcCurrent = m_dataPoints.value("A").toFloatWithSSF(m_a_SF);

    if (m_dataPoints.value("AChaMax").isValid())
        m_maxChargeCurrent = m_dataPoints.value("AChaMax").toFloatWithSSF(m_aMax_SF);

    if (m_dataPoints.value("ADisChaMax").isValid())
        m_maxDischargeCurrent = m_dataPoints.value("ADisChaMax").toFloatWithSSF(m_aMax_SF);

    if (m_dataPoints.value("W").isValid())
        m_totalPower = m_dataPoints.value("W").toFloatWithSSF(m_w_SF);

    if (m_dataPoints.value("ReqInvState").isValid())
        m_inverterStateRequest = static_cast<Reqinvstate>(m_dataPoints.value("ReqInvState").toUInt16());

    if (m_dataPoints.value("ReqW").isValid())
        m_batteryPowerRequest = m_dataPoints.value("ReqW").toFloatWithSSF(m_w_SF);

    if (m_dataPoints.value("SetOp").isValid())
        m_setOperation = static_cast<Setop>(m_dataPoints.value("SetOp").toUInt16());

    if (m_dataPoints.value("SetInvState").isValid())
        m_setInverterState = static_cast<Setinvstate>(m_dataPoints.value("SetInvState").toUInt16());

    if (m_dataPoints.value("AHRtg_SF").isValid())
        m_aHRtg_SF = m_dataPoints.value("AHRtg_SF").toInt16();

    if (m_dataPoints.value("WHRtg_SF").isValid())
        m_wHRtg_SF = m_dataPoints.value("WHRtg_SF").toInt16();

    if (m_dataPoints.value("WChaDisChaMax_SF").isValid())
        m_wChaDisChaMax_SF = m_dataPoints.value("WChaDisChaMax_SF").toInt16();

    if (m_dataPoints.value("DisChaRte_SF").isValid())
        m_disChaRte_SF = m_dataPoints.value("DisChaRte_SF").toInt16();

    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("DoD_SF").isValid())
        m_doD_SF = m_dataPoints.value("DoD_SF").toInt16();

    if (m_dataPoints.value("SoH_SF").isValid())
        m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("A_SF").isValid())
        m_a_SF = m_dataPoints.value("A_SF").toInt16();

    if (m_dataPoints.value("AMax_SF").isValid())
        m_aMax_SF = m_dataPoints.value("AMax_SF").toInt16();

    if (m_dataPoints.value("W_SF").isValid())
        m_w_SF = m_dataPoints.value("W_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecBatteryModel *model)
{
    debug.nospace().noquote() << "SunSpecBatteryModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("AHRtg") << "-->";
    if (model->dataPoints().value("AHRtg").isValid()) {
        debug.nospace().noquote() << model->nameplateChargeCapacity() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WHRtg") << "-->";
    if (model->dataPoints().value("WHRtg").isValid()) {
        debug.nospace().noquote() << model->nameplateEnergyCapacity() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WChaRteMax") << "-->";
    if (model->dataPoints().value("WChaRteMax").isValid()) {
        debug.nospace().noquote() << model->nameplateMaxChargeRate() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WDisChaRteMax") << "-->";
    if (model->dataPoints().value("WDisChaRteMax").isValid()) {
        debug.nospace().noquote() << model->nameplateMaxDischargeRate() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DisChaRte") << "-->";
    if (model->dataPoints().value("DisChaRte").isValid()) {
        debug.nospace().noquote() << model->selfDischargeRate() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoCMax") << "-->";
    if (model->dataPoints().value("SoCMax").isValid()) {
        debug.nospace().noquote() << model->nameplateMaxSoC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoCMin") << "-->";
    if (model->dataPoints().value("SoCMin").isValid()) {
        debug.nospace().noquote() << model->nameplateMinSoC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SocRsvMax") << "-->";
    if (model->dataPoints().value("SocRsvMax").isValid()) {
        debug.nospace().noquote() << model->maxReservePercent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoCRsvMin") << "-->";
    if (model->dataPoints().value("SoCRsvMin").isValid()) {
        debug.nospace().noquote() << model->minReservePercent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoC") << "-->";
    if (model->dataPoints().value("SoC").isValid()) {
        debug.nospace().noquote() << model->stateOfCharge() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DoD") << "-->";
    if (model->dataPoints().value("DoD").isValid()) {
        debug.nospace().noquote() << model->depthOfDischarge() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoH") << "-->";
    if (model->dataPoints().value("SoH").isValid()) {
        debug.nospace().noquote() << model->stateOfHealth() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NCyc") << "-->";
    if (model->dataPoints().value("NCyc").isValid()) {
        debug.nospace().noquote() << model->cycleCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ChaSt") << "-->";
    if (model->dataPoints().value("ChaSt").isValid()) {
        debug.nospace().noquote() << model->chargeStatus() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("LocRemCtl") << "-->";
    if (model->dataPoints().value("LocRemCtl").isValid()) {
        debug.nospace().noquote() << model->controlMode() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Hb") << "-->";
    if (model->dataPoints().value("Hb").isValid()) {
        debug.nospace().noquote() << model->batteryHeartbeat() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CtrlHb") << "-->";
    if (model->dataPoints().value("CtrlHb").isValid()) {
        debug.nospace().noquote() << model->controllerHeartbeat() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("AlmRst") << "-->";
    if (model->dataPoints().value("AlmRst").isValid()) {
        debug.nospace().noquote() << model->alarmReset() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Typ") << "-->";
    if (model->dataPoints().value("Typ").isValid()) {
        debug.nospace().noquote() << model->batteryType() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("State") << "-->";
    if (model->dataPoints().value("State").isValid()) {
        debug.nospace().noquote() << model->stateOfTheBatteryBank() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StateVnd") << "-->";
    if (model->dataPoints().value("StateVnd").isValid()) {
        debug.nospace().noquote() << model->vendorBatteryBankState() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WarrDt") << "-->";
    if (model->dataPoints().value("WarrDt").isValid()) {
        debug.nospace().noquote() << model->warrantyDate() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt1") << "-->";
    if (model->dataPoints().value("Evt1").isValid()) {
        debug.nospace().noquote() << model->batteryEvent1Bitfield() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt2") << "-->";
    if (model->dataPoints().value("Evt2").isValid()) {
        debug.nospace().noquote() << model->batteryEvent2Bitfield() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd1") << "-->";
    if (model->dataPoints().value("EvtVnd1").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd2") << "-->";
    if (model->dataPoints().value("EvtVnd2").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("V") << "-->";
    if (model->dataPoints().value("V").isValid()) {
        debug.nospace().noquote() << model->externalBatteryVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VMax") << "-->";
    if (model->dataPoints().value("VMax").isValid()) {
        debug.nospace().noquote() << model->maxBatteryVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VMin") << "-->";
    if (model->dataPoints().value("VMin").isValid()) {
        debug.nospace().noquote() << model->minBatteryVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMax") << "-->";
    if (model->dataPoints().value("CellVMax").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMaxStr") << "-->";
    if (model->dataPoints().value("CellVMaxStr").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltageString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMaxMod") << "-->";
    if (model->dataPoints().value("CellVMaxMod").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltageModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMin") << "-->";
    if (model->dataPoints().value("CellVMin").isValid()) {
        debug.nospace().noquote() << model->minCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMinStr") << "-->";
    if (model->dataPoints().value("CellVMinStr").isValid()) {
        debug.nospace().noquote() << model->minCellVoltageString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMinMod") << "-->";
    if (model->dataPoints().value("CellVMinMod").isValid()) {
        debug.nospace().noquote() << model->minCellVoltageModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVAvg") << "-->";
    if (model->dataPoints().value("CellVAvg").isValid()) {
        debug.nospace().noquote() << model->averageCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("A") << "-->";
    if (model->dataPoints().value("A").isValid()) {
        debug.nospace().noquote() << model->totalDcCurrent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("AChaMax") << "-->";
    if (model->dataPoints().value("AChaMax").isValid()) {
        debug.nospace().noquote() << model->maxChargeCurrent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ADisChaMax") << "-->";
    if (model->dataPoints().value("ADisChaMax").isValid()) {
        debug.nospace().noquote() << model->maxDischargeCurrent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("W") << "-->";
    if (model->dataPoints().value("W").isValid()) {
        debug.nospace().noquote() << model->totalPower() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ReqInvState") << "-->";
    if (model->dataPoints().value("ReqInvState").isValid()) {
        debug.nospace().noquote() << model->inverterStateRequest() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ReqW") << "-->";
    if (model->dataPoints().value("ReqW").isValid()) {
        debug.nospace().noquote() << model->batteryPowerRequest() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SetOp") << "-->";
    if (model->dataPoints().value("SetOp").isValid()) {
        debug.nospace().noquote() << model->setOperation() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SetInvState") << "-->";
    if (model->dataPoints().value("SetInvState").isValid()) {
        debug.nospace().noquote() << model->setInverterState() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
