/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2023, nymea GmbH
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
