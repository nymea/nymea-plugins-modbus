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

#include "sunspecmpptmodel.h"

SunSpecMpptModel::SunSpecMpptModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 160;
}

SunSpecMpptModel::~SunSpecMpptModel()
{

}

QString SunSpecMpptModel::name() const
{
    return "mppt";
}

QString SunSpecMpptModel::description() const
{
    return QString();
}

QString SunSpecMpptModel::label() const
{
    return "Multiple MPPT Inverter Extension Model";
}

quint16 SunSpecMpptModel::modelId() const
{
    return m_modelId;
}
quint16 SunSpecMpptModel::modelLength() const
{
    return m_modelLength;
}
SunSpecMpptModel::EvtFlags SunSpecMpptModel::globalEvents() const
{
    return m_globalEvents;
}
int SunSpecMpptModel::numberOfModules() const
{
    return m_numberOfModules;
}
quint16 SunSpecMpptModel::timestampPeriod() const
{
    return m_timestampPeriod;
}
void SunSpecMpptModel::initDataPoints()
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

    SunSpecDataPoint currentScaleFactorDataPoint;
    currentScaleFactorDataPoint.setName("DCA_SF");
    currentScaleFactorDataPoint.setLabel("Current Scale Factor");
    currentScaleFactorDataPoint.setSize(1);
    currentScaleFactorDataPoint.setAddressOffset(2);
    currentScaleFactorDataPoint.setBlockOffset(0);
    currentScaleFactorDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(currentScaleFactorDataPoint.name(), currentScaleFactorDataPoint);

    SunSpecDataPoint voltageScaleFactorDataPoint;
    voltageScaleFactorDataPoint.setName("DCV_SF");
    voltageScaleFactorDataPoint.setLabel("Voltage Scale Factor");
    voltageScaleFactorDataPoint.setSize(1);
    voltageScaleFactorDataPoint.setAddressOffset(3);
    voltageScaleFactorDataPoint.setBlockOffset(1);
    voltageScaleFactorDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(voltageScaleFactorDataPoint.name(), voltageScaleFactorDataPoint);

    SunSpecDataPoint powerScaleFactorDataPoint;
    powerScaleFactorDataPoint.setName("DCW_SF");
    powerScaleFactorDataPoint.setLabel("Power Scale Factor");
    powerScaleFactorDataPoint.setSize(1);
    powerScaleFactorDataPoint.setAddressOffset(4);
    powerScaleFactorDataPoint.setBlockOffset(2);
    powerScaleFactorDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(powerScaleFactorDataPoint.name(), powerScaleFactorDataPoint);

    SunSpecDataPoint energyScaleFactorDataPoint;
    energyScaleFactorDataPoint.setName("DCWH_SF");
    energyScaleFactorDataPoint.setLabel("Energy Scale Factor");
    energyScaleFactorDataPoint.setSize(1);
    energyScaleFactorDataPoint.setAddressOffset(5);
    energyScaleFactorDataPoint.setBlockOffset(3);
    energyScaleFactorDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(energyScaleFactorDataPoint.name(), energyScaleFactorDataPoint);

    SunSpecDataPoint globalEventsDataPoint;
    globalEventsDataPoint.setName("Evt");
    globalEventsDataPoint.setLabel("Global Events");
    globalEventsDataPoint.setSize(2);
    globalEventsDataPoint.setAddressOffset(6);
    globalEventsDataPoint.setBlockOffset(4);
    globalEventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(globalEventsDataPoint.name(), globalEventsDataPoint);

    SunSpecDataPoint numberOfModulesDataPoint;
    numberOfModulesDataPoint.setName("N");
    numberOfModulesDataPoint.setLabel("Number of Modules");
    numberOfModulesDataPoint.setSize(1);
    numberOfModulesDataPoint.setAddressOffset(8);
    numberOfModulesDataPoint.setBlockOffset(6);
    numberOfModulesDataPoint.setDataType(SunSpecDataPoint::stringToDataType("count"));
    m_dataPoints.insert(numberOfModulesDataPoint.name(), numberOfModulesDataPoint);

    SunSpecDataPoint timestampPeriodDataPoint;
    timestampPeriodDataPoint.setName("TmsPer");
    timestampPeriodDataPoint.setLabel("Timestamp Period");
    timestampPeriodDataPoint.setSize(1);
    timestampPeriodDataPoint.setAddressOffset(9);
    timestampPeriodDataPoint.setBlockOffset(7);
    timestampPeriodDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(timestampPeriodDataPoint.name(), timestampPeriodDataPoint);

}

