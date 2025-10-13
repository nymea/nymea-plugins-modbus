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

#include "sunspeccommonmodel.h"
#include "sunspecconnection.h"

SunSpecCommonModel::SunSpecCommonModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 1, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecCommonModel::~SunSpecCommonModel()
{

}

QString SunSpecCommonModel::name() const
{
    return "common";
}

QString SunSpecCommonModel::description() const
{
    return "All SunSpec compliant devices must include this as the first model";
}

QString SunSpecCommonModel::label() const
{
    return "Common";
}

QString SunSpecCommonModel::manufacturer() const
{
    return m_manufacturer;
}
QString SunSpecCommonModel::model() const
{
    return m_model;
}
QString SunSpecCommonModel::options() const
{
    return m_options;
}
QString SunSpecCommonModel::version() const
{
    return m_version;
}
QString SunSpecCommonModel::serialNumber() const
{
    return m_serialNumber;
}
quint16 SunSpecCommonModel::deviceAddress() const
{
    return m_deviceAddress;
}

QModbusReply *SunSpecCommonModel::setDeviceAddress(quint16 deviceAddress)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("DA");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(deviceAddress);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecCommonModel::pad() const
{
    return m_pad;
}
void SunSpecCommonModel::initDataPoints()
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

    SunSpecDataPoint manufacturerDataPoint;
    manufacturerDataPoint.setName("Mn");
    manufacturerDataPoint.setLabel("Manufacturer");
    manufacturerDataPoint.setDescription("Well known value registered with SunSpec for compliance");
    manufacturerDataPoint.setMandatory(true);
    manufacturerDataPoint.setSize(16);
    manufacturerDataPoint.setAddressOffset(2);
    manufacturerDataPoint.setBlockOffset(0);
    manufacturerDataPoint.setSunSpecDataType("string");
    manufacturerDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(manufacturerDataPoint.name(), manufacturerDataPoint);

    SunSpecDataPoint modelDataPoint;
    modelDataPoint.setName("Md");
    modelDataPoint.setLabel("Model");
    modelDataPoint.setDescription("Manufacturer specific value (32 chars)");
    modelDataPoint.setMandatory(true);
    modelDataPoint.setSize(16);
    modelDataPoint.setAddressOffset(18);
    modelDataPoint.setBlockOffset(16);
    modelDataPoint.setSunSpecDataType("string");
    modelDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modelDataPoint.name(), modelDataPoint);

    SunSpecDataPoint optionsDataPoint;
    optionsDataPoint.setName("Opt");
    optionsDataPoint.setLabel("Options");
    optionsDataPoint.setDescription("Manufacturer specific value (16 chars)");
    optionsDataPoint.setSize(8);
    optionsDataPoint.setAddressOffset(34);
    optionsDataPoint.setBlockOffset(32);
    optionsDataPoint.setSunSpecDataType("string");
    optionsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(optionsDataPoint.name(), optionsDataPoint);

    SunSpecDataPoint versionDataPoint;
    versionDataPoint.setName("Vr");
    versionDataPoint.setLabel("Version");
    versionDataPoint.setDescription("Manufacturer specific value (16 chars)");
    versionDataPoint.setSize(8);
    versionDataPoint.setAddressOffset(42);
    versionDataPoint.setBlockOffset(40);
    versionDataPoint.setSunSpecDataType("string");
    versionDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(versionDataPoint.name(), versionDataPoint);

    SunSpecDataPoint serialNumberDataPoint;
    serialNumberDataPoint.setName("SN");
    serialNumberDataPoint.setLabel("Serial Number");
    serialNumberDataPoint.setDescription("Manufacturer specific value (32 chars)");
    serialNumberDataPoint.setMandatory(true);
    serialNumberDataPoint.setSize(16);
    serialNumberDataPoint.setAddressOffset(50);
    serialNumberDataPoint.setBlockOffset(48);
    serialNumberDataPoint.setSunSpecDataType("string");
    serialNumberDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(serialNumberDataPoint.name(), serialNumberDataPoint);

    SunSpecDataPoint deviceAddressDataPoint;
    deviceAddressDataPoint.setName("DA");
    deviceAddressDataPoint.setLabel("Device Address");
    deviceAddressDataPoint.setDescription("Modbus device address");
    deviceAddressDataPoint.setSize(1);
    deviceAddressDataPoint.setAddressOffset(66);
    deviceAddressDataPoint.setBlockOffset(64);
    deviceAddressDataPoint.setSunSpecDataType("uint16");
    deviceAddressDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    deviceAddressDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(deviceAddressDataPoint.name(), deviceAddressDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setDescription("Force even alignment");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(67);
    padDataPoint.setBlockOffset(65);
    padDataPoint.setSunSpecDataType("pad");
    padDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

void SunSpecCommonModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("Mn").isValid())
        m_manufacturer = m_dataPoints.value("Mn").toString();

    if (m_dataPoints.value("Md").isValid())
        m_model = m_dataPoints.value("Md").toString();

    if (m_dataPoints.value("Opt").isValid())
        m_options = m_dataPoints.value("Opt").toString();

    if (m_dataPoints.value("Vr").isValid())
        m_version = m_dataPoints.value("Vr").toString();

    if (m_dataPoints.value("SN").isValid())
        m_serialNumber = m_dataPoints.value("SN").toString();

    if (m_dataPoints.value("DA").isValid())
        m_deviceAddress = m_dataPoints.value("DA").toUInt16();

    if (m_dataPoints.value("Pad").isValid())
        m_pad = m_dataPoints.value("Pad").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecCommonModel *model)
{
    debug.nospace().noquote() << "SunSpecCommonModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("Mn") << "-->";
    if (model->dataPoints().value("Mn").isValid()) {
        debug.nospace().noquote() << model->manufacturer() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Md") << "-->";
    if (model->dataPoints().value("Md").isValid()) {
        debug.nospace().noquote() << model->model() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Opt") << "-->";
    if (model->dataPoints().value("Opt").isValid()) {
        debug.nospace().noquote() << model->options() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Vr") << "-->";
    if (model->dataPoints().value("Vr").isValid()) {
        debug.nospace().noquote() << model->version() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SN") << "-->";
    if (model->dataPoints().value("SN").isValid()) {
        debug.nospace().noquote() << model->serialNumber() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DA") << "-->";
    if (model->dataPoints().value("DA").isValid()) {
        debug.nospace().noquote() << model->deviceAddress() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad") << "-->";
    if (model->dataPoints().value("Pad").isValid()) {
        debug.nospace().noquote() << model->pad() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
