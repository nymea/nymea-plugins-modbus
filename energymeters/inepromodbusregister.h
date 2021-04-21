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

#ifndef INEPROMODBUSREGISTER_H
#define INEPROMODBUSREGISTER_H
#include "registerdescriptor.h"

static const QHash<ModbusRegisterType, ModbusRegisterDescriptor> pro380RegisterMap {
    {ModbusRegisterType::Voltage,        ModbusRegisterDescriptor(20482, 3, 2, "V", "float")}, //L1 Voltage
    {ModbusRegisterType::Current,        ModbusRegisterDescriptor(20492, 3, 2, "A", "float")}, //L1 Current
    {ModbusRegisterType::ActivePower,    ModbusRegisterDescriptor(20498, 3, 2, "kW", "float")}, //Total active power
    {ModbusRegisterType::Frequency,      ModbusRegisterDescriptor(20488, 3, 2, "Hz", "float")},
    {ModbusRegisterType::PowerFactor,    ModbusRegisterDescriptor(20522, 3, 2, "Degree", "float")},
    {ModbusRegisterType::EnergyConsumed, ModbusRegisterDescriptor(24588, 3, 2, "kWh", "float")}, //Forward active energy
    {ModbusRegisterType::EnergyProduced, ModbusRegisterDescriptor(24600, 3, 2, "kWh", "float")} //Reverse active energy
};

#endif //INEPROMODBUSREGISTER_H
