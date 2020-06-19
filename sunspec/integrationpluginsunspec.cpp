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

#include "plugininfo.h"
#include "integrationpluginsunspec.h"

#include <QHostAddress>

IntegrationPluginSunSpec::IntegrationPluginSunSpec()
{
}

void IntegrationPluginSunSpec::discoverThings(ThingDiscoveryInfo *info)
{
    // Discovery possible?
    Q_ASSERT_X(false, "discoverThing", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
}

void IntegrationPluginSunSpec::setupThing(ThingSetupInfo *info)
{
    if (info->thing()->thingClassId() == sunspecInverterThingClassId) {

    } else if (info->thing()->thingClassId() == sunspecPvModuleThingClassId) {

    } else if (info->thing()->thingClassId() == sunspecMeterThingClassId) {

    } else if (info->thing()->thingClassId() == sunspecTrackerThingClassId) {

    } else if (info->thing()->thingClassId() == sunspecStorageThingClassId) {

    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginSunSpec::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)

    if (!m_refreshTimer) {
        int refreshTime = configValue(sunSpecPluginUpdateIntervalParamTypeId).toInt();
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginSunSpec::onRefreshTimer);
    }
}

void IntegrationPluginSunSpec::thingRemoved(Thing *thing)
{
    Q_UNUSED(thing)

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginSunSpec::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    //Action action = info->action();

    if (thing->thingClassId() == sunspecInverterThingClassId) {
        info->finish(Thing::ThingErrorActionTypeNotFound);
    } else if (thing->thingClassId() == sunspecMeterThingClassId) {
        info->finish(Thing::ThingErrorActionTypeNotFound);
    } else if (thing->thingClassId() == sunspecTrackerThingClassId) {
        info->finish(Thing::ThingErrorActionTypeNotFound);
    } else if (thing->thingClassId() == sunspecPvModuleThingClassId) {
        info->finish(Thing::ThingErrorActionTypeNotFound);
    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        info->finish(Thing::ThingErrorActionTypeNotFound);
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginSunSpec::onRefreshTimer()
{
    //get data
}

void IntegrationPluginSunSpec::onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value)
{
    // Check refresh schedule
    if (paramTypeId == sunSpecPluginUpdateIntervalParamTypeId) {
        if (m_refreshTimer) {
            int refreshTime = value.toInt();
            m_refreshTimer->stop();
            m_refreshTimer->startTimer(refreshTime);
        }
    }
}
