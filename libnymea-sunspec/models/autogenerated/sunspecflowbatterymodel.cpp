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

#include "sunspecflowbatterymodel.h"

SunSpecFlowBatteryModel::SunSpecFlowBatteryModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 806;
}

SunSpecFlowBatteryModel::~SunSpecFlowBatteryModel()
{

}

QString SunSpecFlowBatteryModel::name() const
{
    return "flow_battery";
}

QString SunSpecFlowBatteryModel::description() const
{
    return QString();
}

QString SunSpecFlowBatteryModel::label() const
{
    return "Flow Battery Model";
}

quint16 SunSpecFlowBatteryModel::modelId() const
{
    return m_modelId;
}
quint16 SunSpecFlowBatteryModel::modelLength() const
{
    return m_modelLength;
}
quint16 SunSpecFlowBatteryModel::batteryPointsToBeDetermined() const
{
    return m_batteryPointsToBeDetermined;
}
void SunSpecFlowBatteryModel::initDataPoints()
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

    SunSpecDataPoint batteryPointsToBeDeterminedDataPoint;
    batteryPointsToBeDeterminedDataPoint.setName("BatTBD");
    batteryPointsToBeDeterminedDataPoint.setLabel("Battery Points To Be Determined");
    batteryPointsToBeDeterminedDataPoint.setMandatory(true);
    batteryPointsToBeDeterminedDataPoint.setSize(1);
    batteryPointsToBeDeterminedDataPoint.setAddressOffset(2);
    batteryPointsToBeDeterminedDataPoint.setBlockOffset(0);
    batteryPointsToBeDeterminedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(batteryPointsToBeDeterminedDataPoint.name(), batteryPointsToBeDeterminedDataPoint);

}

