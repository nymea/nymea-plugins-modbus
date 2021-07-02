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

#include "sunspeccontrolsmodel.h"

SunSpecControlsModel::SunSpecControlsModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, 123, 24, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecControlsModel::~SunSpecControlsModel()
{

}

QString SunSpecControlsModel::name() const
{
    return "controls";
}

QString SunSpecControlsModel::description() const
{
    return "Immediate Inverter Controls ";
}

QString SunSpecControlsModel::label() const
{
    return "Immediate Controls";
}

quint16 SunSpecControlsModel::connWinTms() const
{
    return m_connWinTms;
}
quint16 SunSpecControlsModel::connRvrtTms() const
{
    return m_connRvrtTms;
}
SunSpecControlsModel::Conn SunSpecControlsModel::conn() const
{
    return m_conn;
}
float SunSpecControlsModel::wMaxLimPct() const
{
    return m_wMaxLimPct;
}
quint16 SunSpecControlsModel::wMaxLimPctWinTms() const
{
    return m_wMaxLimPctWinTms;
}
quint16 SunSpecControlsModel::wMaxLimPctRvrtTms() const
{
    return m_wMaxLimPctRvrtTms;
}
quint16 SunSpecControlsModel::wMaxLimPctRmpTms() const
{
    return m_wMaxLimPctRmpTms;
}
SunSpecControlsModel::Wmaxlim_ena SunSpecControlsModel::wMaxLimEna() const
{
    return m_wMaxLimEna;
}
float SunSpecControlsModel::outPfSet() const
{
    return m_outPfSet;
}
quint16 SunSpecControlsModel::outPfSetWinTms() const
{
    return m_outPfSetWinTms;
}
quint16 SunSpecControlsModel::outPfSetRvrtTms() const
{
    return m_outPfSetRvrtTms;
}
quint16 SunSpecControlsModel::outPfSetRmpTms() const
{
    return m_outPfSetRmpTms;
}
SunSpecControlsModel::Outpfset_ena SunSpecControlsModel::outPfSetEna() const
{
    return m_outPfSetEna;
}
float SunSpecControlsModel::vArWMaxPct() const
{
    return m_vArWMaxPct;
}
float SunSpecControlsModel::vArMaxPct() const
{
    return m_vArMaxPct;
}
float SunSpecControlsModel::vArAvalPct() const
{
    return m_vArAvalPct;
}
quint16 SunSpecControlsModel::vArPctWinTms() const
{
    return m_vArPctWinTms;
}
quint16 SunSpecControlsModel::vArPctRvrtTms() const
{
    return m_vArPctRvrtTms;
}
quint16 SunSpecControlsModel::vArPctRmpTms() const
{
    return m_vArPctRmpTms;
}
SunSpecControlsModel::Varpct_mod SunSpecControlsModel::vArPctMod() const
{
    return m_vArPctMod;
}
SunSpecControlsModel::Varpct_ena SunSpecControlsModel::vArPctEna() const
{
    return m_vArPctEna;
}
void SunSpecControlsModel::processBlockData()
{
    // Scale factors
    m_wMaxLimPctSf = m_dataPoints.value("WMaxLimPct_SF").toInt16();
    m_outPfSetSf = m_dataPoints.value("OutPFSet_SF").toInt16();
    m_vArPctSf = m_dataPoints.value("VArPct_SF").toInt16();

    // Update properties according to the data point type
    m_modelId = m_dataPoints.value("ID").toUInt16();
    m_modelLength = m_dataPoints.value("L").toUInt16();
    m_connWinTms = m_dataPoints.value("Conn_WinTms").toUInt16();
    m_connRvrtTms = m_dataPoints.value("Conn_RvrtTms").toUInt16();
    m_conn = static_cast<Conn>(m_dataPoints.value("Conn").toUInt16());
    m_wMaxLimPct = m_dataPoints.value("WMaxLimPct").toFloatWithSSF(m_wMaxLimPctSf);
    m_wMaxLimPctWinTms = m_dataPoints.value("WMaxLimPct_WinTms").toUInt16();
    m_wMaxLimPctRvrtTms = m_dataPoints.value("WMaxLimPct_RvrtTms").toUInt16();
    m_wMaxLimPctRmpTms = m_dataPoints.value("WMaxLimPct_RmpTms").toUInt16();
    m_wMaxLimEna = static_cast<Wmaxlim_ena>(m_dataPoints.value("WMaxLim_Ena").toUInt16());
    m_outPfSet = m_dataPoints.value("OutPFSet").toFloatWithSSF(m_outPfSetSf);
    m_outPfSetWinTms = m_dataPoints.value("OutPFSet_WinTms").toUInt16();
    m_outPfSetRvrtTms = m_dataPoints.value("OutPFSet_RvrtTms").toUInt16();
    m_outPfSetRmpTms = m_dataPoints.value("OutPFSet_RmpTms").toUInt16();
    m_outPfSetEna = static_cast<Outpfset_ena>(m_dataPoints.value("OutPFSet_Ena").toUInt16());
    m_vArWMaxPct = m_dataPoints.value("VArWMaxPct").toFloatWithSSF(m_vArPctSf);
    m_vArMaxPct = m_dataPoints.value("VArMaxPct").toFloatWithSSF(m_vArPctSf);
    m_vArAvalPct = m_dataPoints.value("VArAvalPct").toFloatWithSSF(m_vArPctSf);
    m_vArPctWinTms = m_dataPoints.value("VArPct_WinTms").toUInt16();
    m_vArPctRvrtTms = m_dataPoints.value("VArPct_RvrtTms").toUInt16();
    m_vArPctRmpTms = m_dataPoints.value("VArPct_RmpTms").toUInt16();
    m_vArPctMod = static_cast<Varpct_mod>(m_dataPoints.value("VArPct_Mod").toUInt16());
    m_vArPctEna = static_cast<Varpct_ena>(m_dataPoints.value("VArPct_Ena").toUInt16());
}

void SunSpecControlsModel::initDataPoints()
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

    SunSpecDataPoint connWinTmsDataPoint;
    connWinTmsDataPoint.setName("Conn_WinTms");
    connWinTmsDataPoint.setLabel("Conn_WinTms");
    connWinTmsDataPoint.setDescription("Time window for connect/disconnect.");
    connWinTmsDataPoint.setUnits("Secs");
    connWinTmsDataPoint.setSize(1);
    connWinTmsDataPoint.setAddressOffset(2);
    connWinTmsDataPoint.setBlockOffset(0);
    connWinTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connWinTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connWinTmsDataPoint.name(), connWinTmsDataPoint);

    SunSpecDataPoint connRvrtTmsDataPoint;
    connRvrtTmsDataPoint.setName("Conn_RvrtTms");
    connRvrtTmsDataPoint.setLabel("Conn_RvrtTms");
    connRvrtTmsDataPoint.setDescription("Timeout period for connect/disconnect.");
    connRvrtTmsDataPoint.setUnits("Secs");
    connRvrtTmsDataPoint.setSize(1);
    connRvrtTmsDataPoint.setAddressOffset(3);
    connRvrtTmsDataPoint.setBlockOffset(1);
    connRvrtTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connRvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connRvrtTmsDataPoint.name(), connRvrtTmsDataPoint);

    SunSpecDataPoint connDataPoint;
    connDataPoint.setName("Conn");
    connDataPoint.setLabel("Conn");
    connDataPoint.setDescription("Enumerated valued.  Connection control.");
    connDataPoint.setMandatory(true);
    connDataPoint.setSize(1);
    connDataPoint.setAddressOffset(4);
    connDataPoint.setBlockOffset(2);
    connDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    connDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connDataPoint.name(), connDataPoint);

    SunSpecDataPoint wMaxLimPctDataPoint;
    wMaxLimPctDataPoint.setName("WMaxLimPct");
    wMaxLimPctDataPoint.setLabel("WMaxLimPct");
    wMaxLimPctDataPoint.setDescription("Set power output to specified level.");
    wMaxLimPctDataPoint.setUnits("% WMax");
    wMaxLimPctDataPoint.setMandatory(true);
    wMaxLimPctDataPoint.setSize(1);
    wMaxLimPctDataPoint.setAddressOffset(5);
    wMaxLimPctDataPoint.setBlockOffset(3);
    wMaxLimPctDataPoint.setScaleFactorName("WMaxLimPct_SF");
    wMaxLimPctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wMaxLimPctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wMaxLimPctDataPoint.name(), wMaxLimPctDataPoint);

    SunSpecDataPoint wMaxLimPctWinTmsDataPoint;
    wMaxLimPctWinTmsDataPoint.setName("WMaxLimPct_WinTms");
    wMaxLimPctWinTmsDataPoint.setLabel("WMaxLimPct_WinTms");
    wMaxLimPctWinTmsDataPoint.setDescription("Time window for power limit change.");
    wMaxLimPctWinTmsDataPoint.setUnits("Secs");
    wMaxLimPctWinTmsDataPoint.setSize(1);
    wMaxLimPctWinTmsDataPoint.setAddressOffset(6);
    wMaxLimPctWinTmsDataPoint.setBlockOffset(4);
    wMaxLimPctWinTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wMaxLimPctWinTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wMaxLimPctWinTmsDataPoint.name(), wMaxLimPctWinTmsDataPoint);

    SunSpecDataPoint wMaxLimPctRvrtTmsDataPoint;
    wMaxLimPctRvrtTmsDataPoint.setName("WMaxLimPct_RvrtTms");
    wMaxLimPctRvrtTmsDataPoint.setLabel("WMaxLimPct_RvrtTms");
    wMaxLimPctRvrtTmsDataPoint.setDescription("Timeout period for power limit.");
    wMaxLimPctRvrtTmsDataPoint.setUnits("Secs");
    wMaxLimPctRvrtTmsDataPoint.setSize(1);
    wMaxLimPctRvrtTmsDataPoint.setAddressOffset(7);
    wMaxLimPctRvrtTmsDataPoint.setBlockOffset(5);
    wMaxLimPctRvrtTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wMaxLimPctRvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wMaxLimPctRvrtTmsDataPoint.name(), wMaxLimPctRvrtTmsDataPoint);

    SunSpecDataPoint wMaxLimPctRmpTmsDataPoint;
    wMaxLimPctRmpTmsDataPoint.setName("WMaxLimPct_RmpTms");
    wMaxLimPctRmpTmsDataPoint.setLabel("WMaxLimPct_RmpTms");
    wMaxLimPctRmpTmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    wMaxLimPctRmpTmsDataPoint.setUnits("Secs");
    wMaxLimPctRmpTmsDataPoint.setSize(1);
    wMaxLimPctRmpTmsDataPoint.setAddressOffset(8);
    wMaxLimPctRmpTmsDataPoint.setBlockOffset(6);
    wMaxLimPctRmpTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wMaxLimPctRmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wMaxLimPctRmpTmsDataPoint.name(), wMaxLimPctRmpTmsDataPoint);

    SunSpecDataPoint wMaxLimEnaDataPoint;
    wMaxLimEnaDataPoint.setName("WMaxLim_Ena");
    wMaxLimEnaDataPoint.setLabel("WMaxLim_Ena");
    wMaxLimEnaDataPoint.setDescription("Enumerated valued.  Throttle enable/disable control.");
    wMaxLimEnaDataPoint.setMandatory(true);
    wMaxLimEnaDataPoint.setSize(1);
    wMaxLimEnaDataPoint.setAddressOffset(9);
    wMaxLimEnaDataPoint.setBlockOffset(7);
    wMaxLimEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    wMaxLimEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wMaxLimEnaDataPoint.name(), wMaxLimEnaDataPoint);

    SunSpecDataPoint outPfSetDataPoint;
    outPfSetDataPoint.setName("OutPFSet");
    outPfSetDataPoint.setLabel("OutPFSet");
    outPfSetDataPoint.setDescription("Set power factor to specific value - cosine of angle.");
    outPfSetDataPoint.setUnits("cos()");
    outPfSetDataPoint.setMandatory(true);
    outPfSetDataPoint.setSize(1);
    outPfSetDataPoint.setAddressOffset(10);
    outPfSetDataPoint.setBlockOffset(8);
    outPfSetDataPoint.setScaleFactorName("OutPFSet_SF");
    outPfSetDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    outPfSetDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outPfSetDataPoint.name(), outPfSetDataPoint);

    SunSpecDataPoint outPfSetWinTmsDataPoint;
    outPfSetWinTmsDataPoint.setName("OutPFSet_WinTms");
    outPfSetWinTmsDataPoint.setLabel("OutPFSet_WinTms");
    outPfSetWinTmsDataPoint.setDescription("Time window for power factor change.");
    outPfSetWinTmsDataPoint.setUnits("Secs");
    outPfSetWinTmsDataPoint.setSize(1);
    outPfSetWinTmsDataPoint.setAddressOffset(11);
    outPfSetWinTmsDataPoint.setBlockOffset(9);
    outPfSetWinTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    outPfSetWinTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outPfSetWinTmsDataPoint.name(), outPfSetWinTmsDataPoint);

    SunSpecDataPoint outPfSetRvrtTmsDataPoint;
    outPfSetRvrtTmsDataPoint.setName("OutPFSet_RvrtTms");
    outPfSetRvrtTmsDataPoint.setLabel("OutPFSet_RvrtTms");
    outPfSetRvrtTmsDataPoint.setDescription("Timeout period for power factor.");
    outPfSetRvrtTmsDataPoint.setUnits("Secs");
    outPfSetRvrtTmsDataPoint.setSize(1);
    outPfSetRvrtTmsDataPoint.setAddressOffset(12);
    outPfSetRvrtTmsDataPoint.setBlockOffset(10);
    outPfSetRvrtTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    outPfSetRvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outPfSetRvrtTmsDataPoint.name(), outPfSetRvrtTmsDataPoint);

    SunSpecDataPoint outPfSetRmpTmsDataPoint;
    outPfSetRmpTmsDataPoint.setName("OutPFSet_RmpTms");
    outPfSetRmpTmsDataPoint.setLabel("OutPFSet_RmpTms");
    outPfSetRmpTmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    outPfSetRmpTmsDataPoint.setUnits("Secs");
    outPfSetRmpTmsDataPoint.setSize(1);
    outPfSetRmpTmsDataPoint.setAddressOffset(13);
    outPfSetRmpTmsDataPoint.setBlockOffset(11);
    outPfSetRmpTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    outPfSetRmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outPfSetRmpTmsDataPoint.name(), outPfSetRmpTmsDataPoint);

    SunSpecDataPoint outPfSetEnaDataPoint;
    outPfSetEnaDataPoint.setName("OutPFSet_Ena");
    outPfSetEnaDataPoint.setLabel("OutPFSet_Ena");
    outPfSetEnaDataPoint.setDescription("Enumerated valued.  Fixed power factor enable/disable control.");
    outPfSetEnaDataPoint.setMandatory(true);
    outPfSetEnaDataPoint.setSize(1);
    outPfSetEnaDataPoint.setAddressOffset(14);
    outPfSetEnaDataPoint.setBlockOffset(12);
    outPfSetEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    outPfSetEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outPfSetEnaDataPoint.name(), outPfSetEnaDataPoint);

    SunSpecDataPoint vArWMaxPctDataPoint;
    vArWMaxPctDataPoint.setName("VArWMaxPct");
    vArWMaxPctDataPoint.setLabel("VArWMaxPct");
    vArWMaxPctDataPoint.setDescription("Reactive power in percent of WMax.");
    vArWMaxPctDataPoint.setUnits("% WMax");
    vArWMaxPctDataPoint.setSize(1);
    vArWMaxPctDataPoint.setAddressOffset(15);
    vArWMaxPctDataPoint.setBlockOffset(13);
    vArWMaxPctDataPoint.setScaleFactorName("VArPct_SF");
    vArWMaxPctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    vArWMaxPctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArWMaxPctDataPoint.name(), vArWMaxPctDataPoint);

    SunSpecDataPoint vArMaxPctDataPoint;
    vArMaxPctDataPoint.setName("VArMaxPct");
    vArMaxPctDataPoint.setLabel("VArMaxPct");
    vArMaxPctDataPoint.setDescription("Reactive power in percent of VArMax.");
    vArMaxPctDataPoint.setUnits("% VArMax");
    vArMaxPctDataPoint.setSize(1);
    vArMaxPctDataPoint.setAddressOffset(16);
    vArMaxPctDataPoint.setBlockOffset(14);
    vArMaxPctDataPoint.setScaleFactorName("VArPct_SF");
    vArMaxPctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    vArMaxPctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArMaxPctDataPoint.name(), vArMaxPctDataPoint);

    SunSpecDataPoint vArAvalPctDataPoint;
    vArAvalPctDataPoint.setName("VArAvalPct");
    vArAvalPctDataPoint.setLabel("VArAvalPct");
    vArAvalPctDataPoint.setDescription("Reactive power in percent of VArAval.");
    vArAvalPctDataPoint.setUnits("% VArAval");
    vArAvalPctDataPoint.setSize(1);
    vArAvalPctDataPoint.setAddressOffset(17);
    vArAvalPctDataPoint.setBlockOffset(15);
    vArAvalPctDataPoint.setScaleFactorName("VArPct_SF");
    vArAvalPctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    vArAvalPctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArAvalPctDataPoint.name(), vArAvalPctDataPoint);

    SunSpecDataPoint vArPctWinTmsDataPoint;
    vArPctWinTmsDataPoint.setName("VArPct_WinTms");
    vArPctWinTmsDataPoint.setLabel("VArPct_WinTms");
    vArPctWinTmsDataPoint.setDescription("Time window for VAR limit change.");
    vArPctWinTmsDataPoint.setUnits("Secs");
    vArPctWinTmsDataPoint.setSize(1);
    vArPctWinTmsDataPoint.setAddressOffset(18);
    vArPctWinTmsDataPoint.setBlockOffset(16);
    vArPctWinTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vArPctWinTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArPctWinTmsDataPoint.name(), vArPctWinTmsDataPoint);

    SunSpecDataPoint vArPctRvrtTmsDataPoint;
    vArPctRvrtTmsDataPoint.setName("VArPct_RvrtTms");
    vArPctRvrtTmsDataPoint.setLabel("VArPct_RvrtTms");
    vArPctRvrtTmsDataPoint.setDescription("Timeout period for VAR limit.");
    vArPctRvrtTmsDataPoint.setUnits("Secs");
    vArPctRvrtTmsDataPoint.setSize(1);
    vArPctRvrtTmsDataPoint.setAddressOffset(19);
    vArPctRvrtTmsDataPoint.setBlockOffset(17);
    vArPctRvrtTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vArPctRvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArPctRvrtTmsDataPoint.name(), vArPctRvrtTmsDataPoint);

    SunSpecDataPoint vArPctRmpTmsDataPoint;
    vArPctRmpTmsDataPoint.setName("VArPct_RmpTms");
    vArPctRmpTmsDataPoint.setLabel("VArPct_RmpTms");
    vArPctRmpTmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    vArPctRmpTmsDataPoint.setUnits("Secs");
    vArPctRmpTmsDataPoint.setSize(1);
    vArPctRmpTmsDataPoint.setAddressOffset(20);
    vArPctRmpTmsDataPoint.setBlockOffset(18);
    vArPctRmpTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    vArPctRmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArPctRmpTmsDataPoint.name(), vArPctRmpTmsDataPoint);

    SunSpecDataPoint vArPctModDataPoint;
    vArPctModDataPoint.setName("VArPct_Mod");
    vArPctModDataPoint.setLabel("VArPct_Mod");
    vArPctModDataPoint.setDescription("Enumerated value. VAR percent limit mode.");
    vArPctModDataPoint.setSize(1);
    vArPctModDataPoint.setAddressOffset(21);
    vArPctModDataPoint.setBlockOffset(19);
    vArPctModDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    vArPctModDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArPctModDataPoint.name(), vArPctModDataPoint);

    SunSpecDataPoint vArPctEnaDataPoint;
    vArPctEnaDataPoint.setName("VArPct_Ena");
    vArPctEnaDataPoint.setLabel("VArPct_Ena");
    vArPctEnaDataPoint.setDescription("Enumerated valued.  Percent limit VAr enable/disable control.");
    vArPctEnaDataPoint.setMandatory(true);
    vArPctEnaDataPoint.setSize(1);
    vArPctEnaDataPoint.setAddressOffset(22);
    vArPctEnaDataPoint.setBlockOffset(20);
    vArPctEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    vArPctEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(vArPctEnaDataPoint.name(), vArPctEnaDataPoint);

    SunSpecDataPoint wMaxLimPctSfDataPoint;
    wMaxLimPctSfDataPoint.setName("WMaxLimPct_SF");
    wMaxLimPctSfDataPoint.setLabel("WMaxLimPct_SF");
    wMaxLimPctSfDataPoint.setDescription("Scale factor for power output percent.");
    wMaxLimPctSfDataPoint.setMandatory(true);
    wMaxLimPctSfDataPoint.setSize(1);
    wMaxLimPctSfDataPoint.setAddressOffset(23);
    wMaxLimPctSfDataPoint.setBlockOffset(21);
    wMaxLimPctSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(wMaxLimPctSfDataPoint.name(), wMaxLimPctSfDataPoint);

    SunSpecDataPoint outPfSetSfDataPoint;
    outPfSetSfDataPoint.setName("OutPFSet_SF");
    outPfSetSfDataPoint.setLabel("OutPFSet_SF");
    outPfSetSfDataPoint.setDescription("Scale factor for power factor.");
    outPfSetSfDataPoint.setMandatory(true);
    outPfSetSfDataPoint.setSize(1);
    outPfSetSfDataPoint.setAddressOffset(24);
    outPfSetSfDataPoint.setBlockOffset(22);
    outPfSetSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(outPfSetSfDataPoint.name(), outPfSetSfDataPoint);

    SunSpecDataPoint vArPctSfDataPoint;
    vArPctSfDataPoint.setName("VArPct_SF");
    vArPctSfDataPoint.setLabel("VArPct_SF");
    vArPctSfDataPoint.setDescription("Scale factor for reactive power percent.");
    vArPctSfDataPoint.setSize(1);
    vArPctSfDataPoint.setAddressOffset(25);
    vArPctSfDataPoint.setBlockOffset(23);
    vArPctSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vArPctSfDataPoint.name(), vArPctSfDataPoint);

}

