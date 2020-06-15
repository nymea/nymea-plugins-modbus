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

#ifndef INTEGRATIONPLUGINDREXELUNDWEISS_H
#define INTEGRATIONPLUGINDREXELUNDWEISS_H

#include "integrations/integrationplugin.h"
#include "plugintimer.h"

#include <QDateTime>
#include "modbusrtumaster.h"
#include <QSerialPortInfo>

class DevicePluginDrexelUndWeiss : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationplugindrexelundweiss.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit DevicePluginDrexelUndWeiss();
    ~DevicePluginDrexelUndWeiss() override;
    void init() override;

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    QList<QString> m_usedSerialPorts;
    QHash<Thing *, ModbusRTUMaster *> m_modbusRTUMasters;
    PluginTimer *m_refreshTimer = nullptr;
    QHash<QUuid, ThingActionInfo *> m_pendingActions;

    void updateStates(Device *device);
    void discoverModbusSlaves(ModbusRTUMaster *modbus, int slaveAddress);

private slots:
    void onRefreshTimer();
    void onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value);

    void onConnectionStateChanged(bool status);
    void onReceivedCoil(int slaveAddress, int modbusRegister, bool value);
    void onReceivedDiscreteInput(int slaveAddress, int modbusRegister, bool value);
    void onReceivedHoldingRegister(int slaveAddress, int modbusRegister, int value);
    void onReceivedInputRegister(int slaveAddress, int modbusRegister, int value);

    void onWriteRequestFinished(QUuid requestId, bool success);
};

#endif // INTEGRATIONPLUGINDREXELUNDWEISS_H
