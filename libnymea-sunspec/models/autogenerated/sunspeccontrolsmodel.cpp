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
    m_supportedModelIds << 123;
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

    SunSpecDataPoint connWintmsDataPoint;
    connWintmsDataPoint.setName("Conn_WinTms");
    connWintmsDataPoint.setLabel("Conn_WinTms");
    connWintmsDataPoint.setDescription("Time window for connect/disconnect.");
    connWintmsDataPoint.setUnits("Secs");
    connWintmsDataPoint.setSize(1);
    connWintmsDataPoint.setAddressOffset(2);
    connWintmsDataPoint.setBlockOffset(0);
    connWintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connWintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connWintmsDataPoint.name(), connWintmsDataPoint);

    SunSpecDataPoint connRvrttmsDataPoint;
    connRvrttmsDataPoint.setName("Conn_RvrtTms");
    connRvrttmsDataPoint.setLabel("Conn_RvrtTms");
    connRvrttmsDataPoint.setDescription("Timeout period for connect/disconnect.");
    connRvrttmsDataPoint.setUnits("Secs");
    connRvrttmsDataPoint.setSize(1);
    connRvrttmsDataPoint.setAddressOffset(3);
    connRvrttmsDataPoint.setBlockOffset(1);
    connRvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connRvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connRvrttmsDataPoint.name(), connRvrttmsDataPoint);

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

    SunSpecDataPoint wmaxlimpctDataPoint;
    wmaxlimpctDataPoint.setName("WMaxLimPct");
    wmaxlimpctDataPoint.setLabel("WMaxLimPct");
    wmaxlimpctDataPoint.setDescription("Set power output to specified level.");
    wmaxlimpctDataPoint.setUnits("% WMax");
    wmaxlimpctDataPoint.setMandatory(true);
    wmaxlimpctDataPoint.setSize(1);
    wmaxlimpctDataPoint.setAddressOffset(5);
    wmaxlimpctDataPoint.setBlockOffset(3);
    wmaxlimpctDataPoint.setScaleFactorName("WMaxLimPct_SF");
    wmaxlimpctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wmaxlimpctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wmaxlimpctDataPoint.name(), wmaxlimpctDataPoint);

    SunSpecDataPoint wmaxlimpctWintmsDataPoint;
    wmaxlimpctWintmsDataPoint.setName("WMaxLimPct_WinTms");
    wmaxlimpctWintmsDataPoint.setLabel("WMaxLimPct_WinTms");
    wmaxlimpctWintmsDataPoint.setDescription("Time window for power limit change.");
    wmaxlimpctWintmsDataPoint.setUnits("Secs");
    wmaxlimpctWintmsDataPoint.setSize(1);
    wmaxlimpctWintmsDataPoint.setAddressOffset(6);
    wmaxlimpctWintmsDataPoint.setBlockOffset(4);
    wmaxlimpctWintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wmaxlimpctWintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wmaxlimpctWintmsDataPoint.name(), wmaxlimpctWintmsDataPoint);

    SunSpecDataPoint wmaxlimpctRvrttmsDataPoint;
    wmaxlimpctRvrttmsDataPoint.setName("WMaxLimPct_RvrtTms");
    wmaxlimpctRvrttmsDataPoint.setLabel("WMaxLimPct_RvrtTms");
    wmaxlimpctRvrttmsDataPoint.setDescription("Timeout period for power limit.");
    wmaxlimpctRvrttmsDataPoint.setUnits("Secs");
    wmaxlimpctRvrttmsDataPoint.setSize(1);
    wmaxlimpctRvrttmsDataPoint.setAddressOffset(7);
    wmaxlimpctRvrttmsDataPoint.setBlockOffset(5);
    wmaxlimpctRvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wmaxlimpctRvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wmaxlimpctRvrttmsDataPoint.name(), wmaxlimpctRvrttmsDataPoint);

    SunSpecDataPoint wmaxlimpctRmptmsDataPoint;
    wmaxlimpctRmptmsDataPoint.setName("WMaxLimPct_RmpTms");
    wmaxlimpctRmptmsDataPoint.setLabel("WMaxLimPct_RmpTms");
    wmaxlimpctRmptmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    wmaxlimpctRmptmsDataPoint.setUnits("Secs");
    wmaxlimpctRmptmsDataPoint.setSize(1);
    wmaxlimpctRmptmsDataPoint.setAddressOffset(8);
    wmaxlimpctRmptmsDataPoint.setBlockOffset(6);
    wmaxlimpctRmptmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wmaxlimpctRmptmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wmaxlimpctRmptmsDataPoint.name(), wmaxlimpctRmptmsDataPoint);

    SunSpecDataPoint wmaxlimEnaDataPoint;
    wmaxlimEnaDataPoint.setName("WMaxLim_Ena");
    wmaxlimEnaDataPoint.setLabel("WMaxLim_Ena");
    wmaxlimEnaDataPoint.setDescription("Enumerated valued.  Throttle enable/disable control.");
    wmaxlimEnaDataPoint.setMandatory(true);
    wmaxlimEnaDataPoint.setSize(1);
    wmaxlimEnaDataPoint.setAddressOffset(9);
    wmaxlimEnaDataPoint.setBlockOffset(7);
    wmaxlimEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    wmaxlimEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wmaxlimEnaDataPoint.name(), wmaxlimEnaDataPoint);

    SunSpecDataPoint outpfsetDataPoint;
    outpfsetDataPoint.setName("OutPFSet");
    outpfsetDataPoint.setLabel("OutPFSet");
    outpfsetDataPoint.setDescription("Set power factor to specific value - cosine of angle.");
    outpfsetDataPoint.setUnits("cos()");
    outpfsetDataPoint.setMandatory(true);
    outpfsetDataPoint.setSize(1);
    outpfsetDataPoint.setAddressOffset(10);
    outpfsetDataPoint.setBlockOffset(8);
    outpfsetDataPoint.setScaleFactorName("OutPFSet_SF");
    outpfsetDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    outpfsetDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outpfsetDataPoint.name(), outpfsetDataPoint);

    SunSpecDataPoint outpfsetWintmsDataPoint;
    outpfsetWintmsDataPoint.setName("OutPFSet_WinTms");
    outpfsetWintmsDataPoint.setLabel("OutPFSet_WinTms");
    outpfsetWintmsDataPoint.setDescription("Time window for power factor change.");
    outpfsetWintmsDataPoint.setUnits("Secs");
    outpfsetWintmsDataPoint.setSize(1);
    outpfsetWintmsDataPoint.setAddressOffset(11);
    outpfsetWintmsDataPoint.setBlockOffset(9);
    outpfsetWintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    outpfsetWintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outpfsetWintmsDataPoint.name(), outpfsetWintmsDataPoint);

    SunSpecDataPoint outpfsetRvrttmsDataPoint;
    outpfsetRvrttmsDataPoint.setName("OutPFSet_RvrtTms");
    outpfsetRvrttmsDataPoint.setLabel("OutPFSet_RvrtTms");
    outpfsetRvrttmsDataPoint.setDescription("Timeout period for power factor.");
    outpfsetRvrttmsDataPoint.setUnits("Secs");
    outpfsetRvrttmsDataPoint.setSize(1);
    outpfsetRvrttmsDataPoint.setAddressOffset(12);
    outpfsetRvrttmsDataPoint.setBlockOffset(10);
    outpfsetRvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    outpfsetRvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outpfsetRvrttmsDataPoint.name(), outpfsetRvrttmsDataPoint);

    SunSpecDataPoint outpfsetRmptmsDataPoint;
    outpfsetRmptmsDataPoint.setName("OutPFSet_RmpTms");
    outpfsetRmptmsDataPoint.setLabel("OutPFSet_RmpTms");
    outpfsetRmptmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    outpfsetRmptmsDataPoint.setUnits("Secs");
    outpfsetRmptmsDataPoint.setSize(1);
    outpfsetRmptmsDataPoint.setAddressOffset(13);
    outpfsetRmptmsDataPoint.setBlockOffset(11);
    outpfsetRmptmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    outpfsetRmptmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outpfsetRmptmsDataPoint.name(), outpfsetRmptmsDataPoint);

    SunSpecDataPoint outpfsetEnaDataPoint;
    outpfsetEnaDataPoint.setName("OutPFSet_Ena");
    outpfsetEnaDataPoint.setLabel("OutPFSet_Ena");
    outpfsetEnaDataPoint.setDescription("Enumerated valued.  Fixed power factor enable/disable control.");
    outpfsetEnaDataPoint.setMandatory(true);
    outpfsetEnaDataPoint.setSize(1);
    outpfsetEnaDataPoint.setAddressOffset(14);
    outpfsetEnaDataPoint.setBlockOffset(12);
    outpfsetEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    outpfsetEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(outpfsetEnaDataPoint.name(), outpfsetEnaDataPoint);

    SunSpecDataPoint varwmaxpctDataPoint;
    varwmaxpctDataPoint.setName("VArWMaxPct");
    varwmaxpctDataPoint.setLabel("VArWMaxPct");
    varwmaxpctDataPoint.setDescription("Reactive power in percent of WMax.");
    varwmaxpctDataPoint.setUnits("% WMax");
    varwmaxpctDataPoint.setSize(1);
    varwmaxpctDataPoint.setAddressOffset(15);
    varwmaxpctDataPoint.setBlockOffset(13);
    varwmaxpctDataPoint.setScaleFactorName("VArPct_SF");
    varwmaxpctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varwmaxpctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(varwmaxpctDataPoint.name(), varwmaxpctDataPoint);

    SunSpecDataPoint varmaxpctDataPoint;
    varmaxpctDataPoint.setName("VArMaxPct");
    varmaxpctDataPoint.setLabel("VArMaxPct");
    varmaxpctDataPoint.setDescription("Reactive power in percent of VArMax.");
    varmaxpctDataPoint.setUnits("% VArMax");
    varmaxpctDataPoint.setSize(1);
    varmaxpctDataPoint.setAddressOffset(16);
    varmaxpctDataPoint.setBlockOffset(14);
    varmaxpctDataPoint.setScaleFactorName("VArPct_SF");
    varmaxpctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varmaxpctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(varmaxpctDataPoint.name(), varmaxpctDataPoint);

    SunSpecDataPoint varavalpctDataPoint;
    varavalpctDataPoint.setName("VArAvalPct");
    varavalpctDataPoint.setLabel("VArAvalPct");
    varavalpctDataPoint.setDescription("Reactive power in percent of VArAval.");
    varavalpctDataPoint.setUnits("% VArAval");
    varavalpctDataPoint.setSize(1);
    varavalpctDataPoint.setAddressOffset(17);
    varavalpctDataPoint.setBlockOffset(15);
    varavalpctDataPoint.setScaleFactorName("VArPct_SF");
    varavalpctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    varavalpctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(varavalpctDataPoint.name(), varavalpctDataPoint);

    SunSpecDataPoint varpctWintmsDataPoint;
    varpctWintmsDataPoint.setName("VArPct_WinTms");
    varpctWintmsDataPoint.setLabel("VArPct_WinTms");
    varpctWintmsDataPoint.setDescription("Time window for VAR limit change.");
    varpctWintmsDataPoint.setUnits("Secs");
    varpctWintmsDataPoint.setSize(1);
    varpctWintmsDataPoint.setAddressOffset(18);
    varpctWintmsDataPoint.setBlockOffset(16);
    varpctWintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    varpctWintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(varpctWintmsDataPoint.name(), varpctWintmsDataPoint);

    SunSpecDataPoint varpctRvrttmsDataPoint;
    varpctRvrttmsDataPoint.setName("VArPct_RvrtTms");
    varpctRvrttmsDataPoint.setLabel("VArPct_RvrtTms");
    varpctRvrttmsDataPoint.setDescription("Timeout period for VAR limit.");
    varpctRvrttmsDataPoint.setUnits("Secs");
    varpctRvrttmsDataPoint.setSize(1);
    varpctRvrttmsDataPoint.setAddressOffset(19);
    varpctRvrttmsDataPoint.setBlockOffset(17);
    varpctRvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    varpctRvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(varpctRvrttmsDataPoint.name(), varpctRvrttmsDataPoint);

    SunSpecDataPoint varpctRmptmsDataPoint;
    varpctRmptmsDataPoint.setName("VArPct_RmpTms");
    varpctRmptmsDataPoint.setLabel("VArPct_RmpTms");
    varpctRmptmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    varpctRmptmsDataPoint.setUnits("Secs");
    varpctRmptmsDataPoint.setSize(1);
    varpctRmptmsDataPoint.setAddressOffset(20);
    varpctRmptmsDataPoint.setBlockOffset(18);
    varpctRmptmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    varpctRmptmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(varpctRmptmsDataPoint.name(), varpctRmptmsDataPoint);

    SunSpecDataPoint varpctModDataPoint;
    varpctModDataPoint.setName("VArPct_Mod");
    varpctModDataPoint.setLabel("VArPct_Mod");
    varpctModDataPoint.setDescription("Enumerated value. VAR percent limit mode.");
    varpctModDataPoint.setSize(1);
    varpctModDataPoint.setAddressOffset(21);
    varpctModDataPoint.setBlockOffset(19);
    varpctModDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    varpctModDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(varpctModDataPoint.name(), varpctModDataPoint);

    SunSpecDataPoint varpctEnaDataPoint;
    varpctEnaDataPoint.setName("VArPct_Ena");
    varpctEnaDataPoint.setLabel("VArPct_Ena");
    varpctEnaDataPoint.setDescription("Enumerated valued.  Percent limit VAr enable/disable control.");
    varpctEnaDataPoint.setMandatory(true);
    varpctEnaDataPoint.setSize(1);
    varpctEnaDataPoint.setAddressOffset(22);
    varpctEnaDataPoint.setBlockOffset(20);
    varpctEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    varpctEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(varpctEnaDataPoint.name(), varpctEnaDataPoint);

    SunSpecDataPoint wmaxlimpctSfDataPoint;
    wmaxlimpctSfDataPoint.setName("WMaxLimPct_SF");
    wmaxlimpctSfDataPoint.setLabel("WMaxLimPct_SF");
    wmaxlimpctSfDataPoint.setDescription("Scale factor for power output percent.");
    wmaxlimpctSfDataPoint.setMandatory(true);
    wmaxlimpctSfDataPoint.setSize(1);
    wmaxlimpctSfDataPoint.setAddressOffset(23);
    wmaxlimpctSfDataPoint.setBlockOffset(21);
    wmaxlimpctSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(wmaxlimpctSfDataPoint.name(), wmaxlimpctSfDataPoint);

    SunSpecDataPoint outpfsetSfDataPoint;
    outpfsetSfDataPoint.setName("OutPFSet_SF");
    outpfsetSfDataPoint.setLabel("OutPFSet_SF");
    outpfsetSfDataPoint.setDescription("Scale factor for power factor.");
    outpfsetSfDataPoint.setMandatory(true);
    outpfsetSfDataPoint.setSize(1);
    outpfsetSfDataPoint.setAddressOffset(24);
    outpfsetSfDataPoint.setBlockOffset(22);
    outpfsetSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(outpfsetSfDataPoint.name(), outpfsetSfDataPoint);

    SunSpecDataPoint varpctSfDataPoint;
    varpctSfDataPoint.setName("VArPct_SF");
    varpctSfDataPoint.setLabel("VArPct_SF");
    varpctSfDataPoint.setDescription("Scale factor for reactive power percent.");
    varpctSfDataPoint.setSize(1);
    varpctSfDataPoint.setAddressOffset(25);
    varpctSfDataPoint.setBlockOffset(23);
    varpctSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(varpctSfDataPoint.name(), varpctSfDataPoint);

}

