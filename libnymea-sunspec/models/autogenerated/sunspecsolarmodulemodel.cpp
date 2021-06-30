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

#include "sunspecsolarmodulemodel.h"

SunSpecSolarModuleModel::SunSpecSolarModuleModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecSolarModuleModel::~SunSpecSolarModuleModel()
{

}

QString SunSpecSolarModuleModel::name() const
{
    switch (m_modelId) {
    case 501:
        return "solar_module";
    case 502:
        return "solar_module";
    default:
        return QString();
    }
}

QString SunSpecSolarModuleModel::description() const
{
    switch (m_modelId) {
    case 501:
        return "A solar module model supporting DC-DC converter";
    case 502:
        return "A solar module model supporting DC-DC converter";
    default:
        return QString();
    }
}

QString SunSpecSolarModuleModel::label() const
{
    switch (m_modelId) {
    case 501:
        return "Solar Module";
    case 502:
        return "Solar Module";
    default:
        return QString();
    }
}

void SunSpecSolarModuleModel::readModelHeader()
{

}

void SunSpecSolarModuleModel::readBlockData()
{

}

void SunSpecSolarModuleModel::initDataPoints()
{
    switch (m_modelId) {
    case 501: {
        SunSpecDataPoint modelIdDataPoint;
        modelIdDataPoint.setName("ID");
        modelIdDataPoint.setLabel("Model ID");
        modelIdDataPoint.setDescription("Model identifier");
        modelIdDataPoint.setMandatory(true);
        modelIdDataPoint.setSize(1);
        modelIdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << modelIdDataPoint;

        SunSpecDataPoint modelLengthDataPoint;
        modelLengthDataPoint.setName("L");
        modelLengthDataPoint.setLabel("Model Length");
        modelLengthDataPoint.setDescription("Model length");
        modelLengthDataPoint.setMandatory(true);
        modelLengthDataPoint.setSize(1);
        modelLengthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << modelLengthDataPoint;

        SunSpecDataPoint statusDataPoint;
        statusDataPoint.setName("Stat");
        statusDataPoint.setLabel("Status");
        statusDataPoint.setDescription("Enumerated value.  Module Status Code");
        statusDataPoint.setMandatory(true);
        statusDataPoint.setSize(1);
        statusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << statusDataPoint;

        SunSpecDataPoint vendorStatusDataPoint;
        vendorStatusDataPoint.setName("StatVend");
        vendorStatusDataPoint.setLabel("Vendor Status");
        vendorStatusDataPoint.setDescription("Module Vendor Status Code");
        vendorStatusDataPoint.setSize(1);
        vendorStatusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << vendorStatusDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Bitmask value.  Module Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        SunSpecDataPoint vendorModuleEventFlagsDataPoint;
        vendorModuleEventFlagsDataPoint.setName("EvtVend");
        vendorModuleEventFlagsDataPoint.setLabel("Vendor Module Event Flags");
        vendorModuleEventFlagsDataPoint.setDescription("Vendor specific flags");
        vendorModuleEventFlagsDataPoint.setSize(2);
        vendorModuleEventFlagsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorModuleEventFlagsDataPoint;

        SunSpecDataPoint controlDataPoint;
        controlDataPoint.setName("Ctl");
        controlDataPoint.setLabel("Control");
        controlDataPoint.setDescription("Module Control");
        controlDataPoint.setSize(1);
        controlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        controlDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << controlDataPoint;

        SunSpecDataPoint vendorControlDataPoint;
        vendorControlDataPoint.setName("CtlVend");
        vendorControlDataPoint.setLabel("Vendor Control");
        vendorControlDataPoint.setDescription("Vendor Module Control");
        vendorControlDataPoint.setSize(2);
        vendorControlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum32"));
        vendorControlDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << vendorControlDataPoint;

        SunSpecDataPoint controlValueDataPoint;
        controlValueDataPoint.setName("CtlVal");
        controlValueDataPoint.setLabel("Control Value");
        controlValueDataPoint.setDescription("Module Control Value");
        controlValueDataPoint.setSize(2);
        controlValueDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int32"));
        controlValueDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << controlValueDataPoint;

        SunSpecDataPoint timestampDataPoint;
        timestampDataPoint.setName("Tms");
        timestampDataPoint.setLabel("Timestamp");
        timestampDataPoint.setDescription("Time in seconds since 2000 epoch");
        timestampDataPoint.setUnits("Secs");
        timestampDataPoint.setSize(2);
        timestampDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
        m_dataPoints << timestampDataPoint;

        SunSpecDataPoint outputCurrentDataPoint;
        outputCurrentDataPoint.setName("OutA");
        outputCurrentDataPoint.setLabel("Output Current");
        outputCurrentDataPoint.setDescription("Output Current");
        outputCurrentDataPoint.setUnits("A");
        outputCurrentDataPoint.setSize(2);
        outputCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << outputCurrentDataPoint;

        SunSpecDataPoint outputVoltageDataPoint;
        outputVoltageDataPoint.setName("OutV");
        outputVoltageDataPoint.setLabel("Output Voltage");
        outputVoltageDataPoint.setDescription("Output Voltage");
        outputVoltageDataPoint.setUnits("V");
        outputVoltageDataPoint.setSize(2);
        outputVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << outputVoltageDataPoint;

        SunSpecDataPoint outputEnergyDataPoint;
        outputEnergyDataPoint.setName("OutWh");
        outputEnergyDataPoint.setLabel("Output Energy");
        outputEnergyDataPoint.setDescription("Output Energy");
        outputEnergyDataPoint.setUnits("Wh");
        outputEnergyDataPoint.setSize(2);
        outputEnergyDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << outputEnergyDataPoint;

        SunSpecDataPoint outputPowerDataPoint;
        outputPowerDataPoint.setName("OutW");
        outputPowerDataPoint.setLabel("Output Power");
        outputPowerDataPoint.setDescription("Output Power");
        outputPowerDataPoint.setUnits("W");
        outputPowerDataPoint.setSize(2);
        outputPowerDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << outputPowerDataPoint;

        SunSpecDataPoint tempDataPoint;
        tempDataPoint.setName("Tmp");
        tempDataPoint.setLabel("Temp");
        tempDataPoint.setDescription("Module Temperature");
        tempDataPoint.setUnits("C");
        tempDataPoint.setSize(2);
        tempDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << tempDataPoint;

        SunSpecDataPoint inputCurrentDataPoint;
        inputCurrentDataPoint.setName("InA");
        inputCurrentDataPoint.setLabel("Input Current");
        inputCurrentDataPoint.setDescription("Input Current");
        inputCurrentDataPoint.setUnits("A");
        inputCurrentDataPoint.setSize(2);
        inputCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << inputCurrentDataPoint;

        SunSpecDataPoint inputVoltageDataPoint;
        inputVoltageDataPoint.setName("InV");
        inputVoltageDataPoint.setLabel("Input Voltage");
        inputVoltageDataPoint.setDescription("Input Voltage");
        inputVoltageDataPoint.setUnits("V");
        inputVoltageDataPoint.setSize(2);
        inputVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << inputVoltageDataPoint;

        SunSpecDataPoint inputEnergyDataPoint;
        inputEnergyDataPoint.setName("InWh");
        inputEnergyDataPoint.setLabel("Input Energy");
        inputEnergyDataPoint.setDescription("Input Energy");
        inputEnergyDataPoint.setUnits("Wh");
        inputEnergyDataPoint.setSize(2);
        inputEnergyDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << inputEnergyDataPoint;

        SunSpecDataPoint inputPowerDataPoint;
        inputPowerDataPoint.setName("InW");
        inputPowerDataPoint.setLabel("Input Power");
        inputPowerDataPoint.setDescription("Input Power");
        inputPowerDataPoint.setUnits("W");
        inputPowerDataPoint.setSize(2);
        inputPowerDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << inputPowerDataPoint;

        break;
    }
    case 502: {
        SunSpecDataPoint modelIdDataPoint;
        modelIdDataPoint.setName("ID");
        modelIdDataPoint.setLabel("Model ID");
        modelIdDataPoint.setDescription("Model identifier");
        modelIdDataPoint.setMandatory(true);
        modelIdDataPoint.setSize(1);
        modelIdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << modelIdDataPoint;

        SunSpecDataPoint modelLengthDataPoint;
        modelLengthDataPoint.setName("L");
        modelLengthDataPoint.setLabel("Model Length");
        modelLengthDataPoint.setDescription("Model length");
        modelLengthDataPoint.setMandatory(true);
        modelLengthDataPoint.setSize(1);
        modelLengthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << modelLengthDataPoint;

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << A_SFDataPoint;

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << V_SFDataPoint;

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Power scale factor");
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << W_SFDataPoint;

        SunSpecDataPoint Wh_SFDataPoint;
        Wh_SFDataPoint.setName("Wh_SF");
        Wh_SFDataPoint.setDescription("Energy scale factor");
        Wh_SFDataPoint.setSize(1);
        Wh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Wh_SFDataPoint;

        SunSpecDataPoint statusDataPoint;
        statusDataPoint.setName("Stat");
        statusDataPoint.setLabel("Status");
        statusDataPoint.setDescription("Enumerated value.  Module Status Code");
        statusDataPoint.setMandatory(true);
        statusDataPoint.setSize(1);
        statusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << statusDataPoint;

        SunSpecDataPoint vendorStatusDataPoint;
        vendorStatusDataPoint.setName("StatVend");
        vendorStatusDataPoint.setLabel("Vendor Status");
        vendorStatusDataPoint.setDescription("Module Vendor Status Code");
        vendorStatusDataPoint.setSize(1);
        vendorStatusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << vendorStatusDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Bitmask value.  Module Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        SunSpecDataPoint vendorModuleEventFlagsDataPoint;
        vendorModuleEventFlagsDataPoint.setName("EvtVend");
        vendorModuleEventFlagsDataPoint.setLabel("Vendor Module Event Flags");
        vendorModuleEventFlagsDataPoint.setDescription("Vendor specific flags");
        vendorModuleEventFlagsDataPoint.setSize(2);
        vendorModuleEventFlagsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorModuleEventFlagsDataPoint;

        SunSpecDataPoint controlDataPoint;
        controlDataPoint.setName("Ctl");
        controlDataPoint.setLabel("Control");
        controlDataPoint.setDescription("Module Control");
        controlDataPoint.setSize(1);
        controlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        controlDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << controlDataPoint;

        SunSpecDataPoint vendorControlDataPoint;
        vendorControlDataPoint.setName("CtlVend");
        vendorControlDataPoint.setLabel("Vendor Control");
        vendorControlDataPoint.setDescription("Vendor Module Control");
        vendorControlDataPoint.setSize(2);
        vendorControlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum32"));
        vendorControlDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << vendorControlDataPoint;

        SunSpecDataPoint controlValueDataPoint;
        controlValueDataPoint.setName("CtlVal");
        controlValueDataPoint.setLabel("Control Value");
        controlValueDataPoint.setDescription("Module Control Value");
        controlValueDataPoint.setSize(2);
        controlValueDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int32"));
        controlValueDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << controlValueDataPoint;

        SunSpecDataPoint timestampDataPoint;
        timestampDataPoint.setName("Tms");
        timestampDataPoint.setLabel("Timestamp");
        timestampDataPoint.setDescription("Time in seconds since 2000 epoch");
        timestampDataPoint.setUnits("Secs");
        timestampDataPoint.setSize(2);
        timestampDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
        m_dataPoints << timestampDataPoint;

        SunSpecDataPoint outputCurrentDataPoint;
        outputCurrentDataPoint.setName("OutA");
        outputCurrentDataPoint.setLabel("Output Current");
        outputCurrentDataPoint.setDescription("Output Current");
        outputCurrentDataPoint.setUnits("A");
        outputCurrentDataPoint.setSize(1);
        outputCurrentDataPoint.setScaleFactorName("A_SF");
        outputCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << outputCurrentDataPoint;

        SunSpecDataPoint outputVoltageDataPoint;
        outputVoltageDataPoint.setName("OutV");
        outputVoltageDataPoint.setLabel("Output Voltage");
        outputVoltageDataPoint.setDescription("Output Voltage");
        outputVoltageDataPoint.setUnits("V");
        outputVoltageDataPoint.setSize(1);
        outputVoltageDataPoint.setScaleFactorName("V_SF");
        outputVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << outputVoltageDataPoint;

        SunSpecDataPoint outputEnergyDataPoint;
        outputEnergyDataPoint.setName("OutWh");
        outputEnergyDataPoint.setLabel("Output Energy");
        outputEnergyDataPoint.setDescription("Output Energy");
        outputEnergyDataPoint.setUnits("Wh");
        outputEnergyDataPoint.setSize(2);
        outputEnergyDataPoint.setScaleFactorName("Wh_SF");
        outputEnergyDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << outputEnergyDataPoint;

        SunSpecDataPoint outputPowerDataPoint;
        outputPowerDataPoint.setName("OutPw");
        outputPowerDataPoint.setLabel("Output Power");
        outputPowerDataPoint.setDescription("Output Power");
        outputPowerDataPoint.setUnits("W");
        outputPowerDataPoint.setSize(1);
        outputPowerDataPoint.setScaleFactorName("W_SF");
        outputPowerDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << outputPowerDataPoint;

        SunSpecDataPoint tempDataPoint;
        tempDataPoint.setName("Tmp");
        tempDataPoint.setLabel("Temp");
        tempDataPoint.setDescription("Module Temperature");
        tempDataPoint.setUnits("C");
        tempDataPoint.setSize(1);
        tempDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << tempDataPoint;

        SunSpecDataPoint inputCurrentDataPoint;
        inputCurrentDataPoint.setName("InA");
        inputCurrentDataPoint.setLabel("Input Current");
        inputCurrentDataPoint.setDescription("Input Current");
        inputCurrentDataPoint.setUnits("A");
        inputCurrentDataPoint.setSize(1);
        inputCurrentDataPoint.setScaleFactorName("A_SF");
        inputCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << inputCurrentDataPoint;

        SunSpecDataPoint inputVoltageDataPoint;
        inputVoltageDataPoint.setName("InV");
        inputVoltageDataPoint.setLabel("Input Voltage");
        inputVoltageDataPoint.setDescription("Input Voltage");
        inputVoltageDataPoint.setUnits("V");
        inputVoltageDataPoint.setSize(1);
        inputVoltageDataPoint.setScaleFactorName("V_SF");
        inputVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << inputVoltageDataPoint;

        SunSpecDataPoint inputEnergyDataPoint;
        inputEnergyDataPoint.setName("InWh");
        inputEnergyDataPoint.setLabel("Input Energy");
        inputEnergyDataPoint.setDescription("Input Energy");
        inputEnergyDataPoint.setUnits("Wh");
        inputEnergyDataPoint.setSize(2);
        inputEnergyDataPoint.setScaleFactorName("Wh_SF");
        inputEnergyDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << inputEnergyDataPoint;

        SunSpecDataPoint inputPowerDataPoint;
        inputPowerDataPoint.setName("InW");
        inputPowerDataPoint.setLabel("Input Power");
        inputPowerDataPoint.setDescription("Input Power");
        inputPowerDataPoint.setUnits("W");
        inputPowerDataPoint.setSize(1);
        inputPowerDataPoint.setScaleFactorName("W_SF");
        inputPowerDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << inputPowerDataPoint;

        break;
    }
    default:
        break;
    }
}

