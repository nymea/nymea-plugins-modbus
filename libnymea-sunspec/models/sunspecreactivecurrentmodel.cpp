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

#include "sunspecreactivecurrentmodel.h"
#include "sunspecconnection.h"

SunSpecReactiveCurrentModel::SunSpecReactiveCurrentModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, 128, 14, modbusStartRegister, parent)
{
    Q_ASSERT_X(length == 14,  "SunSpecReactiveCurrentModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecReactiveCurrentModel::~SunSpecReactiveCurrentModel()
{

}

QString SunSpecReactiveCurrentModel::name() const
{
    return "reactive_current";
}

QString SunSpecReactiveCurrentModel::description() const
{
    return "Dynamic Reactive Current ";
}

QString SunSpecReactiveCurrentModel::label() const
{
    return "Dynamic Reactive Current";
}

SunSpecReactiveCurrentModel::Argramod SunSpecReactiveCurrentModel::arGraMod() const
{
    return m_arGraMod;
}

QModbusReply *SunSpecReactiveCurrentModel::setArGraMod(Argramod arGraMod)
{
    Q_UNUSED(arGraMod)
    return nullptr;
}
float SunSpecReactiveCurrentModel::arGraSag() const
{
    return m_arGraSag;
}

QModbusReply *SunSpecReactiveCurrentModel::setArGraSag(float arGraSag)
{
    Q_UNUSED(arGraSag)
    return nullptr;
}
float SunSpecReactiveCurrentModel::arGraSwell() const
{
    return m_arGraSwell;
}

QModbusReply *SunSpecReactiveCurrentModel::setArGraSwell(float arGraSwell)
{
    Q_UNUSED(arGraSwell)
    return nullptr;
}
SunSpecReactiveCurrentModel::ModenaFlags SunSpecReactiveCurrentModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecReactiveCurrentModel::setModEna(ModenaFlags modEna)
{
    Q_UNUSED(modEna)
    return nullptr;
}
quint16 SunSpecReactiveCurrentModel::filTms() const
{
    return m_filTms;
}

QModbusReply *SunSpecReactiveCurrentModel::setFilTms(quint16 filTms)
{
    Q_UNUSED(filTms)
    return nullptr;
}
float SunSpecReactiveCurrentModel::dbVMin() const
{
    return m_dbVMin;
}

QModbusReply *SunSpecReactiveCurrentModel::setDbVMin(float dbVMin)
{
    Q_UNUSED(dbVMin)
    return nullptr;
}
float SunSpecReactiveCurrentModel::dbVMax() const
{
    return m_dbVMax;
}

QModbusReply *SunSpecReactiveCurrentModel::setDbVMax(float dbVMax)
{
    Q_UNUSED(dbVMax)
    return nullptr;
}
float SunSpecReactiveCurrentModel::blkZnV() const
{
    return m_blkZnV;
}

QModbusReply *SunSpecReactiveCurrentModel::setBlkZnV(float blkZnV)
{
    Q_UNUSED(blkZnV)
    return nullptr;
}
float SunSpecReactiveCurrentModel::hysBlkZnV() const
{
    return m_hysBlkZnV;
}

QModbusReply *SunSpecReactiveCurrentModel::setHysBlkZnV(float hysBlkZnV)
{
    Q_UNUSED(hysBlkZnV)
    return nullptr;
}
quint16 SunSpecReactiveCurrentModel::blkZnTmms() const
{
    return m_blkZnTmms;
}

QModbusReply *SunSpecReactiveCurrentModel::setBlkZnTmms(quint16 blkZnTmms)
{
    Q_UNUSED(blkZnTmms)
    return nullptr;
}
quint16 SunSpecReactiveCurrentModel::holdTmms() const
{
    return m_holdTmms;
}

QModbusReply *SunSpecReactiveCurrentModel::setHoldTmms(quint16 holdTmms)
{
    Q_UNUSED(holdTmms)
    return nullptr;
}
quint16 SunSpecReactiveCurrentModel::pad() const
{
    return m_pad;
}
void SunSpecReactiveCurrentModel::processBlockData()
{
    // Scale factors
    m_arGraSf = m_dataPoints.value("ArGra_SF").toInt16();
    m_vRefPctSf = m_dataPoints.value("VRefPct_SF").toInt16();

    // Update properties according to the data point type
    m_arGraMod = static_cast<Argramod>(m_dataPoints.value("ArGraMod").toUInt16());
    m_arGraSag = m_dataPoints.value("ArGraSag").toFloatWithSSF(m_arGraSf);
    m_arGraSwell = m_dataPoints.value("ArGraSwell").toFloatWithSSF(m_arGraSf);
    m_modEna = static_cast<ModenaFlags>(m_dataPoints.value("ModEna").toUInt16());
    m_filTms = m_dataPoints.value("FilTms").toUInt16();
    m_dbVMin = m_dataPoints.value("DbVMin").toFloatWithSSF(m_vRefPctSf);
    m_dbVMax = m_dataPoints.value("DbVMax").toFloatWithSSF(m_vRefPctSf);
    m_blkZnV = m_dataPoints.value("BlkZnV").toFloatWithSSF(m_vRefPctSf);
    m_hysBlkZnV = m_dataPoints.value("HysBlkZnV").toFloatWithSSF(m_vRefPctSf);
    m_blkZnTmms = m_dataPoints.value("BlkZnTmms").toUInt16();
    m_holdTmms = m_dataPoints.value("HoldTmms").toUInt16();
    m_pad = m_dataPoints.value("Pad").toUInt16();
}

void SunSpecReactiveCurrentModel::initDataPoints()
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

    SunSpecDataPoint arGraModDataPoint;
    arGraModDataPoint.setName("ArGraMod");
    arGraModDataPoint.setLabel("ArGraMod");
    arGraModDataPoint.setDescription("Indicates if gradients trend toward zero at the edges of the deadband or trend toward zero at the center of the deadband.");
    arGraModDataPoint.setMandatory(true);
    arGraModDataPoint.setSize(1);
    arGraModDataPoint.setAddressOffset(2);
    arGraModDataPoint.setBlockOffset(0);
    arGraModDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    arGraModDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(arGraModDataPoint.name(), arGraModDataPoint);

    SunSpecDataPoint arGraSagDataPoint;
    arGraSagDataPoint.setName("ArGraSag");
    arGraSagDataPoint.setLabel("ArGraSag");
    arGraSagDataPoint.setDescription("The gradient used to increase capacitive dynamic current. A value of 0 indicates no additional reactive current support.");
    arGraSagDataPoint.setUnits("%ARtg/%dV");
    arGraSagDataPoint.setMandatory(true);
    arGraSagDataPoint.setSize(1);
    arGraSagDataPoint.setAddressOffset(3);
    arGraSagDataPoint.setBlockOffset(1);
    arGraSagDataPoint.setScaleFactorName("ArGra_SF");
    arGraSagDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    arGraSagDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(arGraSagDataPoint.name(), arGraSagDataPoint);

    SunSpecDataPoint arGraSwellDataPoint;
    arGraSwellDataPoint.setName("ArGraSwell");
    arGraSwellDataPoint.setLabel("ArGraSwell");
    arGraSwellDataPoint.setDescription("The gradient used to increase inductive dynamic current.  A value of 0 indicates no additional reactive current support.");
    arGraSwellDataPoint.setUnits("%ARtg/%dV");
    arGraSwellDataPoint.setMandatory(true);
    arGraSwellDataPoint.setSize(1);
    arGraSwellDataPoint.setAddressOffset(4);
    arGraSwellDataPoint.setBlockOffset(2);
    arGraSwellDataPoint.setScaleFactorName("ArGra_SF");
    arGraSwellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    arGraSwellDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(arGraSwellDataPoint.name(), arGraSwellDataPoint);

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("Activate dynamic reactive current model");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(5);
    modEnaDataPoint.setBlockOffset(3);
    modEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint filTmsDataPoint;
    filTmsDataPoint.setName("FilTms");
    filTmsDataPoint.setLabel("FilTms");
    filTmsDataPoint.setDescription("The time window used to calculate the moving average voltage.");
    filTmsDataPoint.setUnits("Secs");
    filTmsDataPoint.setSize(1);
    filTmsDataPoint.setAddressOffset(6);
    filTmsDataPoint.setBlockOffset(4);
    filTmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    filTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(filTmsDataPoint.name(), filTmsDataPoint);

    SunSpecDataPoint dbVMinDataPoint;
    dbVMinDataPoint.setName("DbVMin");
    dbVMinDataPoint.setLabel("DbVMin");
    dbVMinDataPoint.setDescription("The lower delta voltage limit for which negative voltage deviations less than this value no dynamic vars are produced.");
    dbVMinDataPoint.setUnits("% VRef");
    dbVMinDataPoint.setSize(1);
    dbVMinDataPoint.setAddressOffset(7);
    dbVMinDataPoint.setBlockOffset(5);
    dbVMinDataPoint.setScaleFactorName("VRefPct_SF");
    dbVMinDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    dbVMinDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(dbVMinDataPoint.name(), dbVMinDataPoint);

    SunSpecDataPoint dbVMaxDataPoint;
    dbVMaxDataPoint.setName("DbVMax");
    dbVMaxDataPoint.setLabel("DbVMax");
    dbVMaxDataPoint.setDescription("The upper delta voltage limit for which positive voltage deviations less than this value no dynamic current produced.");
    dbVMaxDataPoint.setUnits("% VRef");
    dbVMaxDataPoint.setSize(1);
    dbVMaxDataPoint.setAddressOffset(8);
    dbVMaxDataPoint.setBlockOffset(6);
    dbVMaxDataPoint.setScaleFactorName("VRefPct_SF");
    dbVMaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    dbVMaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(dbVMaxDataPoint.name(), dbVMaxDataPoint);

    SunSpecDataPoint blkZnVDataPoint;
    blkZnVDataPoint.setName("BlkZnV");
    blkZnVDataPoint.setLabel("BlkZnV");
    blkZnVDataPoint.setDescription("Block zone voltage which defines a lower voltage boundary below which no dynamic current is produced.");
    blkZnVDataPoint.setUnits("% VRef");
    blkZnVDataPoint.setSize(1);
    blkZnVDataPoint.setAddressOffset(9);
    blkZnVDataPoint.setBlockOffset(7);
    blkZnVDataPoint.setScaleFactorName("VRefPct_SF");
    blkZnVDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    blkZnVDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(blkZnVDataPoint.name(), blkZnVDataPoint);

    SunSpecDataPoint hysBlkZnVDataPoint;
    hysBlkZnVDataPoint.setName("HysBlkZnV");
    hysBlkZnVDataPoint.setLabel("HysBlkZnV");
    hysBlkZnVDataPoint.setDescription("Hysteresis voltage used with BlkZnV.");
    hysBlkZnVDataPoint.setUnits("% VRef");
    hysBlkZnVDataPoint.setSize(1);
    hysBlkZnVDataPoint.setAddressOffset(10);
    hysBlkZnVDataPoint.setBlockOffset(8);
    hysBlkZnVDataPoint.setScaleFactorName("VRefPct_SF");
    hysBlkZnVDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    hysBlkZnVDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hysBlkZnVDataPoint.name(), hysBlkZnVDataPoint);

    SunSpecDataPoint blkZnTmmsDataPoint;
    blkZnTmmsDataPoint.setName("BlkZnTmms");
    blkZnTmmsDataPoint.setLabel("BlkZnTmms");
    blkZnTmmsDataPoint.setDescription("Block zone time the time before which reactive current support remains active regardless of how low the voltage drops.");
    blkZnTmmsDataPoint.setUnits("mSecs");
    blkZnTmmsDataPoint.setSize(1);
    blkZnTmmsDataPoint.setAddressOffset(11);
    blkZnTmmsDataPoint.setBlockOffset(9);
    blkZnTmmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    blkZnTmmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(blkZnTmmsDataPoint.name(), blkZnTmmsDataPoint);

    SunSpecDataPoint holdTmmsDataPoint;
    holdTmmsDataPoint.setName("HoldTmms");
    holdTmmsDataPoint.setLabel("HoldTmms");
    holdTmmsDataPoint.setDescription("Hold time during which reactive current support continues after the average voltage has entered the dead zone.");
    holdTmmsDataPoint.setUnits("mSecs");
    holdTmmsDataPoint.setSize(1);
    holdTmmsDataPoint.setAddressOffset(12);
    holdTmmsDataPoint.setBlockOffset(10);
    holdTmmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    holdTmmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(holdTmmsDataPoint.name(), holdTmmsDataPoint);

    SunSpecDataPoint arGraSfDataPoint;
    arGraSfDataPoint.setName("ArGra_SF");
    arGraSfDataPoint.setLabel("ArGra_SF");
    arGraSfDataPoint.setDescription("Scale factor for the gradients.");
    arGraSfDataPoint.setMandatory(true);
    arGraSfDataPoint.setSize(1);
    arGraSfDataPoint.setAddressOffset(13);
    arGraSfDataPoint.setBlockOffset(11);
    arGraSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(arGraSfDataPoint.name(), arGraSfDataPoint);

    SunSpecDataPoint vRefPctSfDataPoint;
    vRefPctSfDataPoint.setName("VRefPct_SF");
    vRefPctSfDataPoint.setLabel("VRefPct_SF");
    vRefPctSfDataPoint.setDescription("Scale factor for the voltage zone and limit settings.");
    vRefPctSfDataPoint.setSize(1);
    vRefPctSfDataPoint.setAddressOffset(14);
    vRefPctSfDataPoint.setBlockOffset(12);
    vRefPctSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vRefPctSfDataPoint.name(), vRefPctSfDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(15);
    padDataPoint.setBlockOffset(13);
    padDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

