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

SunSpecControlsModel::SunSpecControlsModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
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

void SunSpecControlsModel::readModelHeader()
{

}

void SunSpecControlsModel::readBlockData()
{

}

void SunSpecControlsModel::initDataPoints()
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

    SunSpecDataPoint connWintmsDataPoint;
    connWintmsDataPoint.setName("Conn_WinTms");
    connWintmsDataPoint.setLabel("Conn_WinTms");
    connWintmsDataPoint.setDescription("Time window for connect/disconnect.");
    connWintmsDataPoint.setUnits("Secs");
    connWintmsDataPoint.setSize(1);
    connWintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connWintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << connWintmsDataPoint;

    SunSpecDataPoint connRvrttmsDataPoint;
    connRvrttmsDataPoint.setName("Conn_RvrtTms");
    connRvrttmsDataPoint.setLabel("Conn_RvrtTms");
    connRvrttmsDataPoint.setDescription("Timeout period for connect/disconnect.");
    connRvrttmsDataPoint.setUnits("Secs");
    connRvrttmsDataPoint.setSize(1);
    connRvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connRvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << connRvrttmsDataPoint;

    SunSpecDataPoint connDataPoint;
    connDataPoint.setName("Conn");
    connDataPoint.setLabel("Conn");
    connDataPoint.setDescription("Enumerated valued.  Connection control.");
    connDataPoint.setMandatory(true);
    connDataPoint.setSize(1);
    connDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    connDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << connDataPoint;

    SunSpecDataPoint wmaxlimpctDataPoint;
    wmaxlimpctDataPoint.setName("WMaxLimPct");
    wmaxlimpctDataPoint.setLabel("WMaxLimPct");
    wmaxlimpctDataPoint.setDescription("Set power output to specified level.");
    wmaxlimpctDataPoint.setUnits("% WMax");
    wmaxlimpctDataPoint.setMandatory(true);
    wmaxlimpctDataPoint.setSize(1);
    wmaxlimpctDataPoint.setScaleFactorName("WMaxLimPct_SF");
    wmaxlimpctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wmaxlimpctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wmaxlimpctDataPoint;

    SunSpecDataPoint wmaxlimpctWintmsDataPoint;
    wmaxlimpctWintmsDataPoint.setName("WMaxLimPct_WinTms");
    wmaxlimpctWintmsDataPoint.setLabel("WMaxLimPct_WinTms");
    wmaxlimpctWintmsDataPoint.setDescription("Time window for power limit change.");
    wmaxlimpctWintmsDataPoint.setUnits("Secs");
    wmaxlimpctWintmsDataPoint.setSize(1);
    wmaxlimpctWintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wmaxlimpctWintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wmaxlimpctWintmsDataPoint;

    SunSpecDataPoint wmaxlimpctRvrttmsDataPoint;
    wmaxlimpctRvrttmsDataPoint.setName("WMaxLimPct_RvrtTms");
    wmaxlimpctRvrttmsDataPoint.setLabel("WMaxLimPct_RvrtTms");
    wmaxlimpctRvrttmsDataPoint.setDescription("Timeout period for power limit.");
    wmaxlimpctRvrttmsDataPoint.setUnits("Secs");
    wmaxlimpctRvrttmsDataPoint.setSize(1);
    wmaxlimpctRvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wmaxlimpctRvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wmaxlimpctRvrttmsDataPoint;

    SunSpecDataPoint wmaxlimpctRmptmsDataPoint;
    wmaxlimpctRmptmsDataPoint.setName("WMaxLimPct_RmpTms");
    wmaxlimpctRmptmsDataPoint.setLabel("WMaxLimPct_RmpTms");
    wmaxlimpctRmptmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    wmaxlimpctRmptmsDataPoint.setUnits("Secs");
    wmaxlimpctRmptmsDataPoint.setSize(1);
    wmaxlimpctRmptmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wmaxlimpctRmptmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wmaxlimpctRmptmsDataPoint;

    SunSpecDataPoint wmaxlimEnaDataPoint;
    wmaxlimEnaDataPoint.setName("WMaxLim_Ena");
    wmaxlimEnaDataPoint.setLabel("WMaxLim_Ena");
    wmaxlimEnaDataPoint.setDescription("Enumerated valued.  Throttle enable/disable control.");
    wmaxlimEnaDataPoint.setMandatory(true);
    wmaxlimEnaDataPoint.setSize(1);
    wmaxlimEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    wmaxlimEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wmaxlimEnaDataPoint;

    SunSpecDataPoint outpfsetDataPoint;
    outpfsetDataPoint.setName("OutPFSet");
    outpfsetDataPoint.setLabel("OutPFSet");
    outpfsetDataPoint.setDescription("Set power factor to specific value - cosine of angle.");
    outpfsetDataPoint.setUnits("cos()");
    outpfsetDataPoint.setMandatory(true);
    outpfsetDataPoint.setSize(1);
    outpfsetDataPoint.setScaleFactorName("OutPFSet_SF");
    outpfsetDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    outpfsetDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << outpfsetDataPoint;

    SunSpecDataPoint outpfsetWintmsDataPoint;
    outpfsetWintmsDataPoint.setName("OutPFSet_WinTms");
    outpfsetWintmsDataPoint.setLabel("OutPFSet_WinTms");
    outpfsetWintmsDataPoint.setDescription("Time window for power factor change.");
    outpfsetWintmsDataPoint.setUnits("Secs");
    outpfsetWintmsDataPoint.setSize(1);
    outpfsetWintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    outpfsetWintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << outpfsetWintmsDataPoint;

    SunSpecDataPoint outpfsetRvrttmsDataPoint;
    outpfsetRvrttmsDataPoint.setName("OutPFSet_RvrtTms");
    outpfsetRvrttmsDataPoint.setLabel("OutPFSet_RvrtTms");
    outpfsetRvrttmsDataPoint.setDescription("Timeout period for power factor.");
    outpfsetRvrttmsDataPoint.setUnits("Secs");
    outpfsetRvrttmsDataPoint.setSize(1);
    outpfsetRvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    outpfsetRvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << outpfsetRvrttmsDataPoint;

    SunSpecDataPoint outpfsetRmptmsDataPoint;
    outpfsetRmptmsDataPoint.setName("OutPFSet_RmpTms");
    outpfsetRmptmsDataPoint.setLabel("OutPFSet_RmpTms");
    outpfsetRmptmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    outpfsetRmptmsDataPoint.setUnits("Secs");
    outpfsetRmptmsDataPoint.setSize(1);
    outpfsetRmptmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    outpfsetRmptmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << outpfsetRmptmsDataPoint;

    SunSpecDataPoint outpfsetEnaDataPoint;
    outpfsetEnaDataPoint.setName("OutPFSet_Ena");
    outpfsetEnaDataPoint.setLabel("OutPFSet_Ena");
    outpfsetEnaDataPoint.setDescription("Enumerated valued.  Fixed power factor enable/disable control.");
    outpfsetEnaDataPoint.setMandatory(true);
    outpfsetEnaDataPoint.setSize(1);
    outpfsetEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    outpfsetEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << outpfsetEnaDataPoint;

    SunSpecDataPoint varwmaxpctDataPoint;
    varwmaxpctDataPoint.setName("VArWMaxPct");
    varwmaxpctDataPoint.setLabel("VArWMaxPct");
    varwmaxpctDataPoint.setDescription("Reactive power in percent of WMax.");
    varwmaxpctDataPoint.setUnits("% WMax");
    varwmaxpctDataPoint.setSize(1);
    varwmaxpctDataPoint.setScaleFactorName("VArPct_SF");
    varwmaxpctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varwmaxpctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varwmaxpctDataPoint;

    SunSpecDataPoint varmaxpctDataPoint;
    varmaxpctDataPoint.setName("VArMaxPct");
    varmaxpctDataPoint.setLabel("VArMaxPct");
    varmaxpctDataPoint.setDescription("Reactive power in percent of VArMax.");
    varmaxpctDataPoint.setUnits("% VArMax");
    varmaxpctDataPoint.setSize(1);
    varmaxpctDataPoint.setScaleFactorName("VArPct_SF");
    varmaxpctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varmaxpctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varmaxpctDataPoint;

    SunSpecDataPoint varavalpctDataPoint;
    varavalpctDataPoint.setName("VArAvalPct");
    varavalpctDataPoint.setLabel("VArAvalPct");
    varavalpctDataPoint.setDescription("Reactive power in percent of VArAval.");
    varavalpctDataPoint.setUnits("% VArAval");
    varavalpctDataPoint.setSize(1);
    varavalpctDataPoint.setScaleFactorName("VArPct_SF");
    varavalpctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varavalpctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varavalpctDataPoint;

    SunSpecDataPoint varpctWintmsDataPoint;
    varpctWintmsDataPoint.setName("VArPct_WinTms");
    varpctWintmsDataPoint.setLabel("VArPct_WinTms");
    varpctWintmsDataPoint.setDescription("Time window for VAR limit change.");
    varpctWintmsDataPoint.setUnits("Secs");
    varpctWintmsDataPoint.setSize(1);
    varpctWintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    varpctWintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varpctWintmsDataPoint;

    SunSpecDataPoint varpctRvrttmsDataPoint;
    varpctRvrttmsDataPoint.setName("VArPct_RvrtTms");
    varpctRvrttmsDataPoint.setLabel("VArPct_RvrtTms");
    varpctRvrttmsDataPoint.setDescription("Timeout period for VAR limit.");
    varpctRvrttmsDataPoint.setUnits("Secs");
    varpctRvrttmsDataPoint.setSize(1);
    varpctRvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    varpctRvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varpctRvrttmsDataPoint;

    SunSpecDataPoint varpctRmptmsDataPoint;
    varpctRmptmsDataPoint.setName("VArPct_RmpTms");
    varpctRmptmsDataPoint.setLabel("VArPct_RmpTms");
    varpctRmptmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    varpctRmptmsDataPoint.setUnits("Secs");
    varpctRmptmsDataPoint.setSize(1);
    varpctRmptmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    varpctRmptmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varpctRmptmsDataPoint;

    SunSpecDataPoint varpctModDataPoint;
    varpctModDataPoint.setName("VArPct_Mod");
    varpctModDataPoint.setLabel("VArPct_Mod");
    varpctModDataPoint.setDescription("Enumerated value. VAR percent limit mode.");
    varpctModDataPoint.setSize(1);
    varpctModDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    varpctModDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varpctModDataPoint;

    SunSpecDataPoint varpctEnaDataPoint;
    varpctEnaDataPoint.setName("VArPct_Ena");
    varpctEnaDataPoint.setLabel("VArPct_Ena");
    varpctEnaDataPoint.setDescription("Enumerated valued.  Percent limit VAr enable/disable control.");
    varpctEnaDataPoint.setMandatory(true);
    varpctEnaDataPoint.setSize(1);
    varpctEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    varpctEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << varpctEnaDataPoint;

    SunSpecDataPoint wmaxlimpctSfDataPoint;
    wmaxlimpctSfDataPoint.setName("WMaxLimPct_SF");
    wmaxlimpctSfDataPoint.setLabel("WMaxLimPct_SF");
    wmaxlimpctSfDataPoint.setDescription("Scale factor for power output percent.");
    wmaxlimpctSfDataPoint.setMandatory(true);
    wmaxlimpctSfDataPoint.setSize(1);
    wmaxlimpctSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << wmaxlimpctSfDataPoint;

    SunSpecDataPoint outpfsetSfDataPoint;
    outpfsetSfDataPoint.setName("OutPFSet_SF");
    outpfsetSfDataPoint.setLabel("OutPFSet_SF");
    outpfsetSfDataPoint.setDescription("Scale factor for power factor.");
    outpfsetSfDataPoint.setMandatory(true);
    outpfsetSfDataPoint.setSize(1);
    outpfsetSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << outpfsetSfDataPoint;

    SunSpecDataPoint varpctSfDataPoint;
    varpctSfDataPoint.setName("VArPct_SF");
    varpctSfDataPoint.setLabel("VArPct_SF");
    varpctSfDataPoint.setDescription("Scale factor for reactive power percent.");
    varpctSfDataPoint.setSize(1);
    varpctSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << varpctSfDataPoint;

}

