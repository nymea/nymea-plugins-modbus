// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef INTEGRATIONPLUGINDREXELUNDWEISS_H
#define INTEGRATIONPLUGINDREXELUNDWEISS_H

#include <integrations/integrationplugin.h>
#include <hardware/modbus/modbusrtumaster.h>
#include <plugintimer.h>

#include "modbusregisterdefinition.h"

#include <QDateTime>

class IntegrationPluginDrexelUndWeiss : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationplugindrexelundweiss.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginDrexelUndWeiss();

    void init() override;
    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    QHash<Thing *, ModbusRtuMaster *> m_modbusRtuMasters;
    PluginTimer *m_refreshTimer = nullptr;

    QHash<ThingClassId, StateTypeId> m_connectedStateTypeIds;
    QHash<ThingClassId, ParamTypeId> m_discoverySlaveAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_slaveIdParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_modbusUuidParamTypeIds;
    void sendWriteRequest(ThingActionInfo *info, uint slaveAddress, uint modbusRegister, uint32_t value);
    void updateStates(Thing *thing);
    void discoverModbusSlaves(ModbusRtuMaster *modbus, uint slaveAddress);
    void readHoldingRegister(Thing *thing, ModbusRtuMaster *modbus, uint slaveAddress, uint modbusRegister);

    VentilationMode getVentilationModeFromString(const QString &modeString);

    void readX2Energy(Thing *thing, ModbusRtuMaster *modbus, uint slaveAddress);
    void readX2Power(Thing *thing, ModbusRtuMaster *modbus, uint slaveAddress);

private slots:
    void onRefreshTimer();
    void onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value);
    void onConnectionStateChanged(bool status);
};

#endif // INTEGRATIONPLUGINDREXELUNDWEISS_H
