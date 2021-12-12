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
#include "sunspecconnection.h"

SunSpecFlowBatteryModuleModelRepeatingBlock::SunSpecFlowBatteryModuleModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecFlowBatteryModuleModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecFlowBatteryModuleModelRepeatingBlock::name() const
{
    return "stack";
}

SunSpecFlowBatteryModuleModel *SunSpecFlowBatteryModuleModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecFlowBatteryModuleModelRepeatingBlock::stackPointsToBeDetermined() const
{
    return m_stackPointsToBeDetermined;
}

void SunSpecFlowBatteryModuleModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint stackPointsToBeDeterminedDataPoint;
    stackPointsToBeDeterminedDataPoint.setName("StackTBD");
    stackPointsToBeDeterminedDataPoint.setLabel("Stack Points To Be Determined");
    stackPointsToBeDeterminedDataPoint.setMandatory(true);
    stackPointsToBeDeterminedDataPoint.setSize(1);
    stackPointsToBeDeterminedDataPoint.setAddressOffset(0);
    stackPointsToBeDeterminedDataPoint.setSunSpecDataType("uint16");
    stackPointsToBeDeterminedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stackPointsToBeDeterminedDataPoint.name(), stackPointsToBeDeterminedDataPoint);

}

void SunSpecFlowBatteryModuleModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("StackTBD").isValid())
        m_stackPointsToBeDetermined = m_dataPoints.value("StackTBD").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecFlowBatteryModuleModel::SunSpecFlowBatteryModuleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 808, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

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
void SunSpecFlowBatteryModuleModel::initDataPoints()
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

    SunSpecDataPoint modulePointsToBeDeterminedDataPoint;
    modulePointsToBeDeterminedDataPoint.setName("ModuleTBD");
    modulePointsToBeDeterminedDataPoint.setLabel("Module Points To Be Determined");
    modulePointsToBeDeterminedDataPoint.setMandatory(true);
    modulePointsToBeDeterminedDataPoint.setSize(1);
    modulePointsToBeDeterminedDataPoint.setAddressOffset(2);
    modulePointsToBeDeterminedDataPoint.setBlockOffset(0);
    modulePointsToBeDeterminedDataPoint.setSunSpecDataType("uint16");
    modulePointsToBeDeterminedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modulePointsToBeDeterminedDataPoint.name(), modulePointsToBeDeterminedDataPoint);

}

void SunSpecFlowBatteryModuleModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("ModuleTBD").isValid())
        m_modulePointsToBeDetermined = m_dataPoints.value("ModuleTBD").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecFlowBatteryModuleModel *model)
{
    debug.nospace().noquote() << "SunSpecFlowBatteryModuleModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModuleTBD") << "-->";
    if (model->dataPoints().value("ModuleTBD").isValid()) {
        debug.nospace().noquote() << model->modulePointsToBeDetermined() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
