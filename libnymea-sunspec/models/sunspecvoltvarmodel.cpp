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

#include "sunspecvoltvarmodel.h"
#include "sunspecconnection.h"

SunSpecVoltVarModel::SunSpecVoltVarModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, 126, 10, modbusStartRegister, parent)
{
    Q_ASSERT_X(length == 10,  "SunSpecVoltVarModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecVoltVarModel::~SunSpecVoltVarModel()
{

}

QString SunSpecVoltVarModel::name() const
{
    return "volt_var";
}

QString SunSpecVoltVarModel::description() const
{
    return "Static Volt-VAR Arrays ";
}

QString SunSpecVoltVarModel::label() const
{
    return "Static Volt-VAR";
}

quint16 SunSpecVoltVarModel::actCrv() const
{
    return m_actCrv;
}

QModbusReply *SunSpecVoltVarModel::setActCrv(quint16 actCrv)
{
    Q_UNUSED(actCrv)
    return nullptr;
}
SunSpecVoltVarModel::ModenaFlags SunSpecVoltVarModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecVoltVarModel::setModEna(ModenaFlags modEna)
{
    Q_UNUSED(modEna)
    return nullptr;
}
quint16 SunSpecVoltVarModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecVoltVarModel::setWinTms(quint16 winTms)
{
    Q_UNUSED(winTms)
    return nullptr;
}
quint16 SunSpecVoltVarModel::rvrtTms() const
{
    return m_rvrtTms;
}

QModbusReply *SunSpecVoltVarModel::setRvrtTms(quint16 rvrtTms)
{
    Q_UNUSED(rvrtTms)
    return nullptr;
}
quint16 SunSpecVoltVarModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecVoltVarModel::setRmpTms(quint16 rmpTms)
{
    Q_UNUSED(rmpTms)
    return nullptr;
}
quint16 SunSpecVoltVarModel::nCrv() const
{
    return m_nCrv;
}
quint16 SunSpecVoltVarModel::nPt() const
{
    return m_nPt;
}
void SunSpecVoltVarModel::processBlockData()
{
    // Scale factors
    m_vSf = m_dataPoints.value("V_SF").toInt16();
    m_deptRefSf = m_dataPoints.value("DeptRef_SF").toInt16();
    m_rmpIncDec_SF = m_dataPoints.value("RmpIncDec_SF").toInt16();

    // Update properties according to the data point type
    m_actCrv = m_dataPoints.value("ActCrv").toUInt16();
    m_modEna = static_cast<ModenaFlags>(m_dataPoints.value("ModEna").toUInt16());
    m_winTms = m_dataPoints.value("WinTms").toUInt16();
    m_rvrtTms = m_dataPoints.value("RvrtTms").toUInt16();
    m_rmpTms = m_dataPoints.value("RmpTms").toUInt16();
    m_nCrv = m_dataPoints.value("NCrv").toUInt16();
    m_nPt = m_dataPoints.value("NPt").toUInt16();
}

void SunSpecVoltVarModel::initDataPoints()
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

    SunSpecDataPoint actCrvDataPoint;
    actCrvDataPoint.setName("ActCrv");
    actCrvDataPoint.setLabel("ActCrv");
    actCrvDataPoint.setDescription("Index of active curve. 0=no active curve.");
    actCrvDataPoint.setMandatory(true);
    actCrvDataPoint.setSize(1);
    actCrvDataPoint.setAddressOffset(2);
    actCrvDataPoint.setBlockOffset(0);
    actCrvDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    actCrvDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(actCrvDataPoint.name(), actCrvDataPoint);

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("Is Volt-VAR control active.");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(3);
    modEnaDataPoint.setBlockOffset(1);
    modEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint winTmsDataPoint;
    winTmsDataPoint.setName("WinTms");
    winTmsDataPoint.setLabel("WinTms");
    winTmsDataPoint.setDescription("Time window for volt-VAR change.");
    winTmsDataPoint.setUnits("Secs");
    winTmsDataPoint.setSize(1);
    winTmsDataPoint.setAddressOffset(4);
    winTmsDataPoint.setBlockOffset(2);
    winTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    winTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(winTmsDataPoint.name(), winTmsDataPoint);

    SunSpecDataPoint rvrtTmsDataPoint;
    rvrtTmsDataPoint.setName("RvrtTms");
    rvrtTmsDataPoint.setLabel("RvrtTms");
    rvrtTmsDataPoint.setDescription("Timeout period for volt-VAR curve selection.");
    rvrtTmsDataPoint.setUnits("Secs");
    rvrtTmsDataPoint.setSize(1);
    rvrtTmsDataPoint.setAddressOffset(5);
    rvrtTmsDataPoint.setBlockOffset(3);
    rvrtTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    rvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(rvrtTmsDataPoint.name(), rvrtTmsDataPoint);

    SunSpecDataPoint rmpTmsDataPoint;
    rmpTmsDataPoint.setName("RmpTms");
    rmpTmsDataPoint.setLabel("RmpTms");
    rmpTmsDataPoint.setDescription("The time of the PT1 in seconds (time to accomplish a change of 95%).");
    rmpTmsDataPoint.setUnits("Secs");
    rmpTmsDataPoint.setSize(1);
    rmpTmsDataPoint.setAddressOffset(6);
    rmpTmsDataPoint.setBlockOffset(4);
    rmpTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    rmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(rmpTmsDataPoint.name(), rmpTmsDataPoint);

    SunSpecDataPoint nCrvDataPoint;
    nCrvDataPoint.setName("NCrv");
    nCrvDataPoint.setLabel("NCrv");
    nCrvDataPoint.setDescription("Number of curves supported (recommend 4).");
    nCrvDataPoint.setMandatory(true);
    nCrvDataPoint.setSize(1);
    nCrvDataPoint.setAddressOffset(7);
    nCrvDataPoint.setBlockOffset(5);
    nCrvDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nCrvDataPoint.name(), nCrvDataPoint);

    SunSpecDataPoint nPtDataPoint;
    nPtDataPoint.setName("NPt");
    nPtDataPoint.setLabel("NPt");
    nPtDataPoint.setDescription("Number of curve points supported (maximum of 20).");
    nPtDataPoint.setMandatory(true);
    nPtDataPoint.setSize(1);
    nPtDataPoint.setAddressOffset(8);
    nPtDataPoint.setBlockOffset(6);
    nPtDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nPtDataPoint.name(), nPtDataPoint);

    SunSpecDataPoint vSfDataPoint;
    vSfDataPoint.setName("V_SF");
    vSfDataPoint.setLabel("V_SF");
    vSfDataPoint.setDescription("Scale factor for percent VRef.");
    vSfDataPoint.setMandatory(true);
    vSfDataPoint.setSize(1);
    vSfDataPoint.setAddressOffset(9);
    vSfDataPoint.setBlockOffset(7);
    vSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vSfDataPoint.name(), vSfDataPoint);

    SunSpecDataPoint deptRefSfDataPoint;
    deptRefSfDataPoint.setName("DeptRef_SF");
    deptRefSfDataPoint.setLabel("DeptRef_SF");
    deptRefSfDataPoint.setDescription("scale factor for dependent variable.");
    deptRefSfDataPoint.setMandatory(true);
    deptRefSfDataPoint.setSize(1);
    deptRefSfDataPoint.setAddressOffset(10);
    deptRefSfDataPoint.setBlockOffset(8);
    deptRefSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(deptRefSfDataPoint.name(), deptRefSfDataPoint);

    SunSpecDataPoint rmpIncDec_SFDataPoint;
    rmpIncDec_SFDataPoint.setName("RmpIncDec_SF");
    rmpIncDec_SFDataPoint.setSize(1);
    rmpIncDec_SFDataPoint.setAddressOffset(11);
    rmpIncDec_SFDataPoint.setBlockOffset(9);
    rmpIncDec_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(rmpIncDec_SFDataPoint.name(), rmpIncDec_SFDataPoint);

}

