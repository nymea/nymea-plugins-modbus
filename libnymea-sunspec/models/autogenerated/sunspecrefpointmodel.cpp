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

#include "sunspecrefpointmodel.h"

SunSpecRefPointModel::SunSpecRefPointModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 306;
}

SunSpecRefPointModel::~SunSpecRefPointModel()
{

}

QString SunSpecRefPointModel::name() const
{
    return "ref_point";
}

QString SunSpecRefPointModel::description() const
{
    return "Include to support a standard reference point";
}

QString SunSpecRefPointModel::label() const
{
    return "Reference Point Model";
}

void SunSpecRefPointModel::readModelHeader()
{

}

void SunSpecRefPointModel::readBlockData()
{

}

void SunSpecRefPointModel::initDataPoints()
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

    SunSpecDataPoint ghiDataPoint;
    ghiDataPoint.setName("GHI");
    ghiDataPoint.setLabel("GHI");
    ghiDataPoint.setDescription("Global Horizontal Irradiance");
    ghiDataPoint.setUnits("W/m2");
    ghiDataPoint.setSize(1);
    ghiDataPoint.setAddressOffset(2);
    ghiDataPoint.setBlockOffset(0);
    ghiDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(ghiDataPoint.name(), ghiDataPoint);

    SunSpecDataPoint ampsDataPoint;
    ampsDataPoint.setName("A");
    ampsDataPoint.setLabel("Amps");
    ampsDataPoint.setDescription("Current measurement at reference point");
    ampsDataPoint.setUnits("W/m2");
    ampsDataPoint.setSize(1);
    ampsDataPoint.setAddressOffset(3);
    ampsDataPoint.setBlockOffset(1);
    ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

    SunSpecDataPoint voltageDataPoint;
    voltageDataPoint.setName("V");
    voltageDataPoint.setLabel("Voltage");
    voltageDataPoint.setDescription("Voltage  measurement at reference point");
    voltageDataPoint.setUnits("W/m2");
    voltageDataPoint.setSize(1);
    voltageDataPoint.setAddressOffset(4);
    voltageDataPoint.setBlockOffset(2);
    voltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

    SunSpecDataPoint temperatureDataPoint;
    temperatureDataPoint.setName("Tmp");
    temperatureDataPoint.setLabel("Temperature");
    temperatureDataPoint.setDescription("Temperature measurement at reference point");
    temperatureDataPoint.setUnits("W/m2");
    temperatureDataPoint.setSize(1);
    temperatureDataPoint.setAddressOffset(5);
    temperatureDataPoint.setBlockOffset(3);
    temperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(temperatureDataPoint.name(), temperatureDataPoint);

}

