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

#include "sunspecwattpfmodel.h"
#include "sunspecconnection.h"

SunSpecWattPfModel::SunSpecWattPfModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, 131, 10, modbusStartRegister, parent)
{
    Q_ASSERT_X(length == 10,  "SunSpecWattPfModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecWattPfModel::~SunSpecWattPfModel()
{

}

QString SunSpecWattPfModel::name() const
{
    return "watt_pf";
}

QString SunSpecWattPfModel::description() const
{
    return "Watt-Power Factor ";
}

QString SunSpecWattPfModel::label() const
{
    return "Watt-PF";
}

quint16 SunSpecWattPfModel::actCrv() const
{
    return m_actCrv;
}

QModbusReply *SunSpecWattPfModel::setActCrv(quint16 actCrv)
{
    Q_UNUSED(actCrv)
    return nullptr;
}
SunSpecWattPfModel::ModenaFlags SunSpecWattPfModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecWattPfModel::setModEna(ModenaFlags modEna)
{
    Q_UNUSED(modEna)
    return nullptr;
}
quint16 SunSpecWattPfModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecWattPfModel::setWinTms(quint16 winTms)
{
    Q_UNUSED(winTms)
    return nullptr;
}
quint16 SunSpecWattPfModel::rvrtTms() const
{
    return m_rvrtTms;
}

QModbusReply *SunSpecWattPfModel::setRvrtTms(quint16 rvrtTms)
{
    Q_UNUSED(rvrtTms)
    return nullptr;
}
quint16 SunSpecWattPfModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecWattPfModel::setRmpTms(quint16 rmpTms)
{
    Q_UNUSED(rmpTms)
    return nullptr;
}
quint16 SunSpecWattPfModel::nCrv() const
{
    return m_nCrv;
}
quint16 SunSpecWattPfModel::nPt() const
{
    return m_nPt;
}
void SunSpecWattPfModel::processBlockData()
{
    // Scale factors
    m_wSf = m_dataPoints.value("W_SF").toInt16();
    m_pfSf = m_dataPoints.value("PF_SF").toInt16();
    m_rmpIncDecSf = m_dataPoints.value("RmpIncDec_SF").toInt16();

    // Update properties according to the data point type
    m_actCrv = m_dataPoints.value("ActCrv").toUInt16();
    m_modEna = static_cast<ModenaFlags>(m_dataPoints.value("ModEna").toUInt16());
    m_winTms = m_dataPoints.value("WinTms").toUInt16();
    m_rvrtTms = m_dataPoints.value("RvrtTms").toUInt16();
    m_rmpTms = m_dataPoints.value("RmpTms").toUInt16();
    m_nCrv = m_dataPoints.value("NCrv").toUInt16();
    m_nPt = m_dataPoints.value("NPt").toUInt16();
}

void SunSpecWattPfModel::initDataPoints()
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
    modEnaDataPoint.setDescription("Is watt-PF mode active.");
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
    winTmsDataPoint.setDescription("Time window for watt-PF change.");
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
    rvrtTmsDataPoint.setDescription("Timeout period for watt-PF curve selection.");
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
    rmpTmsDataPoint.setDescription("Ramp time for moving from current mode to new mode.");
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
    nPtDataPoint.setDescription("Max number of points in array.");
    nPtDataPoint.setMandatory(true);
    nPtDataPoint.setSize(1);
    nPtDataPoint.setAddressOffset(8);
    nPtDataPoint.setBlockOffset(6);
    nPtDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nPtDataPoint.name(), nPtDataPoint);

    SunSpecDataPoint wSfDataPoint;
    wSfDataPoint.setName("W_SF");
    wSfDataPoint.setLabel("W_SF");
    wSfDataPoint.setDescription("Scale factor for percent WMax.");
    wSfDataPoint.setMandatory(true);
    wSfDataPoint.setSize(1);
    wSfDataPoint.setAddressOffset(9);
    wSfDataPoint.setBlockOffset(7);
    wSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(wSfDataPoint.name(), wSfDataPoint);

    SunSpecDataPoint pfSfDataPoint;
    pfSfDataPoint.setName("PF_SF");
    pfSfDataPoint.setLabel("PF_SF");
    pfSfDataPoint.setDescription("Scale factor for PF.");
    pfSfDataPoint.setMandatory(true);
    pfSfDataPoint.setSize(1);
    pfSfDataPoint.setAddressOffset(10);
    pfSfDataPoint.setBlockOffset(8);
    pfSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(pfSfDataPoint.name(), pfSfDataPoint);

    SunSpecDataPoint rmpIncDecSfDataPoint;
    rmpIncDecSfDataPoint.setName("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setLabel("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setDescription("Scale factor for increment and decrement ramps.");
    rmpIncDecSfDataPoint.setSize(1);
    rmpIncDecSfDataPoint.setAddressOffset(11);
    rmpIncDecSfDataPoint.setBlockOffset(9);
    rmpIncDecSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(rmpIncDecSfDataPoint.name(), rmpIncDecSfDataPoint);

}

