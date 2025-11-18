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

#ifndef INTEGRATIONPLUGINSCHRACK_H
#define INTEGRATIONPLUGINSCHRACK_H

#include <integrations/integrationplugin.h>
#include <hardware/modbus/modbusrtuhardwareresource.h>
#include <plugintimer.h>

#include "cionmodbusrtuconnection.h"

#include "extern-plugininfo.h"

#include <QObject>
#include <QTimer>

class IntegrationPluginSchrack : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginschrack.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    enum StatusBit {
        StatusBitPluggedIn = 0x0001,
        StatusBitChargeContactor1Active = 0x0002,
        StatusBitChargeContactor2Active = 0x0004,
        StatusBitVentilationRequired = 0x0008,
        StatusBitPlugLockController = 0x0010,
        StatusBitPlugLockReturn = 0x0020,
        StatusBitCollectiveDisorder = 0x0040,
        StatusBitDisorderFiLs = 0x0080,
        StatusBitCableDisorder = 0x0100,
        StatusBitCableRejected = 0x0200,
        StatusBitContactorError = 0x0400
    };
    Q_ENUM(StatusBit);
    Q_DECLARE_FLAGS(StatusBits, StatusBit)

    explicit IntegrationPluginSchrack();
    void init() override;
    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    void waitForActionFinish(ThingActionInfo *info, ModbusRtuReply *reply, const StateTypeId &stateTypeId, const QVariant &value);
    void updatePhaseCount(Thing *thing, const QString &phases);

private:
    PluginTimer *m_refreshTimer = nullptr;
    QHash<Thing *, CionModbusRtuConnection *> m_cionConnections;
};

#endif // INTEGRATIONPLUGINSCHRACK_H
