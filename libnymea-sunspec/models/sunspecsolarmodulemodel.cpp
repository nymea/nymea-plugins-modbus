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

#include "sunspecsolarmodulemodel.h"
#include "sunspecconnection.h"

SunSpecSolarModuleModel::SunSpecSolarModuleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 502, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecSolarModuleModel::~SunSpecSolarModuleModel()
{

}

QString SunSpecSolarModuleModel::name() const
{
    return "solar_module";
}

QString SunSpecSolarModuleModel::description() const
{
    return "A solar module model supporting DC-DC converter";
}

QString SunSpecSolarModuleModel::label() const
{
    return "Solar Module";
}

qint16 SunSpecSolarModuleModel::a_SF() const
{
    return m_a_SF;
}
qint16 SunSpecSolarModuleModel::v_SF() const
{
    return m_v_SF;
}
qint16 SunSpecSolarModuleModel::w_SF() const
{
    return m_w_SF;
}
qint16 SunSpecSolarModuleModel::wh_SF() const
{
    return m_wh_SF;
}
SunSpecSolarModuleModel::Stat SunSpecSolarModuleModel::status() const
{
    return m_status;
}
quint16 SunSpecSolarModuleModel::vendorStatus() const
{
    return m_vendorStatus;
}
SunSpecSolarModuleModel::EvtFlags SunSpecSolarModuleModel::events() const
{
    return m_events;
}
quint32 SunSpecSolarModuleModel::vendorModuleEventFlags() const
{
    return m_vendorModuleEventFlags;
}
quint16 SunSpecSolarModuleModel::control() const
{
    return m_control;
}

QModbusReply *SunSpecSolarModuleModel::setControl(quint16 control)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("Ctl");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(control));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint32 SunSpecSolarModuleModel::vendorControl() const
{
    return m_vendorControl;
}

QModbusReply *SunSpecSolarModuleModel::setVendorControl(quint32 vendorControl)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("CtlVend");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt32(static_cast<quint32>(vendorControl));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
qint32 SunSpecSolarModuleModel::controlValue() const
{
    return m_controlValue;
}

QModbusReply *SunSpecSolarModuleModel::setControlValue(qint32 controlValue)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("CtlVal");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(controlValue);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint32 SunSpecSolarModuleModel::timestamp() const
{
    return m_timestamp;
}
float SunSpecSolarModuleModel::outputCurrent() const
{
    return m_outputCurrent;
}
float SunSpecSolarModuleModel::outputVoltage() const
{
    return m_outputVoltage;
}
quint32 SunSpecSolarModuleModel::outputEnergy() const
{
    return m_outputEnergy;
}
float SunSpecSolarModuleModel::outputPower() const
{
    return m_outputPower;
}
qint16 SunSpecSolarModuleModel::temp() const
{
    return m_temp;
}
float SunSpecSolarModuleModel::inputCurrent() const
{
    return m_inputCurrent;
}
float SunSpecSolarModuleModel::inputVoltage() const
{
    return m_inputVoltage;
}
quint32 SunSpecSolarModuleModel::inputEnergy() const
{
    return m_inputEnergy;
}
float SunSpecSolarModuleModel::inputPower() const
{
    return m_inputPower;
}
void SunSpecSolarModuleModel::initDataPoints()
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

    SunSpecDataPoint a_SFDataPoint;
    a_SFDataPoint.setName("A_SF");
    a_SFDataPoint.setDescription("Current scale factor");
    a_SFDataPoint.setSize(1);
    a_SFDataPoint.setAddressOffset(2);
    a_SFDataPoint.setBlockOffset(0);
    a_SFDataPoint.setSunSpecDataType("sunssf");
    a_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(a_SFDataPoint.name(), a_SFDataPoint);

    SunSpecDataPoint v_SFDataPoint;
    v_SFDataPoint.setName("V_SF");
    v_SFDataPoint.setDescription("Voltage scale factor");
    v_SFDataPoint.setSize(1);
    v_SFDataPoint.setAddressOffset(3);
    v_SFDataPoint.setBlockOffset(1);
    v_SFDataPoint.setSunSpecDataType("sunssf");
    v_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v_SFDataPoint.name(), v_SFDataPoint);

    SunSpecDataPoint w_SFDataPoint;
    w_SFDataPoint.setName("W_SF");
    w_SFDataPoint.setDescription("Power scale factor");
    w_SFDataPoint.setSize(1);
    w_SFDataPoint.setAddressOffset(4);
    w_SFDataPoint.setBlockOffset(2);
    w_SFDataPoint.setSunSpecDataType("sunssf");
    w_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w_SFDataPoint.name(), w_SFDataPoint);

    SunSpecDataPoint wh_SFDataPoint;
    wh_SFDataPoint.setName("Wh_SF");
    wh_SFDataPoint.setDescription("Energy scale factor");
    wh_SFDataPoint.setSize(1);
    wh_SFDataPoint.setAddressOffset(5);
    wh_SFDataPoint.setBlockOffset(3);
    wh_SFDataPoint.setSunSpecDataType("sunssf");
    wh_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wh_SFDataPoint.name(), wh_SFDataPoint);

    SunSpecDataPoint statusDataPoint;
    statusDataPoint.setName("Stat");
    statusDataPoint.setLabel("Status");
    statusDataPoint.setDescription("Enumerated value.  Module Status Code");
    statusDataPoint.setMandatory(true);
    statusDataPoint.setSize(1);
    statusDataPoint.setAddressOffset(6);
    statusDataPoint.setBlockOffset(4);
    statusDataPoint.setSunSpecDataType("enum16");
    statusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(statusDataPoint.name(), statusDataPoint);

    SunSpecDataPoint vendorStatusDataPoint;
    vendorStatusDataPoint.setName("StatVend");
    vendorStatusDataPoint.setLabel("Vendor Status");
    vendorStatusDataPoint.setDescription("Module Vendor Status Code");
    vendorStatusDataPoint.setSize(1);
    vendorStatusDataPoint.setAddressOffset(7);
    vendorStatusDataPoint.setBlockOffset(5);
    vendorStatusDataPoint.setSunSpecDataType("enum16");
    vendorStatusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorStatusDataPoint.name(), vendorStatusDataPoint);

    SunSpecDataPoint eventsDataPoint;
    eventsDataPoint.setName("Evt");
    eventsDataPoint.setLabel("Events");
    eventsDataPoint.setDescription("Bitmask value.  Module Event Flags");
    eventsDataPoint.setMandatory(true);
    eventsDataPoint.setSize(2);
    eventsDataPoint.setAddressOffset(8);
    eventsDataPoint.setBlockOffset(6);
    eventsDataPoint.setSunSpecDataType("bitfield32");
    eventsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

    SunSpecDataPoint vendorModuleEventFlagsDataPoint;
    vendorModuleEventFlagsDataPoint.setName("EvtVend");
    vendorModuleEventFlagsDataPoint.setLabel("Vendor Module Event Flags");
    vendorModuleEventFlagsDataPoint.setDescription("Vendor specific flags");
    vendorModuleEventFlagsDataPoint.setSize(2);
    vendorModuleEventFlagsDataPoint.setAddressOffset(10);
    vendorModuleEventFlagsDataPoint.setBlockOffset(8);
    vendorModuleEventFlagsDataPoint.setSunSpecDataType("bitfield32");
    vendorModuleEventFlagsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorModuleEventFlagsDataPoint.name(), vendorModuleEventFlagsDataPoint);

    SunSpecDataPoint controlDataPoint;
    controlDataPoint.setName("Ctl");
    controlDataPoint.setLabel("Control");
    controlDataPoint.setDescription("Module Control");
    controlDataPoint.setSize(1);
    controlDataPoint.setAddressOffset(12);
    controlDataPoint.setBlockOffset(10);
    controlDataPoint.setSunSpecDataType("enum16");
    controlDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    controlDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(controlDataPoint.name(), controlDataPoint);

    SunSpecDataPoint vendorControlDataPoint;
    vendorControlDataPoint.setName("CtlVend");
    vendorControlDataPoint.setLabel("Vendor Control");
    vendorControlDataPoint.setDescription("Vendor Module Control");
    vendorControlDataPoint.setSize(2);
    vendorControlDataPoint.setAddressOffset(13);
    vendorControlDataPoint.setBlockOffset(11);
    vendorControlDataPoint.setSunSpecDataType("enum32");
    vendorControlDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vendorControlDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorControlDataPoint.name(), vendorControlDataPoint);

    SunSpecDataPoint controlValueDataPoint;
    controlValueDataPoint.setName("CtlVal");
    controlValueDataPoint.setLabel("Control Value");
    controlValueDataPoint.setDescription("Module Control Value");
    controlValueDataPoint.setSize(2);
    controlValueDataPoint.setAddressOffset(15);
    controlValueDataPoint.setBlockOffset(13);
    controlValueDataPoint.setSunSpecDataType("int32");
    controlValueDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    controlValueDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(controlValueDataPoint.name(), controlValueDataPoint);

    SunSpecDataPoint timestampDataPoint;
    timestampDataPoint.setName("Tms");
    timestampDataPoint.setLabel("Timestamp");
    timestampDataPoint.setDescription("Time in seconds since 2000 epoch");
    timestampDataPoint.setUnits("Secs");
    timestampDataPoint.setSize(2);
    timestampDataPoint.setAddressOffset(17);
    timestampDataPoint.setBlockOffset(15);
    timestampDataPoint.setSunSpecDataType("uint32");
    timestampDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(timestampDataPoint.name(), timestampDataPoint);

    SunSpecDataPoint outputCurrentDataPoint;
    outputCurrentDataPoint.setName("OutA");
    outputCurrentDataPoint.setLabel("Output Current");
    outputCurrentDataPoint.setDescription("Output Current");
    outputCurrentDataPoint.setUnits("A");
    outputCurrentDataPoint.setSize(1);
    outputCurrentDataPoint.setAddressOffset(19);
    outputCurrentDataPoint.setBlockOffset(17);
    outputCurrentDataPoint.setScaleFactorName("A_SF");
    outputCurrentDataPoint.setSunSpecDataType("int16");
    outputCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outputCurrentDataPoint.name(), outputCurrentDataPoint);

    SunSpecDataPoint outputVoltageDataPoint;
    outputVoltageDataPoint.setName("OutV");
    outputVoltageDataPoint.setLabel("Output Voltage");
    outputVoltageDataPoint.setDescription("Output Voltage");
    outputVoltageDataPoint.setUnits("V");
    outputVoltageDataPoint.setSize(1);
    outputVoltageDataPoint.setAddressOffset(20);
    outputVoltageDataPoint.setBlockOffset(18);
    outputVoltageDataPoint.setScaleFactorName("V_SF");
    outputVoltageDataPoint.setSunSpecDataType("int16");
    outputVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outputVoltageDataPoint.name(), outputVoltageDataPoint);

    SunSpecDataPoint outputEnergyDataPoint;
    outputEnergyDataPoint.setName("OutWh");
    outputEnergyDataPoint.setLabel("Output Energy");
    outputEnergyDataPoint.setDescription("Output Energy");
    outputEnergyDataPoint.setUnits("Wh");
    outputEnergyDataPoint.setSize(2);
    outputEnergyDataPoint.setAddressOffset(21);
    outputEnergyDataPoint.setBlockOffset(19);
    outputEnergyDataPoint.setScaleFactorName("Wh_SF");
    outputEnergyDataPoint.setSunSpecDataType("acc32");
    outputEnergyDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outputEnergyDataPoint.name(), outputEnergyDataPoint);

    SunSpecDataPoint outputPowerDataPoint;
    outputPowerDataPoint.setName("OutPw");
    outputPowerDataPoint.setLabel("Output Power");
    outputPowerDataPoint.setDescription("Output Power");
    outputPowerDataPoint.setUnits("W");
    outputPowerDataPoint.setSize(1);
    outputPowerDataPoint.setAddressOffset(23);
    outputPowerDataPoint.setBlockOffset(21);
    outputPowerDataPoint.setScaleFactorName("W_SF");
    outputPowerDataPoint.setSunSpecDataType("int16");
    outputPowerDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outputPowerDataPoint.name(), outputPowerDataPoint);

    SunSpecDataPoint tempDataPoint;
    tempDataPoint.setName("Tmp");
    tempDataPoint.setLabel("Temp");
    tempDataPoint.setDescription("Module Temperature");
    tempDataPoint.setUnits("C");
    tempDataPoint.setSize(1);
    tempDataPoint.setAddressOffset(24);
    tempDataPoint.setBlockOffset(22);
    tempDataPoint.setSunSpecDataType("int16");
    tempDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tempDataPoint.name(), tempDataPoint);

    SunSpecDataPoint inputCurrentDataPoint;
    inputCurrentDataPoint.setName("InA");
    inputCurrentDataPoint.setLabel("Input Current");
    inputCurrentDataPoint.setDescription("Input Current");
    inputCurrentDataPoint.setUnits("A");
    inputCurrentDataPoint.setSize(1);
    inputCurrentDataPoint.setAddressOffset(25);
    inputCurrentDataPoint.setBlockOffset(23);
    inputCurrentDataPoint.setScaleFactorName("A_SF");
    inputCurrentDataPoint.setSunSpecDataType("int16");
    inputCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inputCurrentDataPoint.name(), inputCurrentDataPoint);

    SunSpecDataPoint inputVoltageDataPoint;
    inputVoltageDataPoint.setName("InV");
    inputVoltageDataPoint.setLabel("Input Voltage");
    inputVoltageDataPoint.setDescription("Input Voltage");
    inputVoltageDataPoint.setUnits("V");
    inputVoltageDataPoint.setSize(1);
    inputVoltageDataPoint.setAddressOffset(26);
    inputVoltageDataPoint.setBlockOffset(24);
    inputVoltageDataPoint.setScaleFactorName("V_SF");
    inputVoltageDataPoint.setSunSpecDataType("int16");
    inputVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inputVoltageDataPoint.name(), inputVoltageDataPoint);

    SunSpecDataPoint inputEnergyDataPoint;
    inputEnergyDataPoint.setName("InWh");
    inputEnergyDataPoint.setLabel("Input Energy");
    inputEnergyDataPoint.setDescription("Input Energy");
    inputEnergyDataPoint.setUnits("Wh");
    inputEnergyDataPoint.setSize(2);
    inputEnergyDataPoint.setAddressOffset(27);
    inputEnergyDataPoint.setBlockOffset(25);
    inputEnergyDataPoint.setScaleFactorName("Wh_SF");
    inputEnergyDataPoint.setSunSpecDataType("acc32");
    inputEnergyDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inputEnergyDataPoint.name(), inputEnergyDataPoint);

    SunSpecDataPoint inputPowerDataPoint;
    inputPowerDataPoint.setName("InW");
    inputPowerDataPoint.setLabel("Input Power");
    inputPowerDataPoint.setDescription("Input Power");
    inputPowerDataPoint.setUnits("W");
    inputPowerDataPoint.setSize(1);
    inputPowerDataPoint.setAddressOffset(29);
    inputPowerDataPoint.setBlockOffset(27);
    inputPowerDataPoint.setScaleFactorName("W_SF");
    inputPowerDataPoint.setSunSpecDataType("int16");
    inputPowerDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inputPowerDataPoint.name(), inputPowerDataPoint);

}

void SunSpecSolarModuleModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("A_SF").isValid())
        m_a_SF = m_dataPoints.value("A_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("W_SF").isValid())
        m_w_SF = m_dataPoints.value("W_SF").toInt16();

    if (m_dataPoints.value("Wh_SF").isValid())
        m_wh_SF = m_dataPoints.value("Wh_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("A_SF").isValid())
        m_a_SF = m_dataPoints.value("A_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("W_SF").isValid())
        m_w_SF = m_dataPoints.value("W_SF").toInt16();

    if (m_dataPoints.value("Wh_SF").isValid())
        m_wh_SF = m_dataPoints.value("Wh_SF").toInt16();

    if (m_dataPoints.value("Stat").isValid())
        m_status = static_cast<Stat>(m_dataPoints.value("Stat").toUInt16());

    if (m_dataPoints.value("StatVend").isValid())
        m_vendorStatus = m_dataPoints.value("StatVend").toUInt16();

    if (m_dataPoints.value("Evt").isValid())
        m_events = static_cast<EvtFlags>(m_dataPoints.value("Evt").toUInt32());

    if (m_dataPoints.value("EvtVend").isValid())
        m_vendorModuleEventFlags = m_dataPoints.value("EvtVend").toUInt32();

    if (m_dataPoints.value("Ctl").isValid())
        m_control = m_dataPoints.value("Ctl").toUInt16();

    if (m_dataPoints.value("CtlVend").isValid())
        m_vendorControl = m_dataPoints.value("CtlVend").toUInt32();

    if (m_dataPoints.value("CtlVal").isValid())
        m_controlValue = m_dataPoints.value("CtlVal").toInt32();

    if (m_dataPoints.value("Tms").isValid())
        m_timestamp = m_dataPoints.value("Tms").toUInt32();

    if (m_dataPoints.value("OutA").isValid())
        m_outputCurrent = m_dataPoints.value("OutA").toFloatWithSSF(m_a_SF);

    if (m_dataPoints.value("OutV").isValid())
        m_outputVoltage = m_dataPoints.value("OutV").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("OutWh").isValid())
        m_outputEnergy = m_dataPoints.value("OutWh").toFloatWithSSF(m_wh_SF);

    if (m_dataPoints.value("OutPw").isValid())
        m_outputPower = m_dataPoints.value("OutPw").toFloatWithSSF(m_w_SF);

    if (m_dataPoints.value("Tmp").isValid())
        m_temp = m_dataPoints.value("Tmp").toInt16();

    if (m_dataPoints.value("InA").isValid())
        m_inputCurrent = m_dataPoints.value("InA").toFloatWithSSF(m_a_SF);

    if (m_dataPoints.value("InV").isValid())
        m_inputVoltage = m_dataPoints.value("InV").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("InWh").isValid())
        m_inputEnergy = m_dataPoints.value("InWh").toFloatWithSSF(m_wh_SF);

    if (m_dataPoints.value("InW").isValid())
        m_inputPower = m_dataPoints.value("InW").toFloatWithSSF(m_w_SF);


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecSolarModuleModel *model)
{
    debug.nospace().noquote() << "SunSpecSolarModuleModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("Stat") << "-->";
    if (model->dataPoints().value("Stat").isValid()) {
        debug.nospace().noquote() << model->status() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StatVend") << "-->";
    if (model->dataPoints().value("StatVend").isValid()) {
        debug.nospace().noquote() << model->vendorStatus() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "-->";
    if (model->dataPoints().value("Evt").isValid()) {
        debug.nospace().noquote() << model->events() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVend") << "-->";
    if (model->dataPoints().value("EvtVend").isValid()) {
        debug.nospace().noquote() << model->vendorModuleEventFlags() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Ctl") << "-->";
    if (model->dataPoints().value("Ctl").isValid()) {
        debug.nospace().noquote() << model->control() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CtlVend") << "-->";
    if (model->dataPoints().value("CtlVend").isValid()) {
        debug.nospace().noquote() << model->vendorControl() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CtlVal") << "-->";
    if (model->dataPoints().value("CtlVal").isValid()) {
        debug.nospace().noquote() << model->controlValue() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Tms") << "-->";
    if (model->dataPoints().value("Tms").isValid()) {
        debug.nospace().noquote() << model->timestamp() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutA") << "-->";
    if (model->dataPoints().value("OutA").isValid()) {
        debug.nospace().noquote() << model->outputCurrent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutV") << "-->";
    if (model->dataPoints().value("OutV").isValid()) {
        debug.nospace().noquote() << model->outputVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutWh") << "-->";
    if (model->dataPoints().value("OutWh").isValid()) {
        debug.nospace().noquote() << model->outputEnergy() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutPw") << "-->";
    if (model->dataPoints().value("OutPw").isValid()) {
        debug.nospace().noquote() << model->outputPower() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Tmp") << "-->";
    if (model->dataPoints().value("Tmp").isValid()) {
        debug.nospace().noquote() << model->temp() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("InA") << "-->";
    if (model->dataPoints().value("InA").isValid()) {
        debug.nospace().noquote() << model->inputCurrent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("InV") << "-->";
    if (model->dataPoints().value("InV").isValid()) {
        debug.nospace().noquote() << model->inputVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("InWh") << "-->";
    if (model->dataPoints().value("InWh").isValid()) {
        debug.nospace().noquote() << model->inputEnergy() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("InW") << "-->";
    if (model->dataPoints().value("InW").isValid()) {
        debug.nospace().noquote() << model->inputPower() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
