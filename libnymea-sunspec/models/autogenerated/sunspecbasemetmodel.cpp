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

#include "sunspecbasemetmodel.h"

SunSpecBaseMetModel::SunSpecBaseMetModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecBaseMetModel::~SunSpecBaseMetModel()
{

}

QString SunSpecBaseMetModel::name() const
{
    return "base_met";
}

QString SunSpecBaseMetModel::description() const
{
    return "Base Meteorological Model";
}

QString SunSpecBaseMetModel::label() const
{
    return "Base Met";
}

void SunSpecBaseMetModel::readModelHeader()
{

}

void SunSpecBaseMetModel::readBlockData()
{

}

void SunSpecBaseMetModel::initDataPoints()
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

    SunSpecDataPoint ambientTemperatureDataPoint;
    ambientTemperatureDataPoint.setName("TmpAmb");
    ambientTemperatureDataPoint.setLabel("Ambient Temperature");
    ambientTemperatureDataPoint.setUnits("C");
    ambientTemperatureDataPoint.setSize(1);
    ambientTemperatureDataPoint.setScaleFactorName("-1");
    ambientTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << ambientTemperatureDataPoint;

    SunSpecDataPoint relativeHumidityDataPoint;
    relativeHumidityDataPoint.setName("RH");
    relativeHumidityDataPoint.setLabel("Relative Humidity");
    relativeHumidityDataPoint.setUnits("Pct");
    relativeHumidityDataPoint.setSize(1);
    relativeHumidityDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << relativeHumidityDataPoint;

    SunSpecDataPoint barometricPressureDataPoint;
    barometricPressureDataPoint.setName("Pres");
    barometricPressureDataPoint.setLabel("Barometric Pressure");
    barometricPressureDataPoint.setUnits("HPa");
    barometricPressureDataPoint.setSize(1);
    barometricPressureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << barometricPressureDataPoint;

    SunSpecDataPoint windSpeedDataPoint;
    windSpeedDataPoint.setName("WndSpd");
    windSpeedDataPoint.setLabel("Wind Speed");
    windSpeedDataPoint.setUnits("mps");
    windSpeedDataPoint.setSize(1);
    windSpeedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << windSpeedDataPoint;

    SunSpecDataPoint windDirectionDataPoint;
    windDirectionDataPoint.setName("WndDir");
    windDirectionDataPoint.setLabel("Wind Direction");
    windDirectionDataPoint.setUnits("deg");
    windDirectionDataPoint.setSize(1);
    windDirectionDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << windDirectionDataPoint;

    SunSpecDataPoint rainfallDataPoint;
    rainfallDataPoint.setName("Rain");
    rainfallDataPoint.setLabel("Rainfall");
    rainfallDataPoint.setUnits("mm");
    rainfallDataPoint.setSize(1);
    rainfallDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << rainfallDataPoint;

    SunSpecDataPoint snowDepthDataPoint;
    snowDepthDataPoint.setName("Snw");
    snowDepthDataPoint.setLabel("Snow Depth");
    snowDepthDataPoint.setUnits("mm");
    snowDepthDataPoint.setSize(1);
    snowDepthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << snowDepthDataPoint;

    SunSpecDataPoint precipitationTypeDataPoint;
    precipitationTypeDataPoint.setName("PPT");
    precipitationTypeDataPoint.setLabel("Precipitation Type");
    precipitationTypeDataPoint.setDescription(" Precipitation Type (WMO 4680 SYNOP code reference)");
    precipitationTypeDataPoint.setSize(1);
    precipitationTypeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << precipitationTypeDataPoint;

    SunSpecDataPoint electricFieldDataPoint;
    electricFieldDataPoint.setName("ElecFld");
    electricFieldDataPoint.setLabel("Electric Field");
    electricFieldDataPoint.setUnits("Vm");
    electricFieldDataPoint.setSize(1);
    electricFieldDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << electricFieldDataPoint;

    SunSpecDataPoint surfaceWetnessDataPoint;
    surfaceWetnessDataPoint.setName("SurWet");
    surfaceWetnessDataPoint.setLabel("Surface Wetness");
    surfaceWetnessDataPoint.setUnits("kO");
    surfaceWetnessDataPoint.setSize(1);
    surfaceWetnessDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << surfaceWetnessDataPoint;

    SunSpecDataPoint soilWetnessDataPoint;
    soilWetnessDataPoint.setName("SoilWet");
    soilWetnessDataPoint.setLabel("Soil Wetness");
    soilWetnessDataPoint.setUnits("Pct");
    soilWetnessDataPoint.setSize(1);
    soilWetnessDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << soilWetnessDataPoint;

}

