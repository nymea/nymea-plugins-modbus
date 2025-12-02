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

#ifndef INTEGRATIONPLUGINSMA_H
#define INTEGRATIONPLUGINSMA_H

#include <integrations/integrationplugin.h>
#include <plugintimer.h>
#include <network/networkdevicemonitor.h>

#include "extern-plugininfo.h"

#include "sunnywebbox/sunnywebbox.h"
#include "speedwire/speedwiremeter.h"
#include "speedwire/speedwireinverter.h"
#include "speedwire/speedwireinterface.h"

#include "smasolarinvertermodbustcpconnection.h"
#include "smabatteryinvertermodbustcpconnection.h"

class IntegrationPluginSma: public IntegrationPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginsma.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginSma();

    void init() override;
    void discoverThings(ThingDiscoveryInfo *info) override;

    void startPairing(ThingPairingInfo *info) override;
    void confirmPairing(ThingPairingInfo *info, const QString &username, const QString &secret) override;

    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;

private slots:
    void onConnectedChanged(bool connected);
    void onPlantOverviewReceived(const QString &messageId, SunnyWebBox::Overview overview);

    void setupRefreshTimer();

    void setupModbusSolarInverterConnection(ThingSetupInfo *info);
    void setupModbusBatteryInverterConnection(ThingSetupInfo *info);

private:
    PluginTimer *m_refreshTimer = nullptr;

    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;

    QHash<Thing *, SunnyWebBox *> m_sunnyWebBoxes;
    QHash<Thing *, SpeedwireMeter *> m_speedwireMeters;
    QHash<Thing *, SpeedwireInverter *> m_speedwireInverters;
    QHash<Thing *, SmaSolarInverterModbusTcpConnection *> m_modbusSolarInverters;
    QHash<Thing *, SmaBatteryInverterModbusTcpConnection *> m_modbusBatteryInverters;

    quint32 m_localSerialNumber = 0;

    // Shared interface accross all speedwire devices
    SpeedwireInterface *m_speedwireInterface = nullptr;
    SpeedwireInterface *getSpeedwireInterface();

    void markSpeedwireMeterAsDisconnected(Thing *thing);
    void markSpeedwireInverterAsDisconnected(Thing *thing);
    void markSpeedwireBatteryAsDisconnected(Thing *thing);
    void markModbusSolarInverterAsDisconnected(Thing *thing);
    void markModbusBatteryInverterAsDisconnected(Thing *thing);

    quint64 getLocalSerialNumber();

    // Sma modbus data validation
    bool isModbusValueValid(quint32 value);
    bool isModbusValueValid(qint32 value);
    bool isModbusValueValid(quint64 value);
};

#endif // INTEGRATIONPLUGINSMA_H
