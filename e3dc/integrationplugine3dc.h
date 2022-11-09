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

#ifndef INTEGRATIONPLUGINE3DC_H
#define INTEGRATIONPLUGINE3DC_H




#include <integrations/integrationplugin.h>
#include <plugintimer.h>
#include "tcp_modbusconnection.h"
#include "e3dcinverter.h"
#include "e3dcbattery.h"
#include "e3dcwallbox.h"
#include "e3dcsmartmeter.h"


class IntegrationPluginTemplate: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationplugine3dc.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginTemplate();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;


private:


    // e3dc things
    void setupConnection(ThingSetupInfo *info);
    void setupInverter(ThingSetupInfo *info);
    void setupWallbox(ThingSetupInfo *info);
    void setupBattery(ThingSetupInfo *info);
    void setupSmartMeter(ThingSetupInfo *info);


    void processDiscoveryResult(Thing *thing, TCP_ModbusConnection *TcpConnection);

    PluginTimer *m_pluginTimer = nullptr;
    QHash<Thing *, TCP_ModbusConnection *> m_templateTcpThings;

    // Connection State
    QHash<ThingClassId, StateTypeId> m_connectedStateTypeIds;

    // Connection Param
    QHash<ThingClassId, ParamTypeId> m_connectionIpParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionPortParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionMacAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_connectionSlaveIdParamTypeIds;

    // params map
    QHash<ThingClassId, ParamTypeId> m_modelIdParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_modbusAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_manufacturerParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_deviceModelParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_serialNumberParamTypeIds;


    // Collection of Devices
    QHash<ThingId, TCP_ModbusConnection *> m_e3DCConnections;
    QHash<Thing *, e3dcInverter *> m_e3DCInverters;
    QHash<Thing *, e3dcBattery *> m_e3DCBatteries;
    QHash<Thing *, e3dcWallbox *> m_e3DCWallboxes;
    QHash<Thing *, e3dcSmartMeter *> m_e3DCSmartmeter;


private slots:
    void onPluginTimer();
//    void onInverterUpdated();
//    void onWallboxUpdated();
//    void onBatteryUpdated();

};

#endif // INTEGRATIONPLUGINE3DC_H


