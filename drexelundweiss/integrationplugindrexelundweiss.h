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

#ifndef INTEGRATIONPLUGINDREXELUNDWEISS_H
#define INTEGRATIONPLUGINDREXELUNDWEISS_H

#include <integrations/integrationplugin.h>
#include <hardware/modbus/modbusrtumaster.h>
#include <plugintimer.h>

#include "modbusregisterdefinition.h"

#include <QDateTime>

class IntegrationPluginDrexelUndWeiss : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationplugindrexelundweiss.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginDrexelUndWeiss();

    void init() override;
    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    QHash<Thing *, ModbusRtuMaster *> m_modbusRtuMasters;
    PluginTimer *m_refreshTimer = nullptr;

    QHash<ThingClassId, StateTypeId> m_connectedStateTypeIds;
    QHash<ThingClassId, ParamTypeId> m_discoverySlaveAddressParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_slaveIdParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_modbusUuidParamTypeIds;
    void sendWriteRequest(ThingActionInfo *info, uint slaveAddress, uint modbusRegister, uint32_t value);
    void updateStates(Thing *thing);
    void discoverModbusSlaves(ModbusRtuMaster *modbus, uint slaveAddress);
    void readHoldingRegister(Thing *thing, ModbusRtuMaster *modbus, uint slaveAddress, uint modbusRegister);

    VentilationMode getVentilationModeFromString(const QString &modeString);

    void readX2Energy(Thing *thing, ModbusRtuMaster *modbus, uint slaveAddress);
    void readX2Power(Thing *thing, ModbusRtuMaster *modbus, uint slaveAddress);

private slots:
    void onRefreshTimer();
    void onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value);
    void onConnectionStateChanged(bool status);
};

#endif // INTEGRATIONPLUGINDREXELUNDWEISS_H
