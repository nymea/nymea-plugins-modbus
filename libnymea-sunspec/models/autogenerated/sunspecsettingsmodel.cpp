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

#include "sunspecsettingsmodel.h"

SunSpecSettingsModel::SunSpecSettingsModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecSettingsModel::~SunSpecSettingsModel()
{

}

QString SunSpecSettingsModel::name() const
{
    return "settings";
}

QString SunSpecSettingsModel::description() const
{
    return "Inverter Controls Basic Settings ";
}

QString SunSpecSettingsModel::label() const
{
    return "Basic Settings";
}

void SunSpecSettingsModel::readModelHeader()
{

}

void SunSpecSettingsModel::readBlockData()
{

}

void SunSpecSettingsModel::initDataPoints()
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

    SunSpecDataPoint wmaxDataPoint;
    wmaxDataPoint.setName("WMax");
    wmaxDataPoint.setLabel("WMax");
    wmaxDataPoint.setDescription("Setting for maximum power output. Default to WRtg.");
    wmaxDataPoint.setUnits("W");
    wmaxDataPoint.setMandatory(true);
    wmaxDataPoint.setSize(1);
    wmaxDataPoint.setScaleFactorName("WMax_SF");
    wmaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wmaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wmaxDataPoint;

    SunSpecDataPoint vrefDataPoint;
    vrefDataPoint.setName("VRef");
    vrefDataPoint.setLabel("VRef");
    vrefDataPoint.setDescription("Voltage at the PCC.");
    vrefDataPoint.setUnits("V");
    vrefDataPoint.setMandatory(true);
    vrefDataPoint.setSize(1);
    vrefDataPoint.setScaleFactorName("VRef_SF");
    vrefDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vrefDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << vrefDataPoint;

    SunSpecDataPoint vrefofsDataPoint;
    vrefofsDataPoint.setName("VRefOfs");
    vrefofsDataPoint.setLabel("VRefOfs");
    vrefofsDataPoint.setDescription("Offset  from PCC to inverter.");
    vrefofsDataPoint.setUnits("V");
    vrefofsDataPoint.setMandatory(true);
    vrefofsDataPoint.setSize(1);
    vrefofsDataPoint.setScaleFactorName("VRefOfs_SF");
    vrefofsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    vrefofsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << vrefofsDataPoint;

    SunSpecDataPoint vmaxDataPoint;
    vmaxDataPoint.setName("VMax");
    vmaxDataPoint.setLabel("VMax");
    vmaxDataPoint.setDescription("Setpoint for maximum voltage.");
    vmaxDataPoint.setUnits("V");
    vmaxDataPoint.setSize(1);
    vmaxDataPoint.setScaleFactorName("VMinMax_SF");
    vmaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vmaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << vmaxDataPoint;

    SunSpecDataPoint vminDataPoint;
    vminDataPoint.setName("VMin");
    vminDataPoint.setLabel("VMin");
    vminDataPoint.setDescription("Setpoint for minimum voltage.");
    vminDataPoint.setUnits("V");
    vminDataPoint.setSize(1);
    vminDataPoint.setScaleFactorName("VMinMax_SF");
    vminDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vminDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << vminDataPoint;

    SunSpecDataPoint vamaxDataPoint;
    vamaxDataPoint.setName("VAMax");
    vamaxDataPoint.setLabel("VAMax");
    vamaxDataPoint.setDescription("Setpoint for maximum apparent power. Default to VARtg.");
    vamaxDataPoint.setUnits("VA");
    vamaxDataPoint.setSize(1);
    vamaxDataPoint.setScaleFactorName("VAMax_SF");
    vamaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vamaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << vamaxDataPoint;

    SunSpecDataPoint varmaxq1DataPoint;
    varmaxq1DataPoint.setName("VArMaxQ1");
    varmaxq1DataPoint.setLabel("VArMaxQ1");
    varmaxq1DataPoint.setDescription("Setting for maximum reactive power in quadrant 1. Default to VArRtgQ1.");
    varmaxq1DataPoint.setUnits("var");
    varmaxq1DataPoint.setSize(1);
    varmaxq1DataPoint.setScaleFactorName("VArMax_SF");
    varmaxq1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varmaxq1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varmaxq1DataPoint;

    SunSpecDataPoint varmaxq2DataPoint;
    varmaxq2DataPoint.setName("VArMaxQ2");
    varmaxq2DataPoint.setLabel("VArMaxQ2");
    varmaxq2DataPoint.setDescription("Setting for maximum reactive power in quadrant 2. Default to VArRtgQ2.");
    varmaxq2DataPoint.setUnits("var");
    varmaxq2DataPoint.setSize(1);
    varmaxq2DataPoint.setScaleFactorName("VArMax_SF");
    varmaxq2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varmaxq2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varmaxq2DataPoint;

    SunSpecDataPoint varmaxq3DataPoint;
    varmaxq3DataPoint.setName("VArMaxQ3");
    varmaxq3DataPoint.setLabel("VArMaxQ3");
    varmaxq3DataPoint.setDescription("Setting for maximum reactive power in quadrant 3. Default to VArRtgQ3.");
    varmaxq3DataPoint.setUnits("var");
    varmaxq3DataPoint.setSize(1);
    varmaxq3DataPoint.setScaleFactorName("VArMax_SF");
    varmaxq3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varmaxq3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varmaxq3DataPoint;

    SunSpecDataPoint varmaxq4DataPoint;
    varmaxq4DataPoint.setName("VArMaxQ4");
    varmaxq4DataPoint.setLabel("VArMaxQ4");
    varmaxq4DataPoint.setDescription("Setting for maximum reactive power in quadrant 4. Default to VArRtgQ4.");
    varmaxq4DataPoint.setUnits("var");
    varmaxq4DataPoint.setSize(1);
    varmaxq4DataPoint.setScaleFactorName("VArMax_SF");
    varmaxq4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varmaxq4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varmaxq4DataPoint;

    SunSpecDataPoint wgraDataPoint;
    wgraDataPoint.setName("WGra");
    wgraDataPoint.setLabel("WGra");
    wgraDataPoint.setDescription("Default ramp rate of change of active power due to command or internal action.");
    wgraDataPoint.setUnits("% WMax/sec");
    wgraDataPoint.setSize(1);
    wgraDataPoint.setScaleFactorName("WGra_SF");
    wgraDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wgraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wgraDataPoint;

    SunSpecDataPoint pfminq1DataPoint;
    pfminq1DataPoint.setName("PFMinQ1");
    pfminq1DataPoint.setLabel("PFMinQ1");
    pfminq1DataPoint.setDescription("Setpoint for minimum power factor value in quadrant 1. Default to PFRtgQ1.");
    pfminq1DataPoint.setUnits("cos()");
    pfminq1DataPoint.setSize(1);
    pfminq1DataPoint.setScaleFactorName("PFMin_SF");
    pfminq1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    pfminq1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << pfminq1DataPoint;

    SunSpecDataPoint pfminq2DataPoint;
    pfminq2DataPoint.setName("PFMinQ2");
    pfminq2DataPoint.setLabel("PFMinQ2");
    pfminq2DataPoint.setDescription("Setpoint for minimum power factor value in quadrant 2. Default to PFRtgQ2.");
    pfminq2DataPoint.setUnits("cos()");
    pfminq2DataPoint.setSize(1);
    pfminq2DataPoint.setScaleFactorName("PFMin_SF");
    pfminq2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    pfminq2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << pfminq2DataPoint;

    SunSpecDataPoint pfminq3DataPoint;
    pfminq3DataPoint.setName("PFMinQ3");
    pfminq3DataPoint.setLabel("PFMinQ3");
    pfminq3DataPoint.setDescription("Setpoint for minimum power factor value in quadrant 3. Default to PFRtgQ3.");
    pfminq3DataPoint.setUnits("cos()");
    pfminq3DataPoint.setSize(1);
    pfminq3DataPoint.setScaleFactorName("PFMin_SF");
    pfminq3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    pfminq3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << pfminq3DataPoint;

    SunSpecDataPoint pfminq4DataPoint;
    pfminq4DataPoint.setName("PFMinQ4");
    pfminq4DataPoint.setLabel("PFMinQ4");
    pfminq4DataPoint.setDescription("Setpoint for minimum power factor value in quadrant 4. Default to PFRtgQ4.");
    pfminq4DataPoint.setUnits("cos()");
    pfminq4DataPoint.setSize(1);
    pfminq4DataPoint.setScaleFactorName("PFMin_SF");
    pfminq4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    pfminq4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << pfminq4DataPoint;

    SunSpecDataPoint varactDataPoint;
    varactDataPoint.setName("VArAct");
    varactDataPoint.setLabel("VArAct");
    varactDataPoint.setDescription("VAR action on change between charging and discharging: 1=switch 2=maintain VAR characterization.");
    varactDataPoint.setSize(1);
    varactDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    varactDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varactDataPoint;

    SunSpecDataPoint clctotvaDataPoint;
    clctotvaDataPoint.setName("ClcTotVA");
    clctotvaDataPoint.setLabel("ClcTotVA");
    clctotvaDataPoint.setDescription("Calculation method for total apparent power. 1=vector 2=arithmetic.");
    clctotvaDataPoint.setSize(1);
    clctotvaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    clctotvaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << clctotvaDataPoint;

    SunSpecDataPoint maxrmprteDataPoint;
    maxrmprteDataPoint.setName("MaxRmpRte");
    maxrmprteDataPoint.setLabel("MaxRmpRte");
    maxrmprteDataPoint.setDescription("Setpoint for maximum ramp rate as percentage of nominal maximum ramp rate. This setting will limit the rate that watts delivery to the grid can increase or decrease in response to intermittent PV generation.");
    maxrmprteDataPoint.setUnits("% WGra");
    maxrmprteDataPoint.setSize(1);
    maxrmprteDataPoint.setScaleFactorName("MaxRmpRte_SF");
    maxrmprteDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    maxrmprteDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << maxrmprteDataPoint;

    SunSpecDataPoint ecpnomhzDataPoint;
    ecpnomhzDataPoint.setName("ECPNomHz");
    ecpnomhzDataPoint.setLabel("ECPNomHz");
    ecpnomhzDataPoint.setDescription("Setpoint for nominal frequency at the ECP.");
    ecpnomhzDataPoint.setUnits("Hz");
    ecpnomhzDataPoint.setSize(1);
    ecpnomhzDataPoint.setScaleFactorName("ECPNomHz_SF");
    ecpnomhzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    ecpnomhzDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << ecpnomhzDataPoint;

    SunSpecDataPoint connphDataPoint;
    connphDataPoint.setName("ConnPh");
    connphDataPoint.setLabel("ConnPh");
    connphDataPoint.setDescription("Identity of connected phase for single phase inverters. A=1 B=2 C=3.");
    connphDataPoint.setSize(1);
    connphDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    connphDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << connphDataPoint;

    SunSpecDataPoint wmaxSfDataPoint;
    wmaxSfDataPoint.setName("WMax_SF");
    wmaxSfDataPoint.setLabel("WMax_SF");
    wmaxSfDataPoint.setDescription("Scale factor for real power.");
    wmaxSfDataPoint.setMandatory(true);
    wmaxSfDataPoint.setSize(1);
    wmaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << wmaxSfDataPoint;

    SunSpecDataPoint vrefSfDataPoint;
    vrefSfDataPoint.setName("VRef_SF");
    vrefSfDataPoint.setLabel("VRef_SF");
    vrefSfDataPoint.setDescription("Scale factor for voltage at the PCC.");
    vrefSfDataPoint.setMandatory(true);
    vrefSfDataPoint.setSize(1);
    vrefSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << vrefSfDataPoint;

    SunSpecDataPoint vrefofsSfDataPoint;
    vrefofsSfDataPoint.setName("VRefOfs_SF");
    vrefofsSfDataPoint.setLabel("VRefOfs_SF");
    vrefofsSfDataPoint.setDescription("Scale factor for offset voltage.");
    vrefofsSfDataPoint.setMandatory(true);
    vrefofsSfDataPoint.setSize(1);
    vrefofsSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << vrefofsSfDataPoint;

    SunSpecDataPoint vminmaxSfDataPoint;
    vminmaxSfDataPoint.setName("VMinMax_SF");
    vminmaxSfDataPoint.setLabel("VMinMax_SF");
    vminmaxSfDataPoint.setDescription("Scale factor for min/max voltages.");
    vminmaxSfDataPoint.setSize(1);
    vminmaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << vminmaxSfDataPoint;

    SunSpecDataPoint vamaxSfDataPoint;
    vamaxSfDataPoint.setName("VAMax_SF");
    vamaxSfDataPoint.setLabel("VAMax_SF");
    vamaxSfDataPoint.setDescription("Scale factor for apparent power.");
    vamaxSfDataPoint.setSize(1);
    vamaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << vamaxSfDataPoint;

    SunSpecDataPoint varmaxSfDataPoint;
    varmaxSfDataPoint.setName("VArMax_SF");
    varmaxSfDataPoint.setLabel("VArMax_SF");
    varmaxSfDataPoint.setDescription("Scale factor for reactive power.");
    varmaxSfDataPoint.setSize(1);
    varmaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << varmaxSfDataPoint;

    SunSpecDataPoint wgraSfDataPoint;
    wgraSfDataPoint.setName("WGra_SF");
    wgraSfDataPoint.setLabel("WGra_SF");
    wgraSfDataPoint.setDescription("Scale factor for default ramp rate.");
    wgraSfDataPoint.setSize(1);
    wgraSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << wgraSfDataPoint;

    SunSpecDataPoint pfminSfDataPoint;
    pfminSfDataPoint.setName("PFMin_SF");
    pfminSfDataPoint.setLabel("PFMin_SF");
    pfminSfDataPoint.setDescription("Scale factor for minimum power factor.");
    pfminSfDataPoint.setSize(1);
    pfminSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << pfminSfDataPoint;

    SunSpecDataPoint maxrmprteSfDataPoint;
    maxrmprteSfDataPoint.setName("MaxRmpRte_SF");
    maxrmprteSfDataPoint.setLabel("MaxRmpRte_SF");
    maxrmprteSfDataPoint.setDescription("Scale factor for maximum ramp percentage.");
    maxrmprteSfDataPoint.setSize(1);
    maxrmprteSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << maxrmprteSfDataPoint;

    SunSpecDataPoint ecpnomhzSfDataPoint;
    ecpnomhzSfDataPoint.setName("ECPNomHz_SF");
    ecpnomhzSfDataPoint.setLabel("ECPNomHz_SF");
    ecpnomhzSfDataPoint.setDescription("Scale factor for nominal frequency.");
    ecpnomhzSfDataPoint.setSize(1);
    ecpnomhzSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << ecpnomhzSfDataPoint;

}

