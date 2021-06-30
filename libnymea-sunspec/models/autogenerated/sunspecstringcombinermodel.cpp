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

#include "sunspecstringcombinermodel.h"

SunSpecStringCombinerModel::SunSpecStringCombinerModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 401 << 402 << 403 << 404;
}

SunSpecStringCombinerModel::~SunSpecStringCombinerModel()
{

}

QString SunSpecStringCombinerModel::name() const
{
    return "string_combiner";
}

QString SunSpecStringCombinerModel::description() const
{
    switch (m_modelId) {
    case 401:
        return "A basic string combiner";
    case 402:
        return "An advanced string combiner";
    case 403:
        return "A basic string combiner model";
    case 404:
        return "An advanced string combiner including voltage and energy measurements";
    default:
        return QString();
    }
}

QString SunSpecStringCombinerModel::label() const
{
    switch (m_modelId) {
    case 401:
        return "String Combiner (Current)";
    case 402:
        return "String Combiner (Advanced)";
    case 403:
        return "String Combiner (Current)";
    case 404:
        return "String Combiner (Advanced)";
    default:
        return QString();
    }
}

void SunSpecStringCombinerModel::readModelHeader()
{

}

void SunSpecStringCombinerModel::readBlockData()
{

}

void SunSpecStringCombinerModel::initDataPoints()
{
    switch (m_modelId) {
    case 401: {
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

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setDescription("Current scale factor");
        DCA_SFDataPoint.setMandatory(true);
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setAddressOffset(2);
        DCA_SFDataPoint.setBlockOffset(0);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCA_SFDataPoint.name(), DCA_SFDataPoint);

        SunSpecDataPoint DCAhr_SFDataPoint;
        DCAhr_SFDataPoint.setName("DCAhr_SF");
        DCAhr_SFDataPoint.setDescription("Amp-hour scale factor");
        DCAhr_SFDataPoint.setSize(1);
        DCAhr_SFDataPoint.setAddressOffset(3);
        DCAhr_SFDataPoint.setBlockOffset(1);
        DCAhr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCAhr_SFDataPoint.name(), DCAhr_SFDataPoint);

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setDescription("Voltage scale factor");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setAddressOffset(4);
        DCV_SFDataPoint.setBlockOffset(2);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCV_SFDataPoint.name(), DCV_SFDataPoint);

        SunSpecDataPoint ratingDataPoint;
        ratingDataPoint.setName("DCAMax");
        ratingDataPoint.setLabel("Rating");
        ratingDataPoint.setDescription("Maximum DC Current Rating");
        ratingDataPoint.setUnits("A");
        ratingDataPoint.setMandatory(true);
        ratingDataPoint.setSize(1);
        ratingDataPoint.setAddressOffset(5);
        ratingDataPoint.setBlockOffset(3);
        ratingDataPoint.setScaleFactorName("DCA_SF");
        ratingDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ratingDataPoint.name(), ratingDataPoint);

        SunSpecDataPoint nDataPoint;
        nDataPoint.setName("N");
        nDataPoint.setLabel("N");
        nDataPoint.setDescription("Number of Inputs");
        nDataPoint.setMandatory(true);
        nDataPoint.setSize(1);
        nDataPoint.setAddressOffset(6);
        nDataPoint.setBlockOffset(4);
        nDataPoint.setDataType(SunSpecDataPoint::stringToDataType("count"));
        m_dataPoints.insert(nDataPoint.name(), nDataPoint);

        SunSpecDataPoint eventDataPoint;
        eventDataPoint.setName("Evt");
        eventDataPoint.setLabel("Event");
        eventDataPoint.setDescription("Bitmask value.  Events");
        eventDataPoint.setMandatory(true);
        eventDataPoint.setSize(2);
        eventDataPoint.setAddressOffset(7);
        eventDataPoint.setBlockOffset(5);
        eventDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventDataPoint.name(), eventDataPoint);

        SunSpecDataPoint vendorEventDataPoint;
        vendorEventDataPoint.setName("EvtVnd");
        vendorEventDataPoint.setLabel("Vendor Event");
        vendorEventDataPoint.setDescription("Bitmask value.  Vendor defined events");
        vendorEventDataPoint.setSize(2);
        vendorEventDataPoint.setAddressOffset(9);
        vendorEventDataPoint.setBlockOffset(7);
        vendorEventDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventDataPoint.name(), vendorEventDataPoint);

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("DCA");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("Total measured current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(11);
        ampsDataPoint.setBlockOffset(9);
        ampsDataPoint.setScaleFactorName("DCA_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

        SunSpecDataPoint ampHoursDataPoint;
        ampHoursDataPoint.setName("DCAhr");
        ampHoursDataPoint.setLabel("Amp-hours");
        ampHoursDataPoint.setDescription("Total metered Amp-hours");
        ampHoursDataPoint.setUnits("Ah");
        ampHoursDataPoint.setSize(2);
        ampHoursDataPoint.setAddressOffset(12);
        ampHoursDataPoint.setBlockOffset(10);
        ampHoursDataPoint.setScaleFactorName("DCAhr_SF");
        ampHoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
        m_dataPoints.insert(ampHoursDataPoint.name(), ampHoursDataPoint);

        SunSpecDataPoint voltageDataPoint;
        voltageDataPoint.setName("DCV");
        voltageDataPoint.setLabel("Voltage");
        voltageDataPoint.setDescription("Output Voltage");
        voltageDataPoint.setUnits("V");
        voltageDataPoint.setSize(1);
        voltageDataPoint.setAddressOffset(14);
        voltageDataPoint.setBlockOffset(12);
        voltageDataPoint.setScaleFactorName("DCV_SF");
        voltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

        SunSpecDataPoint tempDataPoint;
        tempDataPoint.setName("Tmp");
        tempDataPoint.setLabel("Temp");
        tempDataPoint.setDescription("Internal operating temperature");
        tempDataPoint.setUnits("C");
        tempDataPoint.setSize(1);
        tempDataPoint.setAddressOffset(15);
        tempDataPoint.setBlockOffset(13);
        tempDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(tempDataPoint.name(), tempDataPoint);

        break;
    }
    case 402: {
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

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setDescription("Current scale factor");
        DCA_SFDataPoint.setMandatory(true);
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setAddressOffset(2);
        DCA_SFDataPoint.setBlockOffset(0);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCA_SFDataPoint.name(), DCA_SFDataPoint);

        SunSpecDataPoint DCAhr_SFDataPoint;
        DCAhr_SFDataPoint.setName("DCAhr_SF");
        DCAhr_SFDataPoint.setDescription("Amp-hour scale factor");
        DCAhr_SFDataPoint.setSize(1);
        DCAhr_SFDataPoint.setAddressOffset(3);
        DCAhr_SFDataPoint.setBlockOffset(1);
        DCAhr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCAhr_SFDataPoint.name(), DCAhr_SFDataPoint);

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setDescription("Voltage scale factor");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setAddressOffset(4);
        DCV_SFDataPoint.setBlockOffset(2);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCV_SFDataPoint.name(), DCV_SFDataPoint);

        SunSpecDataPoint DCW_SFDataPoint;
        DCW_SFDataPoint.setName("DCW_SF");
        DCW_SFDataPoint.setDescription("Power scale factor");
        DCW_SFDataPoint.setSize(1);
        DCW_SFDataPoint.setAddressOffset(5);
        DCW_SFDataPoint.setBlockOffset(3);
        DCW_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCW_SFDataPoint.name(), DCW_SFDataPoint);

        SunSpecDataPoint DCWh_SFDataPoint;
        DCWh_SFDataPoint.setName("DCWh_SF");
        DCWh_SFDataPoint.setDescription("Energy scale factor");
        DCWh_SFDataPoint.setMandatory(true);
        DCWh_SFDataPoint.setSize(1);
        DCWh_SFDataPoint.setAddressOffset(6);
        DCWh_SFDataPoint.setBlockOffset(4);
        DCWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCWh_SFDataPoint.name(), DCWh_SFDataPoint);

        SunSpecDataPoint ratingDataPoint;
        ratingDataPoint.setName("DCAMax");
        ratingDataPoint.setLabel("Rating");
        ratingDataPoint.setDescription("Maximum DC Current Rating");
        ratingDataPoint.setUnits("A");
        ratingDataPoint.setSize(1);
        ratingDataPoint.setAddressOffset(7);
        ratingDataPoint.setBlockOffset(5);
        ratingDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ratingDataPoint.name(), ratingDataPoint);

        SunSpecDataPoint nDataPoint;
        nDataPoint.setName("N");
        nDataPoint.setLabel("N");
        nDataPoint.setDescription("Number of Inputs");
        nDataPoint.setSize(1);
        nDataPoint.setAddressOffset(8);
        nDataPoint.setBlockOffset(6);
        nDataPoint.setDataType(SunSpecDataPoint::stringToDataType("count"));
        m_dataPoints.insert(nDataPoint.name(), nDataPoint);

        SunSpecDataPoint eventDataPoint;
        eventDataPoint.setName("Evt");
        eventDataPoint.setLabel("Event");
        eventDataPoint.setDescription("Bitmask value.  Events");
        eventDataPoint.setMandatory(true);
        eventDataPoint.setSize(2);
        eventDataPoint.setAddressOffset(9);
        eventDataPoint.setBlockOffset(7);
        eventDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventDataPoint.name(), eventDataPoint);

        SunSpecDataPoint vendorEventDataPoint;
        vendorEventDataPoint.setName("EvtVnd");
        vendorEventDataPoint.setLabel("Vendor Event");
        vendorEventDataPoint.setDescription("Bitmask value.  Vendor defined events");
        vendorEventDataPoint.setSize(2);
        vendorEventDataPoint.setAddressOffset(11);
        vendorEventDataPoint.setBlockOffset(9);
        vendorEventDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventDataPoint.name(), vendorEventDataPoint);

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("DCA");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("Total measured current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(13);
        ampsDataPoint.setBlockOffset(11);
        ampsDataPoint.setScaleFactorName("DCA_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

        SunSpecDataPoint ampHoursDataPoint;
        ampHoursDataPoint.setName("DCAhr");
        ampHoursDataPoint.setLabel("Amp-hours");
        ampHoursDataPoint.setDescription("Total metered Amp-hours");
        ampHoursDataPoint.setUnits("Ah");
        ampHoursDataPoint.setSize(2);
        ampHoursDataPoint.setAddressOffset(14);
        ampHoursDataPoint.setBlockOffset(12);
        ampHoursDataPoint.setScaleFactorName("DCAhr_SF");
        ampHoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
        m_dataPoints.insert(ampHoursDataPoint.name(), ampHoursDataPoint);

        SunSpecDataPoint voltageDataPoint;
        voltageDataPoint.setName("DCV");
        voltageDataPoint.setLabel("Voltage");
        voltageDataPoint.setDescription("Output Voltage");
        voltageDataPoint.setUnits("V");
        voltageDataPoint.setSize(1);
        voltageDataPoint.setAddressOffset(16);
        voltageDataPoint.setBlockOffset(14);
        voltageDataPoint.setScaleFactorName("DCV_SF");
        voltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

        SunSpecDataPoint tempDataPoint;
        tempDataPoint.setName("Tmp");
        tempDataPoint.setLabel("Temp");
        tempDataPoint.setDescription("Internal operating temperature");
        tempDataPoint.setUnits("C");
        tempDataPoint.setSize(1);
        tempDataPoint.setAddressOffset(17);
        tempDataPoint.setBlockOffset(15);
        tempDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(tempDataPoint.name(), tempDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("DCW");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Output power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(18);
        wattsDataPoint.setBlockOffset(16);
        wattsDataPoint.setScaleFactorName("DCW_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint prDataPoint;
        prDataPoint.setName("DCPR");
        prDataPoint.setLabel("PR");
        prDataPoint.setDescription("DC Performance ratio value");
        prDataPoint.setUnits("Pct");
        prDataPoint.setSize(1);
        prDataPoint.setAddressOffset(19);
        prDataPoint.setBlockOffset(17);
        prDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(prDataPoint.name(), prDataPoint);

        SunSpecDataPoint wattHoursDataPoint;
        wattHoursDataPoint.setName("DCWh");
        wattHoursDataPoint.setLabel("Watt-hours");
        wattHoursDataPoint.setDescription("Output energy");
        wattHoursDataPoint.setUnits("Wh");
        wattHoursDataPoint.setMandatory(true);
        wattHoursDataPoint.setSize(2);
        wattHoursDataPoint.setAddressOffset(20);
        wattHoursDataPoint.setBlockOffset(18);
        wattHoursDataPoint.setScaleFactorName("DCWh_SF");
        wattHoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
        m_dataPoints.insert(wattHoursDataPoint.name(), wattHoursDataPoint);

        break;
    }
    case 403: {
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

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setDescription("Current scale factor");
        DCA_SFDataPoint.setMandatory(true);
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setAddressOffset(2);
        DCA_SFDataPoint.setBlockOffset(0);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCA_SFDataPoint.name(), DCA_SFDataPoint);

        SunSpecDataPoint DCAhr_SFDataPoint;
        DCAhr_SFDataPoint.setName("DCAhr_SF");
        DCAhr_SFDataPoint.setDescription("Amp-hour scale factor");
        DCAhr_SFDataPoint.setSize(1);
        DCAhr_SFDataPoint.setAddressOffset(3);
        DCAhr_SFDataPoint.setBlockOffset(1);
        DCAhr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCAhr_SFDataPoint.name(), DCAhr_SFDataPoint);

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setDescription("Voltage scale factor");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setAddressOffset(4);
        DCV_SFDataPoint.setBlockOffset(2);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCV_SFDataPoint.name(), DCV_SFDataPoint);

        SunSpecDataPoint ratingDataPoint;
        ratingDataPoint.setName("DCAMax");
        ratingDataPoint.setLabel("Rating");
        ratingDataPoint.setDescription("Maximum DC Current Rating");
        ratingDataPoint.setUnits("A");
        ratingDataPoint.setMandatory(true);
        ratingDataPoint.setSize(1);
        ratingDataPoint.setAddressOffset(5);
        ratingDataPoint.setBlockOffset(3);
        ratingDataPoint.setScaleFactorName("DCA_SF");
        ratingDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ratingDataPoint.name(), ratingDataPoint);

        SunSpecDataPoint nDataPoint;
        nDataPoint.setName("N");
        nDataPoint.setLabel("N");
        nDataPoint.setDescription("Number of Inputs");
        nDataPoint.setMandatory(true);
        nDataPoint.setSize(1);
        nDataPoint.setAddressOffset(6);
        nDataPoint.setBlockOffset(4);
        nDataPoint.setDataType(SunSpecDataPoint::stringToDataType("count"));
        m_dataPoints.insert(nDataPoint.name(), nDataPoint);

        SunSpecDataPoint eventDataPoint;
        eventDataPoint.setName("Evt");
        eventDataPoint.setLabel("Event");
        eventDataPoint.setDescription("Bitmask value.  Events");
        eventDataPoint.setMandatory(true);
        eventDataPoint.setSize(2);
        eventDataPoint.setAddressOffset(7);
        eventDataPoint.setBlockOffset(5);
        eventDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventDataPoint.name(), eventDataPoint);

        SunSpecDataPoint vendorEventDataPoint;
        vendorEventDataPoint.setName("EvtVnd");
        vendorEventDataPoint.setLabel("Vendor Event");
        vendorEventDataPoint.setDescription("Bitmask value.  Vendor defined events");
        vendorEventDataPoint.setSize(2);
        vendorEventDataPoint.setAddressOffset(9);
        vendorEventDataPoint.setBlockOffset(7);
        vendorEventDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventDataPoint.name(), vendorEventDataPoint);

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("DCA");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("Total measured current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(11);
        ampsDataPoint.setBlockOffset(9);
        ampsDataPoint.setScaleFactorName("DCA_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

        SunSpecDataPoint ampHoursDataPoint;
        ampHoursDataPoint.setName("DCAhr");
        ampHoursDataPoint.setLabel("Amp-hours");
        ampHoursDataPoint.setDescription("Total metered Amp-hours");
        ampHoursDataPoint.setUnits("Ah");
        ampHoursDataPoint.setSize(2);
        ampHoursDataPoint.setAddressOffset(12);
        ampHoursDataPoint.setBlockOffset(10);
        ampHoursDataPoint.setScaleFactorName("DCAhr_SF");
        ampHoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(ampHoursDataPoint.name(), ampHoursDataPoint);

        SunSpecDataPoint voltageDataPoint;
        voltageDataPoint.setName("DCV");
        voltageDataPoint.setLabel("Voltage");
        voltageDataPoint.setDescription("Output Voltage");
        voltageDataPoint.setUnits("V");
        voltageDataPoint.setSize(1);
        voltageDataPoint.setAddressOffset(14);
        voltageDataPoint.setBlockOffset(12);
        voltageDataPoint.setScaleFactorName("DCV_SF");
        voltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

        SunSpecDataPoint tempDataPoint;
        tempDataPoint.setName("Tmp");
        tempDataPoint.setLabel("Temp");
        tempDataPoint.setDescription("Internal operating temperature");
        tempDataPoint.setUnits("C");
        tempDataPoint.setSize(1);
        tempDataPoint.setAddressOffset(15);
        tempDataPoint.setBlockOffset(13);
        tempDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(tempDataPoint.name(), tempDataPoint);

        SunSpecDataPoint InDCA_SFDataPoint;
        InDCA_SFDataPoint.setName("InDCA_SF");
        InDCA_SFDataPoint.setDescription("Current scale factor for inputs");
        InDCA_SFDataPoint.setSize(1);
        InDCA_SFDataPoint.setAddressOffset(16);
        InDCA_SFDataPoint.setBlockOffset(14);
        InDCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(InDCA_SFDataPoint.name(), InDCA_SFDataPoint);

        SunSpecDataPoint InDCAhr_SFDataPoint;
        InDCAhr_SFDataPoint.setName("InDCAhr_SF");
        InDCAhr_SFDataPoint.setDescription("Amp-hour scale factor for inputs");
        InDCAhr_SFDataPoint.setSize(1);
        InDCAhr_SFDataPoint.setAddressOffset(17);
        InDCAhr_SFDataPoint.setBlockOffset(15);
        InDCAhr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(InDCAhr_SFDataPoint.name(), InDCAhr_SFDataPoint);

        break;
    }
    case 404: {
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

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setDescription("Current scale factor");
        DCA_SFDataPoint.setMandatory(true);
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setAddressOffset(2);
        DCA_SFDataPoint.setBlockOffset(0);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCA_SFDataPoint.name(), DCA_SFDataPoint);

        SunSpecDataPoint DCAhr_SFDataPoint;
        DCAhr_SFDataPoint.setName("DCAhr_SF");
        DCAhr_SFDataPoint.setDescription("Amp-hour scale factor");
        DCAhr_SFDataPoint.setSize(1);
        DCAhr_SFDataPoint.setAddressOffset(3);
        DCAhr_SFDataPoint.setBlockOffset(1);
        DCAhr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCAhr_SFDataPoint.name(), DCAhr_SFDataPoint);

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setDescription("Voltage scale factor");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setAddressOffset(4);
        DCV_SFDataPoint.setBlockOffset(2);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCV_SFDataPoint.name(), DCV_SFDataPoint);

        SunSpecDataPoint DCW_SFDataPoint;
        DCW_SFDataPoint.setName("DCW_SF");
        DCW_SFDataPoint.setDescription("Power scale factor");
        DCW_SFDataPoint.setSize(1);
        DCW_SFDataPoint.setAddressOffset(5);
        DCW_SFDataPoint.setBlockOffset(3);
        DCW_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCW_SFDataPoint.name(), DCW_SFDataPoint);

        SunSpecDataPoint DCWh_SFDataPoint;
        DCWh_SFDataPoint.setName("DCWh_SF");
        DCWh_SFDataPoint.setDescription("Energy scale factor");
        DCWh_SFDataPoint.setSize(1);
        DCWh_SFDataPoint.setAddressOffset(6);
        DCWh_SFDataPoint.setBlockOffset(4);
        DCWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCWh_SFDataPoint.name(), DCWh_SFDataPoint);

        SunSpecDataPoint ratingDataPoint;
        ratingDataPoint.setName("DCAMax");
        ratingDataPoint.setLabel("Rating");
        ratingDataPoint.setDescription("Maximum DC Current Rating");
        ratingDataPoint.setUnits("A");
        ratingDataPoint.setMandatory(true);
        ratingDataPoint.setSize(1);
        ratingDataPoint.setAddressOffset(7);
        ratingDataPoint.setBlockOffset(5);
        ratingDataPoint.setScaleFactorName("DCA_SF");
        ratingDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ratingDataPoint.name(), ratingDataPoint);

        SunSpecDataPoint nDataPoint;
        nDataPoint.setName("N");
        nDataPoint.setLabel("N");
        nDataPoint.setDescription("Number of Inputs");
        nDataPoint.setMandatory(true);
        nDataPoint.setSize(1);
        nDataPoint.setAddressOffset(8);
        nDataPoint.setBlockOffset(6);
        nDataPoint.setDataType(SunSpecDataPoint::stringToDataType("count"));
        m_dataPoints.insert(nDataPoint.name(), nDataPoint);

        SunSpecDataPoint eventDataPoint;
        eventDataPoint.setName("Evt");
        eventDataPoint.setLabel("Event");
        eventDataPoint.setDescription("Bitmask value.  Events");
        eventDataPoint.setMandatory(true);
        eventDataPoint.setSize(2);
        eventDataPoint.setAddressOffset(9);
        eventDataPoint.setBlockOffset(7);
        eventDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventDataPoint.name(), eventDataPoint);

        SunSpecDataPoint vendorEventDataPoint;
        vendorEventDataPoint.setName("EvtVnd");
        vendorEventDataPoint.setLabel("Vendor Event");
        vendorEventDataPoint.setDescription("Bitmask value.  Vendor defined events");
        vendorEventDataPoint.setSize(2);
        vendorEventDataPoint.setAddressOffset(11);
        vendorEventDataPoint.setBlockOffset(9);
        vendorEventDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventDataPoint.name(), vendorEventDataPoint);

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("DCA");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("Total measured current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(13);
        ampsDataPoint.setBlockOffset(11);
        ampsDataPoint.setScaleFactorName("DCA_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

        SunSpecDataPoint ampHoursDataPoint;
        ampHoursDataPoint.setName("DCAhr");
        ampHoursDataPoint.setLabel("Amp-hours");
        ampHoursDataPoint.setDescription("Total metered Amp-hours");
        ampHoursDataPoint.setUnits("Ah");
        ampHoursDataPoint.setSize(2);
        ampHoursDataPoint.setAddressOffset(14);
        ampHoursDataPoint.setBlockOffset(12);
        ampHoursDataPoint.setScaleFactorName("DCAhr_SF");
        ampHoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(ampHoursDataPoint.name(), ampHoursDataPoint);

        SunSpecDataPoint voltageDataPoint;
        voltageDataPoint.setName("DCV");
        voltageDataPoint.setLabel("Voltage");
        voltageDataPoint.setDescription("Output Voltage");
        voltageDataPoint.setUnits("V");
        voltageDataPoint.setSize(1);
        voltageDataPoint.setAddressOffset(16);
        voltageDataPoint.setBlockOffset(14);
        voltageDataPoint.setScaleFactorName("DCV_SF");
        voltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

        SunSpecDataPoint tempDataPoint;
        tempDataPoint.setName("Tmp");
        tempDataPoint.setLabel("Temp");
        tempDataPoint.setDescription("Internal operating temperature");
        tempDataPoint.setUnits("C");
        tempDataPoint.setSize(1);
        tempDataPoint.setAddressOffset(17);
        tempDataPoint.setBlockOffset(15);
        tempDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(tempDataPoint.name(), tempDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("DCW");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Output power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(18);
        wattsDataPoint.setBlockOffset(16);
        wattsDataPoint.setScaleFactorName("DCW_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint prDataPoint;
        prDataPoint.setName("DCPR");
        prDataPoint.setLabel("PR");
        prDataPoint.setDescription("DC Performance ratio value");
        prDataPoint.setUnits("Pct");
        prDataPoint.setSize(1);
        prDataPoint.setAddressOffset(19);
        prDataPoint.setBlockOffset(17);
        prDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(prDataPoint.name(), prDataPoint);

        SunSpecDataPoint wattHoursDataPoint;
        wattHoursDataPoint.setName("DCWh");
        wattHoursDataPoint.setLabel("Watt-hours");
        wattHoursDataPoint.setDescription("Output energy");
        wattHoursDataPoint.setUnits("Wh");
        wattHoursDataPoint.setSize(2);
        wattHoursDataPoint.setAddressOffset(20);
        wattHoursDataPoint.setBlockOffset(18);
        wattHoursDataPoint.setScaleFactorName("DCWh_SF");
        wattHoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(wattHoursDataPoint.name(), wattHoursDataPoint);

        SunSpecDataPoint InDCA_SFDataPoint;
        InDCA_SFDataPoint.setName("InDCA_SF");
        InDCA_SFDataPoint.setDescription("Current scale factor for inputs");
        InDCA_SFDataPoint.setSize(1);
        InDCA_SFDataPoint.setAddressOffset(22);
        InDCA_SFDataPoint.setBlockOffset(20);
        InDCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(InDCA_SFDataPoint.name(), InDCA_SFDataPoint);

        SunSpecDataPoint InDCAhr_SFDataPoint;
        InDCAhr_SFDataPoint.setName("InDCAhr_SF");
        InDCAhr_SFDataPoint.setDescription("Amp-hour scale factor for inputs");
        InDCAhr_SFDataPoint.setSize(1);
        InDCAhr_SFDataPoint.setAddressOffset(23);
        InDCAhr_SFDataPoint.setBlockOffset(21);
        InDCAhr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(InDCAhr_SFDataPoint.name(), InDCAhr_SFDataPoint);

        SunSpecDataPoint InDCV_SFDataPoint;
        InDCV_SFDataPoint.setName("InDCV_SF");
        InDCV_SFDataPoint.setDescription("Voltage scale factor for inputs");
        InDCV_SFDataPoint.setSize(1);
        InDCV_SFDataPoint.setAddressOffset(24);
        InDCV_SFDataPoint.setBlockOffset(22);
        InDCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(InDCV_SFDataPoint.name(), InDCV_SFDataPoint);

        SunSpecDataPoint InDCW_SFDataPoint;
        InDCW_SFDataPoint.setName("InDCW_SF");
        InDCW_SFDataPoint.setDescription("Power scale factor for inputs");
        InDCW_SFDataPoint.setSize(1);
        InDCW_SFDataPoint.setAddressOffset(25);
        InDCW_SFDataPoint.setBlockOffset(23);
        InDCW_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(InDCW_SFDataPoint.name(), InDCW_SFDataPoint);

        SunSpecDataPoint InDCWh_SFDataPoint;
        InDCWh_SFDataPoint.setName("InDCWh_SF");
        InDCWh_SFDataPoint.setDescription("Energy scale factor for inputs");
        InDCWh_SFDataPoint.setSize(1);
        InDCWh_SFDataPoint.setAddressOffset(26);
        InDCWh_SFDataPoint.setBlockOffset(24);
        InDCWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(InDCWh_SFDataPoint.name(), InDCWh_SFDataPoint);

        break;
    }
    default:
        break;
    }
}

