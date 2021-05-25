/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#ifndef BGETECHMODBUSREGISTER_H
#define BGETECHMODBUSREGISTER_H

#include "registerdescriptor.h"

static const QHash<ModbusRegisterType, ModbusRegisterDescriptor> sdm630RegisterMap {
    {ModbusRegisterType::VoltageL1,      ModbusRegisterDescriptor(0x00, 4, 2, "V", "float")},
    {ModbusRegisterType::VoltageL2,      ModbusRegisterDescriptor(0x02, 4, 2, "V", "float")},
    {ModbusRegisterType::VoltageL3,      ModbusRegisterDescriptor(0x04, 4, 2, "V", "float")},
    {ModbusRegisterType::CurrentL1,      ModbusRegisterDescriptor(0x06, 4, 2, "A", "float")},
    {ModbusRegisterType::CurrentL2,      ModbusRegisterDescriptor(0x08, 4, 2, "A", "float")},
    {ModbusRegisterType::CurrentL3,      ModbusRegisterDescriptor(0x0A, 4, 2, "A", "float")},
    {ModbusRegisterType::ActivePower,    ModbusRegisterDescriptor(0x34, 4, 2, "W", "float")}, //Total system power
    {ModbusRegisterType::Frequency,      ModbusRegisterDescriptor(0x46, 4, 2, "Hz", "float")},
    {ModbusRegisterType::PowerFactor,    ModbusRegisterDescriptor(0x3E, 4, 2, "", "float")}, //Total system power factor
    {ModbusRegisterType::EnergyConsumed, ModbusRegisterDescriptor(0x48, 4, 2, "kWh", "float")}, //Total Import kWh
    {ModbusRegisterType::EnergyProduced, ModbusRegisterDescriptor(0x4A, 4, 2, "kWh", "float")} //Total Export kWh
};

#endif // BGETECHMODBUSREGISTER_H
