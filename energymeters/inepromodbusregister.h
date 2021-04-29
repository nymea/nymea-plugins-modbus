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

#ifndef INEPROMODBUSREGISTER_H
#define INEPROMODBUSREGISTER_H
#include "registerdescriptor.h"

static const QHash<ModbusRegisterType, ModbusRegisterDescriptor> pro380RegisterMap {
    {ModbusRegisterType::VoltageL1,        ModbusRegisterDescriptor(0x5002, 3, 2, "V", "float")}, //L1 Voltage
    {ModbusRegisterType::VoltageL2,        ModbusRegisterDescriptor(0x5004, 3, 2, "V", "float")}, //L2 Voltage
    {ModbusRegisterType::VoltageL3,        ModbusRegisterDescriptor(0x5006, 3, 2, "V", "float")}, //L3 Voltage
    {ModbusRegisterType::CurrentL1,        ModbusRegisterDescriptor(0x500C, 3, 2, "A", "float")}, //L1 Current
    {ModbusRegisterType::CurrentL2,        ModbusRegisterDescriptor(0x500E, 3, 2, "A", "float")}, //L2 Current
    {ModbusRegisterType::CurrentL3,        ModbusRegisterDescriptor(0x5010, 3, 2, "A", "float")}, //L3 Current
    {ModbusRegisterType::ActivePower,    ModbusRegisterDescriptor(0x5012, 3, 2, "kW", "float")}, //Total active power
    {ModbusRegisterType::Frequency,      ModbusRegisterDescriptor(0x5008, 3, 2, "Hz", "float")},
    {ModbusRegisterType::PowerFactor,    ModbusRegisterDescriptor(0x502A, 3, 2, "Degree", "float")},
    {ModbusRegisterType::EnergyConsumed, ModbusRegisterDescriptor(0x600C, 3, 2, "kWh", "float")}, //Forward active energy
    {ModbusRegisterType::EnergyProduced, ModbusRegisterDescriptor(0x6018, 3, 2, "kWh", "float")} //Reverse active energy
};

#endif //INEPROMODBUSREGISTER_H
