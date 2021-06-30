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

#include "sunspecbatterymodel.h"

SunSpecBatteryModel::SunSpecBatteryModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 802;
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

void SunSpecBatteryModel::readModelHeader()
{

}

void SunSpecBatteryModel::readBlockData()
{

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
    modelIdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    nameplateChargeCapacityDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    nameplateEnergyCapacityDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    nameplateMaxChargeRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    nameplateMaxDischargeRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    selfDischargeRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(selfDischargeRateDataPoint.name(), selfDischargeRateDataPoint);

    SunSpecDataPoint nameplateMaxSocDataPoint;
    nameplateMaxSocDataPoint.setName("SoCMax");
    nameplateMaxSocDataPoint.setLabel("Nameplate Max SoC");
    nameplateMaxSocDataPoint.setDescription("Manufacturer maximum state of charge, expressed as a percentage.");
    nameplateMaxSocDataPoint.setUnits("%WHRtg");
    nameplateMaxSocDataPoint.setSize(1);
    nameplateMaxSocDataPoint.setAddressOffset(7);
    nameplateMaxSocDataPoint.setBlockOffset(5);
    nameplateMaxSocDataPoint.setScaleFactorName("SoC_SF");
    nameplateMaxSocDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nameplateMaxSocDataPoint.name(), nameplateMaxSocDataPoint);

    SunSpecDataPoint nameplateMinSocDataPoint;
    nameplateMinSocDataPoint.setName("SoCMin");
    nameplateMinSocDataPoint.setLabel("Nameplate Min SoC");
    nameplateMinSocDataPoint.setDescription("Manufacturer minimum state of charge, expressed as a percentage.");
    nameplateMinSocDataPoint.setUnits("%WHRtg");
    nameplateMinSocDataPoint.setSize(1);
    nameplateMinSocDataPoint.setAddressOffset(8);
    nameplateMinSocDataPoint.setBlockOffset(6);
    nameplateMinSocDataPoint.setScaleFactorName("SoC_SF");
    nameplateMinSocDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nameplateMinSocDataPoint.name(), nameplateMinSocDataPoint);

    SunSpecDataPoint maxReservePercentDataPoint;
    maxReservePercentDataPoint.setName("SocRsvMax");
    maxReservePercentDataPoint.setLabel("Max Reserve Percent");
    maxReservePercentDataPoint.setDescription("Setpoint for maximum reserve for storage as a percentage of the nominal maximum storage.");
    maxReservePercentDataPoint.setUnits("%WHRtg");
    maxReservePercentDataPoint.setSize(1);
    maxReservePercentDataPoint.setAddressOffset(9);
    maxReservePercentDataPoint.setBlockOffset(7);
    maxReservePercentDataPoint.setScaleFactorName("SoC_SF");
    maxReservePercentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    maxReservePercentDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
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
    minReservePercentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    minReservePercentDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
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
    stateOfChargeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    depthOfDischargeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    stateOfHealthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(stateOfHealthDataPoint.name(), stateOfHealthDataPoint);

    SunSpecDataPoint cycleCountDataPoint;
    cycleCountDataPoint.setName("NCyc");
    cycleCountDataPoint.setLabel("Cycle Count");
    cycleCountDataPoint.setDescription("Number of cycles executed in the battery.");
    cycleCountDataPoint.setSize(2);
    cycleCountDataPoint.setAddressOffset(14);
    cycleCountDataPoint.setBlockOffset(12);
    cycleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
    m_dataPoints.insert(cycleCountDataPoint.name(), cycleCountDataPoint);

    SunSpecDataPoint chargeStatusDataPoint;
    chargeStatusDataPoint.setName("ChaSt");
    chargeStatusDataPoint.setLabel("Charge Status");
    chargeStatusDataPoint.setDescription("Charge status of storage device. Enumeration.");
    chargeStatusDataPoint.setSize(1);
    chargeStatusDataPoint.setAddressOffset(16);
    chargeStatusDataPoint.setBlockOffset(14);
    chargeStatusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(chargeStatusDataPoint.name(), chargeStatusDataPoint);

    SunSpecDataPoint controlModeDataPoint;
    controlModeDataPoint.setName("LocRemCtl");
    controlModeDataPoint.setLabel("Control Mode");
    controlModeDataPoint.setDescription("Battery control mode. Enumeration.");
    controlModeDataPoint.setMandatory(true);
    controlModeDataPoint.setSize(1);
    controlModeDataPoint.setAddressOffset(17);
    controlModeDataPoint.setBlockOffset(15);
    controlModeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(controlModeDataPoint.name(), controlModeDataPoint);

    SunSpecDataPoint batteryHeartbeatDataPoint;
    batteryHeartbeatDataPoint.setName("Hb");
    batteryHeartbeatDataPoint.setLabel("Battery Heartbeat");
    batteryHeartbeatDataPoint.setDescription("Value is incremented every second with periodic resets to zero.");
    batteryHeartbeatDataPoint.setSize(1);
    batteryHeartbeatDataPoint.setAddressOffset(18);
    batteryHeartbeatDataPoint.setBlockOffset(16);
    batteryHeartbeatDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(batteryHeartbeatDataPoint.name(), batteryHeartbeatDataPoint);

    SunSpecDataPoint controllerHeartbeatDataPoint;
    controllerHeartbeatDataPoint.setName("CtrlHb");
    controllerHeartbeatDataPoint.setLabel("Controller Heartbeat");
    controllerHeartbeatDataPoint.setDescription("Value is incremented every second with periodic resets to zero.");
    controllerHeartbeatDataPoint.setSize(1);
    controllerHeartbeatDataPoint.setAddressOffset(19);
    controllerHeartbeatDataPoint.setBlockOffset(17);
    controllerHeartbeatDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    controllerHeartbeatDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(controllerHeartbeatDataPoint.name(), controllerHeartbeatDataPoint);

    SunSpecDataPoint alarmResetDataPoint;
    alarmResetDataPoint.setName("AlmRst");
    alarmResetDataPoint.setLabel("Alarm Reset");
    alarmResetDataPoint.setDescription("Used to reset any latched alarms.  1 = Reset.");
    alarmResetDataPoint.setMandatory(true);
    alarmResetDataPoint.setSize(1);
    alarmResetDataPoint.setAddressOffset(20);
    alarmResetDataPoint.setBlockOffset(18);
    alarmResetDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    alarmResetDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(alarmResetDataPoint.name(), alarmResetDataPoint);

    SunSpecDataPoint batteryTypeDataPoint;
    batteryTypeDataPoint.setName("Typ");
    batteryTypeDataPoint.setLabel("Battery Type");
    batteryTypeDataPoint.setDescription("Type of battery. Enumeration.");
    batteryTypeDataPoint.setMandatory(true);
    batteryTypeDataPoint.setSize(1);
    batteryTypeDataPoint.setAddressOffset(21);
    batteryTypeDataPoint.setBlockOffset(19);
    batteryTypeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(batteryTypeDataPoint.name(), batteryTypeDataPoint);

    SunSpecDataPoint stateOfTheBatteryBankDataPoint;
    stateOfTheBatteryBankDataPoint.setName("State");
    stateOfTheBatteryBankDataPoint.setLabel("State of the Battery Bank");
    stateOfTheBatteryBankDataPoint.setDescription("State of the battery bank.  Enumeration.");
    stateOfTheBatteryBankDataPoint.setMandatory(true);
    stateOfTheBatteryBankDataPoint.setSize(1);
    stateOfTheBatteryBankDataPoint.setAddressOffset(22);
    stateOfTheBatteryBankDataPoint.setBlockOffset(20);
    stateOfTheBatteryBankDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(stateOfTheBatteryBankDataPoint.name(), stateOfTheBatteryBankDataPoint);

    SunSpecDataPoint vendorBatteryBankStateDataPoint;
    vendorBatteryBankStateDataPoint.setName("StateVnd");
    vendorBatteryBankStateDataPoint.setLabel("Vendor Battery Bank State");
    vendorBatteryBankStateDataPoint.setDescription("Vendor specific battery bank state.  Enumeration.");
    vendorBatteryBankStateDataPoint.setSize(1);
    vendorBatteryBankStateDataPoint.setAddressOffset(23);
    vendorBatteryBankStateDataPoint.setBlockOffset(21);
    vendorBatteryBankStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(vendorBatteryBankStateDataPoint.name(), vendorBatteryBankStateDataPoint);

    SunSpecDataPoint warrantyDateDataPoint;
    warrantyDateDataPoint.setName("WarrDt");
    warrantyDateDataPoint.setLabel("Warranty Date");
    warrantyDateDataPoint.setDescription("Date the device warranty expires.");
    warrantyDateDataPoint.setSize(2);
    warrantyDateDataPoint.setAddressOffset(24);
    warrantyDateDataPoint.setBlockOffset(22);
    warrantyDateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
    m_dataPoints.insert(warrantyDateDataPoint.name(), warrantyDateDataPoint);

    SunSpecDataPoint batteryEvent1BitfieldDataPoint;
    batteryEvent1BitfieldDataPoint.setName("Evt1");
    batteryEvent1BitfieldDataPoint.setLabel("Battery Event 1 Bitfield");
    batteryEvent1BitfieldDataPoint.setDescription("Alarms and warnings.  Bit flags.");
    batteryEvent1BitfieldDataPoint.setMandatory(true);
    batteryEvent1BitfieldDataPoint.setSize(2);
    batteryEvent1BitfieldDataPoint.setAddressOffset(26);
    batteryEvent1BitfieldDataPoint.setBlockOffset(24);
    batteryEvent1BitfieldDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(batteryEvent1BitfieldDataPoint.name(), batteryEvent1BitfieldDataPoint);

    SunSpecDataPoint batteryEvent2BitfieldDataPoint;
    batteryEvent2BitfieldDataPoint.setName("Evt2");
    batteryEvent2BitfieldDataPoint.setLabel("Battery Event 2 Bitfield");
    batteryEvent2BitfieldDataPoint.setDescription("Alarms and warnings.  Bit flags.");
    batteryEvent2BitfieldDataPoint.setMandatory(true);
    batteryEvent2BitfieldDataPoint.setSize(2);
    batteryEvent2BitfieldDataPoint.setAddressOffset(28);
    batteryEvent2BitfieldDataPoint.setBlockOffset(26);
    batteryEvent2BitfieldDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(batteryEvent2BitfieldDataPoint.name(), batteryEvent2BitfieldDataPoint);

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield1DataPoint.setMandatory(true);
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setAddressOffset(30);
    vendorEventBitfield1DataPoint.setBlockOffset(28);
    vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield2DataPoint.setMandatory(true);
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setAddressOffset(32);
    vendorEventBitfield2DataPoint.setBlockOffset(30);
    vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
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
    externalBatteryVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    maxBatteryVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    minBatteryVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    maxCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageStringDataPoint;
    maxCellVoltageStringDataPoint.setName("CellVMaxStr");
    maxCellVoltageStringDataPoint.setLabel("Max Cell Voltage String");
    maxCellVoltageStringDataPoint.setDescription("String containing the cell with maximum voltage.");
    maxCellVoltageStringDataPoint.setSize(1);
    maxCellVoltageStringDataPoint.setAddressOffset(38);
    maxCellVoltageStringDataPoint.setBlockOffset(36);
    maxCellVoltageStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxCellVoltageStringDataPoint.name(), maxCellVoltageStringDataPoint);

    SunSpecDataPoint maxCellVoltageModuleDataPoint;
    maxCellVoltageModuleDataPoint.setName("CellVMaxMod");
    maxCellVoltageModuleDataPoint.setLabel("Max Cell Voltage Module");
    maxCellVoltageModuleDataPoint.setDescription("Module containing the cell with maximum voltage.");
    maxCellVoltageModuleDataPoint.setSize(1);
    maxCellVoltageModuleDataPoint.setAddressOffset(39);
    maxCellVoltageModuleDataPoint.setBlockOffset(37);
    maxCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    minCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageStringDataPoint;
    minCellVoltageStringDataPoint.setName("CellVMinStr");
    minCellVoltageStringDataPoint.setLabel("Min Cell Voltage String");
    minCellVoltageStringDataPoint.setDescription("String containing the cell with minimum voltage.");
    minCellVoltageStringDataPoint.setSize(1);
    minCellVoltageStringDataPoint.setAddressOffset(41);
    minCellVoltageStringDataPoint.setBlockOffset(39);
    minCellVoltageStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minCellVoltageStringDataPoint.name(), minCellVoltageStringDataPoint);

    SunSpecDataPoint minCellVoltageModuleDataPoint;
    minCellVoltageModuleDataPoint.setName("CellVMinMod");
    minCellVoltageModuleDataPoint.setLabel("Min Cell Voltage Module");
    minCellVoltageModuleDataPoint.setDescription("Module containing the cell with minimum voltage.");
    minCellVoltageModuleDataPoint.setSize(1);
    minCellVoltageModuleDataPoint.setAddressOffset(42);
    minCellVoltageModuleDataPoint.setBlockOffset(40);
    minCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    averageCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    totalDcCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
    maxChargeCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    maxDischargeCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    totalPowerDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(totalPowerDataPoint.name(), totalPowerDataPoint);

    SunSpecDataPoint inverterStateRequestDataPoint;
    inverterStateRequestDataPoint.setName("ReqInvState");
    inverterStateRequestDataPoint.setLabel("Inverter State Request");
    inverterStateRequestDataPoint.setDescription("Request from battery to start or stop the inverter.  Enumeration.");
    inverterStateRequestDataPoint.setSize(1);
    inverterStateRequestDataPoint.setAddressOffset(48);
    inverterStateRequestDataPoint.setBlockOffset(46);
    inverterStateRequestDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
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
    batteryPowerRequestDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(batteryPowerRequestDataPoint.name(), batteryPowerRequestDataPoint);

    SunSpecDataPoint setOperationDataPoint;
    setOperationDataPoint.setName("SetOp");
    setOperationDataPoint.setLabel("Set Operation");
    setOperationDataPoint.setDescription("Instruct the battery bank to perform an operation such as connecting.  Enumeration.");
    setOperationDataPoint.setMandatory(true);
    setOperationDataPoint.setSize(1);
    setOperationDataPoint.setAddressOffset(50);
    setOperationDataPoint.setBlockOffset(48);
    setOperationDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    setOperationDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(setOperationDataPoint.name(), setOperationDataPoint);

    SunSpecDataPoint setInverterStateDataPoint;
    setInverterStateDataPoint.setName("SetInvState");
    setInverterStateDataPoint.setLabel("Set Inverter State");
    setInverterStateDataPoint.setDescription("Set the current state of the inverter.");
    setInverterStateDataPoint.setMandatory(true);
    setInverterStateDataPoint.setSize(1);
    setInverterStateDataPoint.setAddressOffset(51);
    setInverterStateDataPoint.setBlockOffset(49);
    setInverterStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    setInverterStateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(setInverterStateDataPoint.name(), setInverterStateDataPoint);

    SunSpecDataPoint AHRtg_SFDataPoint;
    AHRtg_SFDataPoint.setName("AHRtg_SF");
    AHRtg_SFDataPoint.setDescription("Scale factor for charge capacity.");
    AHRtg_SFDataPoint.setMandatory(true);
    AHRtg_SFDataPoint.setSize(1);
    AHRtg_SFDataPoint.setAddressOffset(52);
    AHRtg_SFDataPoint.setBlockOffset(50);
    AHRtg_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(AHRtg_SFDataPoint.name(), AHRtg_SFDataPoint);

    SunSpecDataPoint WHRtg_SFDataPoint;
    WHRtg_SFDataPoint.setName("WHRtg_SF");
    WHRtg_SFDataPoint.setDescription("Scale factor for energy capacity.");
    WHRtg_SFDataPoint.setMandatory(true);
    WHRtg_SFDataPoint.setSize(1);
    WHRtg_SFDataPoint.setAddressOffset(53);
    WHRtg_SFDataPoint.setBlockOffset(51);
    WHRtg_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(WHRtg_SFDataPoint.name(), WHRtg_SFDataPoint);

    SunSpecDataPoint WChaDisChaMax_SFDataPoint;
    WChaDisChaMax_SFDataPoint.setName("WChaDisChaMax_SF");
    WChaDisChaMax_SFDataPoint.setDescription("Scale factor for maximum charge and discharge rate.");
    WChaDisChaMax_SFDataPoint.setMandatory(true);
    WChaDisChaMax_SFDataPoint.setSize(1);
    WChaDisChaMax_SFDataPoint.setAddressOffset(54);
    WChaDisChaMax_SFDataPoint.setBlockOffset(52);
    WChaDisChaMax_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(WChaDisChaMax_SFDataPoint.name(), WChaDisChaMax_SFDataPoint);

    SunSpecDataPoint DisChaRte_SFDataPoint;
    DisChaRte_SFDataPoint.setName("DisChaRte_SF");
    DisChaRte_SFDataPoint.setDescription("Scale factor for self discharge rate.");
    DisChaRte_SFDataPoint.setSize(1);
    DisChaRte_SFDataPoint.setAddressOffset(55);
    DisChaRte_SFDataPoint.setBlockOffset(53);
    DisChaRte_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(DisChaRte_SFDataPoint.name(), DisChaRte_SFDataPoint);

    SunSpecDataPoint SoC_SFDataPoint;
    SoC_SFDataPoint.setName("SoC_SF");
    SoC_SFDataPoint.setDescription("Scale factor for state of charge values.");
    SoC_SFDataPoint.setMandatory(true);
    SoC_SFDataPoint.setSize(1);
    SoC_SFDataPoint.setAddressOffset(56);
    SoC_SFDataPoint.setBlockOffset(54);
    SoC_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(SoC_SFDataPoint.name(), SoC_SFDataPoint);

    SunSpecDataPoint DoD_SFDataPoint;
    DoD_SFDataPoint.setName("DoD_SF");
    DoD_SFDataPoint.setDescription("Scale factor for depth of discharge.");
    DoD_SFDataPoint.setSize(1);
    DoD_SFDataPoint.setAddressOffset(57);
    DoD_SFDataPoint.setBlockOffset(55);
    DoD_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(DoD_SFDataPoint.name(), DoD_SFDataPoint);

    SunSpecDataPoint SoH_SFDataPoint;
    SoH_SFDataPoint.setName("SoH_SF");
    SoH_SFDataPoint.setDescription("Scale factor for state of health.");
    SoH_SFDataPoint.setSize(1);
    SoH_SFDataPoint.setAddressOffset(58);
    SoH_SFDataPoint.setBlockOffset(56);
    SoH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(SoH_SFDataPoint.name(), SoH_SFDataPoint);

    SunSpecDataPoint V_SFDataPoint;
    V_SFDataPoint.setName("V_SF");
    V_SFDataPoint.setDescription("Scale factor for DC bus voltage.");
    V_SFDataPoint.setMandatory(true);
    V_SFDataPoint.setSize(1);
    V_SFDataPoint.setAddressOffset(59);
    V_SFDataPoint.setBlockOffset(57);
    V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

    SunSpecDataPoint CellV_SFDataPoint;
    CellV_SFDataPoint.setName("CellV_SF");
    CellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    CellV_SFDataPoint.setMandatory(true);
    CellV_SFDataPoint.setSize(1);
    CellV_SFDataPoint.setAddressOffset(60);
    CellV_SFDataPoint.setBlockOffset(58);
    CellV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(CellV_SFDataPoint.name(), CellV_SFDataPoint);

    SunSpecDataPoint A_SFDataPoint;
    A_SFDataPoint.setName("A_SF");
    A_SFDataPoint.setDescription("Scale factor for DC current.");
    A_SFDataPoint.setMandatory(true);
    A_SFDataPoint.setSize(1);
    A_SFDataPoint.setAddressOffset(61);
    A_SFDataPoint.setBlockOffset(59);
    A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

    SunSpecDataPoint AMax_SFDataPoint;
    AMax_SFDataPoint.setName("AMax_SF");
    AMax_SFDataPoint.setDescription("Scale factor for instantaneous DC charge/discharge current.");
    AMax_SFDataPoint.setMandatory(true);
    AMax_SFDataPoint.setSize(1);
    AMax_SFDataPoint.setAddressOffset(62);
    AMax_SFDataPoint.setBlockOffset(60);
    AMax_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(AMax_SFDataPoint.name(), AMax_SFDataPoint);

    SunSpecDataPoint W_SFDataPoint;
    W_SFDataPoint.setName("W_SF");
    W_SFDataPoint.setDescription("Scale factor for AC power request.");
    W_SFDataPoint.setSize(1);
    W_SFDataPoint.setAddressOffset(63);
    W_SFDataPoint.setBlockOffset(61);
    W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(W_SFDataPoint.name(), W_SFDataPoint);

}

