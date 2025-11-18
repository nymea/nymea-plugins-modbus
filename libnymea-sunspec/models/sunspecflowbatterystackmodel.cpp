// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-sunspec.
*
* libnymea-sunspec is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-sunspec is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-sunspec. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "sunspecflowbatterystackmodel.h"
#include "sunspecconnection.h"

SunSpecFlowBatteryStackModelRepeatingBlock::SunSpecFlowBatteryStackModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecFlowBatteryStackModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_parentModel = parent;
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecFlowBatteryStackModelRepeatingBlock::name() const
{
    return "cell";
}

SunSpecFlowBatteryStackModel *SunSpecFlowBatteryStackModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecFlowBatteryStackModelRepeatingBlock::cellPointsToBeDetermined() const
{
    return m_cellPointsToBeDetermined;
}

void SunSpecFlowBatteryStackModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint cellPointsToBeDeterminedDataPoint;
    cellPointsToBeDeterminedDataPoint.setName("CellTBD");
    cellPointsToBeDeterminedDataPoint.setLabel("Cell Points To Be Determined");
    cellPointsToBeDeterminedDataPoint.setMandatory(true);
    cellPointsToBeDeterminedDataPoint.setSize(1);
    cellPointsToBeDeterminedDataPoint.setAddressOffset(0);
    cellPointsToBeDeterminedDataPoint.setSunSpecDataType("uint16");
    cellPointsToBeDeterminedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cellPointsToBeDeterminedDataPoint.name(), cellPointsToBeDeterminedDataPoint);

}

void SunSpecFlowBatteryStackModelRepeatingBlock::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("CellTBD").isValid())
        m_cellPointsToBeDetermined = m_dataPoints.value("CellTBD").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecFlowBatteryStackModel::SunSpecFlowBatteryStackModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 809, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();

    connect(this, &SunSpecModel::initFinished, this, &SunSpecFlowBatteryStackModel::setupRepeatingBlocks);
}

SunSpecFlowBatteryStackModel::~SunSpecFlowBatteryStackModel()
{

}

QString SunSpecFlowBatteryStackModel::name() const
{
    return "flow_battery_stack";
}

QString SunSpecFlowBatteryStackModel::description() const
{
    return QString();
}

QString SunSpecFlowBatteryStackModel::label() const
{
    return "Flow Battery Stack Model";
}

quint16 SunSpecFlowBatteryStackModel::stackPointsToBeDetermined() const
{
    return m_stackPointsToBeDetermined;
}
void SunSpecFlowBatteryStackModel::initDataPoints()
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

    SunSpecDataPoint stackPointsToBeDeterminedDataPoint;
    stackPointsToBeDeterminedDataPoint.setName("StackTBD");
    stackPointsToBeDeterminedDataPoint.setLabel("Stack Points To Be Determined");
    stackPointsToBeDeterminedDataPoint.setMandatory(true);
    stackPointsToBeDeterminedDataPoint.setSize(1);
    stackPointsToBeDeterminedDataPoint.setAddressOffset(2);
    stackPointsToBeDeterminedDataPoint.setBlockOffset(0);
    stackPointsToBeDeterminedDataPoint.setSunSpecDataType("uint16");
    stackPointsToBeDeterminedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stackPointsToBeDeterminedDataPoint.name(), stackPointsToBeDeterminedDataPoint);

}

void SunSpecFlowBatteryStackModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("StackTBD").isValid())
        m_stackPointsToBeDetermined = m_dataPoints.value("StackTBD").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

void SunSpecFlowBatteryStackModel::setupRepeatingBlocks()
{
    if (!m_repeatingBlocks.isEmpty()) {
        foreach (SunSpecModelRepeatingBlock *block, m_repeatingBlocks) {
            block->deleteLater();
        }
        m_repeatingBlocks.clear();
    }

    const auto headerLength = 2;
    const auto repeatingBlocksDataSize = m_blockData.size() - headerLength - m_fixedBlockLength;
    if (repeatingBlocksDataSize % m_repeatingBlockLength != 0) {
        qCWarning(dcSunSpecModelData()) << "Unexpected repeating block data size:"
                                        << repeatingBlocksDataSize
                                        << "(repeating block size:"
                                        << m_repeatingBlockLength
                                        << ", extra bytes:"
                                        << repeatingBlocksDataSize % m_repeatingBlockLength
                                        << "). Repeating blocks will not be handled!";
        return;
    }
    const auto numberOfBlocks = repeatingBlocksDataSize / m_repeatingBlockLength;
    const auto repeatingBlocksOffset = m_fixedBlockLength + headerLength;
    for (int i = 0; i < numberOfBlocks; ++i) {
        const auto blockStartRegister = static_cast<quint16>(modbusStartRegister() + repeatingBlocksOffset + m_repeatingBlockLength * i);
        const auto block = new SunSpecFlowBatteryStackModelRepeatingBlock(i, m_repeatingBlockLength, blockStartRegister, this);
        m_repeatingBlocks.append(block);
    }
}

QDebug operator<<(QDebug debug, SunSpecFlowBatteryStackModel *model)
{
    debug.nospace().noquote() << "SunSpecFlowBatteryStackModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("StackTBD") << "-->";
    if (model->dataPoints().value("StackTBD").isValid()) {
        debug.nospace().noquote() << model->stackPointsToBeDetermined() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
