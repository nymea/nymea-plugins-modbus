/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
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
#include "sunspecconnection.h"

SunSpecBaseMetModel::SunSpecBaseMetModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 307, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

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

float SunSpecBaseMetModel::ambientTemperature() const
{
    return m_ambientTemperature;
}
qint16 SunSpecBaseMetModel::relativeHumidity() const
{
    return m_relativeHumidity;
}
qint16 SunSpecBaseMetModel::barometricPressure() const
{
    return m_barometricPressure;
}
qint16 SunSpecBaseMetModel::windSpeed() const
{
    return m_windSpeed;
}
qint16 SunSpecBaseMetModel::windDirection() const
{
    return m_windDirection;
}
qint16 SunSpecBaseMetModel::rainfall() const
{
    return m_rainfall;
}
qint16 SunSpecBaseMetModel::snowDepth() const
{
    return m_snowDepth;
}
qint16 SunSpecBaseMetModel::precipitationType() const
{
    return m_precipitationType;
}
qint16 SunSpecBaseMetModel::electricField() const
{
    return m_electricField;
}
qint16 SunSpecBaseMetModel::surfaceWetness() const
{
    return m_surfaceWetness;
}
qint16 SunSpecBaseMetModel::soilWetness() const
{
    return m_soilWetness;
}
void SunSpecBaseMetModel::initDataPoints()
{
    SunSpecDataPoint modelIdDataPoint;
    modelIdDataPoint.setName("ID");
    modelIdDataPoint.setLabel("Model ID");
    modelIdDataPoint.setDescription("Model identifier");
    modelIdDataPoint.setMandatory(true);
    modelIdDataPoint.setSize(1);
    modelIdDataPoint.setAddressOffset(0);
    modelIdDataPoint.setSunSpecDataType("uint16");
    modelIdDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setSunSpecDataType("uint16");
    modelLengthDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

    SunSpecDataPoint ambientTemperatureDataPoint;
    ambientTemperatureDataPoint.setName("TmpAmb");
    ambientTemperatureDataPoint.setLabel("Ambient Temperature");
    ambientTemperatureDataPoint.setUnits("C");
    ambientTemperatureDataPoint.setSize(1);
    ambientTemperatureDataPoint.setAddressOffset(2);
    ambientTemperatureDataPoint.setBlockOffset(0);
    ambientTemperatureDataPoint.setScaleFactorName("-1");
    ambientTemperatureDataPoint.setSunSpecDataType("int16");
    ambientTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ambientTemperatureDataPoint.name(), ambientTemperatureDataPoint);

    SunSpecDataPoint relativeHumidityDataPoint;
    relativeHumidityDataPoint.setName("RH");
    relativeHumidityDataPoint.setLabel("Relative Humidity");
    relativeHumidityDataPoint.setUnits("Pct");
    relativeHumidityDataPoint.setSize(1);
    relativeHumidityDataPoint.setAddressOffset(3);
    relativeHumidityDataPoint.setBlockOffset(1);
    relativeHumidityDataPoint.setSunSpecDataType("int16");
    relativeHumidityDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(relativeHumidityDataPoint.name(), relativeHumidityDataPoint);

    SunSpecDataPoint barometricPressureDataPoint;
    barometricPressureDataPoint.setName("Pres");
    barometricPressureDataPoint.setLabel("Barometric Pressure");
    barometricPressureDataPoint.setUnits("HPa");
    barometricPressureDataPoint.setSize(1);
    barometricPressureDataPoint.setAddressOffset(4);
    barometricPressureDataPoint.setBlockOffset(2);
    barometricPressureDataPoint.setSunSpecDataType("int16");
    barometricPressureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(barometricPressureDataPoint.name(), barometricPressureDataPoint);

    SunSpecDataPoint windSpeedDataPoint;
    windSpeedDataPoint.setName("WndSpd");
    windSpeedDataPoint.setLabel("Wind Speed");
    windSpeedDataPoint.setUnits("mps");
    windSpeedDataPoint.setSize(1);
    windSpeedDataPoint.setAddressOffset(5);
    windSpeedDataPoint.setBlockOffset(3);
    windSpeedDataPoint.setSunSpecDataType("int16");
    windSpeedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(windSpeedDataPoint.name(), windSpeedDataPoint);

    SunSpecDataPoint windDirectionDataPoint;
    windDirectionDataPoint.setName("WndDir");
    windDirectionDataPoint.setLabel("Wind Direction");
    windDirectionDataPoint.setUnits("deg");
    windDirectionDataPoint.setSize(1);
    windDirectionDataPoint.setAddressOffset(6);
    windDirectionDataPoint.setBlockOffset(4);
    windDirectionDataPoint.setSunSpecDataType("int16");
    windDirectionDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(windDirectionDataPoint.name(), windDirectionDataPoint);

    SunSpecDataPoint rainfallDataPoint;
    rainfallDataPoint.setName("Rain");
    rainfallDataPoint.setLabel("Rainfall");
    rainfallDataPoint.setUnits("mm");
    rainfallDataPoint.setSize(1);
    rainfallDataPoint.setAddressOffset(7);
    rainfallDataPoint.setBlockOffset(5);
    rainfallDataPoint.setSunSpecDataType("int16");
    rainfallDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rainfallDataPoint.name(), rainfallDataPoint);

    SunSpecDataPoint snowDepthDataPoint;
    snowDepthDataPoint.setName("Snw");
    snowDepthDataPoint.setLabel("Snow Depth");
    snowDepthDataPoint.setUnits("mm");
    snowDepthDataPoint.setSize(1);
    snowDepthDataPoint.setAddressOffset(8);
    snowDepthDataPoint.setBlockOffset(6);
    snowDepthDataPoint.setSunSpecDataType("int16");
    snowDepthDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(snowDepthDataPoint.name(), snowDepthDataPoint);

    SunSpecDataPoint precipitationTypeDataPoint;
    precipitationTypeDataPoint.setName("PPT");
    precipitationTypeDataPoint.setLabel("Precipitation Type");
    precipitationTypeDataPoint.setDescription("Precipitation Type (WMO 4680 SYNOP code reference)");
    precipitationTypeDataPoint.setSize(1);
    precipitationTypeDataPoint.setAddressOffset(9);
    precipitationTypeDataPoint.setBlockOffset(7);
    precipitationTypeDataPoint.setSunSpecDataType("int16");
    precipitationTypeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(precipitationTypeDataPoint.name(), precipitationTypeDataPoint);

    SunSpecDataPoint electricFieldDataPoint;
    electricFieldDataPoint.setName("ElecFld");
    electricFieldDataPoint.setLabel("Electric Field");
    electricFieldDataPoint.setUnits("Vm");
    electricFieldDataPoint.setSize(1);
    electricFieldDataPoint.setAddressOffset(10);
    electricFieldDataPoint.setBlockOffset(8);
    electricFieldDataPoint.setSunSpecDataType("int16");
    electricFieldDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(electricFieldDataPoint.name(), electricFieldDataPoint);

    SunSpecDataPoint surfaceWetnessDataPoint;
    surfaceWetnessDataPoint.setName("SurWet");
    surfaceWetnessDataPoint.setLabel("Surface Wetness");
    surfaceWetnessDataPoint.setUnits("kO");
    surfaceWetnessDataPoint.setSize(1);
    surfaceWetnessDataPoint.setAddressOffset(11);
    surfaceWetnessDataPoint.setBlockOffset(9);
    surfaceWetnessDataPoint.setSunSpecDataType("int16");
    surfaceWetnessDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(surfaceWetnessDataPoint.name(), surfaceWetnessDataPoint);

    SunSpecDataPoint soilWetnessDataPoint;
    soilWetnessDataPoint.setName("SoilWet");
    soilWetnessDataPoint.setLabel("Soil Wetness");
    soilWetnessDataPoint.setUnits("Pct");
    soilWetnessDataPoint.setSize(1);
    soilWetnessDataPoint.setAddressOffset(12);
    soilWetnessDataPoint.setBlockOffset(10);
    soilWetnessDataPoint.setSunSpecDataType("int16");
    soilWetnessDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soilWetnessDataPoint.name(), soilWetnessDataPoint);

}

void SunSpecBaseMetModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("TmpAmb").isValid())
        m_ambientTemperature = m_dataPoints.value("TmpAmb").toFloatWithSSF(-1);

    if (m_dataPoints.value("RH").isValid())
        m_relativeHumidity = m_dataPoints.value("RH").toInt16();

    if (m_dataPoints.value("Pres").isValid())
        m_barometricPressure = m_dataPoints.value("Pres").toInt16();

    if (m_dataPoints.value("WndSpd").isValid())
        m_windSpeed = m_dataPoints.value("WndSpd").toInt16();

    if (m_dataPoints.value("WndDir").isValid())
        m_windDirection = m_dataPoints.value("WndDir").toInt16();

    if (m_dataPoints.value("Rain").isValid())
        m_rainfall = m_dataPoints.value("Rain").toInt16();

    if (m_dataPoints.value("Snw").isValid())
        m_snowDepth = m_dataPoints.value("Snw").toInt16();

    if (m_dataPoints.value("PPT").isValid())
        m_precipitationType = m_dataPoints.value("PPT").toInt16();

    if (m_dataPoints.value("ElecFld").isValid())
        m_electricField = m_dataPoints.value("ElecFld").toInt16();

    if (m_dataPoints.value("SurWet").isValid())
        m_surfaceWetness = m_dataPoints.value("SurWet").toInt16();

    if (m_dataPoints.value("SoilWet").isValid())
        m_soilWetness = m_dataPoints.value("SoilWet").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecBaseMetModel *model)
{
    debug.nospace().noquote() << "SunSpecBaseMetModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpAmb") << "-->";
    if (model->dataPoints().value("TmpAmb").isValid()) {
        debug.nospace().noquote() << model->ambientTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("RH") << "-->";
    if (model->dataPoints().value("RH").isValid()) {
        debug.nospace().noquote() << model->relativeHumidity() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Pres") << "-->";
    if (model->dataPoints().value("Pres").isValid()) {
        debug.nospace().noquote() << model->barometricPressure() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WndSpd") << "-->";
    if (model->dataPoints().value("WndSpd").isValid()) {
        debug.nospace().noquote() << model->windSpeed() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WndDir") << "-->";
    if (model->dataPoints().value("WndDir").isValid()) {
        debug.nospace().noquote() << model->windDirection() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Rain") << "-->";
    if (model->dataPoints().value("Rain").isValid()) {
        debug.nospace().noquote() << model->rainfall() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Snw") << "-->";
    if (model->dataPoints().value("Snw").isValid()) {
        debug.nospace().noquote() << model->snowDepth() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PPT") << "-->";
    if (model->dataPoints().value("PPT").isValid()) {
        debug.nospace().noquote() << model->precipitationType() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ElecFld") << "-->";
    if (model->dataPoints().value("ElecFld").isValid()) {
        debug.nospace().noquote() << model->electricField() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SurWet") << "-->";
    if (model->dataPoints().value("SurWet").isValid()) {
        debug.nospace().noquote() << model->surfaceWetness() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoilWet") << "-->";
    if (model->dataPoints().value("SoilWet").isValid()) {
        debug.nospace().noquote() << model->soilWetness() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
