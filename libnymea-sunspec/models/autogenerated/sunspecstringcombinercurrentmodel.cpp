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

#include "sunspecstringcombinercurrentmodel.h"

SunSpecStringCombinerCurrentModel::SunSpecStringCombinerCurrentModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, 403, 16, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecStringCombinerCurrentModel::~SunSpecStringCombinerCurrentModel()
{

}

QString SunSpecStringCombinerCurrentModel::name() const
{
    return "string_combiner";
}

QString SunSpecStringCombinerCurrentModel::description() const
{
    return "A basic string combiner model";
}

QString SunSpecStringCombinerCurrentModel::label() const
{
    return "String Combiner (Current)";
}

float SunSpecStringCombinerCurrentModel::rating() const
{
    return m_rating;
}
int SunSpecStringCombinerCurrentModel::n() const
{
    return m_n;
}
SunSpecStringCombinerCurrentModel::EvtFlags SunSpecStringCombinerCurrentModel::event() const
{
    return m_event;
}
quint32 SunSpecStringCombinerCurrentModel::vendorEvent() const
{
    return m_vendorEvent;
}
float SunSpecStringCombinerCurrentModel::amps() const
{
    return m_amps;
}
quint32 SunSpecStringCombinerCurrentModel::ampHours() const
{
    return m_ampHours;
}
float SunSpecStringCombinerCurrentModel::voltage() const
{
    return m_voltage;
}
qint16 SunSpecStringCombinerCurrentModel::temp() const
{
    return m_temp;
}
void SunSpecStringCombinerCurrentModel::processBlockData()
{
    // Scale factors
    m_dCA_SF = m_dataPoints.value("DCA_SF").toInt16();
    m_dCAhr_SF = m_dataPoints.value("DCAhr_SF").toInt16();
    m_dCV_SF = m_dataPoints.value("DCV_SF").toInt16();
    m_inDCA_SF = m_dataPoints.value("InDCA_SF").toInt16();
    m_inDCAhr_SF = m_dataPoints.value("InDCAhr_SF").toInt16();

    // Update properties according to the data point type
    m_modelId = m_dataPoints.value("ID").toUInt16();
    m_modelLength = m_dataPoints.value("L").toUInt16();
    m_rating = m_dataPoints.value("DCAMax").toFloatWithSSF(m_dCA_SF);
    m_n = m_dataPoints.value("N").toUInt16();
    m_event = static_cast<EvtFlags>(m_dataPoints.value("Evt").toUInt32());
    m_vendorEvent = m_dataPoints.value("EvtVnd").toUInt32();
    m_amps = m_dataPoints.value("DCA").toFloatWithSSF(m_dCA_SF);
    m_ampHours = m_dataPoints.value("DCAhr").toFloatWithSSF(m_dCAhr_SF);
    m_voltage = m_dataPoints.value("DCV").toFloatWithSSF(m_dCV_SF);
    m_temp = m_dataPoints.value("Tmp").toInt16();
}

void SunSpecStringCombinerCurrentModel::initDataPoints()
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

    SunSpecDataPoint dCA_SFDataPoint;
    dCA_SFDataPoint.setName("DCA_SF");
    dCA_SFDataPoint.setDescription("Current scale factor");
    dCA_SFDataPoint.setMandatory(true);
    dCA_SFDataPoint.setSize(1);
    dCA_SFDataPoint.setAddressOffset(2);
    dCA_SFDataPoint.setBlockOffset(0);
    dCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(dCA_SFDataPoint.name(), dCA_SFDataPoint);

    SunSpecDataPoint dCAhr_SFDataPoint;
    dCAhr_SFDataPoint.setName("DCAhr_SF");
    dCAhr_SFDataPoint.setDescription("Amp-hour scale factor");
    dCAhr_SFDataPoint.setSize(1);
    dCAhr_SFDataPoint.setAddressOffset(3);
    dCAhr_SFDataPoint.setBlockOffset(1);
    dCAhr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(dCAhr_SFDataPoint.name(), dCAhr_SFDataPoint);

    SunSpecDataPoint dCV_SFDataPoint;
    dCV_SFDataPoint.setName("DCV_SF");
    dCV_SFDataPoint.setDescription("Voltage scale factor");
    dCV_SFDataPoint.setSize(1);
    dCV_SFDataPoint.setAddressOffset(4);
    dCV_SFDataPoint.setBlockOffset(2);
    dCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(dCV_SFDataPoint.name(), dCV_SFDataPoint);

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

    SunSpecDataPoint inDCA_SFDataPoint;
    inDCA_SFDataPoint.setName("InDCA_SF");
    inDCA_SFDataPoint.setDescription("Current scale factor for inputs");
    inDCA_SFDataPoint.setSize(1);
    inDCA_SFDataPoint.setAddressOffset(16);
    inDCA_SFDataPoint.setBlockOffset(14);
    inDCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(inDCA_SFDataPoint.name(), inDCA_SFDataPoint);

    SunSpecDataPoint inDCAhr_SFDataPoint;
    inDCAhr_SFDataPoint.setName("InDCAhr_SF");
    inDCAhr_SFDataPoint.setDescription("Amp-hour scale factor for inputs");
    inDCAhr_SFDataPoint.setSize(1);
    inDCAhr_SFDataPoint.setAddressOffset(17);
    inDCAhr_SFDataPoint.setBlockOffset(15);
    inDCAhr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(inDCAhr_SFDataPoint.name(), inDCAhr_SFDataPoint);

}

