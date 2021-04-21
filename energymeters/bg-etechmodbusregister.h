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

#ifndef BGETECHMODBUSREGISTER_H
#define BGETECHMODBUSREGISTER_H

#include "registerdescriptor.h"

static const QHash<ModbusRegisterType, ModbusRegisterDescriptor> sdm630RegisterMap {
    {ModbusRegisterType::VoltageL1,      ModbusRegisterDescriptor(30001, 4, 2, "V", "float")},
    {ModbusRegisterType::VoltageL2,      ModbusRegisterDescriptor(30003, 4, 2, "V", "float")},
    {ModbusRegisterType::VoltageL3,      ModbusRegisterDescriptor(30005, 4, 2, "V", "float")},
    {ModbusRegisterType::CurrentL1,      ModbusRegisterDescriptor(30007, 4, 2, "A", "float")},
    {ModbusRegisterType::CurrentL2,      ModbusRegisterDescriptor(30009, 4, 2, "A", "float")},
    {ModbusRegisterType::CurrentL3,      ModbusRegisterDescriptor(30011, 4, 2, "A", "float")},
    {ModbusRegisterType::ActivePower,    ModbusRegisterDescriptor(30053, 4, 2, "W", "float")}, //Total system power
    {ModbusRegisterType::Frequency,      ModbusRegisterDescriptor(30071, 4, 2, "Hz", "float")},
    {ModbusRegisterType::PowerFactor,    ModbusRegisterDescriptor(30067, 4, 2, "Degree", "float")}, //Total system phase angle
    {ModbusRegisterType::EnergyConsumed, ModbusRegisterDescriptor(30073, 4, 2, "kWh", "float")}, //Total Import kWh
    {ModbusRegisterType::EnergyProduced, ModbusRegisterDescriptor(30075, 4, 2, "kWh", "float")} //Total Export kWh
};

#endif // BGETECHMODBUSREGISTER_H
