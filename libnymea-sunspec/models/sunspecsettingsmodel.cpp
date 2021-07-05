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
#include "sunspecconnection.h"

SunSpecSettingsModel::SunSpecSettingsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, 121, 30, modbusStartRegister, parent)
{
    Q_ASSERT_X(length == 30,  "SunSpecSettingsModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
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

float SunSpecSettingsModel::wMax() const
{
    return m_wMax;
}

QModbusReply *SunSpecSettingsModel::setWMax(float wMax)
{
    Q_UNUSED(wMax)
    return nullptr;
}
float SunSpecSettingsModel::vRef() const
{
    return m_vRef;
}

QModbusReply *SunSpecSettingsModel::setVRef(float vRef)
{
    Q_UNUSED(vRef)
    return nullptr;
}
float SunSpecSettingsModel::vRefOfs() const
{
    return m_vRefOfs;
}

QModbusReply *SunSpecSettingsModel::setVRefOfs(float vRefOfs)
{
    Q_UNUSED(vRefOfs)
    return nullptr;
}
float SunSpecSettingsModel::vMax() const
{
    return m_vMax;
}

QModbusReply *SunSpecSettingsModel::setVMax(float vMax)
{
    Q_UNUSED(vMax)
    return nullptr;
}
float SunSpecSettingsModel::vMin() const
{
    return m_vMin;
}

QModbusReply *SunSpecSettingsModel::setVMin(float vMin)
{
    Q_UNUSED(vMin)
    return nullptr;
}
float SunSpecSettingsModel::vaMax() const
{
    return m_vaMax;
}

QModbusReply *SunSpecSettingsModel::setVaMax(float vaMax)
{
    Q_UNUSED(vaMax)
    return nullptr;
}
float SunSpecSettingsModel::vArMaxQ1() const
{
    return m_vArMaxQ1;
}

QModbusReply *SunSpecSettingsModel::setVArMaxQ1(float vArMaxQ1)
{
    Q_UNUSED(vArMaxQ1)
    return nullptr;
}
float SunSpecSettingsModel::vArMaxQ2() const
{
    return m_vArMaxQ2;
}

QModbusReply *SunSpecSettingsModel::setVArMaxQ2(float vArMaxQ2)
{
    Q_UNUSED(vArMaxQ2)
    return nullptr;
}
float SunSpecSettingsModel::vArMaxQ3() const
{
    return m_vArMaxQ3;
}

QModbusReply *SunSpecSettingsModel::setVArMaxQ3(float vArMaxQ3)
{
    Q_UNUSED(vArMaxQ3)
    return nullptr;
}
float SunSpecSettingsModel::vArMaxQ4() const
{
    return m_vArMaxQ4;
}

QModbusReply *SunSpecSettingsModel::setVArMaxQ4(float vArMaxQ4)
{
    Q_UNUSED(vArMaxQ4)
    return nullptr;
}
float SunSpecSettingsModel::wGra() const
{
    return m_wGra;
}

QModbusReply *SunSpecSettingsModel::setWGra(float wGra)
{
    Q_UNUSED(wGra)
    return nullptr;
}
float SunSpecSettingsModel::pfMinQ1() const
{
    return m_pfMinQ1;
}

QModbusReply *SunSpecSettingsModel::setPfMinQ1(float pfMinQ1)
{
    Q_UNUSED(pfMinQ1)
    return nullptr;
}
float SunSpecSettingsModel::pfMinQ2() const
{
    return m_pfMinQ2;
}

QModbusReply *SunSpecSettingsModel::setPfMinQ2(float pfMinQ2)
{
    Q_UNUSED(pfMinQ2)
    return nullptr;
}
float SunSpecSettingsModel::pfMinQ3() const
{
    return m_pfMinQ3;
}

QModbusReply *SunSpecSettingsModel::setPfMinQ3(float pfMinQ3)
{
    Q_UNUSED(pfMinQ3)
    return nullptr;
}
float SunSpecSettingsModel::pfMinQ4() const
{
    return m_pfMinQ4;
}

QModbusReply *SunSpecSettingsModel::setPfMinQ4(float pfMinQ4)
{
    Q_UNUSED(pfMinQ4)
    return nullptr;
}
SunSpecSettingsModel::Varact SunSpecSettingsModel::vArAct() const
{
    return m_vArAct;
}

QModbusReply *SunSpecSettingsModel::setVArAct(Varact vArAct)
{
    Q_UNUSED(vArAct)
    return nullptr;
}
SunSpecSettingsModel::Clctotva SunSpecSettingsModel::clcTotVa() const
{
    return m_clcTotVa;
}

QModbusReply *SunSpecSettingsModel::setClcTotVa(Clctotva clcTotVa)
{
    Q_UNUSED(clcTotVa)
    return nullptr;
}
float SunSpecSettingsModel::maxRmpRte() const
{
    return m_maxRmpRte;
}

QModbusReply *SunSpecSettingsModel::setMaxRmpRte(float maxRmpRte)
{
    Q_UNUSED(maxRmpRte)
    return nullptr;
}
float SunSpecSettingsModel::ecpNomHz() const
{
    return m_ecpNomHz;
}

QModbusReply *SunSpecSettingsModel::setEcpNomHz(float ecpNomHz)
{
    Q_UNUSED(ecpNomHz)
    return nullptr;
}
SunSpecSettingsModel::Connph SunSpecSettingsModel::connPh() const
{
    return m_connPh;
}

QModbusReply *SunSpecSettingsModel::setConnPh(Connph connPh)
{
    Q_UNUSED(connPh)
    return nullptr;
}
void SunSpecSettingsModel::processBlockData()
{
    // Scale factors
    m_wMaxSf = m_dataPoints.value("WMax_SF").toInt16();
    m_vRefSf = m_dataPoints.value("VRef_SF").toInt16();
    m_vRefOfsSf = m_dataPoints.value("VRefOfs_SF").toInt16();
    m_vMinMaxSf = m_dataPoints.value("VMinMax_SF").toInt16();
    m_vaMaxSf = m_dataPoints.value("VAMax_SF").toInt16();
    m_vArMaxSf = m_dataPoints.value("VArMax_SF").toInt16();
    m_wGraSf = m_dataPoints.value("WGra_SF").toInt16();
    m_pfMinSf = m_dataPoints.value("PFMin_SF").toInt16();
    m_maxRmpRteSf = m_dataPoints.value("MaxRmpRte_SF").toInt16();
    m_ecpNomHzSf = m_dataPoints.value("ECPNomHz_SF").toInt16();

    // Update properties according to the data point type
    m_wMax = m_dataPoints.value("WMax").toFloatWithSSF(m_wMaxSf);
    m_vRef = m_dataPoints.value("VRef").toFloatWithSSF(m_vRefSf);
    m_vRefOfs = m_dataPoints.value("VRefOfs").toFloatWithSSF(m_vRefOfsSf);
    m_vMax = m_dataPoints.value("VMax").toFloatWithSSF(m_vMinMaxSf);
    m_vMin = m_dataPoints.value("VMin").toFloatWithSSF(m_vMinMaxSf);
    m_vaMax = m_dataPoints.value("VAMax").toFloatWithSSF(m_vaMaxSf);
    m_vArMaxQ1 = m_dataPoints.value("VArMaxQ1").toFloatWithSSF(m_vArMaxSf);
    m_vArMaxQ2 = m_dataPoints.value("VArMaxQ2").toFloatWithSSF(m_vArMaxSf);
    m_vArMaxQ3 = m_dataPoints.value("VArMaxQ3").toFloatWithSSF(m_vArMaxSf);
    m_vArMaxQ4 = m_dataPoints.value("VArMaxQ4").toFloatWithSSF(m_vArMaxSf);
    m_wGra = m_dataPoints.value("WGra").toFloatWithSSF(m_wGraSf);
    m_pfMinQ1 = m_dataPoints.value("PFMinQ1").toFloatWithSSF(m_pfMinSf);
    m_pfMinQ2 = m_dataPoints.value("PFMinQ2").toFloatWithSSF(m_pfMinSf);
    m_pfMinQ3 = m_dataPoints.value("PFMinQ3").toFloatWithSSF(m_pfMinSf);
    m_pfMinQ4 = m_dataPoints.value("PFMinQ4").toFloatWithSSF(m_pfMinSf);
    m_vArAct = static_cast<Varact>(m_dataPoints.value("VArAct").toUInt16());
    m_clcTotVa = static_cast<Clctotva>(m_dataPoints.value("ClcTotVA").toUInt16());
    m_maxRmpRte = m_dataPoints.value("MaxRmpRte").toFloatWithSSF(m_maxRmpRteSf);
    m_ecpNomHz = m_dataPoints.value("ECPNomHz").toFloatWithSSF(m_ecpNomHzSf);
    m_connPh = static_cast<Connph>(m_dataPoints.value("ConnPh").toUInt16());
}

void SunSpecSettingsModel::initDataPoints()
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

    SunSpecDataPoint wMaxDataPoint;
    wMaxDataPoint.setName("WMax");
    wMaxDataPoint.setLabel("WMax");
    wMaxDataPoint.setDescription("Setting for maximum power output. Default to WRtg.");
    wMaxDataPoint.setUnits("W");
    wMaxDataPoint.setMandatory(true);
    wMaxDataPoint.setSize(1);
    wMaxDataPoint.setAddressOffset(2);
    wMaxDataPoint.setBlockOffset(0);
    wMaxDataPoint.setScaleFactorName("WMax_SF");
    wMaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wMaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wMaxDataPoint.name(), wMaxDataPoint);

    SunSpecDataPoint vRefDataPoint;
    vRefDataPoint.setName("VRef");
    vRefDataPoint.setLabel("VRef");
    vRefDataPoint.setDescription("Voltage at the PCC.");
    vRefDataPoint.setUnits("V");
    vRefDataPoint.setMandatory(true);
    vRefDataPoint.setSize(1);
    vRefDataPoint.setAddressOffset(3);
    vRefDataPoint.setBlockOffset(1);
    vRefDataPoint.setScaleFactorName("VRef_SF");
    vRefDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vRefDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vRefDataPoint.name(), vRefDataPoint);

    SunSpecDataPoint vRefOfsDataPoint;
    vRefOfsDataPoint.setName("VRefOfs");
    vRefOfsDataPoint.setLabel("VRefOfs");
    vRefOfsDataPoint.setDescription("Offset  from PCC to inverter.");
    vRefOfsDataPoint.setUnits("V");
    vRefOfsDataPoint.setMandatory(true);
    vRefOfsDataPoint.setSize(1);
    vRefOfsDataPoint.setAddressOffset(4);
    vRefOfsDataPoint.setBlockOffset(2);
    vRefOfsDataPoint.setScaleFactorName("VRefOfs_SF");
    vRefOfsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    vRefOfsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vRefOfsDataPoint.name(), vRefOfsDataPoint);

    SunSpecDataPoint vMaxDataPoint;
    vMaxDataPoint.setName("VMax");
    vMaxDataPoint.setLabel("VMax");
    vMaxDataPoint.setDescription("Setpoint for maximum voltage.");
    vMaxDataPoint.setUnits("V");
    vMaxDataPoint.setSize(1);
    vMaxDataPoint.setAddressOffset(5);
    vMaxDataPoint.setBlockOffset(3);
    vMaxDataPoint.setScaleFactorName("VMinMax_SF");
    vMaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vMaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vMaxDataPoint.name(), vMaxDataPoint);

    SunSpecDataPoint vMinDataPoint;
    vMinDataPoint.setName("VMin");
    vMinDataPoint.setLabel("VMin");
    vMinDataPoint.setDescription("Setpoint for minimum voltage.");
    vMinDataPoint.setUnits("V");
    vMinDataPoint.setSize(1);
    vMinDataPoint.setAddressOffset(6);
    vMinDataPoint.setBlockOffset(4);
    vMinDataPoint.setScaleFactorName("VMinMax_SF");
    vMinDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vMinDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vMinDataPoint.name(), vMinDataPoint);

    SunSpecDataPoint vaMaxDataPoint;
    vaMaxDataPoint.setName("VAMax");
    vaMaxDataPoint.setLabel("VAMax");
    vaMaxDataPoint.setDescription("Setpoint for maximum apparent power. Default to VARtg.");
    vaMaxDataPoint.setUnits("VA");
    vaMaxDataPoint.setSize(1);
    vaMaxDataPoint.setAddressOffset(7);
    vaMaxDataPoint.setBlockOffset(5);
    vaMaxDataPoint.setScaleFactorName("VAMax_SF");
    vaMaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vaMaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vaMaxDataPoint.name(), vaMaxDataPoint);

    SunSpecDataPoint vArMaxQ1DataPoint;
    vArMaxQ1DataPoint.setName("VArMaxQ1");
    vArMaxQ1DataPoint.setLabel("VArMaxQ1");
    vArMaxQ1DataPoint.setDescription("Setting for maximum reactive power in quadrant 1. Default to VArRtgQ1.");
    vArMaxQ1DataPoint.setUnits("var");
    vArMaxQ1DataPoint.setSize(1);
    vArMaxQ1DataPoint.setAddressOffset(8);
    vArMaxQ1DataPoint.setBlockOffset(6);
    vArMaxQ1DataPoint.setScaleFactorName("VArMax_SF");
    vArMaxQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    vArMaxQ1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArMaxQ1DataPoint.name(), vArMaxQ1DataPoint);

    SunSpecDataPoint vArMaxQ2DataPoint;
    vArMaxQ2DataPoint.setName("VArMaxQ2");
    vArMaxQ2DataPoint.setLabel("VArMaxQ2");
    vArMaxQ2DataPoint.setDescription("Setting for maximum reactive power in quadrant 2. Default to VArRtgQ2.");
    vArMaxQ2DataPoint.setUnits("var");
    vArMaxQ2DataPoint.setSize(1);
    vArMaxQ2DataPoint.setAddressOffset(9);
    vArMaxQ2DataPoint.setBlockOffset(7);
    vArMaxQ2DataPoint.setScaleFactorName("VArMax_SF");
    vArMaxQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    vArMaxQ2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArMaxQ2DataPoint.name(), vArMaxQ2DataPoint);

    SunSpecDataPoint vArMaxQ3DataPoint;
    vArMaxQ3DataPoint.setName("VArMaxQ3");
    vArMaxQ3DataPoint.setLabel("VArMaxQ3");
    vArMaxQ3DataPoint.setDescription("Setting for maximum reactive power in quadrant 3. Default to VArRtgQ3.");
    vArMaxQ3DataPoint.setUnits("var");
    vArMaxQ3DataPoint.setSize(1);
    vArMaxQ3DataPoint.setAddressOffset(10);
    vArMaxQ3DataPoint.setBlockOffset(8);
    vArMaxQ3DataPoint.setScaleFactorName("VArMax_SF");
    vArMaxQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    vArMaxQ3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArMaxQ3DataPoint.name(), vArMaxQ3DataPoint);

    SunSpecDataPoint vArMaxQ4DataPoint;
    vArMaxQ4DataPoint.setName("VArMaxQ4");
    vArMaxQ4DataPoint.setLabel("VArMaxQ4");
    vArMaxQ4DataPoint.setDescription("Setting for maximum reactive power in quadrant 4. Default to VArRtgQ4.");
    vArMaxQ4DataPoint.setUnits("var");
    vArMaxQ4DataPoint.setSize(1);
    vArMaxQ4DataPoint.setAddressOffset(11);
    vArMaxQ4DataPoint.setBlockOffset(9);
    vArMaxQ4DataPoint.setScaleFactorName("VArMax_SF");
    vArMaxQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    vArMaxQ4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArMaxQ4DataPoint.name(), vArMaxQ4DataPoint);

    SunSpecDataPoint wGraDataPoint;
    wGraDataPoint.setName("WGra");
    wGraDataPoint.setLabel("WGra");
    wGraDataPoint.setDescription("Default ramp rate of change of active power due to command or internal action.");
    wGraDataPoint.setUnits("% WMax/sec");
    wGraDataPoint.setSize(1);
    wGraDataPoint.setAddressOffset(12);
    wGraDataPoint.setBlockOffset(10);
    wGraDataPoint.setScaleFactorName("WGra_SF");
    wGraDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wGraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wGraDataPoint.name(), wGraDataPoint);

    SunSpecDataPoint pfMinQ1DataPoint;
    pfMinQ1DataPoint.setName("PFMinQ1");
    pfMinQ1DataPoint.setLabel("PFMinQ1");
    pfMinQ1DataPoint.setDescription("Setpoint for minimum power factor value in quadrant 1. Default to PFRtgQ1.");
    pfMinQ1DataPoint.setUnits("cos()");
    pfMinQ1DataPoint.setSize(1);
    pfMinQ1DataPoint.setAddressOffset(13);
    pfMinQ1DataPoint.setBlockOffset(11);
    pfMinQ1DataPoint.setScaleFactorName("PFMin_SF");
    pfMinQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    pfMinQ1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(pfMinQ1DataPoint.name(), pfMinQ1DataPoint);

    SunSpecDataPoint pfMinQ2DataPoint;
    pfMinQ2DataPoint.setName("PFMinQ2");
    pfMinQ2DataPoint.setLabel("PFMinQ2");
    pfMinQ2DataPoint.setDescription("Setpoint for minimum power factor value in quadrant 2. Default to PFRtgQ2.");
    pfMinQ2DataPoint.setUnits("cos()");
    pfMinQ2DataPoint.setSize(1);
    pfMinQ2DataPoint.setAddressOffset(14);
    pfMinQ2DataPoint.setBlockOffset(12);
    pfMinQ2DataPoint.setScaleFactorName("PFMin_SF");
    pfMinQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    pfMinQ2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(pfMinQ2DataPoint.name(), pfMinQ2DataPoint);

    SunSpecDataPoint pfMinQ3DataPoint;
    pfMinQ3DataPoint.setName("PFMinQ3");
    pfMinQ3DataPoint.setLabel("PFMinQ3");
    pfMinQ3DataPoint.setDescription("Setpoint for minimum power factor value in quadrant 3. Default to PFRtgQ3.");
    pfMinQ3DataPoint.setUnits("cos()");
    pfMinQ3DataPoint.setSize(1);
    pfMinQ3DataPoint.setAddressOffset(15);
    pfMinQ3DataPoint.setBlockOffset(13);
    pfMinQ3DataPoint.setScaleFactorName("PFMin_SF");
    pfMinQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    pfMinQ3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(pfMinQ3DataPoint.name(), pfMinQ3DataPoint);

    SunSpecDataPoint pfMinQ4DataPoint;
    pfMinQ4DataPoint.setName("PFMinQ4");
    pfMinQ4DataPoint.setLabel("PFMinQ4");
    pfMinQ4DataPoint.setDescription("Setpoint for minimum power factor value in quadrant 4. Default to PFRtgQ4.");
    pfMinQ4DataPoint.setUnits("cos()");
    pfMinQ4DataPoint.setSize(1);
    pfMinQ4DataPoint.setAddressOffset(16);
    pfMinQ4DataPoint.setBlockOffset(14);
    pfMinQ4DataPoint.setScaleFactorName("PFMin_SF");
    pfMinQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    pfMinQ4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(pfMinQ4DataPoint.name(), pfMinQ4DataPoint);

    SunSpecDataPoint vArActDataPoint;
    vArActDataPoint.setName("VArAct");
    vArActDataPoint.setLabel("VArAct");
    vArActDataPoint.setDescription("VAR action on change between charging and discharging: 1=switch 2=maintain VAR characterization.");
    vArActDataPoint.setSize(1);
    vArActDataPoint.setAddressOffset(17);
    vArActDataPoint.setBlockOffset(15);
    vArActDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    vArActDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArActDataPoint.name(), vArActDataPoint);

    SunSpecDataPoint clcTotVaDataPoint;
    clcTotVaDataPoint.setName("ClcTotVA");
    clcTotVaDataPoint.setLabel("ClcTotVA");
    clcTotVaDataPoint.setDescription("Calculation method for total apparent power. 1=vector 2=arithmetic.");
    clcTotVaDataPoint.setSize(1);
    clcTotVaDataPoint.setAddressOffset(18);
    clcTotVaDataPoint.setBlockOffset(16);
    clcTotVaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    clcTotVaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(clcTotVaDataPoint.name(), clcTotVaDataPoint);

    SunSpecDataPoint maxRmpRteDataPoint;
    maxRmpRteDataPoint.setName("MaxRmpRte");
    maxRmpRteDataPoint.setLabel("MaxRmpRte");
    maxRmpRteDataPoint.setDescription("Setpoint for maximum ramp rate as percentage of nominal maximum ramp rate. This setting will limit the rate that watts delivery to the grid can increase or decrease in response to intermittent PV generation.");
    maxRmpRteDataPoint.setUnits("% WGra");
    maxRmpRteDataPoint.setSize(1);
    maxRmpRteDataPoint.setAddressOffset(19);
    maxRmpRteDataPoint.setBlockOffset(17);
    maxRmpRteDataPoint.setScaleFactorName("MaxRmpRte_SF");
    maxRmpRteDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    maxRmpRteDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(maxRmpRteDataPoint.name(), maxRmpRteDataPoint);

    SunSpecDataPoint ecpNomHzDataPoint;
    ecpNomHzDataPoint.setName("ECPNomHz");
    ecpNomHzDataPoint.setLabel("ECPNomHz");
    ecpNomHzDataPoint.setDescription("Setpoint for nominal frequency at the ECP.");
    ecpNomHzDataPoint.setUnits("Hz");
    ecpNomHzDataPoint.setSize(1);
    ecpNomHzDataPoint.setAddressOffset(20);
    ecpNomHzDataPoint.setBlockOffset(18);
    ecpNomHzDataPoint.setScaleFactorName("ECPNomHz_SF");
    ecpNomHzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    ecpNomHzDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(ecpNomHzDataPoint.name(), ecpNomHzDataPoint);

    SunSpecDataPoint connPhDataPoint;
    connPhDataPoint.setName("ConnPh");
    connPhDataPoint.setLabel("ConnPh");
    connPhDataPoint.setDescription("Identity of connected phase for single phase inverters. A=1 B=2 C=3.");
    connPhDataPoint.setSize(1);
    connPhDataPoint.setAddressOffset(21);
    connPhDataPoint.setBlockOffset(19);
    connPhDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    connPhDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connPhDataPoint.name(), connPhDataPoint);

    SunSpecDataPoint wMaxSfDataPoint;
    wMaxSfDataPoint.setName("WMax_SF");
    wMaxSfDataPoint.setLabel("WMax_SF");
    wMaxSfDataPoint.setDescription("Scale factor for real power.");
    wMaxSfDataPoint.setMandatory(true);
    wMaxSfDataPoint.setSize(1);
    wMaxSfDataPoint.setAddressOffset(22);
    wMaxSfDataPoint.setBlockOffset(20);
    wMaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(wMaxSfDataPoint.name(), wMaxSfDataPoint);

    SunSpecDataPoint vRefSfDataPoint;
    vRefSfDataPoint.setName("VRef_SF");
    vRefSfDataPoint.setLabel("VRef_SF");
    vRefSfDataPoint.setDescription("Scale factor for voltage at the PCC.");
    vRefSfDataPoint.setMandatory(true);
    vRefSfDataPoint.setSize(1);
    vRefSfDataPoint.setAddressOffset(23);
    vRefSfDataPoint.setBlockOffset(21);
    vRefSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vRefSfDataPoint.name(), vRefSfDataPoint);

    SunSpecDataPoint vRefOfsSfDataPoint;
    vRefOfsSfDataPoint.setName("VRefOfs_SF");
    vRefOfsSfDataPoint.setLabel("VRefOfs_SF");
    vRefOfsSfDataPoint.setDescription("Scale factor for offset voltage.");
    vRefOfsSfDataPoint.setMandatory(true);
    vRefOfsSfDataPoint.setSize(1);
    vRefOfsSfDataPoint.setAddressOffset(24);
    vRefOfsSfDataPoint.setBlockOffset(22);
    vRefOfsSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vRefOfsSfDataPoint.name(), vRefOfsSfDataPoint);

    SunSpecDataPoint vMinMaxSfDataPoint;
    vMinMaxSfDataPoint.setName("VMinMax_SF");
    vMinMaxSfDataPoint.setLabel("VMinMax_SF");
    vMinMaxSfDataPoint.setDescription("Scale factor for min/max voltages.");
    vMinMaxSfDataPoint.setSize(1);
    vMinMaxSfDataPoint.setAddressOffset(25);
    vMinMaxSfDataPoint.setBlockOffset(23);
    vMinMaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vMinMaxSfDataPoint.name(), vMinMaxSfDataPoint);

    SunSpecDataPoint vaMaxSfDataPoint;
    vaMaxSfDataPoint.setName("VAMax_SF");
    vaMaxSfDataPoint.setLabel("VAMax_SF");
    vaMaxSfDataPoint.setDescription("Scale factor for apparent power.");
    vaMaxSfDataPoint.setSize(1);
    vaMaxSfDataPoint.setAddressOffset(26);
    vaMaxSfDataPoint.setBlockOffset(24);
    vaMaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vaMaxSfDataPoint.name(), vaMaxSfDataPoint);

    SunSpecDataPoint vArMaxSfDataPoint;
    vArMaxSfDataPoint.setName("VArMax_SF");
    vArMaxSfDataPoint.setLabel("VArMax_SF");
    vArMaxSfDataPoint.setDescription("Scale factor for reactive power.");
    vArMaxSfDataPoint.setSize(1);
    vArMaxSfDataPoint.setAddressOffset(27);
    vArMaxSfDataPoint.setBlockOffset(25);
    vArMaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vArMaxSfDataPoint.name(), vArMaxSfDataPoint);

    SunSpecDataPoint wGraSfDataPoint;
    wGraSfDataPoint.setName("WGra_SF");
    wGraSfDataPoint.setLabel("WGra_SF");
    wGraSfDataPoint.setDescription("Scale factor for default ramp rate.");
    wGraSfDataPoint.setSize(1);
    wGraSfDataPoint.setAddressOffset(28);
    wGraSfDataPoint.setBlockOffset(26);
    wGraSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(wGraSfDataPoint.name(), wGraSfDataPoint);

    SunSpecDataPoint pfMinSfDataPoint;
    pfMinSfDataPoint.setName("PFMin_SF");
    pfMinSfDataPoint.setLabel("PFMin_SF");
    pfMinSfDataPoint.setDescription("Scale factor for minimum power factor.");
    pfMinSfDataPoint.setSize(1);
    pfMinSfDataPoint.setAddressOffset(29);
    pfMinSfDataPoint.setBlockOffset(27);
    pfMinSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(pfMinSfDataPoint.name(), pfMinSfDataPoint);

    SunSpecDataPoint maxRmpRteSfDataPoint;
    maxRmpRteSfDataPoint.setName("MaxRmpRte_SF");
    maxRmpRteSfDataPoint.setLabel("MaxRmpRte_SF");
    maxRmpRteSfDataPoint.setDescription("Scale factor for maximum ramp percentage.");
    maxRmpRteSfDataPoint.setSize(1);
    maxRmpRteSfDataPoint.setAddressOffset(30);
    maxRmpRteSfDataPoint.setBlockOffset(28);
    maxRmpRteSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(maxRmpRteSfDataPoint.name(), maxRmpRteSfDataPoint);

    SunSpecDataPoint ecpNomHzSfDataPoint;
    ecpNomHzSfDataPoint.setName("ECPNomHz_SF");
    ecpNomHzSfDataPoint.setLabel("ECPNomHz_SF");
    ecpNomHzSfDataPoint.setDescription("Scale factor for nominal frequency.");
    ecpNomHzSfDataPoint.setSize(1);
    ecpNomHzSfDataPoint.setAddressOffset(31);
    ecpNomHzSfDataPoint.setBlockOffset(29);
    ecpNomHzSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(ecpNomHzSfDataPoint.name(), ecpNomHzSfDataPoint);

}

