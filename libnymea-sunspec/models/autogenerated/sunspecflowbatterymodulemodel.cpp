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

#include "sunspecflowbatterymodulemodel.h"

SunSpecFlowBatteryModuleModel::SunSpecFlowBatteryModuleModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, 808, 1, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecFlowBatteryModuleModel::~SunSpecFlowBatteryModuleModel()
{

}

QString SunSpecFlowBatteryModuleModel::name() const
{
    return "flow_battery_module";
}

QString SunSpecFlowBatteryModuleModel::description() const
{
    return QString();
}

QString SunSpecFlowBatteryModuleModel::label() const
{
    return "Flow Battery Module Model";
}

quint16 SunSpecFlowBatteryModuleModel::modulePointsToBeDetermined() const
{
    return m_modulePointsToBeDetermined;
}
void SunSpecFlowBatteryModuleModel::processBlockData()
{
    // Update properties according to the data point type
    m_modelId = m_dataPoints.value("ID").toUInt16();
    m_modelLength = m_dataPoints.value("L").toUInt16();
    m_modulePointsToBeDetermined = m_dataPoints.value("ModuleTBD").toUInt16();
}

void SunSpecFlowBatteryModuleModel::initDataPoints()
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

    SunSpecDataPoint modulePointsToBeDeterminedDataPoint;
    modulePointsToBeDeterminedDataPoint.setName("ModuleTBD");
    modulePointsToBeDeterminedDataPoint.setLabel("Module Points To Be Determined");
    modulePointsToBeDeterminedDataPoint.setMandatory(true);
    modulePointsToBeDeterminedDataPoint.setSize(1);
    modulePointsToBeDeterminedDataPoint.setAddressOffset(2);
    modulePointsToBeDeterminedDataPoint.setBlockOffset(0);
    modulePointsToBeDeterminedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(modulePointsToBeDeterminedDataPoint.name(), modulePointsToBeDeterminedDataPoint);

}

