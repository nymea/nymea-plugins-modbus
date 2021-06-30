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

SunSpecReactiveCurrentModel::SunSpecReactiveCurrentModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 128;
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

void SunSpecReactiveCurrentModel::readModelHeader()
{

}

void SunSpecReactiveCurrentModel::readBlockData()
{

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

    SunSpecDataPoint argramodDataPoint;
    argramodDataPoint.setName("ArGraMod");
    argramodDataPoint.setLabel("ArGraMod");
    argramodDataPoint.setDescription("Indicates if gradients trend toward zero at the edges of the deadband or trend toward zero at the center of the deadband.");
    argramodDataPoint.setMandatory(true);
    argramodDataPoint.setSize(1);
    argramodDataPoint.setAddressOffset(2);
    argramodDataPoint.setBlockOffset(0);
    argramodDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    argramodDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(argramodDataPoint.name(), argramodDataPoint);

    SunSpecDataPoint argrasagDataPoint;
    argrasagDataPoint.setName("ArGraSag");
    argrasagDataPoint.setLabel("ArGraSag");
    argrasagDataPoint.setDescription("The gradient used to increase capacitive dynamic current. A value of 0 indicates no additional reactive current support.");
    argrasagDataPoint.setUnits("%ARtg/%dV");
    argrasagDataPoint.setMandatory(true);
    argrasagDataPoint.setSize(1);
    argrasagDataPoint.setAddressOffset(3);
    argrasagDataPoint.setBlockOffset(1);
    argrasagDataPoint.setScaleFactorName("ArGra_SF");
    argrasagDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    argrasagDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(argrasagDataPoint.name(), argrasagDataPoint);

    SunSpecDataPoint argraswellDataPoint;
    argraswellDataPoint.setName("ArGraSwell");
    argraswellDataPoint.setLabel("ArGraSwell");
    argraswellDataPoint.setDescription("The gradient used to increase inductive dynamic current.  A value of 0 indicates no additional reactive current support.");
    argraswellDataPoint.setUnits("%ARtg/%dV");
    argraswellDataPoint.setMandatory(true);
    argraswellDataPoint.setSize(1);
    argraswellDataPoint.setAddressOffset(4);
    argraswellDataPoint.setBlockOffset(2);
    argraswellDataPoint.setScaleFactorName("ArGra_SF");
    argraswellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    argraswellDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(argraswellDataPoint.name(), argraswellDataPoint);

    SunSpecDataPoint modenaDataPoint;
    modenaDataPoint.setName("ModEna");
    modenaDataPoint.setLabel("ModEna");
    modenaDataPoint.setDescription("Activate dynamic reactive current model");
    modenaDataPoint.setMandatory(true);
    modenaDataPoint.setSize(1);
    modenaDataPoint.setAddressOffset(5);
    modenaDataPoint.setBlockOffset(3);
    modenaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modenaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modenaDataPoint.name(), modenaDataPoint);

    SunSpecDataPoint filtmsDataPoint;
    filtmsDataPoint.setName("FilTms");
    filtmsDataPoint.setLabel("FilTms");
    filtmsDataPoint.setDescription("The time window used to calculate the moving average voltage.");
    filtmsDataPoint.setUnits("Secs");
    filtmsDataPoint.setSize(1);
    filtmsDataPoint.setAddressOffset(6);
    filtmsDataPoint.setBlockOffset(4);
    filtmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    filtmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(filtmsDataPoint.name(), filtmsDataPoint);

    SunSpecDataPoint dbvminDataPoint;
    dbvminDataPoint.setName("DbVMin");
    dbvminDataPoint.setLabel("DbVMin");
    dbvminDataPoint.setDescription("The lower delta voltage limit for which negative voltage deviations less than this value no dynamic vars are produced.");
    dbvminDataPoint.setUnits("% VRef");
    dbvminDataPoint.setSize(1);
    dbvminDataPoint.setAddressOffset(7);
    dbvminDataPoint.setBlockOffset(5);
    dbvminDataPoint.setScaleFactorName("VRefPct_SF");
    dbvminDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    dbvminDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(dbvminDataPoint.name(), dbvminDataPoint);

    SunSpecDataPoint dbvmaxDataPoint;
    dbvmaxDataPoint.setName("DbVMax");
    dbvmaxDataPoint.setLabel("DbVMax");
    dbvmaxDataPoint.setDescription("The upper delta voltage limit for which positive voltage deviations less than this value no dynamic current produced.");
    dbvmaxDataPoint.setUnits("% VRef");
    dbvmaxDataPoint.setSize(1);
    dbvmaxDataPoint.setAddressOffset(8);
    dbvmaxDataPoint.setBlockOffset(6);
    dbvmaxDataPoint.setScaleFactorName("VRefPct_SF");
    dbvmaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    dbvmaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(dbvmaxDataPoint.name(), dbvmaxDataPoint);

    SunSpecDataPoint blkznvDataPoint;
    blkznvDataPoint.setName("BlkZnV");
    blkznvDataPoint.setLabel("BlkZnV");
    blkznvDataPoint.setDescription("Block zone voltage which defines a lower voltage boundary below which no dynamic current is produced.");
    blkznvDataPoint.setUnits("% VRef");
    blkznvDataPoint.setSize(1);
    blkznvDataPoint.setAddressOffset(9);
    blkznvDataPoint.setBlockOffset(7);
    blkznvDataPoint.setScaleFactorName("VRefPct_SF");
    blkznvDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    blkznvDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(blkznvDataPoint.name(), blkznvDataPoint);

    SunSpecDataPoint hysblkznvDataPoint;
    hysblkznvDataPoint.setName("HysBlkZnV");
    hysblkznvDataPoint.setLabel("HysBlkZnV");
    hysblkznvDataPoint.setDescription("Hysteresis voltage used with BlkZnV.");
    hysblkznvDataPoint.setUnits("% VRef");
    hysblkznvDataPoint.setSize(1);
    hysblkznvDataPoint.setAddressOffset(10);
    hysblkznvDataPoint.setBlockOffset(8);
    hysblkznvDataPoint.setScaleFactorName("VRefPct_SF");
    hysblkznvDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    hysblkznvDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hysblkznvDataPoint.name(), hysblkznvDataPoint);

    SunSpecDataPoint blkzntmmsDataPoint;
    blkzntmmsDataPoint.setName("BlkZnTmms");
    blkzntmmsDataPoint.setLabel("BlkZnTmms");
    blkzntmmsDataPoint.setDescription("Block zone time the time before which reactive current support remains active regardless of how low the voltage drops.");
    blkzntmmsDataPoint.setUnits("mSecs");
    blkzntmmsDataPoint.setSize(1);
    blkzntmmsDataPoint.setAddressOffset(11);
    blkzntmmsDataPoint.setBlockOffset(9);
    blkzntmmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    blkzntmmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(blkzntmmsDataPoint.name(), blkzntmmsDataPoint);

    SunSpecDataPoint holdtmmsDataPoint;
    holdtmmsDataPoint.setName("HoldTmms");
    holdtmmsDataPoint.setLabel("HoldTmms");
    holdtmmsDataPoint.setDescription("Hold time during which reactive current support continues after the average voltage has entered the dead zone.");
    holdtmmsDataPoint.setUnits("mSecs");
    holdtmmsDataPoint.setSize(1);
    holdtmmsDataPoint.setAddressOffset(12);
    holdtmmsDataPoint.setBlockOffset(10);
    holdtmmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    holdtmmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(holdtmmsDataPoint.name(), holdtmmsDataPoint);

    SunSpecDataPoint argraSfDataPoint;
    argraSfDataPoint.setName("ArGra_SF");
    argraSfDataPoint.setLabel("ArGra_SF");
    argraSfDataPoint.setDescription("Scale factor for the gradients.");
    argraSfDataPoint.setMandatory(true);
    argraSfDataPoint.setSize(1);
    argraSfDataPoint.setAddressOffset(13);
    argraSfDataPoint.setBlockOffset(11);
    argraSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(argraSfDataPoint.name(), argraSfDataPoint);

    SunSpecDataPoint vrefpctSfDataPoint;
    vrefpctSfDataPoint.setName("VRefPct_SF");
    vrefpctSfDataPoint.setLabel("VRefPct_SF");
    vrefpctSfDataPoint.setDescription("Scale factor for the voltage zone and limit settings.");
    vrefpctSfDataPoint.setSize(1);
    vrefpctSfDataPoint.setAddressOffset(14);
    vrefpctSfDataPoint.setBlockOffset(12);
    vrefpctSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vrefpctSfDataPoint.name(), vrefpctSfDataPoint);

    SunSpecDataPoint PadDataPoint;
    PadDataPoint.setName("Pad");
    PadDataPoint.setSize(1);
    PadDataPoint.setAddressOffset(15);
    PadDataPoint.setBlockOffset(13);
    PadDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(PadDataPoint.name(), PadDataPoint);

}

