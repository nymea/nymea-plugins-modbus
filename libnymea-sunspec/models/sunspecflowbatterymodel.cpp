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
#include "sunspecconnection.h"

SunSpecFlowBatteryModel::SunSpecFlowBatteryModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 806, 1, parent)
{
    //Q_ASSERT_X(length == 1,  "SunSpecFlowBatteryModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
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

quint16 SunSpecFlowBatteryModel::batteryPointsToBeDetermined() const
{
    return m_batteryPointsToBeDetermined;
}
void SunSpecFlowBatteryModel::processBlockData()
{
    // Update properties according to the data point type
    m_batteryPointsToBeDetermined = m_dataPoints.value("BatTBD").toUInt16();

    qCDebug(dcSunSpec()) << this;
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
    modelIdDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

    SunSpecDataPoint batteryPointsToBeDeterminedDataPoint;
    batteryPointsToBeDeterminedDataPoint.setName("BatTBD");
    batteryPointsToBeDeterminedDataPoint.setLabel("Battery Points To Be Determined");
    batteryPointsToBeDeterminedDataPoint.setMandatory(true);
    batteryPointsToBeDeterminedDataPoint.setSize(1);
    batteryPointsToBeDeterminedDataPoint.setAddressOffset(2);
    batteryPointsToBeDeterminedDataPoint.setBlockOffset(0);
    batteryPointsToBeDeterminedDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(batteryPointsToBeDeterminedDataPoint.name(), batteryPointsToBeDeterminedDataPoint);

}

QDebug operator<<(QDebug debug, SunSpecFlowBatteryModel *model)
{
    debug.nospace().noquote() << "SunSpecFlowBatteryModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    if (model->dataPoints().value("BatTBD").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("BatTBD") << "--> " << model->batteryPointsToBeDetermined() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("BatTBD") << "--> NaN" << endl;
    }


    return debug.space().quote();
}
