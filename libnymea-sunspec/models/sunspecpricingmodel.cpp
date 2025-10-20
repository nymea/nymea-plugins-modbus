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

#include "sunspecpricingmodel.h"
#include "sunspecconnection.h"

SunSpecPricingModel::SunSpecPricingModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 125, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecPricingModel::~SunSpecPricingModel()
{

}

QString SunSpecPricingModel::name() const
{
    return "pricing";
}

QString SunSpecPricingModel::description() const
{
    return "Pricing Signal  ";
}

QString SunSpecPricingModel::label() const
{
    return "Pricing";
}

SunSpecPricingModel::ModenaFlags SunSpecPricingModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecPricingModel::setModEna(ModenaFlags modEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ModEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(modEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecPricingModel::Sigtype SunSpecPricingModel::sigType() const
{
    return m_sigType;
}

QModbusReply *SunSpecPricingModel::setSigType(Sigtype sigType)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("SigType");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(sigType));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecPricingModel::sig() const
{
    return m_sig;
}

QModbusReply *SunSpecPricingModel::setSig(float sig)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("Sig");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(sig, m_sigSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecPricingModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecPricingModel::setWinTms(quint16 winTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(winTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecPricingModel::rvtTms() const
{
    return m_rvtTms;
}

QModbusReply *SunSpecPricingModel::setRvtTms(quint16 rvtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RvtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rvtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecPricingModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecPricingModel::setRmpTms(quint16 rmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
qint16 SunSpecPricingModel::sigSf() const
{
    return m_sigSf;
}
quint16 SunSpecPricingModel::pad() const
{
    return m_pad;
}
void SunSpecPricingModel::initDataPoints()
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

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("Is price-based charge/discharge mode active?");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(2);
    modEnaDataPoint.setBlockOffset(0);
    modEnaDataPoint.setSunSpecDataType("bitfield16");
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    modEnaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint sigTypeDataPoint;
    sigTypeDataPoint.setName("SigType");
    sigTypeDataPoint.setLabel("SigType");
    sigTypeDataPoint.setDescription("Meaning of the pricing signal. When a Price schedule is used, type must match the schedule range variable description.");
    sigTypeDataPoint.setSize(1);
    sigTypeDataPoint.setAddressOffset(3);
    sigTypeDataPoint.setBlockOffset(1);
    sigTypeDataPoint.setSunSpecDataType("enum16");
    sigTypeDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    sigTypeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(sigTypeDataPoint.name(), sigTypeDataPoint);

    SunSpecDataPoint sigDataPoint;
    sigDataPoint.setName("Sig");
    sigDataPoint.setLabel("Sig");
    sigDataPoint.setDescription("Utility/ESP specific pricing signal. Content depends on pricing signal type. When H/M/L type is specified. Low=0; Med=1; High=2.");
    sigDataPoint.setMandatory(true);
    sigDataPoint.setSize(1);
    sigDataPoint.setAddressOffset(4);
    sigDataPoint.setBlockOffset(2);
    sigDataPoint.setScaleFactorName("Sig_SF");
    sigDataPoint.setSunSpecDataType("int16");
    sigDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    sigDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(sigDataPoint.name(), sigDataPoint);

    SunSpecDataPoint winTmsDataPoint;
    winTmsDataPoint.setName("WinTms");
    winTmsDataPoint.setLabel("WinTms");
    winTmsDataPoint.setDescription("Time window for charge/discharge pricing change.");
    winTmsDataPoint.setUnits("Secs");
    winTmsDataPoint.setSize(1);
    winTmsDataPoint.setAddressOffset(5);
    winTmsDataPoint.setBlockOffset(3);
    winTmsDataPoint.setSunSpecDataType("uint16");
    winTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    winTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(winTmsDataPoint.name(), winTmsDataPoint);

    SunSpecDataPoint rvtTmsDataPoint;
    rvtTmsDataPoint.setName("RvtTms");
    rvtTmsDataPoint.setLabel("RvtTms");
    rvtTmsDataPoint.setDescription("Timeout period for charge/discharge pricing change.");
    rvtTmsDataPoint.setUnits("Secs");
    rvtTmsDataPoint.setSize(1);
    rvtTmsDataPoint.setAddressOffset(6);
    rvtTmsDataPoint.setBlockOffset(4);
    rvtTmsDataPoint.setSunSpecDataType("uint16");
    rvtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rvtTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rvtTmsDataPoint.name(), rvtTmsDataPoint);

    SunSpecDataPoint rmpTmsDataPoint;
    rmpTmsDataPoint.setName("RmpTms");
    rmpTmsDataPoint.setLabel("RmpTms");
    rmpTmsDataPoint.setDescription("Ramp time for moving from current charge or discharge level to new level.");
    rmpTmsDataPoint.setUnits("Secs");
    rmpTmsDataPoint.setSize(1);
    rmpTmsDataPoint.setAddressOffset(7);
    rmpTmsDataPoint.setBlockOffset(5);
    rmpTmsDataPoint.setSunSpecDataType("uint16");
    rmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpTmsDataPoint.name(), rmpTmsDataPoint);

    SunSpecDataPoint sigSfDataPoint;
    sigSfDataPoint.setName("Sig_SF");
    sigSfDataPoint.setLabel("Sig_SF");
    sigSfDataPoint.setDescription("Pricing signal scale factor.");
    sigSfDataPoint.setMandatory(true);
    sigSfDataPoint.setSize(1);
    sigSfDataPoint.setAddressOffset(8);
    sigSfDataPoint.setBlockOffset(6);
    sigSfDataPoint.setSunSpecDataType("sunssf");
    sigSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(sigSfDataPoint.name(), sigSfDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(9);
    padDataPoint.setBlockOffset(7);
    padDataPoint.setSunSpecDataType("pad");
    padDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

void SunSpecPricingModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("Sig_SF").isValid())
        m_sigSf = m_dataPoints.value("Sig_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("ModEna").isValid())
        m_modEna = static_cast<ModenaFlags>(m_dataPoints.value("ModEna").toUInt16());

    if (m_dataPoints.value("SigType").isValid())
        m_sigType = static_cast<Sigtype>(m_dataPoints.value("SigType").toUInt16());

    if (m_dataPoints.value("Sig").isValid())
        m_sig = m_dataPoints.value("Sig").toFloatWithSSF(m_sigSf);

    if (m_dataPoints.value("WinTms").isValid())
        m_winTms = m_dataPoints.value("WinTms").toUInt16();

    if (m_dataPoints.value("RvtTms").isValid())
        m_rvtTms = m_dataPoints.value("RvtTms").toUInt16();

    if (m_dataPoints.value("RmpTms").isValid())
        m_rmpTms = m_dataPoints.value("RmpTms").toUInt16();

    if (m_dataPoints.value("Sig_SF").isValid())
        m_sigSf = m_dataPoints.value("Sig_SF").toInt16();

    if (m_dataPoints.value("Pad").isValid())
        m_pad = m_dataPoints.value("Pad").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecPricingModel *model)
{
    debug.nospace().noquote() << "SunSpecPricingModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModEna") << "-->";
    if (model->dataPoints().value("ModEna").isValid()) {
        debug.nospace().noquote() << model->modEna() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SigType") << "-->";
    if (model->dataPoints().value("SigType").isValid()) {
        debug.nospace().noquote() << model->sigType() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Sig") << "-->";
    if (model->dataPoints().value("Sig").isValid()) {
        debug.nospace().noquote() << model->sig() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WinTms") << "-->";
    if (model->dataPoints().value("WinTms").isValid()) {
        debug.nospace().noquote() << model->winTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("RvtTms") << "-->";
    if (model->dataPoints().value("RvtTms").isValid()) {
        debug.nospace().noquote() << model->rvtTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("RmpTms") << "-->";
    if (model->dataPoints().value("RmpTms").isValid()) {
        debug.nospace().noquote() << model->rmpTms() << "\n";
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
