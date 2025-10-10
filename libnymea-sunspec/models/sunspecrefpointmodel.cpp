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

#include "sunspecrefpointmodel.h"
#include "sunspecconnection.h"

SunSpecRefPointModel::SunSpecRefPointModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 306, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
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

quint16 SunSpecRefPointModel::ghi() const
{
    return m_ghi;
}
quint16 SunSpecRefPointModel::amps() const
{
    return m_amps;
}
quint16 SunSpecRefPointModel::voltage() const
{
    return m_voltage;
}
quint16 SunSpecRefPointModel::temperature() const
{
    return m_temperature;
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

    SunSpecDataPoint ampsDataPoint;
    ampsDataPoint.setName("A");
    ampsDataPoint.setLabel("Amps");
    ampsDataPoint.setDescription("Current measurement at reference point");
    ampsDataPoint.setUnits("W/m2");
    ampsDataPoint.setSize(1);
    ampsDataPoint.setAddressOffset(3);
    ampsDataPoint.setBlockOffset(1);
    ampsDataPoint.setSunSpecDataType("uint16");
    ampsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

    SunSpecDataPoint voltageDataPoint;
    voltageDataPoint.setName("V");
    voltageDataPoint.setLabel("Voltage");
    voltageDataPoint.setDescription("Voltage  measurement at reference point");
    voltageDataPoint.setUnits("W/m2");
    voltageDataPoint.setSize(1);
    voltageDataPoint.setAddressOffset(4);
    voltageDataPoint.setBlockOffset(2);
    voltageDataPoint.setSunSpecDataType("uint16");
    voltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

    SunSpecDataPoint temperatureDataPoint;
    temperatureDataPoint.setName("Tmp");
    temperatureDataPoint.setLabel("Temperature");
    temperatureDataPoint.setDescription("Temperature measurement at reference point");
    temperatureDataPoint.setUnits("W/m2");
    temperatureDataPoint.setSize(1);
    temperatureDataPoint.setAddressOffset(5);
    temperatureDataPoint.setBlockOffset(3);
    temperatureDataPoint.setSunSpecDataType("uint16");
    temperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(temperatureDataPoint.name(), temperatureDataPoint);

}

void SunSpecRefPointModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("GHI").isValid())
        m_ghi = m_dataPoints.value("GHI").toUInt16();

    if (m_dataPoints.value("A").isValid())
        m_amps = m_dataPoints.value("A").toUInt16();

    if (m_dataPoints.value("V").isValid())
        m_voltage = m_dataPoints.value("V").toUInt16();

    if (m_dataPoints.value("Tmp").isValid())
        m_temperature = m_dataPoints.value("Tmp").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecRefPointModel *model)
{
    debug.nospace().noquote() << "SunSpecRefPointModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("GHI") << "-->";
    if (model->dataPoints().value("GHI").isValid()) {
        debug.nospace().noquote() << model->ghi() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("A") << "-->";
    if (model->dataPoints().value("A").isValid()) {
        debug.nospace().noquote() << model->amps() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("V") << "-->";
    if (model->dataPoints().value("V").isValid()) {
        debug.nospace().noquote() << model->voltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Tmp") << "-->";
    if (model->dataPoints().value("Tmp").isValid()) {
        debug.nospace().noquote() << model->temperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
