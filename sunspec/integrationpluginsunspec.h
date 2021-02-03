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

#ifndef INTEGRATIONPLUGINSUNSPEC_H
#define INTEGRATIONPLUGINSUNSPEC_H

#include "integrations/integrationplugin.h"
#include "plugintimer.h"

#include "../modbus/modbustcpmaster.h"

#include "sunspecinverter.h"
#include "sunspecstorage.h"
#include "sunspecmeter.h"

#include <QUuid>

class IntegrationPluginSunSpec: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginsunspec.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginSunSpec();
    void init() override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    QHash<ThingClassId, ParamTypeId> m_modelIdParamTypeIds;
    QHash<ThingClassId, ParamTypeId> m_modbusAddressParamTypeIds;

    QHash<ThingClassId, StateTypeId> m_connectedStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_frequencyStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_acPowerStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_acEnergyStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_inverterOperatingStateTypeIds;
    QHash<ThingClassId, StateTypeId> m_inverterErrorStateTypeIds;

    PluginTimer *m_refreshTimer = nullptr;
    QHash<QUuid, ThingActionInfo *> m_asyncActions;
    QHash<ThingId, SunSpec *> m_sunSpecConnections;

    QHash<Thing *, SunSpecInverter *> m_sunSpecInverters;
    QHash<Thing *, SunSpecStorage *> m_sunSpecStorages;
    QHash<Thing *, SunSpecMeter *> m_sunSpecMeters;

    bool checkIfThingExists(uint modelId, uint modbusAddress);

    void setupInverter(ThingSetupInfo *info);
    void setupMeter(ThingSetupInfo *info);
    void setupStorage(ThingSetupInfo *info);

private slots:
    void onRefreshTimer();

    void onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value);

    void onConnectionStateChanged(bool status);

    void onFoundSunSpecModel(SunSpec::ModelId modelId, int modbusStartRegister);
    void onSunSpecModelSearchFinished(const QHash<SunSpec::ModelId, int> &modelIds);

    void onWriteRequestExecuted(QUuid requestId, bool success);
    void onWriteRequestError(QUuid requestId, const QString &error);

    void onInverterDataReceived(const SunSpecInverter::InverterData &inverterData);
    void onStorageDataReceived(const SunSpecStorage::StorageData &mandatory, const SunSpecStorage::StorageDataOptional &optional);
    void onMeterDataReceived(const SunSpecMeter::MeterData &meterData);
};
#endif // INTEGRATIONPLUGINSUNSPEC_H

