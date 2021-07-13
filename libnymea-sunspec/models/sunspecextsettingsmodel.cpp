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

#include "sunspecextsettingsmodel.h"
#include "sunspecconnection.h"

SunSpecExtSettingsModel::SunSpecExtSettingsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 145, 8, parent)
{
    //Q_ASSERT_X(length == 8,  "SunSpecExtSettingsModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecExtSettingsModel::~SunSpecExtSettingsModel()
{

}

QString SunSpecExtSettingsModel::name() const
{
    return "ext_settings";
}

QString SunSpecExtSettingsModel::description() const
{
    return "Inverter controls extended settings ";
}

QString SunSpecExtSettingsModel::label() const
{
    return "Extended Settings";
}

float SunSpecExtSettingsModel::rampUpRate() const
{
    return m_rampUpRate;
}

QModbusReply *SunSpecExtSettingsModel::setRampUpRate(float rampUpRate)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("NomRmpUpRte");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rampUpRate, m_rampRateScaleFactor, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecExtSettingsModel::nomRmpDnRte() const
{
    return m_nomRmpDnRte;
}

QModbusReply *SunSpecExtSettingsModel::setNomRmpDnRte(float nomRmpDnRte)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("NomRmpDnRte");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(nomRmpDnRte, m_rampRateScaleFactor, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecExtSettingsModel::emergencyRampUpRate() const
{
    return m_emergencyRampUpRate;
}

QModbusReply *SunSpecExtSettingsModel::setEmergencyRampUpRate(float emergencyRampUpRate)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("EmgRmpUpRte");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(emergencyRampUpRate, m_rampRateScaleFactor, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecExtSettingsModel::emergencyRampDownRate() const
{
    return m_emergencyRampDownRate;
}

QModbusReply *SunSpecExtSettingsModel::setEmergencyRampDownRate(float emergencyRampDownRate)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("EmgRmpDnRte");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(emergencyRampDownRate, m_rampRateScaleFactor, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecExtSettingsModel::connectRampUpRate() const
{
    return m_connectRampUpRate;
}

QModbusReply *SunSpecExtSettingsModel::setConnectRampUpRate(float connectRampUpRate)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ConnRmpUpRte");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(connectRampUpRate, m_rampRateScaleFactor, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecExtSettingsModel::connectRampDownRate() const
{
    return m_connectRampDownRate;
}

QModbusReply *SunSpecExtSettingsModel::setConnectRampDownRate(float connectRampDownRate)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ConnRmpDnRte");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(connectRampDownRate, m_rampRateScaleFactor, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecExtSettingsModel::defaultRampRate() const
{
    return m_defaultRampRate;
}

QModbusReply *SunSpecExtSettingsModel::setDefaultRampRate(float defaultRampRate)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("AGra");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(defaultRampRate, m_rampRateScaleFactor, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
void SunSpecExtSettingsModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("Rmp_SF").isValid())
        m_rampRateScaleFactor = m_dataPoints.value("Rmp_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("NomRmpUpRte").isValid())
        m_rampUpRate = m_dataPoints.value("NomRmpUpRte").toFloatWithSSF(m_rampRateScaleFactor);

    if (m_dataPoints.value("NomRmpDnRte").isValid())
        m_nomRmpDnRte = m_dataPoints.value("NomRmpDnRte").toFloatWithSSF(m_rampRateScaleFactor);

    if (m_dataPoints.value("EmgRmpUpRte").isValid())
        m_emergencyRampUpRate = m_dataPoints.value("EmgRmpUpRte").toFloatWithSSF(m_rampRateScaleFactor);

    if (m_dataPoints.value("EmgRmpDnRte").isValid())
        m_emergencyRampDownRate = m_dataPoints.value("EmgRmpDnRte").toFloatWithSSF(m_rampRateScaleFactor);

    if (m_dataPoints.value("ConnRmpUpRte").isValid())
        m_connectRampUpRate = m_dataPoints.value("ConnRmpUpRte").toFloatWithSSF(m_rampRateScaleFactor);

    if (m_dataPoints.value("ConnRmpDnRte").isValid())
        m_connectRampDownRate = m_dataPoints.value("ConnRmpDnRte").toFloatWithSSF(m_rampRateScaleFactor);

    if (m_dataPoints.value("AGra").isValid())
        m_defaultRampRate = m_dataPoints.value("AGra").toFloatWithSSF(m_rampRateScaleFactor);


    qCDebug(dcSunSpecModelData()) << this;
}

void SunSpecExtSettingsModel::initDataPoints()
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

    SunSpecDataPoint rampUpRateDataPoint;
    rampUpRateDataPoint.setName("NomRmpUpRte");
    rampUpRateDataPoint.setLabel("Ramp Up Rate");
    rampUpRateDataPoint.setDescription("Ramp up rate as a percentage of max current.");
    rampUpRateDataPoint.setUnits("Pct");
    rampUpRateDataPoint.setSize(1);
    rampUpRateDataPoint.setAddressOffset(2);
    rampUpRateDataPoint.setBlockOffset(0);
    rampUpRateDataPoint.setScaleFactorName("Rmp_SF");
    rampUpRateDataPoint.setSunSpecDataType("uint16");
    rampUpRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(rampUpRateDataPoint.name(), rampUpRateDataPoint);

    SunSpecDataPoint nomRmpDnRteDataPoint;
    nomRmpDnRteDataPoint.setName("NomRmpDnRte");
    nomRmpDnRteDataPoint.setLabel("NomRmpDnRte");
    nomRmpDnRteDataPoint.setDescription("Ramp down rate as a percentage of max current.");
    nomRmpDnRteDataPoint.setUnits("Pct");
    nomRmpDnRteDataPoint.setSize(1);
    nomRmpDnRteDataPoint.setAddressOffset(3);
    nomRmpDnRteDataPoint.setBlockOffset(1);
    nomRmpDnRteDataPoint.setScaleFactorName("Rmp_SF");
    nomRmpDnRteDataPoint.setSunSpecDataType("uint16");
    nomRmpDnRteDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(nomRmpDnRteDataPoint.name(), nomRmpDnRteDataPoint);

    SunSpecDataPoint emergencyRampUpRateDataPoint;
    emergencyRampUpRateDataPoint.setName("EmgRmpUpRte");
    emergencyRampUpRateDataPoint.setLabel("Emergency Ramp Up Rate");
    emergencyRampUpRateDataPoint.setDescription("Emergency ramp up rate as a percentage of max current.");
    emergencyRampUpRateDataPoint.setUnits("Pct");
    emergencyRampUpRateDataPoint.setSize(1);
    emergencyRampUpRateDataPoint.setAddressOffset(4);
    emergencyRampUpRateDataPoint.setBlockOffset(2);
    emergencyRampUpRateDataPoint.setScaleFactorName("Rmp_SF");
    emergencyRampUpRateDataPoint.setSunSpecDataType("uint16");
    emergencyRampUpRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(emergencyRampUpRateDataPoint.name(), emergencyRampUpRateDataPoint);

    SunSpecDataPoint emergencyRampDownRateDataPoint;
    emergencyRampDownRateDataPoint.setName("EmgRmpDnRte");
    emergencyRampDownRateDataPoint.setLabel("Emergency Ramp Down Rate");
    emergencyRampDownRateDataPoint.setDescription("Emergency ramp down rate as a percentage of max current.");
    emergencyRampDownRateDataPoint.setUnits("Pct");
    emergencyRampDownRateDataPoint.setSize(1);
    emergencyRampDownRateDataPoint.setAddressOffset(5);
    emergencyRampDownRateDataPoint.setBlockOffset(3);
    emergencyRampDownRateDataPoint.setScaleFactorName("Rmp_SF");
    emergencyRampDownRateDataPoint.setSunSpecDataType("uint16");
    emergencyRampDownRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(emergencyRampDownRateDataPoint.name(), emergencyRampDownRateDataPoint);

    SunSpecDataPoint connectRampUpRateDataPoint;
    connectRampUpRateDataPoint.setName("ConnRmpUpRte");
    connectRampUpRateDataPoint.setLabel("Connect Ramp Up Rate");
    connectRampUpRateDataPoint.setDescription("Connect ramp up rate as a percentage of max current.");
    connectRampUpRateDataPoint.setUnits("Pct");
    connectRampUpRateDataPoint.setSize(1);
    connectRampUpRateDataPoint.setAddressOffset(6);
    connectRampUpRateDataPoint.setBlockOffset(4);
    connectRampUpRateDataPoint.setScaleFactorName("Rmp_SF");
    connectRampUpRateDataPoint.setSunSpecDataType("uint16");
    connectRampUpRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connectRampUpRateDataPoint.name(), connectRampUpRateDataPoint);

    SunSpecDataPoint connectRampDownRateDataPoint;
    connectRampDownRateDataPoint.setName("ConnRmpDnRte");
    connectRampDownRateDataPoint.setLabel("Connect Ramp Down Rate");
    connectRampDownRateDataPoint.setDescription("Connect ramp down rate as a percentage of max current.");
    connectRampDownRateDataPoint.setUnits("Pct");
    connectRampDownRateDataPoint.setSize(1);
    connectRampDownRateDataPoint.setAddressOffset(7);
    connectRampDownRateDataPoint.setBlockOffset(5);
    connectRampDownRateDataPoint.setScaleFactorName("Rmp_SF");
    connectRampDownRateDataPoint.setSunSpecDataType("uint16");
    connectRampDownRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connectRampDownRateDataPoint.name(), connectRampDownRateDataPoint);

    SunSpecDataPoint defaultRampRateDataPoint;
    defaultRampRateDataPoint.setName("AGra");
    defaultRampRateDataPoint.setLabel("Default Ramp Rate");
    defaultRampRateDataPoint.setDescription("Ramp rate specified in percent of max current.");
    defaultRampRateDataPoint.setUnits("Pct");
    defaultRampRateDataPoint.setSize(1);
    defaultRampRateDataPoint.setAddressOffset(8);
    defaultRampRateDataPoint.setBlockOffset(6);
    defaultRampRateDataPoint.setScaleFactorName("Rmp_SF");
    defaultRampRateDataPoint.setSunSpecDataType("uint16");
    defaultRampRateDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(defaultRampRateDataPoint.name(), defaultRampRateDataPoint);

    SunSpecDataPoint rampRateScaleFactorDataPoint;
    rampRateScaleFactorDataPoint.setName("Rmp_SF");
    rampRateScaleFactorDataPoint.setLabel("Ramp Rate Scale Factor");
    rampRateScaleFactorDataPoint.setDescription("Ramp Rate Scale Factor");
    rampRateScaleFactorDataPoint.setSize(1);
    rampRateScaleFactorDataPoint.setAddressOffset(9);
    rampRateScaleFactorDataPoint.setBlockOffset(7);
    rampRateScaleFactorDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(rampRateScaleFactorDataPoint.name(), rampRateScaleFactorDataPoint);

}

QDebug operator<<(QDebug debug, SunSpecExtSettingsModel *model)
{
    debug.nospace().noquote() << "SunSpecExtSettingsModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    if (model->dataPoints().value("NomRmpUpRte").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NomRmpUpRte") << "--> " << model->rampUpRate() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NomRmpUpRte") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("NomRmpDnRte").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NomRmpDnRte") << "--> " << model->nomRmpDnRte() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NomRmpDnRte") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("EmgRmpUpRte").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("EmgRmpUpRte") << "--> " << model->emergencyRampUpRate() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("EmgRmpUpRte") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("EmgRmpDnRte").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("EmgRmpDnRte") << "--> " << model->emergencyRampDownRate() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("EmgRmpDnRte") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ConnRmpUpRte").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ConnRmpUpRte") << "--> " << model->connectRampUpRate() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ConnRmpUpRte") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ConnRmpDnRte").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ConnRmpDnRte") << "--> " << model->connectRampDownRate() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ConnRmpDnRte") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("AGra").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("AGra") << "--> " << model->defaultRampRate() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("AGra") << "--> NaN" << endl;
    }


    return debug.space().quote();
}
