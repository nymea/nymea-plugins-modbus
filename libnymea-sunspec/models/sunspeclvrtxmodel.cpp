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

#include "sunspeclvrtxmodel.h"
#include "sunspecconnection.h"

SunSpecLvrtxModel::SunSpecLvrtxModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 139, 10, parent)
{
    //Q_ASSERT_X(length == 10,  "SunSpecLvrtxModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecLvrtxModel::~SunSpecLvrtxModel()
{

}

QString SunSpecLvrtxModel::name() const
{
    return "lvrtx";
}

QString SunSpecLvrtxModel::description() const
{
    return "LVRT extended curve";
}

QString SunSpecLvrtxModel::label() const
{
    return "LVRTX";
}

quint16 SunSpecLvrtxModel::actCrv() const
{
    return m_actCrv;
}

QModbusReply *SunSpecLvrtxModel::setActCrv(quint16 actCrv)
{
    Q_UNUSED(actCrv)
    return nullptr;
}
SunSpecLvrtxModel::ModenaFlags SunSpecLvrtxModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecLvrtxModel::setModEna(ModenaFlags modEna)
{
    Q_UNUSED(modEna)
    return nullptr;
}
quint16 SunSpecLvrtxModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecLvrtxModel::setWinTms(quint16 winTms)
{
    Q_UNUSED(winTms)
    return nullptr;
}
quint16 SunSpecLvrtxModel::rvrtTms() const
{
    return m_rvrtTms;
}

QModbusReply *SunSpecLvrtxModel::setRvrtTms(quint16 rvrtTms)
{
    Q_UNUSED(rvrtTms)
    return nullptr;
}
quint16 SunSpecLvrtxModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecLvrtxModel::setRmpTms(quint16 rmpTms)
{
    Q_UNUSED(rmpTms)
    return nullptr;
}
quint16 SunSpecLvrtxModel::nCrv() const
{
    return m_nCrv;
}
quint16 SunSpecLvrtxModel::nPt() const
{
    return m_nPt;
}
SunSpecLvrtxModel::Crvtype SunSpecLvrtxModel::crvType() const
{
    return m_crvType;
}
void SunSpecLvrtxModel::processBlockData()
{
    // Scale factors
    m_tmsSf = m_dataPoints.value("Tms_SF").toInt16();
    m_vSf = m_dataPoints.value("V_SF").toInt16();

    // Update properties according to the data point type
    m_actCrv = m_dataPoints.value("ActCrv").toUInt16();
    m_modEna = static_cast<ModenaFlags>(m_dataPoints.value("ModEna").toUInt16());
    m_winTms = m_dataPoints.value("WinTms").toUInt16();
    m_rvrtTms = m_dataPoints.value("RvrtTms").toUInt16();
    m_rmpTms = m_dataPoints.value("RmpTms").toUInt16();
    m_nCrv = m_dataPoints.value("NCrv").toUInt16();
    m_nPt = m_dataPoints.value("NPt").toUInt16();
    m_crvType = static_cast<Crvtype>(m_dataPoints.value("CrvType").toUInt16());

    qCDebug(dcSunSpec()) << this;
}

void SunSpecLvrtxModel::initDataPoints()
{
    SunSpecDataPoint modelIdDataPoint;
    modelIdDataPoint.setName("ID");
    modelIdDataPoint.setLabel("Model ID");
    modelIdDataPoint.setDescription("Model identifier");
    modelIdDataPoint.setMandatory(true);
    modelIdDataPoint.setSize(1);
    modelIdDataPoint.setAddressOffset(0);
    modelIdDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

    SunSpecDataPoint actCrvDataPoint;
    actCrvDataPoint.setName("ActCrv");
    actCrvDataPoint.setLabel("ActCrv");
    actCrvDataPoint.setDescription("Index of active curve. 0=no active curve.");
    actCrvDataPoint.setMandatory(true);
    actCrvDataPoint.setSize(1);
    actCrvDataPoint.setAddressOffset(2);
    actCrvDataPoint.setBlockOffset(0);
    actCrvDataPoint.setSunSpecDataType("uint16");
    actCrvDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(actCrvDataPoint.name(), actCrvDataPoint);

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("LVRT control mode. Enable active curve.  Bitfield value.");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(3);
    modEnaDataPoint.setBlockOffset(1);
    modEnaDataPoint.setSunSpecDataType("bitfield16");
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint winTmsDataPoint;
    winTmsDataPoint.setName("WinTms");
    winTmsDataPoint.setLabel("WinTms");
    winTmsDataPoint.setDescription("Time window for LVRT change.");
    winTmsDataPoint.setUnits("Secs");
    winTmsDataPoint.setSize(1);
    winTmsDataPoint.setAddressOffset(4);
    winTmsDataPoint.setBlockOffset(2);
    winTmsDataPoint.setSunSpecDataType("uint16");
    winTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(winTmsDataPoint.name(), winTmsDataPoint);

    SunSpecDataPoint rvrtTmsDataPoint;
    rvrtTmsDataPoint.setName("RvrtTms");
    rvrtTmsDataPoint.setLabel("RvrtTms");
    rvrtTmsDataPoint.setDescription("Timeout period for LVRT curve selection.");
    rvrtTmsDataPoint.setUnits("Secs");
    rvrtTmsDataPoint.setSize(1);
    rvrtTmsDataPoint.setAddressOffset(5);
    rvrtTmsDataPoint.setBlockOffset(3);
    rvrtTmsDataPoint.setSunSpecDataType("uint16");
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
    rmpTmsDataPoint.setSunSpecDataType("uint16");
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
    nCrvDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(nCrvDataPoint.name(), nCrvDataPoint);

    SunSpecDataPoint nPtDataPoint;
    nPtDataPoint.setName("NPt");
    nPtDataPoint.setLabel("NPt");
    nPtDataPoint.setDescription("Number of curve points supported (maximum of 20).");
    nPtDataPoint.setMandatory(true);
    nPtDataPoint.setSize(1);
    nPtDataPoint.setAddressOffset(8);
    nPtDataPoint.setBlockOffset(6);
    nPtDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(nPtDataPoint.name(), nPtDataPoint);

    SunSpecDataPoint tmsSfDataPoint;
    tmsSfDataPoint.setName("Tms_SF");
    tmsSfDataPoint.setLabel("Tms_SF");
    tmsSfDataPoint.setDescription("Scale factor for duration.");
    tmsSfDataPoint.setMandatory(true);
    tmsSfDataPoint.setSize(1);
    tmsSfDataPoint.setAddressOffset(9);
    tmsSfDataPoint.setBlockOffset(7);
    tmsSfDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(tmsSfDataPoint.name(), tmsSfDataPoint);

    SunSpecDataPoint vSfDataPoint;
    vSfDataPoint.setName("V_SF");
    vSfDataPoint.setLabel("V_SF");
    vSfDataPoint.setDescription("Scale factor for percent VRef.");
    vSfDataPoint.setMandatory(true);
    vSfDataPoint.setSize(1);
    vSfDataPoint.setAddressOffset(10);
    vSfDataPoint.setBlockOffset(8);
    vSfDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(vSfDataPoint.name(), vSfDataPoint);

    SunSpecDataPoint crvTypeDataPoint;
    crvTypeDataPoint.setName("CrvType");
    crvTypeDataPoint.setMandatory(true);
    crvTypeDataPoint.setSize(1);
    crvTypeDataPoint.setAddressOffset(11);
    crvTypeDataPoint.setBlockOffset(9);
    crvTypeDataPoint.setSunSpecDataType("enum16");
    m_dataPoints.insert(crvTypeDataPoint.name(), crvTypeDataPoint);

}

QDebug operator<<(QDebug debug, SunSpecLvrtxModel *model)
{
    debug.nospace().noquote() << "SunSpecLvrtxModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    if (model->dataPoints().value("ActCrv").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ActCrv") << "--> " << model->actCrv() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ActCrv") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ModEna").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModEna") << "--> " << model->modEna() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModEna") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("WinTms").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("WinTms") << "--> " << model->winTms() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("WinTms") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("RvrtTms").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("RvrtTms") << "--> " << model->rvrtTms() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("RvrtTms") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("RmpTms").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("RmpTms") << "--> " << model->rmpTms() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("RmpTms") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("NCrv").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NCrv") << "--> " << model->nCrv() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NCrv") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("NPt").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NPt") << "--> " << model->nPt() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NPt") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("CrvType").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("CrvType") << "--> " << model->crvType() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("CrvType") << "--> NaN" << endl;
    }


    return debug.space().quote();
}
