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

#include "sunspecextsettingsmodel.h"

SunSpecExtSettingsModel::SunSpecExtSettingsModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, 145, 8, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecExtSettingsModel::~SunSpecExtSettingsModel()
{

}

QString SunSpecExtSettingsModel::name() const
{
    return "ext_settings";
}

QString SunSpecExtSettingsModel::description() const
{
    return "Inverter controls extended settings ";
}

QString SunSpecExtSettingsModel::label() const
{
    return "Extended Settings";
}

float SunSpecExtSettingsModel::rampUpRate() const
{
    return m_rampUpRate;
}
float SunSpecExtSettingsModel::nomRmpDnRte() const
{
    return m_nomRmpDnRte;
}
float SunSpecExtSettingsModel::emergencyRampUpRate() const
{
    return m_emergencyRampUpRate;
}
float SunSpecExtSettingsModel::emergencyRampDownRate() const
{
    return m_emergencyRampDownRate;
}
float SunSpecExtSettingsModel::connectRampUpRate() const
{
    return m_connectRampUpRate;
}
float SunSpecExtSettingsModel::connectRampDownRate() const
{
    return m_connectRampDownRate;
}
float SunSpecExtSettingsModel::defaultRampRate() const
{
    return m_defaultRampRate;
}
void SunSpecExtSettingsModel::processBlockData()
{
    // Scale factors
    m_rampRateScaleFactor = m_dataPoints.value("Rmp_SF").toInt16();

    // Update properties according to the data point type
    m_modelId = m_dataPoints.value("ID").toUInt16();
    m_modelLength = m_dataPoints.value("L").toUInt16();
    m_rampUpRate = m_dataPoints.value("NomRmpUpRte").toFloatWithSSF(m_rampRateScaleFactor);
    m_nomRmpDnRte = m_dataPoints.value("NomRmpDnRte").toFloatWithSSF(m_rampRateScaleFactor);
    m_emergencyRampUpRate = m_dataPoints.value("EmgRmpUpRte").toFloatWithSSF(m_rampRateScaleFactor);
    m_emergencyRampDownRate = m_dataPoints.value("EmgRmpDnRte").toFloatWithSSF(m_rampRateScaleFactor);
    m_connectRampUpRate = m_dataPoints.value("ConnRmpUpRte").toFloatWithSSF(m_rampRateScaleFactor);
    m_connectRampDownRate = m_dataPoints.value("ConnRmpDnRte").toFloatWithSSF(m_rampRateScaleFactor);
    m_defaultRampRate = m_dataPoints.value("AGra").toFloatWithSSF(m_rampRateScaleFactor);
}

void SunSpecExtSettingsModel::initDataPoints()
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

    SunSpecDataPoint rampUpRateDataPoint;
    rampUpRateDataPoint.setName("NomRmpUpRte");
    rampUpRateDataPoint.setLabel("Ramp Up Rate");
    rampUpRateDataPoint.setDescription("Ramp up rate as a percentage of max current.");
    rampUpRateDataPoint.setUnits("Pct");
    rampUpRateDataPoint.setSize(1);
    rampUpRateDataPoint.setAddressOffset(2);
    rampUpRateDataPoint.setBlockOffset(0);
    rampUpRateDataPoint.setScaleFactorName("Rmp_SF");
    rampUpRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    rampUpRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(rampUpRateDataPoint.name(), rampUpRateDataPoint);

    SunSpecDataPoint nomRmpDnRteDataPoint;
    nomRmpDnRteDataPoint.setName("NomRmpDnRte");
    nomRmpDnRteDataPoint.setLabel("NomRmpDnRte");
    nomRmpDnRteDataPoint.setDescription("Ramp down rate as a percentage of max current.");
    nomRmpDnRteDataPoint.setUnits("Pct");
    nomRmpDnRteDataPoint.setSize(1);
    nomRmpDnRteDataPoint.setAddressOffset(3);
    nomRmpDnRteDataPoint.setBlockOffset(1);
    nomRmpDnRteDataPoint.setScaleFactorName("Rmp_SF");
    nomRmpDnRteDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    nomRmpDnRteDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(nomRmpDnRteDataPoint.name(), nomRmpDnRteDataPoint);

    SunSpecDataPoint emergencyRampUpRateDataPoint;
    emergencyRampUpRateDataPoint.setName("EmgRmpUpRte");
    emergencyRampUpRateDataPoint.setLabel("Emergency Ramp Up Rate");
    emergencyRampUpRateDataPoint.setDescription("Emergency ramp up rate as a percentage of max current.");
    emergencyRampUpRateDataPoint.setUnits("Pct");
    emergencyRampUpRateDataPoint.setSize(1);
    emergencyRampUpRateDataPoint.setAddressOffset(4);
    emergencyRampUpRateDataPoint.setBlockOffset(2);
    emergencyRampUpRateDataPoint.setScaleFactorName("Rmp_SF");
    emergencyRampUpRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    emergencyRampUpRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(emergencyRampUpRateDataPoint.name(), emergencyRampUpRateDataPoint);

    SunSpecDataPoint emergencyRampDownRateDataPoint;
    emergencyRampDownRateDataPoint.setName("EmgRmpDnRte");
    emergencyRampDownRateDataPoint.setLabel("Emergency Ramp Down Rate");
    emergencyRampDownRateDataPoint.setDescription("Emergency ramp down rate as a percentage of max current.");
    emergencyRampDownRateDataPoint.setUnits("Pct");
    emergencyRampDownRateDataPoint.setSize(1);
    emergencyRampDownRateDataPoint.setAddressOffset(5);
    emergencyRampDownRateDataPoint.setBlockOffset(3);
    emergencyRampDownRateDataPoint.setScaleFactorName("Rmp_SF");
    emergencyRampDownRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    emergencyRampDownRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(emergencyRampDownRateDataPoint.name(), emergencyRampDownRateDataPoint);

    SunSpecDataPoint connectRampUpRateDataPoint;
    connectRampUpRateDataPoint.setName("ConnRmpUpRte");
    connectRampUpRateDataPoint.setLabel("Connect Ramp Up Rate");
    connectRampUpRateDataPoint.setDescription("Connect ramp up rate as a percentage of max current.");
    connectRampUpRateDataPoint.setUnits("Pct");
    connectRampUpRateDataPoint.setSize(1);
    connectRampUpRateDataPoint.setAddressOffset(6);
    connectRampUpRateDataPoint.setBlockOffset(4);
    connectRampUpRateDataPoint.setScaleFactorName("Rmp_SF");
    connectRampUpRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connectRampUpRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connectRampUpRateDataPoint.name(), connectRampUpRateDataPoint);

    SunSpecDataPoint connectRampDownRateDataPoint;
    connectRampDownRateDataPoint.setName("ConnRmpDnRte");
    connectRampDownRateDataPoint.setLabel("Connect Ramp Down Rate");
    connectRampDownRateDataPoint.setDescription("Connect ramp down rate as a percentage of max current.");
    connectRampDownRateDataPoint.setUnits("Pct");
    connectRampDownRateDataPoint.setSize(1);
    connectRampDownRateDataPoint.setAddressOffset(7);
    connectRampDownRateDataPoint.setBlockOffset(5);
    connectRampDownRateDataPoint.setScaleFactorName("Rmp_SF");
    connectRampDownRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connectRampDownRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connectRampDownRateDataPoint.name(), connectRampDownRateDataPoint);

    SunSpecDataPoint defaultRampRateDataPoint;
    defaultRampRateDataPoint.setName("AGra");
    defaultRampRateDataPoint.setLabel("Default Ramp Rate");
    defaultRampRateDataPoint.setDescription("Ramp rate specified in percent of max current.");
    defaultRampRateDataPoint.setUnits("Pct");
    defaultRampRateDataPoint.setSize(1);
    defaultRampRateDataPoint.setAddressOffset(8);
    defaultRampRateDataPoint.setBlockOffset(6);
    defaultRampRateDataPoint.setScaleFactorName("Rmp_SF");
    defaultRampRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    defaultRampRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(defaultRampRateDataPoint.name(), defaultRampRateDataPoint);

    SunSpecDataPoint rampRateScaleFactorDataPoint;
    rampRateScaleFactorDataPoint.setName("Rmp_SF");
    rampRateScaleFactorDataPoint.setLabel("Ramp Rate Scale Factor");
    rampRateScaleFactorDataPoint.setDescription("Ramp Rate Scale Factor");
    rampRateScaleFactorDataPoint.setSize(1);
    rampRateScaleFactorDataPoint.setAddressOffset(9);
    rampRateScaleFactorDataPoint.setBlockOffset(7);
    rampRateScaleFactorDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(rampRateScaleFactorDataPoint.name(), rampRateScaleFactorDataPoint);

}

