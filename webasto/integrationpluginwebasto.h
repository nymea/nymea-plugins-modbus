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

#ifndef INTEGRATIONPLUGINWEBASTO_H
#define INTEGRATIONPLUGINWEBASTO_H

#include <plugintimer.h>
#include <integrations/integrationplugin.h>
#include <network/networkdevicemonitor.h>

#include "webasto.h"
#include "webastonextmodbustcpconnection.h"
#include "evc04modbustcpconnection.h"

#include <QUuid>
#include <QObject>
#include <QHostAddress>

class QNetworkReply;

class IntegrationPluginWebasto : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginwebasto.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginWebasto();
    void init() override;
    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    PluginTimer *m_pluginTimer = nullptr;

    QHash<QUuid, ThingActionInfo *> m_asyncActions;

    QHash<Thing *, Webasto *> m_webastoLiveConnections;
    QHash<Thing *, WebastoNextModbusTcpConnection *> m_webastoNextConnections;
    QHash<Thing *, EVC04ModbusTcpConnection *> m_evc04Connections;

    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;

    void setupWebastoNextConnection(ThingSetupInfo *info);

    void update(Webasto *webasto);
    void evaluatePhaseCount(Thing *thing);

    void executeWebastoNextPowerAction(ThingActionInfo *info, bool power);

    void setupEVC04Connection(ThingSetupInfo *info);
    void updateEVC04MaxCurrent(Thing *thing);
    QHash<Thing *, quint32> m_lastWallboxTime;
    QHash<Thing *, QPair<QString, QDateTime>> m_webastoUniteTokens;
    bool validTokenAvailable(Thing *thing);
    QNetworkReply *requestWebstoUniteAccessToken(const QHostAddress &address);
    QNetworkReply *requestWebstoUnitePhaseCountChange(const QHostAddress &address, const QString &accessToken, uint desiredPhaseCount);
    void executeWebastoUnitePhaseCountAction(ThingActionInfo *info);

private slots:
    void onWriteRequestExecuted(const QUuid &requestId, bool success);
    void onWriteRequestError(const QUuid &requestId, const QString &error);

    void onReceivedRegister(Webasto::TqModbusRegister registerAddress, const QVector<quint16> &data);
};

#endif // INTEGRATIONPLUGINWEBASTO_H
