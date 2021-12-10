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

#include "sunspecstringcombineradvancedmodel.h"
#include "sunspecconnection.h"

SunSpecStringCombinerAdvancedModelRepeatingBlock::SunSpecStringCombinerAdvancedModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecStringCombinerAdvancedModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecStringCombinerAdvancedModelRepeatingBlock::name() const
{
    return "string";
}

SunSpecStringCombinerAdvancedModel *SunSpecStringCombinerAdvancedModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecStringCombinerAdvancedModelRepeatingBlock::id() const
{
    return m_id;
}
SunSpecStringCombinerAdvancedModelRepeatingBlock::InevtFlags SunSpecStringCombinerAdvancedModelRepeatingBlock::inputEvent() const
{
    return m_inputEvent;
}
quint32 SunSpecStringCombinerAdvancedModelRepeatingBlock::inputEventVendor() const
{
    return m_inputEventVendor;
}
float SunSpecStringCombinerAdvancedModelRepeatingBlock::amps() const
{
    return m_amps;
}
quint32 SunSpecStringCombinerAdvancedModelRepeatingBlock::ampHours() const
{
    return m_ampHours;
}
float SunSpecStringCombinerAdvancedModelRepeatingBlock::voltage() const
{
    return m_voltage;
}
float SunSpecStringCombinerAdvancedModelRepeatingBlock::watts() const
{
    return m_watts;
}
quint32 SunSpecStringCombinerAdvancedModelRepeatingBlock::wattHours() const
{
    return m_wattHours;
}
quint16 SunSpecStringCombinerAdvancedModelRepeatingBlock::pr() const
{
    return m_pr;
}
quint16 SunSpecStringCombinerAdvancedModelRepeatingBlock::n() const
{
    return m_n;
}

void SunSpecStringCombinerAdvancedModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint idDataPoint;
    idDataPoint.setName("InID");
    idDataPoint.setLabel("ID");
    idDataPoint.setDescription("Uniquely identifies this input set");
    idDataPoint.setMandatory(true);
    idDataPoint.setSize(1);
    idDataPoint.setAddressOffset(0);
    idDataPoint.setSunSpecDataType("uint16");
    idDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(idDataPoint.name(), idDataPoint);

    SunSpecDataPoint inputEventDataPoint;
    inputEventDataPoint.setName("InEvt");
    inputEventDataPoint.setLabel("Input Event");
    inputEventDataPoint.setDescription("String Input Event Flags");
    inputEventDataPoint.setMandatory(true);
    inputEventDataPoint.setSize(2);
    inputEventDataPoint.setAddressOffset(1);
    inputEventDataPoint.setSunSpecDataType("bitfield32");
    inputEventDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inputEventDataPoint.name(), inputEventDataPoint);

    SunSpecDataPoint inputEventVendorDataPoint;
    inputEventVendorDataPoint.setName("InEvtVnd");
    inputEventVendorDataPoint.setLabel("Input Event Vendor");
    inputEventVendorDataPoint.setDescription("String Input Vendor Event Flags");
    inputEventVendorDataPoint.setSize(2);
    inputEventVendorDataPoint.setAddressOffset(3);
    inputEventVendorDataPoint.setBlockOffset(1);
    inputEventVendorDataPoint.setSunSpecDataType("bitfield32");
    inputEventVendorDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inputEventVendorDataPoint.name(), inputEventVendorDataPoint);

    SunSpecDataPoint ampsDataPoint;
    ampsDataPoint.setName("InDCA");
    ampsDataPoint.setLabel("Amps");
    ampsDataPoint.setDescription("String Input Current");
    ampsDataPoint.setUnits("A");
    ampsDataPoint.setMandatory(true);
    ampsDataPoint.setSize(1);
    ampsDataPoint.setAddressOffset(5);
    ampsDataPoint.setBlockOffset(3);
    ampsDataPoint.setScaleFactorName("InDCA_SF");
    ampsDataPoint.setSunSpecDataType("int16");
    ampsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

    SunSpecDataPoint ampHoursDataPoint;
    ampHoursDataPoint.setName("InDCAhr");
    ampHoursDataPoint.setLabel("Amp-hours");
    ampHoursDataPoint.setDescription("String Input Amp-Hours");
    ampHoursDataPoint.setUnits("Ah");
    ampHoursDataPoint.setSize(2);
    ampHoursDataPoint.setAddressOffset(6);
    ampHoursDataPoint.setBlockOffset(4);
    ampHoursDataPoint.setScaleFactorName("InDCAhr_SF");
    ampHoursDataPoint.setSunSpecDataType("acc32");
    ampHoursDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ampHoursDataPoint.name(), ampHoursDataPoint);

    SunSpecDataPoint voltageDataPoint;
    voltageDataPoint.setName("InDCV");
    voltageDataPoint.setLabel("Voltage");
    voltageDataPoint.setDescription("String Input Voltage");
    voltageDataPoint.setUnits("V");
    voltageDataPoint.setSize(1);
    voltageDataPoint.setAddressOffset(8);
    voltageDataPoint.setBlockOffset(6);
    voltageDataPoint.setScaleFactorName("InDCV_SF");
    voltageDataPoint.setSunSpecDataType("int16");
    voltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

    SunSpecDataPoint wattsDataPoint;
    wattsDataPoint.setName("InDCW");
    wattsDataPoint.setLabel("Watts");
    wattsDataPoint.setDescription("String Input Power");
    wattsDataPoint.setUnits("W");
    wattsDataPoint.setSize(1);
    wattsDataPoint.setAddressOffset(9);
    wattsDataPoint.setBlockOffset(7);
    wattsDataPoint.setScaleFactorName("InDCW_SF");
    wattsDataPoint.setSunSpecDataType("int16");
    wattsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

    SunSpecDataPoint wattHoursDataPoint;
    wattHoursDataPoint.setName("InDCWh");
    wattHoursDataPoint.setLabel("Watt-hours");
    wattHoursDataPoint.setDescription("String Input Energy");
    wattHoursDataPoint.setUnits("Wh");
    wattHoursDataPoint.setSize(2);
    wattHoursDataPoint.setAddressOffset(10);
    wattHoursDataPoint.setBlockOffset(8);
    wattHoursDataPoint.setScaleFactorName("InDCWh_SF");
    wattHoursDataPoint.setSunSpecDataType("acc32");
    wattHoursDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wattHoursDataPoint.name(), wattHoursDataPoint);

    SunSpecDataPoint prDataPoint;
    prDataPoint.setName("InDCPR");
    prDataPoint.setLabel("PR");
    prDataPoint.setDescription("String Performance Ratio");
    prDataPoint.setUnits("Pct");
    prDataPoint.setSize(1);
    prDataPoint.setAddressOffset(12);
    prDataPoint.setBlockOffset(10);
    prDataPoint.setSunSpecDataType("uint16");
    prDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(prDataPoint.name(), prDataPoint);

    SunSpecDataPoint nDataPoint;
    nDataPoint.setName("InN");
    nDataPoint.setLabel("N");
    nDataPoint.setDescription("Number of modules in this input string");
    nDataPoint.setSize(1);
    nDataPoint.setAddressOffset(13);
    nDataPoint.setBlockOffset(11);
    nDataPoint.setSunSpecDataType("uint16");
    nDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nDataPoint.name(), nDataPoint);

}

void SunSpecStringCombinerAdvancedModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("InID").isValid())
        m_id = m_dataPoints.value("InID").toUInt16();

    if (m_dataPoints.value("InEvt").isValid())
        m_inputEvent = static_cast<InevtFlags>(m_dataPoints.value("InEvt").toUInt32());

    if (m_dataPoints.value("InEvtVnd").isValid())
        m_inputEventVendor = m_dataPoints.value("InEvtVnd").toUInt32();

    if (m_dataPoints.value("InDCA").isValid())
        m_amps = m_dataPoints.value("InDCA").toFloatWithSSF(m_parentModel->inDCA_SF());

    if (m_dataPoints.value("InDCAhr").isValid())
        m_ampHours = m_dataPoints.value("InDCAhr").toFloatWithSSF(m_parentModel->inDCAhr_SF());

    if (m_dataPoints.value("InDCV").isValid())
        m_voltage = m_dataPoints.value("InDCV").toFloatWithSSF(m_parentModel->inDCV_SF());

    if (m_dataPoints.value("InDCW").isValid())
        m_watts = m_dataPoints.value("InDCW").toFloatWithSSF(m_parentModel->inDCW_SF());

    if (m_dataPoints.value("InDCWh").isValid())
        m_wattHours = m_dataPoints.value("InDCWh").toFloatWithSSF(m_parentModel->inDCWh_SF());

    if (m_dataPoints.value("InDCPR").isValid())
        m_pr = m_dataPoints.value("InDCPR").toUInt16();

    if (m_dataPoints.value("InN").isValid())
        m_n = m_dataPoints.value("InN").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecStringCombinerAdvancedModel::SunSpecStringCombinerAdvancedModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 404, modelLength, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecStringCombinerAdvancedModel::~SunSpecStringCombinerAdvancedModel()
{

}

QString SunSpecStringCombinerAdvancedModel::name() const
{
    return "string_combiner";
}

QString SunSpecStringCombinerAdvancedModel::description() const
{
    return "An advanced string combiner including voltage and energy measurements";
}

QString SunSpecStringCombinerAdvancedModel::label() const
{
    return "String Combiner (Advanced)";
}

qint16 SunSpecStringCombinerAdvancedModel::dCA_SF() const
{
    return m_dCA_SF;
}
qint16 SunSpecStringCombinerAdvancedModel::dCAhr_SF() const
{
    return m_dCAhr_SF;
}
qint16 SunSpecStringCombinerAdvancedModel::dCV_SF() const
{
    return m_dCV_SF;
}
qint16 SunSpecStringCombinerAdvancedModel::dCW_SF() const
{
    return m_dCW_SF;
}
qint16 SunSpecStringCombinerAdvancedModel::dCWh_SF() const
{
    return m_dCWh_SF;
}
float SunSpecStringCombinerAdvancedModel::rating() const
{
    return m_rating;
}
int SunSpecStringCombinerAdvancedModel::n() const
{
    return m_n;
}
SunSpecStringCombinerAdvancedModel::EvtFlags SunSpecStringCombinerAdvancedModel::eventFlags() const
{
    return m_eventFlags;
}
quint32 SunSpecStringCombinerAdvancedModel::vendorEvent() const
{
    return m_vendorEvent;
}
float SunSpecStringCombinerAdvancedModel::amps() const
{
    return m_amps;
}
quint32 SunSpecStringCombinerAdvancedModel::ampHours() const
{
    return m_ampHours;
}
float SunSpecStringCombinerAdvancedModel::voltage() const
{
    return m_voltage;
}
qint16 SunSpecStringCombinerAdvancedModel::temp() const
{
    return m_temp;
}
float SunSpecStringCombinerAdvancedModel::watts() const
{
    return m_watts;
}
qint16 SunSpecStringCombinerAdvancedModel::pr() const
{
    return m_pr;
}
quint32 SunSpecStringCombinerAdvancedModel::wattHours() const
{
    return m_wattHours;
}
qint16 SunSpecStringCombinerAdvancedModel::inDCA_SF() const
{
    return m_inDCA_SF;
}
qint16 SunSpecStringCombinerAdvancedModel::inDCAhr_SF() const
{
    return m_inDCAhr_SF;
}
qint16 SunSpecStringCombinerAdvancedModel::inDCV_SF() const
{
    return m_inDCV_SF;
}
qint16 SunSpecStringCombinerAdvancedModel::inDCW_SF() const
{
    return m_inDCW_SF;
}
qint16 SunSpecStringCombinerAdvancedModel::inDCWh_SF() const
{
    return m_inDCWh_SF;
}
void SunSpecStringCombinerAdvancedModel::initDataPoints()
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

    SunSpecDataPoint dCA_SFDataPoint;
    dCA_SFDataPoint.setName("DCA_SF");
    dCA_SFDataPoint.setDescription("Current scale factor");
    dCA_SFDataPoint.setMandatory(true);
    dCA_SFDataPoint.setSize(1);
    dCA_SFDataPoint.setAddressOffset(2);
    dCA_SFDataPoint.setBlockOffset(0);
    dCA_SFDataPoint.setSunSpecDataType("sunssf");
    dCA_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dCA_SFDataPoint.name(), dCA_SFDataPoint);

    SunSpecDataPoint dCAhr_SFDataPoint;
    dCAhr_SFDataPoint.setName("DCAhr_SF");
    dCAhr_SFDataPoint.setDescription("Amp-hour scale factor");
    dCAhr_SFDataPoint.setSize(1);
    dCAhr_SFDataPoint.setAddressOffset(3);
    dCAhr_SFDataPoint.setBlockOffset(1);
    dCAhr_SFDataPoint.setSunSpecDataType("sunssf");
    dCAhr_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dCAhr_SFDataPoint.name(), dCAhr_SFDataPoint);

    SunSpecDataPoint dCV_SFDataPoint;
    dCV_SFDataPoint.setName("DCV_SF");
    dCV_SFDataPoint.setDescription("Voltage scale factor");
    dCV_SFDataPoint.setSize(1);
    dCV_SFDataPoint.setAddressOffset(4);
    dCV_SFDataPoint.setBlockOffset(2);
    dCV_SFDataPoint.setSunSpecDataType("sunssf");
    dCV_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dCV_SFDataPoint.name(), dCV_SFDataPoint);

    SunSpecDataPoint dCW_SFDataPoint;
    dCW_SFDataPoint.setName("DCW_SF");
    dCW_SFDataPoint.setDescription("Power scale factor");
    dCW_SFDataPoint.setSize(1);
    dCW_SFDataPoint.setAddressOffset(5);
    dCW_SFDataPoint.setBlockOffset(3);
    dCW_SFDataPoint.setSunSpecDataType("sunssf");
    dCW_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dCW_SFDataPoint.name(), dCW_SFDataPoint);

    SunSpecDataPoint dCWh_SFDataPoint;
    dCWh_SFDataPoint.setName("DCWh_SF");
    dCWh_SFDataPoint.setDescription("Energy scale factor");
    dCWh_SFDataPoint.setSize(1);
    dCWh_SFDataPoint.setAddressOffset(6);
    dCWh_SFDataPoint.setBlockOffset(4);
    dCWh_SFDataPoint.setSunSpecDataType("sunssf");
    dCWh_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dCWh_SFDataPoint.name(), dCWh_SFDataPoint);

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
    ratingDataPoint.setSunSpecDataType("uint16");
    ratingDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ratingDataPoint.name(), ratingDataPoint);

    SunSpecDataPoint nDataPoint;
    nDataPoint.setName("N");
    nDataPoint.setLabel("N");
    nDataPoint.setDescription("Number of Inputs");
    nDataPoint.setMandatory(true);
    nDataPoint.setSize(1);
    nDataPoint.setAddressOffset(8);
    nDataPoint.setBlockOffset(6);
    nDataPoint.setSunSpecDataType("count");
    nDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nDataPoint.name(), nDataPoint);

    SunSpecDataPoint eventFlagsDataPoint;
    eventFlagsDataPoint.setName("Evt");
    eventFlagsDataPoint.setLabel("Event");
    eventFlagsDataPoint.setDescription("Bitmask value.  Events");
    eventFlagsDataPoint.setMandatory(true);
    eventFlagsDataPoint.setSize(2);
    eventFlagsDataPoint.setAddressOffset(9);
    eventFlagsDataPoint.setBlockOffset(7);
    eventFlagsDataPoint.setSunSpecDataType("bitfield32");
    eventFlagsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(eventFlagsDataPoint.name(), eventFlagsDataPoint);

    SunSpecDataPoint vendorEventDataPoint;
    vendorEventDataPoint.setName("EvtVnd");
    vendorEventDataPoint.setLabel("Vendor Event");
    vendorEventDataPoint.setDescription("Bitmask value.  Vendor defined events");
    vendorEventDataPoint.setSize(2);
    vendorEventDataPoint.setAddressOffset(11);
    vendorEventDataPoint.setBlockOffset(9);
    vendorEventDataPoint.setSunSpecDataType("bitfield32");
    vendorEventDataPoint.setByteOrder(m_byteOrder);
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
    ampsDataPoint.setSunSpecDataType("int16");
    ampsDataPoint.setByteOrder(m_byteOrder);
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
    ampHoursDataPoint.setSunSpecDataType("acc32");
    ampHoursDataPoint.setByteOrder(m_byteOrder);
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
    voltageDataPoint.setSunSpecDataType("int16");
    voltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

    SunSpecDataPoint tempDataPoint;
    tempDataPoint.setName("Tmp");
    tempDataPoint.setLabel("Temp");
    tempDataPoint.setDescription("Internal operating temperature");
    tempDataPoint.setUnits("C");
    tempDataPoint.setSize(1);
    tempDataPoint.setAddressOffset(17);
    tempDataPoint.setBlockOffset(15);
    tempDataPoint.setSunSpecDataType("int16");
    tempDataPoint.setByteOrder(m_byteOrder);
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
    wattsDataPoint.setSunSpecDataType("int16");
    wattsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

    SunSpecDataPoint prDataPoint;
    prDataPoint.setName("DCPR");
    prDataPoint.setLabel("PR");
    prDataPoint.setDescription("DC Performance ratio value");
    prDataPoint.setUnits("Pct");
    prDataPoint.setSize(1);
    prDataPoint.setAddressOffset(19);
    prDataPoint.setBlockOffset(17);
    prDataPoint.setSunSpecDataType("int16");
    prDataPoint.setByteOrder(m_byteOrder);
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
    wattHoursDataPoint.setSunSpecDataType("acc32");
    wattHoursDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wattHoursDataPoint.name(), wattHoursDataPoint);

    SunSpecDataPoint inDCA_SFDataPoint;
    inDCA_SFDataPoint.setName("InDCA_SF");
    inDCA_SFDataPoint.setDescription("Current scale factor for inputs");
    inDCA_SFDataPoint.setSize(1);
    inDCA_SFDataPoint.setAddressOffset(22);
    inDCA_SFDataPoint.setBlockOffset(20);
    inDCA_SFDataPoint.setSunSpecDataType("sunssf");
    inDCA_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inDCA_SFDataPoint.name(), inDCA_SFDataPoint);

    SunSpecDataPoint inDCAhr_SFDataPoint;
    inDCAhr_SFDataPoint.setName("InDCAhr_SF");
    inDCAhr_SFDataPoint.setDescription("Amp-hour scale factor for inputs");
    inDCAhr_SFDataPoint.setSize(1);
    inDCAhr_SFDataPoint.setAddressOffset(23);
    inDCAhr_SFDataPoint.setBlockOffset(21);
    inDCAhr_SFDataPoint.setSunSpecDataType("sunssf");
    inDCAhr_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inDCAhr_SFDataPoint.name(), inDCAhr_SFDataPoint);

    SunSpecDataPoint inDCV_SFDataPoint;
    inDCV_SFDataPoint.setName("InDCV_SF");
    inDCV_SFDataPoint.setDescription("Voltage scale factor for inputs");
    inDCV_SFDataPoint.setSize(1);
    inDCV_SFDataPoint.setAddressOffset(24);
    inDCV_SFDataPoint.setBlockOffset(22);
    inDCV_SFDataPoint.setSunSpecDataType("sunssf");
    inDCV_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inDCV_SFDataPoint.name(), inDCV_SFDataPoint);

    SunSpecDataPoint inDCW_SFDataPoint;
    inDCW_SFDataPoint.setName("InDCW_SF");
    inDCW_SFDataPoint.setDescription("Power scale factor for inputs");
    inDCW_SFDataPoint.setSize(1);
    inDCW_SFDataPoint.setAddressOffset(25);
    inDCW_SFDataPoint.setBlockOffset(23);
    inDCW_SFDataPoint.setSunSpecDataType("sunssf");
    inDCW_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inDCW_SFDataPoint.name(), inDCW_SFDataPoint);

    SunSpecDataPoint inDCWh_SFDataPoint;
    inDCWh_SFDataPoint.setName("InDCWh_SF");
    inDCWh_SFDataPoint.setDescription("Energy scale factor for inputs");
    inDCWh_SFDataPoint.setSize(1);
    inDCWh_SFDataPoint.setAddressOffset(26);
    inDCWh_SFDataPoint.setBlockOffset(24);
    inDCWh_SFDataPoint.setSunSpecDataType("sunssf");
    inDCWh_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inDCWh_SFDataPoint.name(), inDCWh_SFDataPoint);

}

void SunSpecStringCombinerAdvancedModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("DCA_SF").isValid())
        m_dCA_SF = m_dataPoints.value("DCA_SF").toInt16();

    if (m_dataPoints.value("DCAhr_SF").isValid())
        m_dCAhr_SF = m_dataPoints.value("DCAhr_SF").toInt16();

    if (m_dataPoints.value("DCV_SF").isValid())
        m_dCV_SF = m_dataPoints.value("DCV_SF").toInt16();

    if (m_dataPoints.value("DCW_SF").isValid())
        m_dCW_SF = m_dataPoints.value("DCW_SF").toInt16();

    if (m_dataPoints.value("DCWh_SF").isValid())
        m_dCWh_SF = m_dataPoints.value("DCWh_SF").toInt16();

    if (m_dataPoints.value("InDCA_SF").isValid())
        m_inDCA_SF = m_dataPoints.value("InDCA_SF").toInt16();

    if (m_dataPoints.value("InDCAhr_SF").isValid())
        m_inDCAhr_SF = m_dataPoints.value("InDCAhr_SF").toInt16();

    if (m_dataPoints.value("InDCV_SF").isValid())
        m_inDCV_SF = m_dataPoints.value("InDCV_SF").toInt16();

    if (m_dataPoints.value("InDCW_SF").isValid())
        m_inDCW_SF = m_dataPoints.value("InDCW_SF").toInt16();

    if (m_dataPoints.value("InDCWh_SF").isValid())
        m_inDCWh_SF = m_dataPoints.value("InDCWh_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("DCA_SF").isValid())
        m_dCA_SF = m_dataPoints.value("DCA_SF").toInt16();

    if (m_dataPoints.value("DCAhr_SF").isValid())
        m_dCAhr_SF = m_dataPoints.value("DCAhr_SF").toInt16();

    if (m_dataPoints.value("DCV_SF").isValid())
        m_dCV_SF = m_dataPoints.value("DCV_SF").toInt16();

    if (m_dataPoints.value("DCW_SF").isValid())
        m_dCW_SF = m_dataPoints.value("DCW_SF").toInt16();

    if (m_dataPoints.value("DCWh_SF").isValid())
        m_dCWh_SF = m_dataPoints.value("DCWh_SF").toInt16();

    if (m_dataPoints.value("DCAMax").isValid())
        m_rating = m_dataPoints.value("DCAMax").toFloatWithSSF(m_dCA_SF);

    if (m_dataPoints.value("N").isValid())
        m_n = m_dataPoints.value("N").toUInt16();

    if (m_dataPoints.value("Evt").isValid())
        m_eventFlags = static_cast<EvtFlags>(m_dataPoints.value("Evt").toUInt32());

    if (m_dataPoints.value("EvtVnd").isValid())
        m_vendorEvent = m_dataPoints.value("EvtVnd").toUInt32();

    if (m_dataPoints.value("DCA").isValid())
        m_amps = m_dataPoints.value("DCA").toFloatWithSSF(m_dCA_SF);

    if (m_dataPoints.value("DCAhr").isValid())
        m_ampHours = m_dataPoints.value("DCAhr").toFloatWithSSF(m_dCAhr_SF);

    if (m_dataPoints.value("DCV").isValid())
        m_voltage = m_dataPoints.value("DCV").toFloatWithSSF(m_dCV_SF);

    if (m_dataPoints.value("Tmp").isValid())
        m_temp = m_dataPoints.value("Tmp").toInt16();

    if (m_dataPoints.value("DCW").isValid())
        m_watts = m_dataPoints.value("DCW").toFloatWithSSF(m_dCW_SF);

    if (m_dataPoints.value("DCPR").isValid())
        m_pr = m_dataPoints.value("DCPR").toInt16();

    if (m_dataPoints.value("DCWh").isValid())
        m_wattHours = m_dataPoints.value("DCWh").toFloatWithSSF(m_dCWh_SF);

    if (m_dataPoints.value("InDCA_SF").isValid())
        m_inDCA_SF = m_dataPoints.value("InDCA_SF").toInt16();

    if (m_dataPoints.value("InDCAhr_SF").isValid())
        m_inDCAhr_SF = m_dataPoints.value("InDCAhr_SF").toInt16();

    if (m_dataPoints.value("InDCV_SF").isValid())
        m_inDCV_SF = m_dataPoints.value("InDCV_SF").toInt16();

    if (m_dataPoints.value("InDCW_SF").isValid())
        m_inDCW_SF = m_dataPoints.value("InDCW_SF").toInt16();

    if (m_dataPoints.value("InDCWh_SF").isValid())
        m_inDCWh_SF = m_dataPoints.value("InDCWh_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecStringCombinerAdvancedModel *model)
{
    debug.nospace().noquote() << "SunSpecStringCombinerAdvancedModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCAMax") << "-->";
    if (model->dataPoints().value("DCAMax").isValid()) {
        debug.nospace().noquote() << model->rating() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("N") << "-->";
    if (model->dataPoints().value("N").isValid()) {
        debug.nospace().noquote() << model->n() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "-->";
    if (model->dataPoints().value("Evt").isValid()) {
        debug.nospace().noquote() << model->eventFlags() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd") << "-->";
    if (model->dataPoints().value("EvtVnd").isValid()) {
        debug.nospace().noquote() << model->vendorEvent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCA") << "-->";
    if (model->dataPoints().value("DCA").isValid()) {
        debug.nospace().noquote() << model->amps() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCAhr") << "-->";
    if (model->dataPoints().value("DCAhr").isValid()) {
        debug.nospace().noquote() << model->ampHours() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCV") << "-->";
    if (model->dataPoints().value("DCV").isValid()) {
        debug.nospace().noquote() << model->voltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Tmp") << "-->";
    if (model->dataPoints().value("Tmp").isValid()) {
        debug.nospace().noquote() << model->temp() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCW") << "-->";
    if (model->dataPoints().value("DCW").isValid()) {
        debug.nospace().noquote() << model->watts() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCPR") << "-->";
    if (model->dataPoints().value("DCPR").isValid()) {
        debug.nospace().noquote() << model->pr() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCWh") << "-->";
    if (model->dataPoints().value("DCWh").isValid()) {
        debug.nospace().noquote() << model->wattHours() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
