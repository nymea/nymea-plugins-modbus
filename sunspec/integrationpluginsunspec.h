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

#ifndef INTEGRATIONPLUGINSUNSPEC_H
#define INTEGRATIONPLUGINSUNSPEC_H

#include <integrations/integrationplugin.h>
#include <plugintimer.h>

#include <sunspecconnection.h>
#include <models/sunspecmodelfactory.h>

#include "sunspecthing.h"

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
    QHash<ThingClassId, ParamTypeId> m_connectionIpParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionPortParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionMacAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionSlaveIdParamTypeIds;

    // SunSpec thing params map
    QHash<ThingClassId, ParamTypeId> m_modelIdParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_modbusAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_manufacturerParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_deviceModelParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_serialNumberParamTypeIds;

    PluginTimer *m_refreshTimer = nullptr;

    QHash<ThingId, SunSpecConnection *> m_sunSpecConnections;
    QHash<Thing *, SunSpecThing *> m_sunSpecThings;

    QHash<Thing *, SunSpecModel *> m_sunSpecInverters;
    QHash<Thing *, SunSpecModel *> m_sunSpecMeters;
    QHash<Thing *, SunSpecModel *> m_sunSpecStorages;

    bool sunspecThingAlreadyAdded(uint modelId, uint modbusAddress, const ThingId &parentId);
    void processDiscoveryResult(Thing *thing, SunSpecConnection *connection);
    void checkAutoSetupModels(Thing *connectionThing, QList<SunSpecModel *> models);

    // SunSpec things
    void setupConnection(ThingSetupInfo *info);
    void setupInverter(ThingSetupInfo *info);
    void setupMeter(ThingSetupInfo *info);
    void setupStorage(ThingSetupInfo *info);

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


private slots:
    void onRefreshTimer();
    void onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value);

    void onInverterBlockUpdated();
    void onMeterBlockUpdated();
    void onStorageBlockUpdated();
    void onSolarEdgeBatteryBlockUpdated();

};
#endif // INTEGRATIONPLUGINSUNSPEC_H

