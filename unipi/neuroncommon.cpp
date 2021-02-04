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

#include "neuroncommon.h"

NeuronCommon::NeuronCommon(QObject *parent) : QObject(parent)
{

}

QList<QString> NeuronCommon::digitalInputs()
{
    return m_modbusDigitalInputRegisters.keys();
}

QList<QString> NeuronCommon::digitalOutputs()
{
    return m_modbusDigitalOutputRegisters.keys();
}

QList<QString> NeuronCommon::analogInputs()
{
    QList<QString> circuits;
    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogInputRegisters.values()) {
        circuits.append(descriptor.circuit);
    }
    return circuits;
}

QList<QString> NeuronCommon::analogOutputs()
{
    QList<QString> circuits;
    Q_FOREACH(RegisterDescriptor descriptor, m_modbusAnalogOutputRegisters.values()) {
        circuits.append(descriptor.circuit);
    }
    return circuits;
}

QList<QString> NeuronCommon::userLEDs()
{
    return m_modbusUserLEDRegisters.keys();
}

NeuronCommon::RegisterDescriptor NeuronCommon::registerDescriptorFromStringList(const QStringList &data)
{
    RegisterDescriptor descriptor;
    if (data.count() < 7) {
        return descriptor;
    }
    descriptor.address = data[0].toInt();
    descriptor.count = data[2].toInt();
    if (data[3] == "RW") {
        descriptor.readWrite = RWPermissionReadWrite;
    } else if (data[3] == "W") {
        descriptor.readWrite = RWPermissionWrite;
    } else if (data[3] == "R") {
        descriptor.readWrite = RWPermissionRead;
    }
    descriptor.circuit = data[5].split(" ").last();
    descriptor.category = data.last();

    if (data[5].contains("Analog Input Value", Qt::CaseSensitivity::CaseInsensitive)) {
        descriptor.registerType = QModbusDataUnit::RegisterType::InputRegisters;
    } else if (data[5].contains("Analog Output Value", Qt::CaseSensitivity::CaseInsensitive)) {
        descriptor.registerType = QModbusDataUnit::RegisterType::HoldingRegisters;
    }
    return descriptor;
}

bool NeuronCommon::circuitValueChanged(const QString &circuit, quint32 value)
{
    if (m_previousCircuitValue.contains(circuit)) {
        if (m_previousCircuitValue.value(circuit) == value) {
            // Only emit status change of the circuit value has changed
            return false;
        } else  {
            m_previousCircuitValue.insert(circuit, value); //update existing value
            return true;
        }
    } else {
        m_previousCircuitValue.insert(circuit, value);
        return true;
    }
}
