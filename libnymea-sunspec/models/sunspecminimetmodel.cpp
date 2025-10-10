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

#include "sunspecminimetmodel.h"
#include "sunspecconnection.h"

SunSpecMiniMetModel::SunSpecMiniMetModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 308, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecMiniMetModel::~SunSpecMiniMetModel()
{

}

QString SunSpecMiniMetModel::name() const
{
    return "mini_met";
}

QString SunSpecMiniMetModel::description() const
{
    return "Include to support a few basic measurements";
}

QString SunSpecMiniMetModel::label() const
{
    return "Mini Met Model";
}

quint16 SunSpecMiniMetModel::ghi() const
{
    return m_ghi;
}
float SunSpecMiniMetModel::temp() const
{
    return m_temp;
}
float SunSpecMiniMetModel::ambientTemperature() const
{
    return m_ambientTemperature;
}
quint16 SunSpecMiniMetModel::windSpeed() const
{
    return m_windSpeed;
}
void SunSpecMiniMetModel::initDataPoints()
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

    SunSpecDataPoint ghiDataPoint;
    ghiDataPoint.setName("GHI");
    ghiDataPoint.setLabel("GHI");
    ghiDataPoint.setDescription("Global Horizontal Irradiance");
    ghiDataPoint.setUnits("W/m2");
    ghiDataPoint.setSize(1);
    ghiDataPoint.setAddressOffset(2);
    ghiDataPoint.setBlockOffset(0);
    ghiDataPoint.setSunSpecDataType("uint16");
    ghiDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ghiDataPoint.name(), ghiDataPoint);

    SunSpecDataPoint tempDataPoint;
    tempDataPoint.setName("TmpBOM");
    tempDataPoint.setLabel("Temp");
    tempDataPoint.setDescription("Back of module temperature measurement");
    tempDataPoint.setUnits("C");
    tempDataPoint.setSize(1);
    tempDataPoint.setAddressOffset(3);
    tempDataPoint.setBlockOffset(1);
    tempDataPoint.setScaleFactorName("-1");
    tempDataPoint.setSunSpecDataType("int16");
    tempDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tempDataPoint.name(), tempDataPoint);

    SunSpecDataPoint ambientTemperatureDataPoint;
    ambientTemperatureDataPoint.setName("TmpAmb");
    ambientTemperatureDataPoint.setLabel("Ambient Temperature");
    ambientTemperatureDataPoint.setUnits("C");
    ambientTemperatureDataPoint.setSize(1);
    ambientTemperatureDataPoint.setAddressOffset(4);
    ambientTemperatureDataPoint.setBlockOffset(2);
    ambientTemperatureDataPoint.setScaleFactorName("-1");
    ambientTemperatureDataPoint.setSunSpecDataType("int16");
    ambientTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ambientTemperatureDataPoint.name(), ambientTemperatureDataPoint);

    SunSpecDataPoint windSpeedDataPoint;
    windSpeedDataPoint.setName("WndSpd");
    windSpeedDataPoint.setLabel("Wind Speed");
    windSpeedDataPoint.setUnits("m/s");
    windSpeedDataPoint.setSize(1);
    windSpeedDataPoint.setAddressOffset(5);
    windSpeedDataPoint.setBlockOffset(3);
    windSpeedDataPoint.setSunSpecDataType("uint16");
    windSpeedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(windSpeedDataPoint.name(), windSpeedDataPoint);

}

void SunSpecMiniMetModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("GHI").isValid())
        m_ghi = m_dataPoints.value("GHI").toUInt16();

    if (m_dataPoints.value("TmpBOM").isValid())
        m_temp = m_dataPoints.value("TmpBOM").toFloatWithSSF(-1);

    if (m_dataPoints.value("TmpAmb").isValid())
        m_ambientTemperature = m_dataPoints.value("TmpAmb").toFloatWithSSF(-1);

    if (m_dataPoints.value("WndSpd").isValid())
        m_windSpeed = m_dataPoints.value("WndSpd").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecMiniMetModel *model)
{
    debug.nospace().noquote() << "SunSpecMiniMetModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("GHI") << "-->";
    if (model->dataPoints().value("GHI").isValid()) {
        debug.nospace().noquote() << model->ghi() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpBOM") << "-->";
    if (model->dataPoints().value("TmpBOM").isValid()) {
        debug.nospace().noquote() << model->temp() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpAmb") << "-->";
    if (model->dataPoints().value("TmpAmb").isValid()) {
        debug.nospace().noquote() << model->ambientTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WndSpd") << "-->";
    if (model->dataPoints().value("WndSpd").isValid()) {
        debug.nospace().noquote() << model->windSpeed() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
