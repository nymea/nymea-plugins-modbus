// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2026, chargebyte austria GmbH
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

#ifndef INTEGRATIONPLUGINPCELECTRIC_H
#define INTEGRATIONPLUGINPCELECTRIC_H

#include <QDebug>
#include <QObject>
#include <QSslCertificate>
#include <QSslKey>

#include <functional>

#include <integrations/integrationplugin.h>
#include <network/networkdevicediscovery.h>
#include <network/zeroconf/zeroconfserviceentry.h>
#include <plugintimer.h>

#include "extern-plugininfo.h"
#include "pcewallbox.h"

class ZeroConfServiceBrowser;
class ZeroConfServiceEntry;
class QProcess;
class QTemporaryDir;

class IntegrationPluginPcElectric : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginpcelectric.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginPcElectric();
    void init() override;

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    PluginTimer *m_refreshTimer = nullptr;

    QHash<Thing *, PceWallbox *> m_connections;
    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;
    QHash<Thing *, bool> m_initialUpdate;

    // We need to buffer the desired power / current / phase count states because all 3 states
    // will be represented by one register (200 - chaegingCurrent). If all 3 actions get executed, they might
    // overwrite each other, since the action gets started right the way, but the request gets queued.
    // If the actions would be queued, there would be still the issue with the order of the actions
    // (set power to false and then set charging current would always enable charging in the end).
    QHash<Thing *, PceWallbox::ChargingCurrentState> m_chargingCurrentStateBuffer;

    QHash<ThingClassId, ParamTypeId> m_addressParamTypes;
    QHash<ThingClassId, ParamTypeId> m_hostNameParamTypes;
    QHash<ThingClassId, ParamTypeId> m_macParamTypes;
    QHash<ThingClassId, ParamTypeId> m_serialNumberParamTypes;
    QHash<ThingClassId, ParamTypeId> m_tlsAdvertisedParamTypes;

    ZeroConfServiceBrowser *m_modbusServiceBrowser = nullptr;
    ZeroConfServiceBrowser *m_modbusTlsServiceBrowser = nullptr;

    void setupConnection(ThingSetupInfo *info, const QHostAddress &address, NetworkDeviceMonitor *monitor = nullptr,
                         const ZeroConfServiceEntry &serviceEntry = ZeroConfServiceEntry());
    bool isZeroConfManaged(Thing *thing) const;
    bool isMatchingZeroConfService(Thing *thing, const ZeroConfServiceEntry &entry) const;
    ZeroConfServiceEntry findZeroConfService(Thing *thing, bool tlsPreferred = true) const;
    bool configureTls(Thing *thing, PceWallbox *connection, const ZeroConfServiceEntry &entry = ZeroConfServiceEntry());
    bool ensureClientIdentity(QString *errorString = nullptr);
    void ensureClientIdentityAsync(const std::function<void(bool, const QString &)> &callback);
    QString resolvedIdentityPath(const QString &configuredPath) const;
    QString storagePrefix(Thing *thing) const;
    QString wallboxSerialNumber(PceWallbox *connection) const;
    void handleZeroConfServiceAdded(const ZeroConfServiceEntry &entry);
    void handleZeroConfServiceRemoved(const ZeroConfServiceEntry &entry);

    QSslCertificate m_clientCertificate;
    QSslKey m_clientPrivateKey;
    QProcess *m_identityProcess = nullptr;
    QTemporaryDir *m_identityTemporaryDirectory = nullptr;
    QList<std::function<void(bool, const QString &)>> m_identityCallbacks;
};

#endif // INTEGRATIONPLUGINPCELECTRIC_H
