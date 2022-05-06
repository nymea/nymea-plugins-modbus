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

#ifndef INTEGRATIONPLUGINWALLBE_H
#define INTEGRATIONPLUGINWALLBE_H

#include <integrations/integrationplugin.h>
#include <plugintimer.h>

#include <modbustcpmaster.h>

#include <QObject>
#include <QHostAddress>

class IntegrationPluginWallbe : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginwallbe.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    enum WallbeRegisterAddress {
        EVStatus        = 100,
        ChargingTime    = 102,
        FirmwareVersion = 105,
        ErrorCode       = 107,
        ChargingCurrent = 300,
        EnableCharging  = 400,
        Output1         = 405,
        Output2         = 406,
        Output3         = 407,
        Output4         = 408
    };
    Q_ENUM(WallbeRegisterAddress)

    explicit IntegrationPluginWallbe();
    void init() override;
    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;
    void thingRemoved(Thing *thing) override;

private:
    QHash<Thing *, ModbusTCPMaster *> m_connections;
    PluginTimer *m_pluginTimer = nullptr;
    QHash<QUuid, ThingActionInfo *> m_asyncActions;
    int m_slaveAddress = 180;

    void update(Thing *thing);

private slots:
    void onConnectionStateChanged(bool status);
    void onReceivedInputRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value);
    void onReceivedCoil(int slaveAddress, int modbusRegister, const QVector<quint16> &value);
    void onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value);

    void onWriteRequestExecuted(const QUuid &requestId, bool success);
    void onWriteRequestError(const QUuid &requestId, const QString &error);
};

#endif // INTEGRATIONPLUGINWALLBE_H
