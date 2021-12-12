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

#include "sunspeclithiumionstringmodel.h"
#include "sunspecconnection.h"

SunSpecLithiumIonStringModelRepeatingBlock::SunSpecLithiumIonStringModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLithiumIonStringModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecLithiumIonStringModelRepeatingBlock::name() const
{
    return "lithium_ion_string_module";
}

SunSpecLithiumIonStringModel *SunSpecLithiumIonStringModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecLithiumIonStringModelRepeatingBlock::moduleCellCount() const
{
    return m_moduleCellCount;
}
float SunSpecLithiumIonStringModelRepeatingBlock::moduleSoC() const
{
    return m_moduleSoC;
}
float SunSpecLithiumIonStringModelRepeatingBlock::moduleSoH() const
{
    return m_moduleSoH;
}
float SunSpecLithiumIonStringModelRepeatingBlock::maxCellVoltage() const
{
    return m_maxCellVoltage;
}
quint16 SunSpecLithiumIonStringModelRepeatingBlock::maxCellVoltageCell() const
{
    return m_maxCellVoltageCell;
}
float SunSpecLithiumIonStringModelRepeatingBlock::minCellVoltage() const
{
    return m_minCellVoltage;
}
float SunSpecLithiumIonStringModelRepeatingBlock::minCellVoltageCell() const
{
    return m_minCellVoltageCell;
}
float SunSpecLithiumIonStringModelRepeatingBlock::averageCellVoltage() const
{
    return m_averageCellVoltage;
}
float SunSpecLithiumIonStringModelRepeatingBlock::maxCellTemperature() const
{
    return m_maxCellTemperature;
}
quint16 SunSpecLithiumIonStringModelRepeatingBlock::maxCellTemperatureCell() const
{
    return m_maxCellTemperatureCell;
}
float SunSpecLithiumIonStringModelRepeatingBlock::minCellTemperature() const
{
    return m_minCellTemperature;
}
quint16 SunSpecLithiumIonStringModelRepeatingBlock::minCellTemperatureCell() const
{
    return m_minCellTemperatureCell;
}
float SunSpecLithiumIonStringModelRepeatingBlock::averageCellTemperature() const
{
    return m_averageCellTemperature;
}
quint16 SunSpecLithiumIonStringModelRepeatingBlock::pad5() const
{
    return m_pad5;
}
quint16 SunSpecLithiumIonStringModelRepeatingBlock::pad6() const
{
    return m_pad6;
}
quint16 SunSpecLithiumIonStringModelRepeatingBlock::pad7() const
{
    return m_pad7;
}

void SunSpecLithiumIonStringModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint moduleCellCountDataPoint;
    moduleCellCountDataPoint.setName("ModNCell");
    moduleCellCountDataPoint.setLabel("Module Cell Count");
    moduleCellCountDataPoint.setDescription("Count of all cells in the module.");
    moduleCellCountDataPoint.setMandatory(true);
    moduleCellCountDataPoint.setSize(1);
    moduleCellCountDataPoint.setAddressOffset(0);
    moduleCellCountDataPoint.setSunSpecDataType("uint16");
    moduleCellCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleCellCountDataPoint.name(), moduleCellCountDataPoint);

    SunSpecDataPoint moduleSoCDataPoint;
    moduleSoCDataPoint.setName("ModSoC");
    moduleSoCDataPoint.setLabel("Module SoC");
    moduleSoCDataPoint.setDescription("Module state of charge, expressed as a percentage.");
    moduleSoCDataPoint.setUnits("%");
    moduleSoCDataPoint.setSize(1);
    moduleSoCDataPoint.setAddressOffset(1);
    moduleSoCDataPoint.setScaleFactorName("SoC_SF");
    moduleSoCDataPoint.setSunSpecDataType("uint16");
    moduleSoCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleSoCDataPoint.name(), moduleSoCDataPoint);

    SunSpecDataPoint moduleSoHDataPoint;
    moduleSoHDataPoint.setName("ModSoH");
    moduleSoHDataPoint.setLabel("Module SoH");
    moduleSoHDataPoint.setDescription("Module state of health, expressed as a percentage.");
    moduleSoHDataPoint.setUnits("%");
    moduleSoHDataPoint.setSize(1);
    moduleSoHDataPoint.setAddressOffset(2);
    moduleSoHDataPoint.setBlockOffset(0);
    moduleSoHDataPoint.setScaleFactorName("SoH_SF");
    moduleSoHDataPoint.setSunSpecDataType("uint16");
    moduleSoHDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleSoHDataPoint.name(), moduleSoHDataPoint);

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("ModCellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the module.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setMandatory(true);
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setAddressOffset(3);
    maxCellVoltageDataPoint.setBlockOffset(1);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageCellDataPoint;
    maxCellVoltageCellDataPoint.setName("ModCellVMaxCell");
    maxCellVoltageCellDataPoint.setLabel("Max Cell Voltage Cell");
    maxCellVoltageCellDataPoint.setDescription("Cell with maximum voltage.");
    maxCellVoltageCellDataPoint.setSize(1);
    maxCellVoltageCellDataPoint.setAddressOffset(4);
    maxCellVoltageCellDataPoint.setBlockOffset(2);
    maxCellVoltageCellDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageCellDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageCellDataPoint.name(), maxCellVoltageCellDataPoint);

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("ModCellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the module.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setMandatory(true);
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setAddressOffset(5);
    minCellVoltageDataPoint.setBlockOffset(3);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setSunSpecDataType("uint16");
    minCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageCellDataPoint;
    minCellVoltageCellDataPoint.setName("ModCellVMinCell");
    minCellVoltageCellDataPoint.setLabel("Min Cell Voltage Cell");
    minCellVoltageCellDataPoint.setDescription("Cell with minimum voltage.");
    minCellVoltageCellDataPoint.setUnits("V");
    minCellVoltageCellDataPoint.setSize(1);
    minCellVoltageCellDataPoint.setAddressOffset(6);
    minCellVoltageCellDataPoint.setBlockOffset(4);
    minCellVoltageCellDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageCellDataPoint.setSunSpecDataType("uint16");
    minCellVoltageCellDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageCellDataPoint.name(), minCellVoltageCellDataPoint);

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("ModCellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in the module.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setMandatory(true);
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setAddressOffset(7);
    averageCellVoltageDataPoint.setBlockOffset(5);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setSunSpecDataType("uint16");
    averageCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageCellVoltageDataPoint.name(), averageCellVoltageDataPoint);

    SunSpecDataPoint maxCellTemperatureDataPoint;
    maxCellTemperatureDataPoint.setName("ModCellTmpMax");
    maxCellTemperatureDataPoint.setLabel("Max Cell Temperature");
    maxCellTemperatureDataPoint.setDescription("Maximum temperature for all cells in the module.");
    maxCellTemperatureDataPoint.setUnits("C");
    maxCellTemperatureDataPoint.setMandatory(true);
    maxCellTemperatureDataPoint.setSize(1);
    maxCellTemperatureDataPoint.setAddressOffset(8);
    maxCellTemperatureDataPoint.setBlockOffset(6);
    maxCellTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    maxCellTemperatureDataPoint.setSunSpecDataType("int16");
    maxCellTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellTemperatureDataPoint.name(), maxCellTemperatureDataPoint);

    SunSpecDataPoint maxCellTemperatureCellDataPoint;
    maxCellTemperatureCellDataPoint.setName("ModCellTmpMaxCell");
    maxCellTemperatureCellDataPoint.setLabel("Max Cell Temperature Cell");
    maxCellTemperatureCellDataPoint.setDescription("Cell with maximum temperature.");
    maxCellTemperatureCellDataPoint.setSize(1);
    maxCellTemperatureCellDataPoint.setAddressOffset(9);
    maxCellTemperatureCellDataPoint.setBlockOffset(7);
    maxCellTemperatureCellDataPoint.setSunSpecDataType("uint16");
    maxCellTemperatureCellDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellTemperatureCellDataPoint.name(), maxCellTemperatureCellDataPoint);

    SunSpecDataPoint minCellTemperatureDataPoint;
    minCellTemperatureDataPoint.setName("ModCellTmpMin");
    minCellTemperatureDataPoint.setLabel("Min Cell Temperature");
    minCellTemperatureDataPoint.setDescription("Minimum temperature for all cells in the module.");
    minCellTemperatureDataPoint.setUnits("C");
    minCellTemperatureDataPoint.setMandatory(true);
    minCellTemperatureDataPoint.setSize(1);
    minCellTemperatureDataPoint.setAddressOffset(10);
    minCellTemperatureDataPoint.setBlockOffset(8);
    minCellTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    minCellTemperatureDataPoint.setSunSpecDataType("int16");
    minCellTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellTemperatureDataPoint.name(), minCellTemperatureDataPoint);

    SunSpecDataPoint minCellTemperatureCellDataPoint;
    minCellTemperatureCellDataPoint.setName("ModCellTmpMinCell");
    minCellTemperatureCellDataPoint.setLabel("Min Cell Temperature Cell");
    minCellTemperatureCellDataPoint.setDescription("Cell with minimum temperature.");
    minCellTemperatureCellDataPoint.setSize(1);
    minCellTemperatureCellDataPoint.setAddressOffset(11);
    minCellTemperatureCellDataPoint.setBlockOffset(9);
    minCellTemperatureCellDataPoint.setSunSpecDataType("uint16");
    minCellTemperatureCellDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellTemperatureCellDataPoint.name(), minCellTemperatureCellDataPoint);

    SunSpecDataPoint averageCellTemperatureDataPoint;
    averageCellTemperatureDataPoint.setName("ModCellTmpAvg");
    averageCellTemperatureDataPoint.setLabel("Average Cell Temperature");
    averageCellTemperatureDataPoint.setDescription("Average temperature for all cells in the module.");
    averageCellTemperatureDataPoint.setUnits("C");
    averageCellTemperatureDataPoint.setMandatory(true);
    averageCellTemperatureDataPoint.setSize(1);
    averageCellTemperatureDataPoint.setAddressOffset(12);
    averageCellTemperatureDataPoint.setBlockOffset(10);
    averageCellTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    averageCellTemperatureDataPoint.setSunSpecDataType("int16");
    averageCellTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageCellTemperatureDataPoint.name(), averageCellTemperatureDataPoint);

    SunSpecDataPoint pad5DataPoint;
    pad5DataPoint.setName("Pad5");
    pad5DataPoint.setLabel("Pad");
    pad5DataPoint.setDescription("Pad register.");
    pad5DataPoint.setMandatory(true);
    pad5DataPoint.setSize(1);
    pad5DataPoint.setAddressOffset(13);
    pad5DataPoint.setBlockOffset(11);
    pad5DataPoint.setSunSpecDataType("pad");
    pad5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad5DataPoint.name(), pad5DataPoint);

    SunSpecDataPoint pad6DataPoint;
    pad6DataPoint.setName("Pad6");
    pad6DataPoint.setLabel("Pad");
    pad6DataPoint.setDescription("Pad register.");
    pad6DataPoint.setMandatory(true);
    pad6DataPoint.setSize(1);
    pad6DataPoint.setAddressOffset(14);
    pad6DataPoint.setBlockOffset(12);
    pad6DataPoint.setSunSpecDataType("pad");
    pad6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad6DataPoint.name(), pad6DataPoint);

    SunSpecDataPoint pad7DataPoint;
    pad7DataPoint.setName("Pad7");
    pad7DataPoint.setLabel("Pad");
    pad7DataPoint.setDescription("Pad register.");
    pad7DataPoint.setMandatory(true);
    pad7DataPoint.setSize(1);
    pad7DataPoint.setAddressOffset(15);
    pad7DataPoint.setBlockOffset(13);
    pad7DataPoint.setSunSpecDataType("pad");
    pad7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad7DataPoint.name(), pad7DataPoint);

}

void SunSpecLithiumIonStringModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("ModNCell").isValid())
        m_moduleCellCount = m_dataPoints.value("ModNCell").toUInt16();

    if (m_dataPoints.value("ModSoC").isValid())
        m_moduleSoC = m_dataPoints.value("ModSoC").toFloatWithSSF(m_parentModel->soC_SF());

    if (m_dataPoints.value("ModSoH").isValid())
        m_moduleSoH = m_dataPoints.value("ModSoH").toFloatWithSSF(m_parentModel->soH_SF());

    if (m_dataPoints.value("ModCellVMax").isValid())
        m_maxCellVoltage = m_dataPoints.value("ModCellVMax").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("ModCellVMaxCell").isValid())
        m_maxCellVoltageCell = m_dataPoints.value("ModCellVMaxCell").toUInt16();

    if (m_dataPoints.value("ModCellVMin").isValid())
        m_minCellVoltage = m_dataPoints.value("ModCellVMin").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("ModCellVMinCell").isValid())
        m_minCellVoltageCell = m_dataPoints.value("ModCellVMinCell").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("ModCellVAvg").isValid())
        m_averageCellVoltage = m_dataPoints.value("ModCellVAvg").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("ModCellTmpMax").isValid())
        m_maxCellTemperature = m_dataPoints.value("ModCellTmpMax").toFloatWithSSF(m_parentModel->modTmp_SF());

    if (m_dataPoints.value("ModCellTmpMaxCell").isValid())
        m_maxCellTemperatureCell = m_dataPoints.value("ModCellTmpMaxCell").toUInt16();

    if (m_dataPoints.value("ModCellTmpMin").isValid())
        m_minCellTemperature = m_dataPoints.value("ModCellTmpMin").toFloatWithSSF(m_parentModel->modTmp_SF());

    if (m_dataPoints.value("ModCellTmpMinCell").isValid())
        m_minCellTemperatureCell = m_dataPoints.value("ModCellTmpMinCell").toUInt16();

    if (m_dataPoints.value("ModCellTmpAvg").isValid())
        m_averageCellTemperature = m_dataPoints.value("ModCellTmpAvg").toFloatWithSSF(m_parentModel->modTmp_SF());

    if (m_dataPoints.value("Pad5").isValid())
        m_pad5 = m_dataPoints.value("Pad5").toUInt16();

    if (m_dataPoints.value("Pad6").isValid())
        m_pad6 = m_dataPoints.value("Pad6").toUInt16();

    if (m_dataPoints.value("Pad7").isValid())
        m_pad7 = m_dataPoints.value("Pad7").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecLithiumIonStringModel::SunSpecLithiumIonStringModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 804, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecLithiumIonStringModel::~SunSpecLithiumIonStringModel()
{

}

QString SunSpecLithiumIonStringModel::name() const
{
    return "lithium_ion_string";
}

QString SunSpecLithiumIonStringModel::description() const
{
    return QString();
}

QString SunSpecLithiumIonStringModel::label() const
{
    return "Lithium-Ion String Model";
}

quint16 SunSpecLithiumIonStringModel::stringIndex() const
{
    return m_stringIndex;
}
quint16 SunSpecLithiumIonStringModel::moduleCount() const
{
    return m_moduleCount;
}
SunSpecLithiumIonStringModel::StFlags SunSpecLithiumIonStringModel::stringStatus() const
{
    return m_stringStatus;
}
SunSpecLithiumIonStringModel::Confail SunSpecLithiumIonStringModel::connectionFailureReason() const
{
    return m_connectionFailureReason;
}
quint16 SunSpecLithiumIonStringModel::stringCellBalancingCount() const
{
    return m_stringCellBalancingCount;
}
float SunSpecLithiumIonStringModel::stringStateOfCharge() const
{
    return m_stringStateOfCharge;
}
float SunSpecLithiumIonStringModel::stringDepthOfDischarge() const
{
    return m_stringDepthOfDischarge;
}
quint32 SunSpecLithiumIonStringModel::stringCycleCount() const
{
    return m_stringCycleCount;
}
float SunSpecLithiumIonStringModel::stringStateOfHealth() const
{
    return m_stringStateOfHealth;
}
float SunSpecLithiumIonStringModel::stringCurrent() const
{
    return m_stringCurrent;
}
float SunSpecLithiumIonStringModel::stringVoltage() const
{
    return m_stringVoltage;
}
float SunSpecLithiumIonStringModel::maxCellVoltage() const
{
    return m_maxCellVoltage;
}
quint16 SunSpecLithiumIonStringModel::maxCellVoltageModule() const
{
    return m_maxCellVoltageModule;
}
float SunSpecLithiumIonStringModel::minCellVoltage() const
{
    return m_minCellVoltage;
}
quint16 SunSpecLithiumIonStringModel::minCellVoltageModule() const
{
    return m_minCellVoltageModule;
}
float SunSpecLithiumIonStringModel::averageCellVoltage() const
{
    return m_averageCellVoltage;
}
float SunSpecLithiumIonStringModel::maxModuleTemperature() const
{
    return m_maxModuleTemperature;
}
quint16 SunSpecLithiumIonStringModel::maxModuleTemperatureModule() const
{
    return m_maxModuleTemperatureModule;
}
float SunSpecLithiumIonStringModel::minModuleTemperature() const
{
    return m_minModuleTemperature;
}
quint16 SunSpecLithiumIonStringModel::minModuleTemperatureModule() const
{
    return m_minModuleTemperatureModule;
}
float SunSpecLithiumIonStringModel::averageModuleTemperature() const
{
    return m_averageModuleTemperature;
}
quint16 SunSpecLithiumIonStringModel::pad1() const
{
    return m_pad1;
}
SunSpecLithiumIonStringModel::ConstFlags SunSpecLithiumIonStringModel::contactorStatus() const
{
    return m_contactorStatus;
}
SunSpecLithiumIonStringModel::Evt1Flags SunSpecLithiumIonStringModel::stringEvent1() const
{
    return m_stringEvent1;
}
quint32 SunSpecLithiumIonStringModel::stringEvent2() const
{
    return m_stringEvent2;
}
quint32 SunSpecLithiumIonStringModel::vendorEventBitfield1() const
{
    return m_vendorEventBitfield1;
}
quint32 SunSpecLithiumIonStringModel::vendorEventBitfield2() const
{
    return m_vendorEventBitfield2;
}
quint16 SunSpecLithiumIonStringModel::enableDisableString() const
{
    return m_enableDisableString;
}

QModbusReply *SunSpecLithiumIonStringModel::setEnableDisableString(quint16 enableDisableString)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("SetEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(enableDisableString));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecLithiumIonStringModel::Setcon SunSpecLithiumIonStringModel::connectDisconnectString() const
{
    return m_connectDisconnectString;
}

QModbusReply *SunSpecLithiumIonStringModel::setConnectDisconnectString(Setcon connectDisconnectString)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("SetCon");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(connectDisconnectString));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
qint16 SunSpecLithiumIonStringModel::soC_SF() const
{
    return m_soC_SF;
}
qint16 SunSpecLithiumIonStringModel::soH_SF() const
{
    return m_soH_SF;
}
qint16 SunSpecLithiumIonStringModel::doD_SF() const
{
    return m_doD_SF;
}
qint16 SunSpecLithiumIonStringModel::a_SF() const
{
    return m_a_SF;
}
qint16 SunSpecLithiumIonStringModel::v_SF() const
{
    return m_v_SF;
}
qint16 SunSpecLithiumIonStringModel::cellV_SF() const
{
    return m_cellV_SF;
}
qint16 SunSpecLithiumIonStringModel::modTmp_SF() const
{
    return m_modTmp_SF;
}
quint16 SunSpecLithiumIonStringModel::pad2() const
{
    return m_pad2;
}
quint16 SunSpecLithiumIonStringModel::pad3() const
{
    return m_pad3;
}
quint16 SunSpecLithiumIonStringModel::pad4() const
{
    return m_pad4;
}
void SunSpecLithiumIonStringModel::initDataPoints()
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

    SunSpecDataPoint stringIndexDataPoint;
    stringIndexDataPoint.setName("Idx");
    stringIndexDataPoint.setLabel("String Index");
    stringIndexDataPoint.setDescription("Index of the string within the bank.");
    stringIndexDataPoint.setMandatory(true);
    stringIndexDataPoint.setSize(1);
    stringIndexDataPoint.setAddressOffset(2);
    stringIndexDataPoint.setBlockOffset(0);
    stringIndexDataPoint.setSunSpecDataType("uint16");
    stringIndexDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringIndexDataPoint.name(), stringIndexDataPoint);

    SunSpecDataPoint moduleCountDataPoint;
    moduleCountDataPoint.setName("NMod");
    moduleCountDataPoint.setLabel("Module Count");
    moduleCountDataPoint.setDescription("Count of modules in the string.");
    moduleCountDataPoint.setMandatory(true);
    moduleCountDataPoint.setSize(1);
    moduleCountDataPoint.setAddressOffset(3);
    moduleCountDataPoint.setBlockOffset(1);
    moduleCountDataPoint.setSunSpecDataType("uint16");
    moduleCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleCountDataPoint.name(), moduleCountDataPoint);

    SunSpecDataPoint stringStatusDataPoint;
    stringStatusDataPoint.setName("St");
    stringStatusDataPoint.setLabel("String Status");
    stringStatusDataPoint.setDescription("Current status of the string.");
    stringStatusDataPoint.setMandatory(true);
    stringStatusDataPoint.setSize(2);
    stringStatusDataPoint.setAddressOffset(4);
    stringStatusDataPoint.setBlockOffset(2);
    stringStatusDataPoint.setSunSpecDataType("bitfield32");
    stringStatusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringStatusDataPoint.name(), stringStatusDataPoint);

    SunSpecDataPoint connectionFailureReasonDataPoint;
    connectionFailureReasonDataPoint.setName("ConFail");
    connectionFailureReasonDataPoint.setLabel("Connection Failure Reason");
    connectionFailureReasonDataPoint.setSize(1);
    connectionFailureReasonDataPoint.setAddressOffset(6);
    connectionFailureReasonDataPoint.setBlockOffset(4);
    connectionFailureReasonDataPoint.setSunSpecDataType("enum16");
    connectionFailureReasonDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connectionFailureReasonDataPoint.name(), connectionFailureReasonDataPoint);

    SunSpecDataPoint stringCellBalancingCountDataPoint;
    stringCellBalancingCountDataPoint.setName("NCellBal");
    stringCellBalancingCountDataPoint.setLabel("String Cell Balancing Count");
    stringCellBalancingCountDataPoint.setDescription("Number of cells currently being balanced in the string.");
    stringCellBalancingCountDataPoint.setSize(1);
    stringCellBalancingCountDataPoint.setAddressOffset(7);
    stringCellBalancingCountDataPoint.setBlockOffset(5);
    stringCellBalancingCountDataPoint.setSunSpecDataType("uint16");
    stringCellBalancingCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringCellBalancingCountDataPoint.name(), stringCellBalancingCountDataPoint);

    SunSpecDataPoint stringStateOfChargeDataPoint;
    stringStateOfChargeDataPoint.setName("SoC");
    stringStateOfChargeDataPoint.setLabel("String State of Charge");
    stringStateOfChargeDataPoint.setDescription("Battery string state of charge, expressed as a percentage.");
    stringStateOfChargeDataPoint.setUnits("%");
    stringStateOfChargeDataPoint.setMandatory(true);
    stringStateOfChargeDataPoint.setSize(1);
    stringStateOfChargeDataPoint.setAddressOffset(8);
    stringStateOfChargeDataPoint.setBlockOffset(6);
    stringStateOfChargeDataPoint.setScaleFactorName("SoC_SF");
    stringStateOfChargeDataPoint.setSunSpecDataType("uint16");
    stringStateOfChargeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringStateOfChargeDataPoint.name(), stringStateOfChargeDataPoint);

    SunSpecDataPoint stringDepthOfDischargeDataPoint;
    stringDepthOfDischargeDataPoint.setName("DoD");
    stringDepthOfDischargeDataPoint.setLabel("String Depth of Discharge");
    stringDepthOfDischargeDataPoint.setDescription("Depth of discharge for the string, expressed as a percentage.");
    stringDepthOfDischargeDataPoint.setUnits("%");
    stringDepthOfDischargeDataPoint.setSize(1);
    stringDepthOfDischargeDataPoint.setAddressOffset(9);
    stringDepthOfDischargeDataPoint.setBlockOffset(7);
    stringDepthOfDischargeDataPoint.setScaleFactorName("DoD_SF");
    stringDepthOfDischargeDataPoint.setSunSpecDataType("uint16");
    stringDepthOfDischargeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringDepthOfDischargeDataPoint.name(), stringDepthOfDischargeDataPoint);

    SunSpecDataPoint stringCycleCountDataPoint;
    stringCycleCountDataPoint.setName("NCyc");
    stringCycleCountDataPoint.setLabel("String Cycle Count");
    stringCycleCountDataPoint.setDescription("Number of discharge cycles executed upon the string.");
    stringCycleCountDataPoint.setSize(2);
    stringCycleCountDataPoint.setAddressOffset(10);
    stringCycleCountDataPoint.setBlockOffset(8);
    stringCycleCountDataPoint.setSunSpecDataType("uint32");
    stringCycleCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringCycleCountDataPoint.name(), stringCycleCountDataPoint);

    SunSpecDataPoint stringStateOfHealthDataPoint;
    stringStateOfHealthDataPoint.setName("SoH");
    stringStateOfHealthDataPoint.setLabel("String State of Health");
    stringStateOfHealthDataPoint.setDescription("Battery string state of health, expressed as a percentage.");
    stringStateOfHealthDataPoint.setUnits("%");
    stringStateOfHealthDataPoint.setSize(1);
    stringStateOfHealthDataPoint.setAddressOffset(12);
    stringStateOfHealthDataPoint.setBlockOffset(10);
    stringStateOfHealthDataPoint.setScaleFactorName("SoH_SF");
    stringStateOfHealthDataPoint.setSunSpecDataType("uint16");
    stringStateOfHealthDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringStateOfHealthDataPoint.name(), stringStateOfHealthDataPoint);

    SunSpecDataPoint stringCurrentDataPoint;
    stringCurrentDataPoint.setName("A");
    stringCurrentDataPoint.setLabel("String Current");
    stringCurrentDataPoint.setDescription("String current measurement.");
    stringCurrentDataPoint.setUnits("A");
    stringCurrentDataPoint.setMandatory(true);
    stringCurrentDataPoint.setSize(1);
    stringCurrentDataPoint.setAddressOffset(13);
    stringCurrentDataPoint.setBlockOffset(11);
    stringCurrentDataPoint.setScaleFactorName("A_SF");
    stringCurrentDataPoint.setSunSpecDataType("int16");
    stringCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringCurrentDataPoint.name(), stringCurrentDataPoint);

    SunSpecDataPoint stringVoltageDataPoint;
    stringVoltageDataPoint.setName("V");
    stringVoltageDataPoint.setLabel("String Voltage");
    stringVoltageDataPoint.setDescription("String voltage measurement.");
    stringVoltageDataPoint.setUnits("V");
    stringVoltageDataPoint.setSize(1);
    stringVoltageDataPoint.setAddressOffset(14);
    stringVoltageDataPoint.setBlockOffset(12);
    stringVoltageDataPoint.setScaleFactorName("V_SF");
    stringVoltageDataPoint.setSunSpecDataType("uint16");
    stringVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringVoltageDataPoint.name(), stringVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("CellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the string.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setMandatory(true);
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setAddressOffset(15);
    maxCellVoltageDataPoint.setBlockOffset(13);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageModuleDataPoint;
    maxCellVoltageModuleDataPoint.setName("CellVMaxMod");
    maxCellVoltageModuleDataPoint.setLabel("Max Cell Voltage Module");
    maxCellVoltageModuleDataPoint.setDescription("Module containing the cell with maximum cell voltage.");
    maxCellVoltageModuleDataPoint.setSize(1);
    maxCellVoltageModuleDataPoint.setAddressOffset(16);
    maxCellVoltageModuleDataPoint.setBlockOffset(14);
    maxCellVoltageModuleDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageModuleDataPoint.name(), maxCellVoltageModuleDataPoint);

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("CellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the string.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setMandatory(true);
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setAddressOffset(17);
    minCellVoltageDataPoint.setBlockOffset(15);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setSunSpecDataType("uint16");
    minCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageModuleDataPoint;
    minCellVoltageModuleDataPoint.setName("CellVMinMod");
    minCellVoltageModuleDataPoint.setLabel("Min Cell Voltage Module");
    minCellVoltageModuleDataPoint.setDescription("Module containing the cell with minimum cell voltage.");
    minCellVoltageModuleDataPoint.setSize(1);
    minCellVoltageModuleDataPoint.setAddressOffset(18);
    minCellVoltageModuleDataPoint.setBlockOffset(16);
    minCellVoltageModuleDataPoint.setSunSpecDataType("uint16");
    minCellVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageModuleDataPoint.name(), minCellVoltageModuleDataPoint);

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("CellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in the string.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setMandatory(true);
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setAddressOffset(19);
    averageCellVoltageDataPoint.setBlockOffset(17);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setSunSpecDataType("uint16");
    averageCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageCellVoltageDataPoint.name(), averageCellVoltageDataPoint);

    SunSpecDataPoint maxModuleTemperatureDataPoint;
    maxModuleTemperatureDataPoint.setName("ModTmpMax");
    maxModuleTemperatureDataPoint.setLabel("Max Module Temperature");
    maxModuleTemperatureDataPoint.setDescription("Maximum temperature for all modules in the string.");
    maxModuleTemperatureDataPoint.setUnits("C");
    maxModuleTemperatureDataPoint.setMandatory(true);
    maxModuleTemperatureDataPoint.setSize(1);
    maxModuleTemperatureDataPoint.setAddressOffset(20);
    maxModuleTemperatureDataPoint.setBlockOffset(18);
    maxModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    maxModuleTemperatureDataPoint.setSunSpecDataType("int16");
    maxModuleTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxModuleTemperatureDataPoint.name(), maxModuleTemperatureDataPoint);

    SunSpecDataPoint maxModuleTemperatureModuleDataPoint;
    maxModuleTemperatureModuleDataPoint.setName("ModTmpMaxMod");
    maxModuleTemperatureModuleDataPoint.setLabel("Max Module Temperature Module");
    maxModuleTemperatureModuleDataPoint.setDescription("Module with the maximum temperature.");
    maxModuleTemperatureModuleDataPoint.setMandatory(true);
    maxModuleTemperatureModuleDataPoint.setSize(1);
    maxModuleTemperatureModuleDataPoint.setAddressOffset(21);
    maxModuleTemperatureModuleDataPoint.setBlockOffset(19);
    maxModuleTemperatureModuleDataPoint.setSunSpecDataType("uint16");
    maxModuleTemperatureModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxModuleTemperatureModuleDataPoint.name(), maxModuleTemperatureModuleDataPoint);

    SunSpecDataPoint minModuleTemperatureDataPoint;
    minModuleTemperatureDataPoint.setName("ModTmpMin");
    minModuleTemperatureDataPoint.setLabel("Min Module Temperature");
    minModuleTemperatureDataPoint.setDescription("Minimum temperature for all modules in the string.");
    minModuleTemperatureDataPoint.setUnits("C");
    minModuleTemperatureDataPoint.setMandatory(true);
    minModuleTemperatureDataPoint.setSize(1);
    minModuleTemperatureDataPoint.setAddressOffset(22);
    minModuleTemperatureDataPoint.setBlockOffset(20);
    minModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    minModuleTemperatureDataPoint.setSunSpecDataType("int16");
    minModuleTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minModuleTemperatureDataPoint.name(), minModuleTemperatureDataPoint);

    SunSpecDataPoint minModuleTemperatureModuleDataPoint;
    minModuleTemperatureModuleDataPoint.setName("ModTmpMinMod");
    minModuleTemperatureModuleDataPoint.setLabel("Min Module Temperature Module");
    minModuleTemperatureModuleDataPoint.setDescription("Module with the minimum temperature.");
    minModuleTemperatureModuleDataPoint.setMandatory(true);
    minModuleTemperatureModuleDataPoint.setSize(1);
    minModuleTemperatureModuleDataPoint.setAddressOffset(23);
    minModuleTemperatureModuleDataPoint.setBlockOffset(21);
    minModuleTemperatureModuleDataPoint.setSunSpecDataType("uint16");
    minModuleTemperatureModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minModuleTemperatureModuleDataPoint.name(), minModuleTemperatureModuleDataPoint);

    SunSpecDataPoint averageModuleTemperatureDataPoint;
    averageModuleTemperatureDataPoint.setName("ModTmpAvg");
    averageModuleTemperatureDataPoint.setLabel("Average Module Temperature");
    averageModuleTemperatureDataPoint.setDescription("Average temperature for all modules in the string.");
    averageModuleTemperatureDataPoint.setUnits("C");
    averageModuleTemperatureDataPoint.setMandatory(true);
    averageModuleTemperatureDataPoint.setSize(1);
    averageModuleTemperatureDataPoint.setAddressOffset(24);
    averageModuleTemperatureDataPoint.setBlockOffset(22);
    averageModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    averageModuleTemperatureDataPoint.setSunSpecDataType("int16");
    averageModuleTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageModuleTemperatureDataPoint.name(), averageModuleTemperatureDataPoint);

    SunSpecDataPoint pad1DataPoint;
    pad1DataPoint.setName("Pad1");
    pad1DataPoint.setLabel("Pad");
    pad1DataPoint.setDescription("Pad register.");
    pad1DataPoint.setMandatory(true);
    pad1DataPoint.setSize(1);
    pad1DataPoint.setAddressOffset(25);
    pad1DataPoint.setBlockOffset(23);
    pad1DataPoint.setSunSpecDataType("pad");
    pad1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad1DataPoint.name(), pad1DataPoint);

    SunSpecDataPoint contactorStatusDataPoint;
    contactorStatusDataPoint.setName("ConSt");
    contactorStatusDataPoint.setLabel("Contactor Status");
    contactorStatusDataPoint.setDescription("Status of the contactor(s) for the string.");
    contactorStatusDataPoint.setSize(2);
    contactorStatusDataPoint.setAddressOffset(26);
    contactorStatusDataPoint.setBlockOffset(24);
    contactorStatusDataPoint.setSunSpecDataType("bitfield32");
    contactorStatusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(contactorStatusDataPoint.name(), contactorStatusDataPoint);

    SunSpecDataPoint stringEvent1DataPoint;
    stringEvent1DataPoint.setName("Evt1");
    stringEvent1DataPoint.setLabel("String Event 1");
    stringEvent1DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent1DataPoint.setMandatory(true);
    stringEvent1DataPoint.setSize(2);
    stringEvent1DataPoint.setAddressOffset(28);
    stringEvent1DataPoint.setBlockOffset(26);
    stringEvent1DataPoint.setSunSpecDataType("bitfield32");
    stringEvent1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringEvent1DataPoint.name(), stringEvent1DataPoint);

    SunSpecDataPoint stringEvent2DataPoint;
    stringEvent2DataPoint.setName("Evt2");
    stringEvent2DataPoint.setLabel("String Event 2");
    stringEvent2DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent2DataPoint.setSize(2);
    stringEvent2DataPoint.setAddressOffset(30);
    stringEvent2DataPoint.setBlockOffset(28);
    stringEvent2DataPoint.setSunSpecDataType("bitfield32");
    stringEvent2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringEvent2DataPoint.name(), stringEvent2DataPoint);

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setAddressOffset(32);
    vendorEventBitfield1DataPoint.setBlockOffset(30);
    vendorEventBitfield1DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setAddressOffset(34);
    vendorEventBitfield2DataPoint.setBlockOffset(32);
    vendorEventBitfield2DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

    SunSpecDataPoint enableDisableStringDataPoint;
    enableDisableStringDataPoint.setName("SetEna");
    enableDisableStringDataPoint.setLabel("Enable/Disable String");
    enableDisableStringDataPoint.setDescription("Enables and disables the string.  Should reset to 0 upon completion.");
    enableDisableStringDataPoint.setSize(1);
    enableDisableStringDataPoint.setAddressOffset(36);
    enableDisableStringDataPoint.setBlockOffset(34);
    enableDisableStringDataPoint.setSunSpecDataType("enum16");
    enableDisableStringDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    enableDisableStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(enableDisableStringDataPoint.name(), enableDisableStringDataPoint);

    SunSpecDataPoint connectDisconnectStringDataPoint;
    connectDisconnectStringDataPoint.setName("SetCon");
    connectDisconnectStringDataPoint.setLabel("Connect/Disconnect String");
    connectDisconnectStringDataPoint.setDescription("Connects and disconnects the string.");
    connectDisconnectStringDataPoint.setSize(1);
    connectDisconnectStringDataPoint.setAddressOffset(37);
    connectDisconnectStringDataPoint.setBlockOffset(35);
    connectDisconnectStringDataPoint.setSunSpecDataType("enum16");
    connectDisconnectStringDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    connectDisconnectStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connectDisconnectStringDataPoint.name(), connectDisconnectStringDataPoint);

    SunSpecDataPoint soC_SFDataPoint;
    soC_SFDataPoint.setName("SoC_SF");
    soC_SFDataPoint.setDescription("Scale factor for string state of charge.");
    soC_SFDataPoint.setMandatory(true);
    soC_SFDataPoint.setSize(1);
    soC_SFDataPoint.setAddressOffset(38);
    soC_SFDataPoint.setBlockOffset(36);
    soC_SFDataPoint.setSunSpecDataType("sunssf");
    soC_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soC_SFDataPoint.name(), soC_SFDataPoint);

    SunSpecDataPoint soH_SFDataPoint;
    soH_SFDataPoint.setName("SoH_SF");
    soH_SFDataPoint.setDescription("Scale factor for string state of health.");
    soH_SFDataPoint.setSize(1);
    soH_SFDataPoint.setAddressOffset(39);
    soH_SFDataPoint.setBlockOffset(37);
    soH_SFDataPoint.setSunSpecDataType("sunssf");
    soH_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soH_SFDataPoint.name(), soH_SFDataPoint);

    SunSpecDataPoint doD_SFDataPoint;
    doD_SFDataPoint.setName("DoD_SF");
    doD_SFDataPoint.setDescription("Scale factor for string depth of discharge.");
    doD_SFDataPoint.setSize(1);
    doD_SFDataPoint.setAddressOffset(40);
    doD_SFDataPoint.setBlockOffset(38);
    doD_SFDataPoint.setSunSpecDataType("sunssf");
    doD_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(doD_SFDataPoint.name(), doD_SFDataPoint);

    SunSpecDataPoint a_SFDataPoint;
    a_SFDataPoint.setName("A_SF");
    a_SFDataPoint.setDescription("Scale factor for string current.");
    a_SFDataPoint.setMandatory(true);
    a_SFDataPoint.setSize(1);
    a_SFDataPoint.setAddressOffset(41);
    a_SFDataPoint.setBlockOffset(39);
    a_SFDataPoint.setSunSpecDataType("sunssf");
    a_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(a_SFDataPoint.name(), a_SFDataPoint);

    SunSpecDataPoint v_SFDataPoint;
    v_SFDataPoint.setName("V_SF");
    v_SFDataPoint.setDescription("Scale factor for string voltage.");
    v_SFDataPoint.setSize(1);
    v_SFDataPoint.setAddressOffset(42);
    v_SFDataPoint.setBlockOffset(40);
    v_SFDataPoint.setSunSpecDataType("sunssf");
    v_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v_SFDataPoint.name(), v_SFDataPoint);

    SunSpecDataPoint cellV_SFDataPoint;
    cellV_SFDataPoint.setName("CellV_SF");
    cellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    cellV_SFDataPoint.setMandatory(true);
    cellV_SFDataPoint.setSize(1);
    cellV_SFDataPoint.setAddressOffset(43);
    cellV_SFDataPoint.setBlockOffset(41);
    cellV_SFDataPoint.setSunSpecDataType("sunssf");
    cellV_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cellV_SFDataPoint.name(), cellV_SFDataPoint);

    SunSpecDataPoint modTmp_SFDataPoint;
    modTmp_SFDataPoint.setName("ModTmp_SF");
    modTmp_SFDataPoint.setDescription("Scale factor for module temperature.");
    modTmp_SFDataPoint.setMandatory(true);
    modTmp_SFDataPoint.setSize(1);
    modTmp_SFDataPoint.setAddressOffset(44);
    modTmp_SFDataPoint.setBlockOffset(42);
    modTmp_SFDataPoint.setSunSpecDataType("sunssf");
    modTmp_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modTmp_SFDataPoint.name(), modTmp_SFDataPoint);

    SunSpecDataPoint pad2DataPoint;
    pad2DataPoint.setName("Pad2");
    pad2DataPoint.setLabel("Pad2");
    pad2DataPoint.setDescription("Pad register.");
    pad2DataPoint.setMandatory(true);
    pad2DataPoint.setSize(1);
    pad2DataPoint.setAddressOffset(45);
    pad2DataPoint.setBlockOffset(43);
    pad2DataPoint.setSunSpecDataType("pad");
    pad2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad2DataPoint.name(), pad2DataPoint);

    SunSpecDataPoint pad3DataPoint;
    pad3DataPoint.setName("Pad3");
    pad3DataPoint.setLabel("Pad3");
    pad3DataPoint.setDescription("Pad register.");
    pad3DataPoint.setMandatory(true);
    pad3DataPoint.setSize(1);
    pad3DataPoint.setAddressOffset(46);
    pad3DataPoint.setBlockOffset(44);
    pad3DataPoint.setSunSpecDataType("pad");
    pad3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad3DataPoint.name(), pad3DataPoint);

    SunSpecDataPoint pad4DataPoint;
    pad4DataPoint.setName("Pad4");
    pad4DataPoint.setLabel("Pad4");
    pad4DataPoint.setDescription("Pad register.");
    pad4DataPoint.setMandatory(true);
    pad4DataPoint.setSize(1);
    pad4DataPoint.setAddressOffset(47);
    pad4DataPoint.setBlockOffset(45);
    pad4DataPoint.setSunSpecDataType("pad");
    pad4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad4DataPoint.name(), pad4DataPoint);

}

void SunSpecLithiumIonStringModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("SoH_SF").isValid())
        m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();

    if (m_dataPoints.value("DoD_SF").isValid())
        m_doD_SF = m_dataPoints.value("DoD_SF").toInt16();

    if (m_dataPoints.value("A_SF").isValid())
        m_a_SF = m_dataPoints.value("A_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("ModTmp_SF").isValid())
        m_modTmp_SF = m_dataPoints.value("ModTmp_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("Idx").isValid())
        m_stringIndex = m_dataPoints.value("Idx").toUInt16();

    if (m_dataPoints.value("NMod").isValid())
        m_moduleCount = m_dataPoints.value("NMod").toUInt16();

    if (m_dataPoints.value("St").isValid())
        m_stringStatus = static_cast<StFlags>(m_dataPoints.value("St").toUInt32());

    if (m_dataPoints.value("ConFail").isValid())
        m_connectionFailureReason = static_cast<Confail>(m_dataPoints.value("ConFail").toUInt16());

    if (m_dataPoints.value("NCellBal").isValid())
        m_stringCellBalancingCount = m_dataPoints.value("NCellBal").toUInt16();

    if (m_dataPoints.value("SoC").isValid())
        m_stringStateOfCharge = m_dataPoints.value("SoC").toFloatWithSSF(m_soC_SF);

    if (m_dataPoints.value("DoD").isValid())
        m_stringDepthOfDischarge = m_dataPoints.value("DoD").toFloatWithSSF(m_doD_SF);

    if (m_dataPoints.value("NCyc").isValid())
        m_stringCycleCount = m_dataPoints.value("NCyc").toUInt32();

    if (m_dataPoints.value("SoH").isValid())
        m_stringStateOfHealth = m_dataPoints.value("SoH").toFloatWithSSF(m_soH_SF);

    if (m_dataPoints.value("A").isValid())
        m_stringCurrent = m_dataPoints.value("A").toFloatWithSSF(m_a_SF);

    if (m_dataPoints.value("V").isValid())
        m_stringVoltage = m_dataPoints.value("V").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("CellVMax").isValid())
        m_maxCellVoltage = m_dataPoints.value("CellVMax").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("CellVMaxMod").isValid())
        m_maxCellVoltageModule = m_dataPoints.value("CellVMaxMod").toUInt16();

    if (m_dataPoints.value("CellVMin").isValid())
        m_minCellVoltage = m_dataPoints.value("CellVMin").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("CellVMinMod").isValid())
        m_minCellVoltageModule = m_dataPoints.value("CellVMinMod").toUInt16();

    if (m_dataPoints.value("CellVAvg").isValid())
        m_averageCellVoltage = m_dataPoints.value("CellVAvg").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("ModTmpMax").isValid())
        m_maxModuleTemperature = m_dataPoints.value("ModTmpMax").toFloatWithSSF(m_modTmp_SF);

    if (m_dataPoints.value("ModTmpMaxMod").isValid())
        m_maxModuleTemperatureModule = m_dataPoints.value("ModTmpMaxMod").toUInt16();

    if (m_dataPoints.value("ModTmpMin").isValid())
        m_minModuleTemperature = m_dataPoints.value("ModTmpMin").toFloatWithSSF(m_modTmp_SF);

    if (m_dataPoints.value("ModTmpMinMod").isValid())
        m_minModuleTemperatureModule = m_dataPoints.value("ModTmpMinMod").toUInt16();

    if (m_dataPoints.value("ModTmpAvg").isValid())
        m_averageModuleTemperature = m_dataPoints.value("ModTmpAvg").toFloatWithSSF(m_modTmp_SF);

    if (m_dataPoints.value("Pad1").isValid())
        m_pad1 = m_dataPoints.value("Pad1").toUInt16();

    if (m_dataPoints.value("ConSt").isValid())
        m_contactorStatus = static_cast<ConstFlags>(m_dataPoints.value("ConSt").toUInt32());

    if (m_dataPoints.value("Evt1").isValid())
        m_stringEvent1 = static_cast<Evt1Flags>(m_dataPoints.value("Evt1").toUInt32());

    if (m_dataPoints.value("Evt2").isValid())
        m_stringEvent2 = m_dataPoints.value("Evt2").toUInt32();

    if (m_dataPoints.value("EvtVnd1").isValid())
        m_vendorEventBitfield1 = m_dataPoints.value("EvtVnd1").toUInt32();

    if (m_dataPoints.value("EvtVnd2").isValid())
        m_vendorEventBitfield2 = m_dataPoints.value("EvtVnd2").toUInt32();

    if (m_dataPoints.value("SetEna").isValid())
        m_enableDisableString = m_dataPoints.value("SetEna").toUInt16();

    if (m_dataPoints.value("SetCon").isValid())
        m_connectDisconnectString = static_cast<Setcon>(m_dataPoints.value("SetCon").toUInt16());

    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("SoH_SF").isValid())
        m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();

    if (m_dataPoints.value("DoD_SF").isValid())
        m_doD_SF = m_dataPoints.value("DoD_SF").toInt16();

    if (m_dataPoints.value("A_SF").isValid())
        m_a_SF = m_dataPoints.value("A_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("ModTmp_SF").isValid())
        m_modTmp_SF = m_dataPoints.value("ModTmp_SF").toInt16();

    if (m_dataPoints.value("Pad2").isValid())
        m_pad2 = m_dataPoints.value("Pad2").toUInt16();

    if (m_dataPoints.value("Pad3").isValid())
        m_pad3 = m_dataPoints.value("Pad3").toUInt16();

    if (m_dataPoints.value("Pad4").isValid())
        m_pad4 = m_dataPoints.value("Pad4").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecLithiumIonStringModel *model)
{
    debug.nospace().noquote() << "SunSpecLithiumIonStringModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("Idx") << "-->";
    if (model->dataPoints().value("Idx").isValid()) {
        debug.nospace().noquote() << model->stringIndex() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NMod") << "-->";
    if (model->dataPoints().value("NMod").isValid()) {
        debug.nospace().noquote() << model->moduleCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("St") << "-->";
    if (model->dataPoints().value("St").isValid()) {
        debug.nospace().noquote() << model->stringStatus() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ConFail") << "-->";
    if (model->dataPoints().value("ConFail").isValid()) {
        debug.nospace().noquote() << model->connectionFailureReason() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NCellBal") << "-->";
    if (model->dataPoints().value("NCellBal").isValid()) {
        debug.nospace().noquote() << model->stringCellBalancingCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoC") << "-->";
    if (model->dataPoints().value("SoC").isValid()) {
        debug.nospace().noquote() << model->stringStateOfCharge() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DoD") << "-->";
    if (model->dataPoints().value("DoD").isValid()) {
        debug.nospace().noquote() << model->stringDepthOfDischarge() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NCyc") << "-->";
    if (model->dataPoints().value("NCyc").isValid()) {
        debug.nospace().noquote() << model->stringCycleCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoH") << "-->";
    if (model->dataPoints().value("SoH").isValid()) {
        debug.nospace().noquote() << model->stringStateOfHealth() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("A") << "-->";
    if (model->dataPoints().value("A").isValid()) {
        debug.nospace().noquote() << model->stringCurrent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("V") << "-->";
    if (model->dataPoints().value("V").isValid()) {
        debug.nospace().noquote() << model->stringVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMax") << "-->";
    if (model->dataPoints().value("CellVMax").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMaxMod") << "-->";
    if (model->dataPoints().value("CellVMaxMod").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltageModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMin") << "-->";
    if (model->dataPoints().value("CellVMin").isValid()) {
        debug.nospace().noquote() << model->minCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMinMod") << "-->";
    if (model->dataPoints().value("CellVMinMod").isValid()) {
        debug.nospace().noquote() << model->minCellVoltageModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVAvg") << "-->";
    if (model->dataPoints().value("CellVAvg").isValid()) {
        debug.nospace().noquote() << model->averageCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMax") << "-->";
    if (model->dataPoints().value("ModTmpMax").isValid()) {
        debug.nospace().noquote() << model->maxModuleTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMaxMod") << "-->";
    if (model->dataPoints().value("ModTmpMaxMod").isValid()) {
        debug.nospace().noquote() << model->maxModuleTemperatureModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMin") << "-->";
    if (model->dataPoints().value("ModTmpMin").isValid()) {
        debug.nospace().noquote() << model->minModuleTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMinMod") << "-->";
    if (model->dataPoints().value("ModTmpMinMod").isValid()) {
        debug.nospace().noquote() << model->minModuleTemperatureModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpAvg") << "-->";
    if (model->dataPoints().value("ModTmpAvg").isValid()) {
        debug.nospace().noquote() << model->averageModuleTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad1") << "-->";
    if (model->dataPoints().value("Pad1").isValid()) {
        debug.nospace().noquote() << model->pad1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ConSt") << "-->";
    if (model->dataPoints().value("ConSt").isValid()) {
        debug.nospace().noquote() << model->contactorStatus() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt1") << "-->";
    if (model->dataPoints().value("Evt1").isValid()) {
        debug.nospace().noquote() << model->stringEvent1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt2") << "-->";
    if (model->dataPoints().value("Evt2").isValid()) {
        debug.nospace().noquote() << model->stringEvent2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd1") << "-->";
    if (model->dataPoints().value("EvtVnd1").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd2") << "-->";
    if (model->dataPoints().value("EvtVnd2").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SetEna") << "-->";
    if (model->dataPoints().value("SetEna").isValid()) {
        debug.nospace().noquote() << model->enableDisableString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SetCon") << "-->";
    if (model->dataPoints().value("SetCon").isValid()) {
        debug.nospace().noquote() << model->connectDisconnectString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad2") << "-->";
    if (model->dataPoints().value("Pad2").isValid()) {
        debug.nospace().noquote() << model->pad2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad3") << "-->";
    if (model->dataPoints().value("Pad3").isValid()) {
        debug.nospace().noquote() << model->pad3() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad4") << "-->";
    if (model->dataPoints().value("Pad4").isValid()) {
        debug.nospace().noquote() << model->pad4() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
