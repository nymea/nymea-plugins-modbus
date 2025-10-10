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

#include "sunspecfreqwattparammodel.h"
#include "sunspecconnection.h"

SunSpecFreqWattParamModel::SunSpecFreqWattParamModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 127, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecFreqWattParamModel::~SunSpecFreqWattParamModel()
{

}

QString SunSpecFreqWattParamModel::name() const
{
    return "freq_watt_param";
}

QString SunSpecFreqWattParamModel::description() const
{
    return "Parameterized Frequency-Watt ";
}

QString SunSpecFreqWattParamModel::label() const
{
    return "Freq-Watt Param";
}

float SunSpecFreqWattParamModel::wGra() const
{
    return m_wGra;
}

QModbusReply *SunSpecFreqWattParamModel::setWGra(float wGra)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WGra");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(wGra, m_wGraSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecFreqWattParamModel::hzStr() const
{
    return m_hzStr;
}

QModbusReply *SunSpecFreqWattParamModel::setHzStr(float hzStr)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("HzStr");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hzStr, m_hzStrStopSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecFreqWattParamModel::hzStop() const
{
    return m_hzStop;
}

QModbusReply *SunSpecFreqWattParamModel::setHzStop(float hzStop)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("HzStop");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hzStop, m_hzStrStopSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecFreqWattParamModel::HysenaFlags SunSpecFreqWattParamModel::hysEna() const
{
    return m_hysEna;
}

QModbusReply *SunSpecFreqWattParamModel::setHysEna(HysenaFlags hysEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("HysEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(hysEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecFreqWattParamModel::ModenaFlags SunSpecFreqWattParamModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecFreqWattParamModel::setModEna(ModenaFlags modEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ModEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(modEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecFreqWattParamModel::hzStopWGra() const
{
    return m_hzStopWGra;
}

QModbusReply *SunSpecFreqWattParamModel::setHzStopWGra(float hzStopWGra)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("HzStopWGra");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hzStopWGra, m_rmpIncDecSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
qint16 SunSpecFreqWattParamModel::wGraSf() const
{
    return m_wGraSf;
}
qint16 SunSpecFreqWattParamModel::hzStrStopSf() const
{
    return m_hzStrStopSf;
}
qint16 SunSpecFreqWattParamModel::rmpIncDecSf() const
{
    return m_rmpIncDecSf;
}
quint16 SunSpecFreqWattParamModel::pad() const
{
    return m_pad;
}
void SunSpecFreqWattParamModel::initDataPoints()
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

    SunSpecDataPoint wGraDataPoint;
    wGraDataPoint.setName("WGra");
    wGraDataPoint.setLabel("WGra");
    wGraDataPoint.setDescription("The slope of the reduction in the maximum allowed watts output as a function of frequency.");
    wGraDataPoint.setUnits("% PM/Hz");
    wGraDataPoint.setMandatory(true);
    wGraDataPoint.setSize(1);
    wGraDataPoint.setAddressOffset(2);
    wGraDataPoint.setBlockOffset(0);
    wGraDataPoint.setScaleFactorName("WGra_SF");
    wGraDataPoint.setSunSpecDataType("uint16");
    wGraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wGraDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wGraDataPoint.name(), wGraDataPoint);

    SunSpecDataPoint hzStrDataPoint;
    hzStrDataPoint.setName("HzStr");
    hzStrDataPoint.setLabel("HzStr");
    hzStrDataPoint.setDescription("The frequency deviation from nominal frequency (ECPNomHz) at which a snapshot of the instantaneous power output is taken to act as the CAPPED power level (PM) and above which reduction in power output occurs.");
    hzStrDataPoint.setUnits("Hz");
    hzStrDataPoint.setMandatory(true);
    hzStrDataPoint.setSize(1);
    hzStrDataPoint.setAddressOffset(3);
    hzStrDataPoint.setBlockOffset(1);
    hzStrDataPoint.setScaleFactorName("HzStrStop_SF");
    hzStrDataPoint.setSunSpecDataType("int16");
    hzStrDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hzStrDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hzStrDataPoint.name(), hzStrDataPoint);

    SunSpecDataPoint hzStopDataPoint;
    hzStopDataPoint.setName("HzStop");
    hzStopDataPoint.setLabel("HzStop");
    hzStopDataPoint.setDescription("The frequency deviation from nominal frequency (ECPNomHz) at which curtailed power output may return to normal and the cap on the power level value is removed.");
    hzStopDataPoint.setUnits("Hz");
    hzStopDataPoint.setMandatory(true);
    hzStopDataPoint.setSize(1);
    hzStopDataPoint.setAddressOffset(4);
    hzStopDataPoint.setBlockOffset(2);
    hzStopDataPoint.setScaleFactorName("HzStrStop_SF");
    hzStopDataPoint.setSunSpecDataType("int16");
    hzStopDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hzStopDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hzStopDataPoint.name(), hzStopDataPoint);

    SunSpecDataPoint hysEnaDataPoint;
    hysEnaDataPoint.setName("HysEna");
    hysEnaDataPoint.setLabel("HysEna");
    hysEnaDataPoint.setDescription("Enable hysteresis");
    hysEnaDataPoint.setMandatory(true);
    hysEnaDataPoint.setSize(1);
    hysEnaDataPoint.setAddressOffset(5);
    hysEnaDataPoint.setBlockOffset(3);
    hysEnaDataPoint.setSunSpecDataType("bitfield16");
    hysEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hysEnaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hysEnaDataPoint.name(), hysEnaDataPoint);

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("Is Parameterized Frequency-Watt control active.");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(6);
    modEnaDataPoint.setBlockOffset(4);
    modEnaDataPoint.setSunSpecDataType("bitfield16");
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    modEnaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint hzStopWGraDataPoint;
    hzStopWGraDataPoint.setName("HzStopWGra");
    hzStopWGraDataPoint.setLabel("HzStopWGra");
    hzStopWGraDataPoint.setDescription("The maximum time-based rate of change at which power output returns to normal after having been capped by an over frequency event.");
    hzStopWGraDataPoint.setUnits("% WMax/min");
    hzStopWGraDataPoint.setSize(1);
    hzStopWGraDataPoint.setAddressOffset(7);
    hzStopWGraDataPoint.setBlockOffset(5);
    hzStopWGraDataPoint.setScaleFactorName("RmpIncDec_SF");
    hzStopWGraDataPoint.setSunSpecDataType("uint16");
    hzStopWGraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hzStopWGraDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hzStopWGraDataPoint.name(), hzStopWGraDataPoint);

    SunSpecDataPoint wGraSfDataPoint;
    wGraSfDataPoint.setName("WGra_SF");
    wGraSfDataPoint.setLabel("WGra_SF");
    wGraSfDataPoint.setDescription("Scale factor for output gradient.");
    wGraSfDataPoint.setSize(1);
    wGraSfDataPoint.setAddressOffset(8);
    wGraSfDataPoint.setBlockOffset(6);
    wGraSfDataPoint.setSunSpecDataType("sunssf");
    wGraSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wGraSfDataPoint.name(), wGraSfDataPoint);

    SunSpecDataPoint hzStrStopSfDataPoint;
    hzStrStopSfDataPoint.setName("HzStrStop_SF");
    hzStrStopSfDataPoint.setLabel("HzStrStop_SF");
    hzStrStopSfDataPoint.setDescription("Scale factor for frequency deviations.");
    hzStrStopSfDataPoint.setSize(1);
    hzStrStopSfDataPoint.setAddressOffset(9);
    hzStrStopSfDataPoint.setBlockOffset(7);
    hzStrStopSfDataPoint.setSunSpecDataType("sunssf");
    hzStrStopSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hzStrStopSfDataPoint.name(), hzStrStopSfDataPoint);

    SunSpecDataPoint rmpIncDecSfDataPoint;
    rmpIncDecSfDataPoint.setName("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setLabel("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setDescription("Scale factor for increment and decrement ramps.");
    rmpIncDecSfDataPoint.setSize(1);
    rmpIncDecSfDataPoint.setAddressOffset(10);
    rmpIncDecSfDataPoint.setBlockOffset(8);
    rmpIncDecSfDataPoint.setSunSpecDataType("sunssf");
    rmpIncDecSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpIncDecSfDataPoint.name(), rmpIncDecSfDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(11);
    padDataPoint.setBlockOffset(9);
    padDataPoint.setSunSpecDataType("pad");
    padDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

void SunSpecFreqWattParamModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("WGra_SF").isValid())
        m_wGraSf = m_dataPoints.value("WGra_SF").toInt16();

    if (m_dataPoints.value("HzStrStop_SF").isValid())
        m_hzStrStopSf = m_dataPoints.value("HzStrStop_SF").toInt16();

    if (m_dataPoints.value("RmpIncDec_SF").isValid())
        m_rmpIncDecSf = m_dataPoints.value("RmpIncDec_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("WGra").isValid())
        m_wGra = m_dataPoints.value("WGra").toFloatWithSSF(m_wGraSf);

    if (m_dataPoints.value("HzStr").isValid())
        m_hzStr = m_dataPoints.value("HzStr").toFloatWithSSF(m_hzStrStopSf);

    if (m_dataPoints.value("HzStop").isValid())
        m_hzStop = m_dataPoints.value("HzStop").toFloatWithSSF(m_hzStrStopSf);

    if (m_dataPoints.value("HysEna").isValid())
        m_hysEna = static_cast<HysenaFlags>(m_dataPoints.value("HysEna").toUInt16());

    if (m_dataPoints.value("ModEna").isValid())
        m_modEna = static_cast<ModenaFlags>(m_dataPoints.value("ModEna").toUInt16());

    if (m_dataPoints.value("HzStopWGra").isValid())
        m_hzStopWGra = m_dataPoints.value("HzStopWGra").toFloatWithSSF(m_rmpIncDecSf);

    if (m_dataPoints.value("WGra_SF").isValid())
        m_wGraSf = m_dataPoints.value("WGra_SF").toInt16();

    if (m_dataPoints.value("HzStrStop_SF").isValid())
        m_hzStrStopSf = m_dataPoints.value("HzStrStop_SF").toInt16();

    if (m_dataPoints.value("RmpIncDec_SF").isValid())
        m_rmpIncDecSf = m_dataPoints.value("RmpIncDec_SF").toInt16();

    if (m_dataPoints.value("Pad").isValid())
        m_pad = m_dataPoints.value("Pad").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecFreqWattParamModel *model)
{
    debug.nospace().noquote() << "SunSpecFreqWattParamModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("WGra") << "-->";
    if (model->dataPoints().value("WGra").isValid()) {
        debug.nospace().noquote() << model->wGra() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("HzStr") << "-->";
    if (model->dataPoints().value("HzStr").isValid()) {
        debug.nospace().noquote() << model->hzStr() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("HzStop") << "-->";
    if (model->dataPoints().value("HzStop").isValid()) {
        debug.nospace().noquote() << model->hzStop() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("HysEna") << "-->";
    if (model->dataPoints().value("HysEna").isValid()) {
        debug.nospace().noquote() << model->hysEna() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModEna") << "-->";
    if (model->dataPoints().value("ModEna").isValid()) {
        debug.nospace().noquote() << model->modEna() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("HzStopWGra") << "-->";
    if (model->dataPoints().value("HzStopWGra").isValid()) {
        debug.nospace().noquote() << model->hzStopWGra() << "\n";
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
