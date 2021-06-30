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

#include "sunspecpricingmodel.h"

SunSpecPricingModel::SunSpecPricingModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 125;
}

SunSpecPricingModel::~SunSpecPricingModel()
{

}

QString SunSpecPricingModel::name() const
{
    return "pricing";
}

QString SunSpecPricingModel::description() const
{
    return "Pricing Signal  ";
}

QString SunSpecPricingModel::label() const
{
    return "Pricing";
}

void SunSpecPricingModel::readModelHeader()
{

}

void SunSpecPricingModel::readBlockData()
{

}

void SunSpecPricingModel::initDataPoints()
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

    SunSpecDataPoint modenaDataPoint;
    modenaDataPoint.setName("ModEna");
    modenaDataPoint.setLabel("ModEna");
    modenaDataPoint.setDescription("Is price-based charge/discharge mode active?");
    modenaDataPoint.setMandatory(true);
    modenaDataPoint.setSize(1);
    modenaDataPoint.setAddressOffset(2);
    modenaDataPoint.setBlockOffset(0);
    modenaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modenaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modenaDataPoint.name(), modenaDataPoint);

    SunSpecDataPoint sigtypeDataPoint;
    sigtypeDataPoint.setName("SigType");
    sigtypeDataPoint.setLabel("SigType");
    sigtypeDataPoint.setDescription("Meaning of the pricing signal. When a Price schedule is used, type must match the schedule range variable description.");
    sigtypeDataPoint.setSize(1);
    sigtypeDataPoint.setAddressOffset(3);
    sigtypeDataPoint.setBlockOffset(1);
    sigtypeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    sigtypeDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(sigtypeDataPoint.name(), sigtypeDataPoint);

    SunSpecDataPoint sigDataPoint;
    sigDataPoint.setName("Sig");
    sigDataPoint.setLabel("Sig");
    sigDataPoint.setDescription("Utility/ESP specific pricing signal. Content depends on pricing signal type. When H/M/L type is specified. Low=0; Med=1; High=2.");
    sigDataPoint.setMandatory(true);
    sigDataPoint.setSize(1);
    sigDataPoint.setAddressOffset(4);
    sigDataPoint.setBlockOffset(2);
    sigDataPoint.setScaleFactorName("Sig_SF");
    sigDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    sigDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(sigDataPoint.name(), sigDataPoint);

    SunSpecDataPoint wintmsDataPoint;
    wintmsDataPoint.setName("WinTms");
    wintmsDataPoint.setLabel("WinTms");
    wintmsDataPoint.setDescription("Time window for charge/discharge pricing change.");
    wintmsDataPoint.setUnits("Secs");
    wintmsDataPoint.setSize(1);
    wintmsDataPoint.setAddressOffset(5);
    wintmsDataPoint.setBlockOffset(3);
    wintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wintmsDataPoint.name(), wintmsDataPoint);

    SunSpecDataPoint rvttmsDataPoint;
    rvttmsDataPoint.setName("RvtTms");
    rvttmsDataPoint.setLabel("RvtTms");
    rvttmsDataPoint.setDescription("Timeout period for charge/discharge pricing change.");
    rvttmsDataPoint.setUnits("Secs");
    rvttmsDataPoint.setSize(1);
    rvttmsDataPoint.setAddressOffset(6);
    rvttmsDataPoint.setBlockOffset(4);
    rvttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    rvttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(rvttmsDataPoint.name(), rvttmsDataPoint);

    SunSpecDataPoint rmptmsDataPoint;
    rmptmsDataPoint.setName("RmpTms");
    rmptmsDataPoint.setLabel("RmpTms");
    rmptmsDataPoint.setDescription("Ramp time for moving from current charge or discharge level to new level.");
    rmptmsDataPoint.setUnits("Secs");
    rmptmsDataPoint.setSize(1);
    rmptmsDataPoint.setAddressOffset(7);
    rmptmsDataPoint.setBlockOffset(5);
    rmptmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    rmptmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(rmptmsDataPoint.name(), rmptmsDataPoint);

    SunSpecDataPoint sigSfDataPoint;
    sigSfDataPoint.setName("Sig_SF");
    sigSfDataPoint.setLabel("Sig_SF");
    sigSfDataPoint.setDescription("Pricing signal scale factor.");
    sigSfDataPoint.setMandatory(true);
    sigSfDataPoint.setSize(1);
    sigSfDataPoint.setAddressOffset(8);
    sigSfDataPoint.setBlockOffset(6);
    sigSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(sigSfDataPoint.name(), sigSfDataPoint);

    SunSpecDataPoint PadDataPoint;
    PadDataPoint.setName("Pad");
    PadDataPoint.setSize(1);
    PadDataPoint.setAddressOffset(9);
    PadDataPoint.setBlockOffset(7);
    PadDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(PadDataPoint.name(), PadDataPoint);

}

