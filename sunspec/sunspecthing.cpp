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

#include "sunspecthing.h"

SunSpecThing::SunSpecThing(Thing *thing, SunSpecModel *model, QObject *parent) :
    QObject(parent),
    m_thing(thing),
    m_model(model)
{

}

SunSpecConnection *SunSpecThing::connection() const
{
    if (!m_model)
        return nullptr;

    return m_model->connection();
}

SunSpecModel *SunSpecThing::model() const
{
    return m_model;
}

Thing *SunSpecThing::thing() const
{
    return m_thing;
}

quint16 SunSpecThing::modbusStartRegister() const
{
    if (!m_model)
        return 0xFFFF;

    return m_model->modbusStartRegister();
}

void SunSpecThing::executeAction(ThingActionInfo *info)
{
    Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(info->action().actionTypeId().toString()).toUtf8());
    info->finish(Thing::ThingErrorActionTypeNotFound);
}

void SunSpecThing::onBlockDataUpdated()
{
    // Reimplement if required
}
