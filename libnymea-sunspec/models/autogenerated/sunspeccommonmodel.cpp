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

#include "sunspeccommonmodel.h"

SunSpecCommonModel::SunSpecCommonModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 1;
}

SunSpecCommonModel::~SunSpecCommonModel()
{

}

QString SunSpecCommonModel::name() const
{
    return "common";
}

QString SunSpecCommonModel::description() const
{
    return "All SunSpec compliant devices must include this as the first model";
}

QString SunSpecCommonModel::label() const
{
    return "Common";
}

void SunSpecCommonModel::readModelHeader()
{

}

void SunSpecCommonModel::readBlockData()
{

}

void SunSpecCommonModel::initDataPoints()
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

    SunSpecDataPoint manufacturerDataPoint;
    manufacturerDataPoint.setName("Mn");
    manufacturerDataPoint.setLabel("Manufacturer");
    manufacturerDataPoint.setDescription("Well known value registered with SunSpec for compliance");
    manufacturerDataPoint.setMandatory(true);
    manufacturerDataPoint.setSize(16);
    manufacturerDataPoint.setAddressOffset(2);
    manufacturerDataPoint.setBlockOffset(0);
    manufacturerDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(manufacturerDataPoint.name(), manufacturerDataPoint);

    SunSpecDataPoint modelDataPoint;
    modelDataPoint.setName("Md");
    modelDataPoint.setLabel("Model");
    modelDataPoint.setDescription("Manufacturer specific value (32 chars)");
    modelDataPoint.setMandatory(true);
    modelDataPoint.setSize(16);
    modelDataPoint.setAddressOffset(18);
    modelDataPoint.setBlockOffset(16);
    modelDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(modelDataPoint.name(), modelDataPoint);

    SunSpecDataPoint optionsDataPoint;
    optionsDataPoint.setName("Opt");
    optionsDataPoint.setLabel("Options");
    optionsDataPoint.setDescription("Manufacturer specific value (16 chars)");
    optionsDataPoint.setSize(8);
    optionsDataPoint.setAddressOffset(34);
    optionsDataPoint.setBlockOffset(32);
    optionsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(optionsDataPoint.name(), optionsDataPoint);

    SunSpecDataPoint versionDataPoint;
    versionDataPoint.setName("Vr");
    versionDataPoint.setLabel("Version");
    versionDataPoint.setDescription("Manufacturer specific value (16 chars)");
    versionDataPoint.setSize(8);
    versionDataPoint.setAddressOffset(42);
    versionDataPoint.setBlockOffset(40);
    versionDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(versionDataPoint.name(), versionDataPoint);

    SunSpecDataPoint serialNumberDataPoint;
    serialNumberDataPoint.setName("SN");
    serialNumberDataPoint.setLabel("Serial Number");
    serialNumberDataPoint.setDescription("Manufacturer specific value (32 chars)");
    serialNumberDataPoint.setMandatory(true);
    serialNumberDataPoint.setSize(16);
    serialNumberDataPoint.setAddressOffset(50);
    serialNumberDataPoint.setBlockOffset(48);
    serialNumberDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(serialNumberDataPoint.name(), serialNumberDataPoint);

    SunSpecDataPoint deviceAddressDataPoint;
    deviceAddressDataPoint.setName("DA");
    deviceAddressDataPoint.setLabel("Device Address");
    deviceAddressDataPoint.setDescription("Modbus device address");
    deviceAddressDataPoint.setSize(1);
    deviceAddressDataPoint.setAddressOffset(66);
    deviceAddressDataPoint.setBlockOffset(64);
    deviceAddressDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    deviceAddressDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(deviceAddressDataPoint.name(), deviceAddressDataPoint);

    SunSpecDataPoint PadDataPoint;
    PadDataPoint.setName("Pad");
    PadDataPoint.setDescription("Force even alignment");
    PadDataPoint.setSize(1);
    PadDataPoint.setAddressOffset(67);
    PadDataPoint.setBlockOffset(65);
    PadDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(PadDataPoint.name(), PadDataPoint);

}

