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

SunSpecExtSettingsModel::SunSpecExtSettingsModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
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

void SunSpecExtSettingsModel::readModelHeader()
{

}

void SunSpecExtSettingsModel::readBlockData()
{

}

void SunSpecExtSettingsModel::initDataPoints()
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

    SunSpecDataPoint rampUpRateDataPoint;
    rampUpRateDataPoint.setName("NomRmpUpRte");
    rampUpRateDataPoint.setLabel("Ramp Up Rate");
    rampUpRateDataPoint.setDescription("Ramp up rate as a percentage of max current.");
    rampUpRateDataPoint.setUnits("Pct");
    rampUpRateDataPoint.setSize(1);
    rampUpRateDataPoint.setScaleFactorName("Rmp_SF");
    rampUpRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    rampUpRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << rampUpRateDataPoint;

    SunSpecDataPoint nomrmpdnrteDataPoint;
    nomrmpdnrteDataPoint.setName("NomRmpDnRte");
    nomrmpdnrteDataPoint.setLabel("NomRmpDnRte");
    nomrmpdnrteDataPoint.setDescription("Ramp down rate as a percentage of max current.");
    nomrmpdnrteDataPoint.setUnits("Pct");
    nomrmpdnrteDataPoint.setSize(1);
    nomrmpdnrteDataPoint.setScaleFactorName("Rmp_SF");
    nomrmpdnrteDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    nomrmpdnrteDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << nomrmpdnrteDataPoint;

    SunSpecDataPoint emergencyRampUpRateDataPoint;
    emergencyRampUpRateDataPoint.setName("EmgRmpUpRte");
    emergencyRampUpRateDataPoint.setLabel("Emergency Ramp Up Rate");
    emergencyRampUpRateDataPoint.setDescription("Emergency ramp up rate as a percentage of max current.");
    emergencyRampUpRateDataPoint.setUnits("Pct");
    emergencyRampUpRateDataPoint.setSize(1);
    emergencyRampUpRateDataPoint.setScaleFactorName("Rmp_SF");
    emergencyRampUpRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    emergencyRampUpRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << emergencyRampUpRateDataPoint;

    SunSpecDataPoint emergencyRampDownRateDataPoint;
    emergencyRampDownRateDataPoint.setName("EmgRmpDnRte");
    emergencyRampDownRateDataPoint.setLabel("Emergency Ramp Down Rate");
    emergencyRampDownRateDataPoint.setDescription("Emergency ramp down rate as a percentage of max current.");
    emergencyRampDownRateDataPoint.setUnits("Pct");
    emergencyRampDownRateDataPoint.setSize(1);
    emergencyRampDownRateDataPoint.setScaleFactorName("Rmp_SF");
    emergencyRampDownRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    emergencyRampDownRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << emergencyRampDownRateDataPoint;

    SunSpecDataPoint connectRampUpRateDataPoint;
    connectRampUpRateDataPoint.setName("ConnRmpUpRte");
    connectRampUpRateDataPoint.setLabel("Connect Ramp Up Rate");
    connectRampUpRateDataPoint.setDescription("Connect ramp up rate as a percentage of max current.");
    connectRampUpRateDataPoint.setUnits("Pct");
    connectRampUpRateDataPoint.setSize(1);
    connectRampUpRateDataPoint.setScaleFactorName("Rmp_SF");
    connectRampUpRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connectRampUpRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << connectRampUpRateDataPoint;

    SunSpecDataPoint connectRampDownRateDataPoint;
    connectRampDownRateDataPoint.setName("ConnRmpDnRte");
    connectRampDownRateDataPoint.setLabel("Connect Ramp Down Rate");
    connectRampDownRateDataPoint.setDescription("Connect ramp down rate as a percentage of max current.");
    connectRampDownRateDataPoint.setUnits("Pct");
    connectRampDownRateDataPoint.setSize(1);
    connectRampDownRateDataPoint.setScaleFactorName("Rmp_SF");
    connectRampDownRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    connectRampDownRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << connectRampDownRateDataPoint;

    SunSpecDataPoint defaultRampRateDataPoint;
    defaultRampRateDataPoint.setName("AGra");
    defaultRampRateDataPoint.setLabel("Default Ramp Rate");
    defaultRampRateDataPoint.setDescription("Ramp rate specified in percent of max current.");
    defaultRampRateDataPoint.setUnits("Pct");
    defaultRampRateDataPoint.setSize(1);
    defaultRampRateDataPoint.setScaleFactorName("Rmp_SF");
    defaultRampRateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    defaultRampRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << defaultRampRateDataPoint;

    SunSpecDataPoint rampRateScaleFactorDataPoint;
    rampRateScaleFactorDataPoint.setName("Rmp_SF");
    rampRateScaleFactorDataPoint.setLabel("Ramp Rate Scale Factor");
    rampRateScaleFactorDataPoint.setDescription("Ramp Rate Scale Factor");
    rampRateScaleFactorDataPoint.setSize(1);
    rampRateScaleFactorDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << rampRateScaleFactorDataPoint;

}

