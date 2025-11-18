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

#ifndef INTEGRATIONPLUGINSUNSPEC_H
#define INTEGRATIONPLUGINSUNSPEC_H

#include <integrations/integrationplugin.h>
#include <network/networkdevicemonitor.h>
#include <plugintimer.h>

#include <sunspecconnection.h>
#include <models/sunspecmodelfactory.h>

#include "sunspecthing.h"
#include "extern-plugininfo.h"

#include <QUuid>

class IntegrationPluginSunSpec: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginsunspec.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginSunSpec();

    void init() override;
    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    // SunSpec Connection params map
    QHash<ThingClassId, ParamTypeId> m_connectionDiscoverySlaveIdParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionMacAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionHostNameParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionPortParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionSlaveIdParamTypeIds;

    // SunSpec thing params map
    QHash<ThingClassId, ParamTypeId> m_modelIdParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_modbusAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_manufacturerParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_deviceModelParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_serialNumberParamTypeIds;

    PluginTimer *m_refreshTimer = nullptr;

    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;

    QHash<ThingId, SunSpecConnection *> m_sunSpecConnections;
    QHash<Thing *, SunSpecThing *> m_sunSpecThings;

    QHash<Thing *, SunSpecModel *> m_sunSpecInverters;
    QHash<Thing *, SunSpecModel *> m_sunSpecMeters;
    QHash<Thing *, SunSpecModel *> m_sunSpecStorages;


    Thing *getThingForSunSpecModel(uint modelId, uint modbusAddress, const ThingId &parentId);
    bool sunspecThingAlreadyAdded(uint modelId, uint modbusAddress, const ThingId &parentId);
    void processDiscoveryResult(Thing *thing, SunSpecConnection *connection);

    // SunSpec things
    SunSpecConnection *createConnection(Thing *thing);

    // Custom types
    void setupSolarEdgeBattery(ThingSetupInfo *info);

    // Discover SolarEdge batteries
    void searchSolarEdgeBatteries(SunSpecConnection *connection);
    void searchSolarEdgeBattery(SunSpecConnection *connection, const ThingId &parentThingId, quint16 startRegister);

    // SolarEdge
    double calculateSolarEdgePvProduction(Thing *thing, double acPower, double dcPower);

    void autocreateSunSpecModelThing(const ThingClassId &thingClassId, const QString &thingName, const ThingId &parentId, SunSpecModel *model);

    QString getInverterStateString(quint16 state);
    QString getInverterErrorString(quint32 flag);

    double fixValueSign(double targetValue, double powerValue);
    bool hasManufacturer(const QStringList &manufacturers, const QString &manufacturer);
    void markThingStatesDisconnected(Thing *thing);

private slots:
    void onRefreshTimer();
    void onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value);

    void onInverterBlockUpdated();
    void onMeterBlockUpdated();
    void onStorageBlockUpdated();
    void onSolarEdgeBatteryBlockUpdated();

    void evaluateEnergyProducedValue(Thing *inverterThing, float energyProduced);


};
#endif // INTEGRATIONPLUGINSUNSPEC_H

