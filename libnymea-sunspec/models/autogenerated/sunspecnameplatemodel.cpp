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

#include "sunspecnameplatemodel.h"

SunSpecNameplateModel::SunSpecNameplateModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecNameplateModel::~SunSpecNameplateModel()
{

}

QString SunSpecNameplateModel::name() const
{
    return "nameplate";
}

QString SunSpecNameplateModel::description() const
{
    return "Inverter Controls Nameplate Ratings ";
}

QString SunSpecNameplateModel::label() const
{
    return "Nameplate";
}

void SunSpecNameplateModel::readModelHeader()
{

}

void SunSpecNameplateModel::readBlockData()
{

}

void SunSpecNameplateModel::initDataPoints()
{
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

    SunSpecDataPoint dertypDataPoint;
    dertypDataPoint.setName("DERTyp");
    dertypDataPoint.setLabel("DERTyp");
    dertypDataPoint.setDescription("Type of DER device. Default value is 4 to indicate PV device.");
    dertypDataPoint.setMandatory(true);
    dertypDataPoint.setSize(1);
    dertypDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints << dertypDataPoint;

    SunSpecDataPoint wrtgDataPoint;
    wrtgDataPoint.setName("WRtg");
    wrtgDataPoint.setLabel("WRtg");
    wrtgDataPoint.setDescription("Continuous power output capability of the inverter.");
    wrtgDataPoint.setUnits("W");
    wrtgDataPoint.setMandatory(true);
    wrtgDataPoint.setSize(1);
    wrtgDataPoint.setScaleFactorName("WRtg_SF");
    wrtgDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << wrtgDataPoint;

    SunSpecDataPoint wrtgSfDataPoint;
    wrtgSfDataPoint.setName("WRtg_SF");
    wrtgSfDataPoint.setLabel("WRtg_SF");
    wrtgSfDataPoint.setDescription("Scale factor");
    wrtgSfDataPoint.setMandatory(true);
    wrtgSfDataPoint.setSize(1);
    wrtgSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << wrtgSfDataPoint;

    SunSpecDataPoint vartgDataPoint;
    vartgDataPoint.setName("VARtg");
    vartgDataPoint.setLabel("VARtg");
    vartgDataPoint.setDescription("Continuous Volt-Ampere capability of the inverter.");
    vartgDataPoint.setUnits("VA");
    vartgDataPoint.setMandatory(true);
    vartgDataPoint.setSize(1);
    vartgDataPoint.setScaleFactorName("VARtg_SF");
    vartgDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << vartgDataPoint;

    SunSpecDataPoint vartgSfDataPoint;
    vartgSfDataPoint.setName("VARtg_SF");
    vartgSfDataPoint.setLabel("VARtg_SF");
    vartgSfDataPoint.setDescription("Scale factor");
    vartgSfDataPoint.setMandatory(true);
    vartgSfDataPoint.setSize(1);
    vartgSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << vartgSfDataPoint;

    SunSpecDataPoint varrtgq1DataPoint;
    varrtgq1DataPoint.setName("VArRtgQ1");
    varrtgq1DataPoint.setLabel("VArRtgQ1");
    varrtgq1DataPoint.setDescription("Continuous VAR capability of the inverter in quadrant 1.");
    varrtgq1DataPoint.setUnits("var");
    varrtgq1DataPoint.setMandatory(true);
    varrtgq1DataPoint.setSize(1);
    varrtgq1DataPoint.setScaleFactorName("VArRtg_SF");
    varrtgq1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << varrtgq1DataPoint;

    SunSpecDataPoint varrtgq2DataPoint;
    varrtgq2DataPoint.setName("VArRtgQ2");
    varrtgq2DataPoint.setLabel("VArRtgQ2");
    varrtgq2DataPoint.setDescription("Continuous VAR capability of the inverter in quadrant 2.");
    varrtgq2DataPoint.setUnits("var");
    varrtgq2DataPoint.setMandatory(true);
    varrtgq2DataPoint.setSize(1);
    varrtgq2DataPoint.setScaleFactorName("VArRtg_SF");
    varrtgq2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << varrtgq2DataPoint;

    SunSpecDataPoint varrtgq3DataPoint;
    varrtgq3DataPoint.setName("VArRtgQ3");
    varrtgq3DataPoint.setLabel("VArRtgQ3");
    varrtgq3DataPoint.setDescription("Continuous VAR capability of the inverter in quadrant 3.");
    varrtgq3DataPoint.setUnits("var");
    varrtgq3DataPoint.setMandatory(true);
    varrtgq3DataPoint.setSize(1);
    varrtgq3DataPoint.setScaleFactorName("VArRtg_SF");
    varrtgq3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << varrtgq3DataPoint;

    SunSpecDataPoint varrtgq4DataPoint;
    varrtgq4DataPoint.setName("VArRtgQ4");
    varrtgq4DataPoint.setLabel("VArRtgQ4");
    varrtgq4DataPoint.setDescription("Continuous VAR capability of the inverter in quadrant 4.");
    varrtgq4DataPoint.setUnits("var");
    varrtgq4DataPoint.setMandatory(true);
    varrtgq4DataPoint.setSize(1);
    varrtgq4DataPoint.setScaleFactorName("VArRtg_SF");
    varrtgq4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << varrtgq4DataPoint;

    SunSpecDataPoint varrtgSfDataPoint;
    varrtgSfDataPoint.setName("VArRtg_SF");
    varrtgSfDataPoint.setLabel("VArRtg_SF");
    varrtgSfDataPoint.setDescription("Scale factor");
    varrtgSfDataPoint.setMandatory(true);
    varrtgSfDataPoint.setSize(1);
    varrtgSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << varrtgSfDataPoint;

    SunSpecDataPoint artgDataPoint;
    artgDataPoint.setName("ARtg");
    artgDataPoint.setLabel("ARtg");
    artgDataPoint.setDescription("Maximum RMS AC current level capability of the inverter.");
    artgDataPoint.setUnits("A");
    artgDataPoint.setMandatory(true);
    artgDataPoint.setSize(1);
    artgDataPoint.setScaleFactorName("ARtg_SF");
    artgDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << artgDataPoint;

    SunSpecDataPoint artgSfDataPoint;
    artgSfDataPoint.setName("ARtg_SF");
    artgSfDataPoint.setLabel("ARtg_SF");
    artgSfDataPoint.setDescription("Scale factor");
    artgSfDataPoint.setMandatory(true);
    artgSfDataPoint.setSize(1);
    artgSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << artgSfDataPoint;

    SunSpecDataPoint pfrtgq1DataPoint;
    pfrtgq1DataPoint.setName("PFRtgQ1");
    pfrtgq1DataPoint.setLabel("PFRtgQ1");
    pfrtgq1DataPoint.setDescription("Minimum power factor capability of the inverter in quadrant 1.");
    pfrtgq1DataPoint.setUnits("cos()");
    pfrtgq1DataPoint.setMandatory(true);
    pfrtgq1DataPoint.setSize(1);
    pfrtgq1DataPoint.setScaleFactorName("PFRtg_SF");
    pfrtgq1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << pfrtgq1DataPoint;

    SunSpecDataPoint pfrtgq2DataPoint;
    pfrtgq2DataPoint.setName("PFRtgQ2");
    pfrtgq2DataPoint.setLabel("PFRtgQ2");
    pfrtgq2DataPoint.setDescription("Minimum power factor capability of the inverter in quadrant 2.");
    pfrtgq2DataPoint.setUnits("cos()");
    pfrtgq2DataPoint.setMandatory(true);
    pfrtgq2DataPoint.setSize(1);
    pfrtgq2DataPoint.setScaleFactorName("PFRtg_SF");
    pfrtgq2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << pfrtgq2DataPoint;

    SunSpecDataPoint pfrtgq3DataPoint;
    pfrtgq3DataPoint.setName("PFRtgQ3");
    pfrtgq3DataPoint.setLabel("PFRtgQ3");
    pfrtgq3DataPoint.setDescription("Minimum power factor capability of the inverter in quadrant 3.");
    pfrtgq3DataPoint.setUnits("cos()");
    pfrtgq3DataPoint.setMandatory(true);
    pfrtgq3DataPoint.setSize(1);
    pfrtgq3DataPoint.setScaleFactorName("PFRtg_SF");
    pfrtgq3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << pfrtgq3DataPoint;

    SunSpecDataPoint pfrtgq4DataPoint;
    pfrtgq4DataPoint.setName("PFRtgQ4");
    pfrtgq4DataPoint.setLabel("PFRtgQ4");
    pfrtgq4DataPoint.setDescription("Minimum power factor capability of the inverter in quadrant 4.");
    pfrtgq4DataPoint.setUnits("cos()");
    pfrtgq4DataPoint.setMandatory(true);
    pfrtgq4DataPoint.setSize(1);
    pfrtgq4DataPoint.setScaleFactorName("PFRtg_SF");
    pfrtgq4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << pfrtgq4DataPoint;

    SunSpecDataPoint pfrtgSfDataPoint;
    pfrtgSfDataPoint.setName("PFRtg_SF");
    pfrtgSfDataPoint.setLabel("PFRtg_SF");
    pfrtgSfDataPoint.setDescription("Scale factor");
    pfrtgSfDataPoint.setMandatory(true);
    pfrtgSfDataPoint.setSize(1);
    pfrtgSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << pfrtgSfDataPoint;

    SunSpecDataPoint whrtgDataPoint;
    whrtgDataPoint.setName("WHRtg");
    whrtgDataPoint.setLabel("WHRtg");
    whrtgDataPoint.setDescription("Nominal energy rating of storage device.");
    whrtgDataPoint.setUnits("Wh");
    whrtgDataPoint.setSize(1);
    whrtgDataPoint.setScaleFactorName("WHRtg_SF");
    whrtgDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << whrtgDataPoint;

    SunSpecDataPoint whrtgSfDataPoint;
    whrtgSfDataPoint.setName("WHRtg_SF");
    whrtgSfDataPoint.setLabel("WHRtg_SF");
    whrtgSfDataPoint.setDescription("Scale factor");
    whrtgSfDataPoint.setSize(1);
    whrtgSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << whrtgSfDataPoint;

    SunSpecDataPoint ahrrtgDataPoint;
    ahrrtgDataPoint.setName("AhrRtg");
    ahrrtgDataPoint.setLabel("AhrRtg");
    ahrrtgDataPoint.setDescription("The usable capacity of the battery.  Maximum charge minus minimum charge from a technology capability perspective (Amp-hour capacity rating).");
    ahrrtgDataPoint.setUnits("AH");
    ahrrtgDataPoint.setSize(1);
    ahrrtgDataPoint.setScaleFactorName("AhrRtg_SF");
    ahrrtgDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << ahrrtgDataPoint;

    SunSpecDataPoint ahrrtgSfDataPoint;
    ahrrtgSfDataPoint.setName("AhrRtg_SF");
    ahrrtgSfDataPoint.setLabel("AhrRtg_SF");
    ahrrtgSfDataPoint.setDescription("Scale factor for amp-hour rating.");
    ahrrtgSfDataPoint.setSize(1);
    ahrrtgSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << ahrrtgSfDataPoint;

    SunSpecDataPoint maxcharteDataPoint;
    maxcharteDataPoint.setName("MaxChaRte");
    maxcharteDataPoint.setLabel("MaxChaRte");
    maxcharteDataPoint.setDescription("Maximum rate of energy transfer into the storage device.");
    maxcharteDataPoint.setUnits("W");
    maxcharteDataPoint.setSize(1);
    maxcharteDataPoint.setScaleFactorName("MaxChaRte_SF");
    maxcharteDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxcharteDataPoint;

    SunSpecDataPoint maxcharteSfDataPoint;
    maxcharteSfDataPoint.setName("MaxChaRte_SF");
    maxcharteSfDataPoint.setLabel("MaxChaRte_SF");
    maxcharteSfDataPoint.setDescription("Scale factor");
    maxcharteSfDataPoint.setSize(1);
    maxcharteSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << maxcharteSfDataPoint;

    SunSpecDataPoint maxdischarteDataPoint;
    maxdischarteDataPoint.setName("MaxDisChaRte");
    maxdischarteDataPoint.setLabel("MaxDisChaRte");
    maxdischarteDataPoint.setDescription("Maximum rate of energy transfer out of the storage device.");
    maxdischarteDataPoint.setUnits("W");
    maxdischarteDataPoint.setSize(1);
    maxdischarteDataPoint.setScaleFactorName("MaxDisChaRte_SF");
    maxdischarteDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxdischarteDataPoint;

    SunSpecDataPoint maxdischarteSfDataPoint;
    maxdischarteSfDataPoint.setName("MaxDisChaRte_SF");
    maxdischarteSfDataPoint.setLabel("MaxDisChaRte_SF");
    maxdischarteSfDataPoint.setDescription("Scale factor");
    maxdischarteSfDataPoint.setSize(1);
    maxdischarteSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << maxdischarteSfDataPoint;

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setLabel("Pad");
    padDataPoint.setDescription("Pad register.");
    padDataPoint.setSize(1);
    padDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints << padDataPoint;

}

