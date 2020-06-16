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

#ifndef INTEGRATIONPLUGINMODBUSCOMMANDER_H
#define INTEGRATIONPLUGINMODBUSCOMMANDER_H

#include "integrations/integrationplugin.h"
#include "plugintimer.h"

#include "../modbus/modbustcpmaster.h"
#include "../modbus/modbusrtumaster.h"

#include <QSerialPortInfo>
#include <QUuid>

class IntegrationPluginModbusCommander: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginmodbuscommander.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginModbusCommander();

    void init() override;
    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *info) override;
    void executeAction(ThingActionInfo *info) override;
    void thingRemoved(Thing *thing) override;

private:
    PluginTimer *m_refreshTimer = nullptr;

    QHash<Thing*, ModbusRTUMaster*> m_modbusRTUMasters;
    QHash<Thing*, ModbusTCPMaster*> m_modbusTCPMasters;
    QHash<QUuid, ThingActionInfo*> m_asyncActions;
    QHash<QUuid, Thing*> m_readRequests;

    QHash<ModbusRTUMaster*, ThingSetupInfo *> m_asyncRTUSetup;
    QHash<ModbusTCPMaster*, ThingSetupInfo *> m_asyncTCPSetup;

    void readRegister(Thing *thing);
    void writeRegister(Thing *thing, ThingActionInfo *info);

    QHash<ThingClassId, ParamTypeId> m_slaveAddressParamTypeId;
    QHash<ThingClassId, ParamTypeId> m_registerAddressParamTypeId;
    QHash<ThingClassId, StateTypeId> m_connectedStateTypeId;
    QHash<ThingClassId, StateTypeId> m_valueStateTypeId;

private slots:
    void onRefreshTimer();

    void onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value);

    void onConnectionStateChanged(bool status);
    void onRequestExecuted(QUuid requestId, bool success);
    void onRequestError(QUuid requestId, const QString &error);
    void onReceivedCoil(quint32 slaveAddress, quint32 modbusRegister, bool value);
    void onReceivedDiscreteInput(quint32 slaveAddress, quint32 modbusRegister, bool value);
    void onReceivedHoldingRegister(quint32 slaveAddress, quint32 modbusRegister, int value);
    void onReceivedInputRegister(quint32 slaveAddress, quint32 modbusRegister, int value);
};

#endif // INTEGRATIONPLUGINMODBUSCOMMANDER_H
