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

#include "sunspeclocationmodel.h"
#include "sunspecconnection.h"

SunSpecLocationModel::SunSpecLocationModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 305, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecLocationModel::~SunSpecLocationModel()
{

}

QString SunSpecLocationModel::name() const
{
    return "location";
}

QString SunSpecLocationModel::description() const
{
    return "Include to support location measurements";
}

QString SunSpecLocationModel::label() const
{
    return "GPS";
}

QString SunSpecLocationModel::tm() const
{
    return m_tm;
}
QString SunSpecLocationModel::date() const
{
    return m_date;
}
QString SunSpecLocationModel::location() const
{
    return m_location;
}
float SunSpecLocationModel::lat() const
{
    return m_lat;
}
float SunSpecLocationModel::longitude() const
{
    return m_longitude;
}
qint32 SunSpecLocationModel::altitude() const
{
    return m_altitude;
}
void SunSpecLocationModel::initDataPoints()
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

    SunSpecDataPoint tmDataPoint;
    tmDataPoint.setName("Tm");
    tmDataPoint.setLabel("Tm");
    tmDataPoint.setDescription("UTC 24 hour time stamp to millisecond hhmmss.sssZ format");
    tmDataPoint.setUnits("hhmmss.sssZ");
    tmDataPoint.setSize(6);
    tmDataPoint.setAddressOffset(2);
    tmDataPoint.setBlockOffset(0);
    tmDataPoint.setSunSpecDataType("string");
    tmDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tmDataPoint.name(), tmDataPoint);

    SunSpecDataPoint dateDataPoint;
    dateDataPoint.setName("Date");
    dateDataPoint.setLabel("Date");
    dateDataPoint.setDescription("UTC Date string YYYYMMDD format");
    dateDataPoint.setUnits("YYYYMMDD");
    dateDataPoint.setSize(4);
    dateDataPoint.setAddressOffset(8);
    dateDataPoint.setBlockOffset(6);
    dateDataPoint.setSunSpecDataType("string");
    dateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dateDataPoint.name(), dateDataPoint);

    SunSpecDataPoint locationDataPoint;
    locationDataPoint.setName("Loc");
    locationDataPoint.setLabel("Location");
    locationDataPoint.setDescription("Location string (40 chars max)");
    locationDataPoint.setUnits("text");
    locationDataPoint.setSize(20);
    locationDataPoint.setAddressOffset(12);
    locationDataPoint.setBlockOffset(10);
    locationDataPoint.setSunSpecDataType("string");
    locationDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(locationDataPoint.name(), locationDataPoint);

    SunSpecDataPoint latDataPoint;
    latDataPoint.setName("Lat");
    latDataPoint.setLabel("Lat");
    latDataPoint.setDescription("Latitude with seven degrees of precision");
    latDataPoint.setUnits("Degrees");
    latDataPoint.setSize(2);
    latDataPoint.setAddressOffset(32);
    latDataPoint.setBlockOffset(30);
    latDataPoint.setScaleFactorName("-7");
    latDataPoint.setSunSpecDataType("int32");
    latDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(latDataPoint.name(), latDataPoint);

    SunSpecDataPoint longitudeDataPoint;
    longitudeDataPoint.setName("Long");
    longitudeDataPoint.setLabel("Long");
    longitudeDataPoint.setDescription("Longitude with seven degrees of precision");
    longitudeDataPoint.setUnits("Degrees");
    longitudeDataPoint.setSize(2);
    longitudeDataPoint.setAddressOffset(34);
    longitudeDataPoint.setBlockOffset(32);
    longitudeDataPoint.setScaleFactorName("-7");
    longitudeDataPoint.setSunSpecDataType("int32");
    longitudeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(longitudeDataPoint.name(), longitudeDataPoint);

    SunSpecDataPoint altitudeDataPoint;
    altitudeDataPoint.setName("Alt");
    altitudeDataPoint.setLabel("Altitude");
    altitudeDataPoint.setDescription("Altitude measurement in meters");
    altitudeDataPoint.setUnits("meters");
    altitudeDataPoint.setSize(2);
    altitudeDataPoint.setAddressOffset(36);
    altitudeDataPoint.setBlockOffset(34);
    altitudeDataPoint.setSunSpecDataType("int32");
    altitudeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(altitudeDataPoint.name(), altitudeDataPoint);

}

void SunSpecLocationModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("Tm").isValid())
        m_tm = m_dataPoints.value("Tm").toString();

    if (m_dataPoints.value("Date").isValid())
        m_date = m_dataPoints.value("Date").toString();

    if (m_dataPoints.value("Loc").isValid())
        m_location = m_dataPoints.value("Loc").toString();

    if (m_dataPoints.value("Lat").isValid())
        m_lat = m_dataPoints.value("Lat").toFloatWithSSF(-7);

    if (m_dataPoints.value("Long").isValid())
        m_longitude = m_dataPoints.value("Long").toFloatWithSSF(-7);

    if (m_dataPoints.value("Alt").isValid())
        m_altitude = m_dataPoints.value("Alt").toInt32();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecLocationModel *model)
{
    debug.nospace().noquote() << "SunSpecLocationModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("Tm") << "-->";
    if (model->dataPoints().value("Tm").isValid()) {
        debug.nospace().noquote() << model->tm() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Date") << "-->";
    if (model->dataPoints().value("Date").isValid()) {
        debug.nospace().noquote() << model->date() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Loc") << "-->";
    if (model->dataPoints().value("Loc").isValid()) {
        debug.nospace().noquote() << model->location() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Lat") << "-->";
    if (model->dataPoints().value("Lat").isValid()) {
        debug.nospace().noquote() << model->lat() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Long") << "-->";
    if (model->dataPoints().value("Long").isValid()) {
        debug.nospace().noquote() << model->longitude() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Alt") << "-->";
    if (model->dataPoints().value("Alt").isValid()) {
        debug.nospace().noquote() << model->altitude() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
