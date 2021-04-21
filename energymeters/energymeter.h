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

#ifndef ENERGYMETER_H
#define ENERGYMETER_H

#include <QObject>

#include "registerdescriptor.h"
#include "hardware/modbus/modbusrtumaster.h"

class EnergyMeter : public QObject
{
    Q_OBJECT
public:
    explicit EnergyMeter(ModbusRtuMaster *modbusMaster, int slaveAddress, const QHash<ModbusRegisterType, ModbusRegisterDescriptor> &modbusRegisters, QObject *parent = nullptr);
    bool init();

    bool connected();
    bool getVoltage();
    bool getCurrent();
    bool getFrequency();
    bool getPowerFactor();
    bool getActivePower();
    bool getEnergyProduced();
    bool getEnergyConsumed();

private:
    bool m_connected = false;

    ModbusRtuMaster *m_modbusRtuMaster = nullptr;
    int m_slaveAddress;

    QHash<ModbusRegisterType, ModbusRegisterDescriptor> m_modbusRegisters;

    void getRegister(ModbusRegisterType type, ModbusRegisterDescriptor descriptor);

signals:
    void connectedChanged(bool connected);

    void voltageReceived(double voltage);
    void currentReceived(double current);
    void frequencyReceived(double freqeuncy);
    void activePowerReceived(double power);
    void powerFactorReceived(double powerFactor);
    void producedEnergyReceived(double energy);
    void consumedEnergyReceived(double energy);

//private slot:
//    void onRegisterReceived();
};

#endif // ENERGYMETER_H
