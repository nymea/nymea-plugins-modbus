/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
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

#include "energymeter.h"
#include "hardware/modbus/modbusrtureply.h"

#include "extern-plugininfo.h"

EnergyMeter::EnergyMeter(ModbusRtuMaster *modbusMaster, int slaveAddress, const QHash<ModbusRegisterType, ModbusRegisterDescriptor> &modbusRegisters, QObject *parent) :
    QObject(parent),
    m_modbusRtuMaster(modbusMaster),
    m_slaveAddress(slaveAddress),
    m_modbusRegisters(modbusRegisters)
{

}

bool EnergyMeter::init()
{
    return true;
}

bool EnergyMeter::connected()
{
    return m_connected;
}

bool EnergyMeter::getVoltage()
{
    if (!m_modbusRegisters.contains(ModbusRegisterType::Voltage))
        return false;

    ModbusRegisterDescriptor descriptor = m_modbusRegisters.value(ModbusRegisterType::Voltage);
    getRegister(ModbusRegisterType::Voltage, descriptor);
    return true;
}

bool EnergyMeter::getCurrent()
{
    if (!m_modbusRegisters.contains(ModbusRegisterType::Current))
        return false;

    ModbusRegisterDescriptor descriptor = m_modbusRegisters.value(ModbusRegisterType::Current);
    getRegister(ModbusRegisterType::Current, descriptor);
    return true;
}

bool EnergyMeter::getFrequency()
{
    if (!m_modbusRegisters.contains(ModbusRegisterType::Frequency))
        return false;

    ModbusRegisterDescriptor descriptor = m_modbusRegisters.value(ModbusRegisterType::Frequency);
    getRegister(ModbusRegisterType::Frequency, descriptor);
    return true;
}

bool EnergyMeter::getPowerFactor()
{
    if (!m_modbusRegisters.contains(ModbusRegisterType::PowerFactor))
        return false;

    ModbusRegisterDescriptor descriptor = m_modbusRegisters.value(ModbusRegisterType::PowerFactor);
    getRegister(ModbusRegisterType::PowerFactor, descriptor);
    return true;
}

bool EnergyMeter::getActivePower()
{
    if (!m_modbusRegisters.contains(ModbusRegisterType::ActivePower))
        return false;

    ModbusRegisterDescriptor descriptor = m_modbusRegisters.value(ModbusRegisterType::ActivePower);
    getRegister(ModbusRegisterType::ActivePower, descriptor);
    return true;
}

bool EnergyMeter::getEnergyProduced()
{
    if (!m_modbusRegisters.contains(ModbusRegisterType::EnergyProduced))
        return false;

    ModbusRegisterDescriptor descriptor = m_modbusRegisters.value(ModbusRegisterType::EnergyProduced);
    getRegister(ModbusRegisterType::EnergyProduced, descriptor);
    return true;
}

bool EnergyMeter::getEnergyConsumed()
{
    if (!m_modbusRegisters.contains(ModbusRegisterType::EnergyConsumed))
        return false;

    ModbusRegisterDescriptor descriptor = m_modbusRegisters.value(ModbusRegisterType::EnergyConsumed);
    getRegister(ModbusRegisterType::EnergyConsumed, descriptor);
    return true;
}

void EnergyMeter::getRegister(ModbusRegisterType type, ModbusRegisterDescriptor descriptor)
{

    ModbusRtuReply *reply = nullptr;
    if (descriptor.functionCode() == 3){
        reply = m_modbusRtuMaster->readHoldingRegister(m_slaveAddress, descriptor.address(), descriptor.length());
    } else if (descriptor.functionCode() == 4){
        reply = m_modbusRtuMaster->readInputRegister(m_slaveAddress, descriptor.address(), descriptor.length());
    }
    connect(reply, &ModbusRtuReply::finished, reply, &ModbusRtuReply::deleteLater);
    connect(reply, &ModbusRtuReply::finished, this, [reply, type, descriptor, this] {
        if (reply->error() != ModbusRtuReply::NoError) {
            return;
        }
        double value = 0;
        if (reply->result().length() == 1) {
            value = static_cast<float>(reply->result().at(0));
        } else if (reply->result().length() == 2) {
            if (descriptor.dataType() == "float") {
            value = static_cast<float>(reply->result().at(0) << 16 | reply->result().at(1));
            } else {
                qCWarning(dcEnergyMeters()) << "Data type not supported" << descriptor.dataType();
            }
        }

        if (type == ModbusRegisterType::Voltage) {
            if (descriptor.unit() == "mV")
                value /= 1000;

            emit voltageReceived(value);
        } else if (type == ModbusRegisterType::Current) {
            if (descriptor.unit() == "mA")
                value /= 1000;

            emit currentReceived(value);
        } else if (type == ModbusRegisterType::ActivePower) {
            if (descriptor.unit() == "kW") {
                value *= 1000;
            } else if (descriptor.unit() == "mW") {
                value /= 1000;
            }
            emit activePowerReceived(value);
        } else if (type == ModbusRegisterType::PowerFactor) {
            emit powerFactorReceived(value);
        } else if (type == ModbusRegisterType::Frequency) {
            emit frequencyReceived(value);
        } else if (type == ModbusRegisterType::EnergyConsumed) {
            if (descriptor.unit() == "Wh") {
                value /= 1000;
            }
            emit consumedEnergyReceived(value);
        } else if (type == ModbusRegisterType::EnergyProduced) {
            if (descriptor.unit() == "Wh") {
                value /= 1000;
            }
            emit producedEnergyReceived(value);
        }
    });
}
