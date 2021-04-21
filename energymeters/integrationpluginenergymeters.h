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

#ifndef INTEGRATIONPLUGINENERGYMETERS_H
#define INTEGRATIONPLUGINENERGYMETERS_H

#include "integrations/integrationplugin.h"
#include "hardware/modbus/modbusrtuhardwareresource.h"
#include "plugintimer.h"

#include "energymeter.h"

#include <QObject>
#include <QTimer>

class IntegrationPluginEnergyMeters : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginenergymeters.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginEnergyMeters();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;

private:
    QTimer *m_updateTimer = nullptr;
    QHash<ThingClassId, StateTypeId> m_connectionStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_voltageStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_currentStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_activePowerStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_frequencyStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_powerFactorStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_totalEnergyConsumedStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_totalEnergyProducedStateTypeIds;

    QHash<ThingClassId, ParamTypeId> m_discoverySlaveAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_slaveIdParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_modbusUuidParamTypeIds;

    QHash<ThingClassId, QHash<ModbusRegisterType, ModbusRegisterDescriptor>> m_registerMaps;

    QHash<Thing *, EnergyMeter *> m_energyMeters;
    PluginTimer *m_pluginTimer = nullptr;

private slots:
    void onConnectionStateChanged(bool status);
    void onVoltageReceived(double voltage);
    void onCurrentReceived(double current);
    void onActivePowerReceived(double power);
    void onFrequencyReceived(double frequency);
    void onPowerFactorReceived(double powerFactor);
    void onProducedEnergyReceived(double energy);
    void onConsumedEnergyReceived(double energy);
};

#endif // INTEGRATIONPLUGINENERGYMETERS_H
