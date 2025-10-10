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

#include "sunspecmpptmodel.h"
#include "sunspecconnection.h"

SunSpecMpptModelRepeatingBlock::SunSpecMpptModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecMpptModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_parentModel = parent;
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecMpptModelRepeatingBlock::name() const
{
    return "module";
}

SunSpecMpptModel *SunSpecMpptModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecMpptModelRepeatingBlock::inputId() const
{
    return m_inputId;
}
QString SunSpecMpptModelRepeatingBlock::inputIdSting() const
{
    return m_inputIdSting;
}
float SunSpecMpptModelRepeatingBlock::dcCurrent() const
{
    return m_dcCurrent;
}
float SunSpecMpptModelRepeatingBlock::dcVoltage() const
{
    return m_dcVoltage;
}
float SunSpecMpptModelRepeatingBlock::dcPower() const
{
    return m_dcPower;
}
quint32 SunSpecMpptModelRepeatingBlock::lifetimeEnergy() const
{
    return m_lifetimeEnergy;
}
quint32 SunSpecMpptModelRepeatingBlock::timestamp() const
{
    return m_timestamp;
}
qint16 SunSpecMpptModelRepeatingBlock::temperature() const
{
    return m_temperature;
}
SunSpecMpptModelRepeatingBlock::Dcst SunSpecMpptModelRepeatingBlock::operatingState() const
{
    return m_operatingState;
}
SunSpecMpptModelRepeatingBlock::DcevtFlags SunSpecMpptModelRepeatingBlock::moduleEvents() const
{
    return m_moduleEvents;
}

void SunSpecMpptModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint inputIdDataPoint;
    inputIdDataPoint.setName("ID");
    inputIdDataPoint.setLabel("Input ID");
    inputIdDataPoint.setSize(1);
    inputIdDataPoint.setAddressOffset(0);
    inputIdDataPoint.setSunSpecDataType("uint16");
    inputIdDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inputIdDataPoint.name(), inputIdDataPoint);

    SunSpecDataPoint inputIdStingDataPoint;
    inputIdStingDataPoint.setName("IDStr");
    inputIdStingDataPoint.setLabel("Input ID Sting");
    inputIdStingDataPoint.setSize(8);
    inputIdStingDataPoint.setAddressOffset(1);
    inputIdStingDataPoint.setSunSpecDataType("string");
    inputIdStingDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inputIdStingDataPoint.name(), inputIdStingDataPoint);

    SunSpecDataPoint dcCurrentDataPoint;
    dcCurrentDataPoint.setName("DCA");
    dcCurrentDataPoint.setLabel("DC Current");
    dcCurrentDataPoint.setUnits("A");
    dcCurrentDataPoint.setSize(1);
    dcCurrentDataPoint.setAddressOffset(9);
    dcCurrentDataPoint.setBlockOffset(7);
    dcCurrentDataPoint.setScaleFactorName("DCA_SF");
    dcCurrentDataPoint.setSunSpecDataType("uint16");
    dcCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dcCurrentDataPoint.name(), dcCurrentDataPoint);

    SunSpecDataPoint dcVoltageDataPoint;
    dcVoltageDataPoint.setName("DCV");
    dcVoltageDataPoint.setLabel("DC Voltage");
    dcVoltageDataPoint.setUnits("V");
    dcVoltageDataPoint.setSize(1);
    dcVoltageDataPoint.setAddressOffset(10);
    dcVoltageDataPoint.setBlockOffset(8);
    dcVoltageDataPoint.setScaleFactorName("DCV_SF");
    dcVoltageDataPoint.setSunSpecDataType("uint16");
    dcVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dcVoltageDataPoint.name(), dcVoltageDataPoint);

    SunSpecDataPoint dcPowerDataPoint;
    dcPowerDataPoint.setName("DCW");
    dcPowerDataPoint.setLabel("DC Power");
    dcPowerDataPoint.setUnits("W");
    dcPowerDataPoint.setSize(1);
    dcPowerDataPoint.setAddressOffset(11);
    dcPowerDataPoint.setBlockOffset(9);
    dcPowerDataPoint.setScaleFactorName("DCW_SF");
    dcPowerDataPoint.setSunSpecDataType("uint16");
    dcPowerDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dcPowerDataPoint.name(), dcPowerDataPoint);

    SunSpecDataPoint lifetimeEnergyDataPoint;
    lifetimeEnergyDataPoint.setName("DCWH");
    lifetimeEnergyDataPoint.setLabel("Lifetime Energy");
    lifetimeEnergyDataPoint.setUnits("Wh");
    lifetimeEnergyDataPoint.setSize(2);
    lifetimeEnergyDataPoint.setAddressOffset(12);
    lifetimeEnergyDataPoint.setBlockOffset(10);
    lifetimeEnergyDataPoint.setScaleFactorName("DCWH_SF");
    lifetimeEnergyDataPoint.setSunSpecDataType("acc32");
    lifetimeEnergyDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(lifetimeEnergyDataPoint.name(), lifetimeEnergyDataPoint);

    SunSpecDataPoint timestampDataPoint;
    timestampDataPoint.setName("Tms");
    timestampDataPoint.setLabel("Timestamp");
    timestampDataPoint.setUnits("Secs");
    timestampDataPoint.setSize(2);
    timestampDataPoint.setAddressOffset(14);
    timestampDataPoint.setBlockOffset(12);
    timestampDataPoint.setSunSpecDataType("uint32");
    timestampDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(timestampDataPoint.name(), timestampDataPoint);

    SunSpecDataPoint temperatureDataPoint;
    temperatureDataPoint.setName("Tmp");
    temperatureDataPoint.setLabel("Temperature");
    temperatureDataPoint.setUnits("C");
    temperatureDataPoint.setSize(1);
    temperatureDataPoint.setAddressOffset(16);
    temperatureDataPoint.setBlockOffset(14);
    temperatureDataPoint.setSunSpecDataType("int16");
    temperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(temperatureDataPoint.name(), temperatureDataPoint);

    SunSpecDataPoint operatingStateDataPoint;
    operatingStateDataPoint.setName("DCSt");
    operatingStateDataPoint.setLabel("Operating State");
    operatingStateDataPoint.setSize(1);
    operatingStateDataPoint.setAddressOffset(17);
    operatingStateDataPoint.setBlockOffset(15);
    operatingStateDataPoint.setSunSpecDataType("enum16");
    operatingStateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(operatingStateDataPoint.name(), operatingStateDataPoint);

    SunSpecDataPoint moduleEventsDataPoint;
    moduleEventsDataPoint.setName("DCEvt");
    moduleEventsDataPoint.setLabel("Module Events");
    moduleEventsDataPoint.setSize(2);
    moduleEventsDataPoint.setAddressOffset(18);
    moduleEventsDataPoint.setBlockOffset(16);
    moduleEventsDataPoint.setSunSpecDataType("bitfield32");
    moduleEventsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleEventsDataPoint.name(), moduleEventsDataPoint);

}

void SunSpecMpptModelRepeatingBlock::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("ID").isValid())
        m_inputId = m_dataPoints.value("ID").toUInt16();

    if (m_dataPoints.value("IDStr").isValid())
        m_inputIdSting = m_dataPoints.value("IDStr").toString();

    if (m_dataPoints.value("DCA").isValid())
        m_dcCurrent = m_dataPoints.value("DCA").toFloatWithSSF(m_parentModel->currentScaleFactor());

    if (m_dataPoints.value("DCV").isValid())
        m_dcVoltage = m_dataPoints.value("DCV").toFloatWithSSF(m_parentModel->voltageScaleFactor());

    if (m_dataPoints.value("DCW").isValid())
        m_dcPower = m_dataPoints.value("DCW").toFloatWithSSF(m_parentModel->powerScaleFactor());

    if (m_dataPoints.value("DCWH").isValid())
        m_lifetimeEnergy = m_dataPoints.value("DCWH").toFloatWithSSF(m_parentModel->energyScaleFactor());

    if (m_dataPoints.value("Tms").isValid())
        m_timestamp = m_dataPoints.value("Tms").toUInt32();

    if (m_dataPoints.value("Tmp").isValid())
        m_temperature = m_dataPoints.value("Tmp").toInt16();

    if (m_dataPoints.value("DCSt").isValid())
        m_operatingState = static_cast<Dcst>(m_dataPoints.value("DCSt").toUInt16());

    if (m_dataPoints.value("DCEvt").isValid())
        m_moduleEvents = static_cast<DcevtFlags>(m_dataPoints.value("DCEvt").toUInt32());


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecMpptModel::SunSpecMpptModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 160, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();

    connect(this, &SunSpecModel::initFinished, this, &SunSpecMpptModel::setupRepeatingBlocks);
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

qint16 SunSpecMpptModel::currentScaleFactor() const
{
    return m_currentScaleFactor;
}
qint16 SunSpecMpptModel::voltageScaleFactor() const
{
    return m_voltageScaleFactor;
}
qint16 SunSpecMpptModel::powerScaleFactor() const
{
    return m_powerScaleFactor;
}
qint16 SunSpecMpptModel::energyScaleFactor() const
{
    return m_energyScaleFactor;
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

    SunSpecDataPoint currentScaleFactorDataPoint;
    currentScaleFactorDataPoint.setName("DCA_SF");
    currentScaleFactorDataPoint.setLabel("Current Scale Factor");
    currentScaleFactorDataPoint.setSize(1);
    currentScaleFactorDataPoint.setAddressOffset(2);
    currentScaleFactorDataPoint.setBlockOffset(0);
    currentScaleFactorDataPoint.setSunSpecDataType("sunssf");
    currentScaleFactorDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(currentScaleFactorDataPoint.name(), currentScaleFactorDataPoint);

    SunSpecDataPoint voltageScaleFactorDataPoint;
    voltageScaleFactorDataPoint.setName("DCV_SF");
    voltageScaleFactorDataPoint.setLabel("Voltage Scale Factor");
    voltageScaleFactorDataPoint.setSize(1);
    voltageScaleFactorDataPoint.setAddressOffset(3);
    voltageScaleFactorDataPoint.setBlockOffset(1);
    voltageScaleFactorDataPoint.setSunSpecDataType("sunssf");
    voltageScaleFactorDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(voltageScaleFactorDataPoint.name(), voltageScaleFactorDataPoint);

    SunSpecDataPoint powerScaleFactorDataPoint;
    powerScaleFactorDataPoint.setName("DCW_SF");
    powerScaleFactorDataPoint.setLabel("Power Scale Factor");
    powerScaleFactorDataPoint.setSize(1);
    powerScaleFactorDataPoint.setAddressOffset(4);
    powerScaleFactorDataPoint.setBlockOffset(2);
    powerScaleFactorDataPoint.setSunSpecDataType("sunssf");
    powerScaleFactorDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(powerScaleFactorDataPoint.name(), powerScaleFactorDataPoint);

    SunSpecDataPoint energyScaleFactorDataPoint;
    energyScaleFactorDataPoint.setName("DCWH_SF");
    energyScaleFactorDataPoint.setLabel("Energy Scale Factor");
    energyScaleFactorDataPoint.setSize(1);
    energyScaleFactorDataPoint.setAddressOffset(5);
    energyScaleFactorDataPoint.setBlockOffset(3);
    energyScaleFactorDataPoint.setSunSpecDataType("sunssf");
    energyScaleFactorDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(energyScaleFactorDataPoint.name(), energyScaleFactorDataPoint);

    SunSpecDataPoint globalEventsDataPoint;
    globalEventsDataPoint.setName("Evt");
    globalEventsDataPoint.setLabel("Global Events");
    globalEventsDataPoint.setSize(2);
    globalEventsDataPoint.setAddressOffset(6);
    globalEventsDataPoint.setBlockOffset(4);
    globalEventsDataPoint.setSunSpecDataType("bitfield32");
    globalEventsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(globalEventsDataPoint.name(), globalEventsDataPoint);

    SunSpecDataPoint numberOfModulesDataPoint;
    numberOfModulesDataPoint.setName("N");
    numberOfModulesDataPoint.setLabel("Number of Modules");
    numberOfModulesDataPoint.setSize(1);
    numberOfModulesDataPoint.setAddressOffset(8);
    numberOfModulesDataPoint.setBlockOffset(6);
    numberOfModulesDataPoint.setSunSpecDataType("count");
    numberOfModulesDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(numberOfModulesDataPoint.name(), numberOfModulesDataPoint);

    SunSpecDataPoint timestampPeriodDataPoint;
    timestampPeriodDataPoint.setName("TmsPer");
    timestampPeriodDataPoint.setLabel("Timestamp Period");
    timestampPeriodDataPoint.setSize(1);
    timestampPeriodDataPoint.setAddressOffset(9);
    timestampPeriodDataPoint.setBlockOffset(7);
    timestampPeriodDataPoint.setSunSpecDataType("uint16");
    timestampPeriodDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(timestampPeriodDataPoint.name(), timestampPeriodDataPoint);

}

void SunSpecMpptModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("DCA_SF").isValid())
        m_currentScaleFactor = m_dataPoints.value("DCA_SF").toInt16();

    if (m_dataPoints.value("DCV_SF").isValid())
        m_voltageScaleFactor = m_dataPoints.value("DCV_SF").toInt16();

    if (m_dataPoints.value("DCW_SF").isValid())
        m_powerScaleFactor = m_dataPoints.value("DCW_SF").toInt16();

    if (m_dataPoints.value("DCWH_SF").isValid())
        m_energyScaleFactor = m_dataPoints.value("DCWH_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("DCA_SF").isValid())
        m_currentScaleFactor = m_dataPoints.value("DCA_SF").toInt16();

    if (m_dataPoints.value("DCV_SF").isValid())
        m_voltageScaleFactor = m_dataPoints.value("DCV_SF").toInt16();

    if (m_dataPoints.value("DCW_SF").isValid())
        m_powerScaleFactor = m_dataPoints.value("DCW_SF").toInt16();

    if (m_dataPoints.value("DCWH_SF").isValid())
        m_energyScaleFactor = m_dataPoints.value("DCWH_SF").toInt16();

    if (m_dataPoints.value("Evt").isValid())
        m_globalEvents = static_cast<EvtFlags>(m_dataPoints.value("Evt").toUInt32());

    if (m_dataPoints.value("N").isValid())
        m_numberOfModules = m_dataPoints.value("N").toUInt16();

    if (m_dataPoints.value("TmsPer").isValid())
        m_timestampPeriod = m_dataPoints.value("TmsPer").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

void SunSpecMpptModel::setupRepeatingBlocks()
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
                                            << "), extra bytes:"
                                            << repeatingBlocksDataSize % m_repeatingBlockLength;
    }
    const auto numberOfBlocks = repeatingBlocksDataSize / m_repeatingBlockLength;
    const auto repeatingBlocksOffset = m_fixedBlockLength + headerLength;
    for (int i = 0; i < numberOfBlocks; ++i) {
        const auto blockStartRegister = static_cast<quint16>(modbusStartRegister() + repeatingBlocksOffset + m_repeatingBlockLength * i);
        const auto block = new SunSpecMpptModelRepeatingBlock(i, m_repeatingBlockLength, blockStartRegister, this);
        m_repeatingBlocks.append(block);
    }
}

QDebug operator<<(QDebug debug, SunSpecMpptModel *model)
{
    debug.nospace().noquote() << "SunSpecMpptModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "-->";
    if (model->dataPoints().value("Evt").isValid()) {
        debug.nospace().noquote() << model->globalEvents() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("N") << "-->";
    if (model->dataPoints().value("N").isValid()) {
        debug.nospace().noquote() << model->numberOfModules() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmsPer") << "-->";
    if (model->dataPoints().value("TmsPer").isValid()) {
        debug.nospace().noquote() << model->timestampPeriod() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
