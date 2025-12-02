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

#ifndef SUNSPECTHING_H
#define SUNSPECTHING_H

#include <QObject>

#include "extern-plugininfo.h"

#include <integrations/integrationplugin.h>

#include <sunspecmodel.h>
#include <sunspecconnection.h>

class SunSpecThing : public QObject
{
    Q_OBJECT
public:
    explicit SunSpecThing(Thing *thing, SunSpecModel *model, QObject *parent = nullptr);
    virtual ~SunSpecThing() = default;

    virtual SunSpecConnection *connection() const;
    SunSpecModel *model() const;
    Thing *thing() const;

    virtual quint16 modbusStartRegister() const;

    virtual void readBlockData() = 0;

    virtual void executeAction(ThingActionInfo *info);

protected:
    Thing *m_thing = nullptr;
    SunSpecModel *m_model = nullptr;

private slots:
    virtual void onBlockDataUpdated();

};

#endif // SUNSPECTHING_H
