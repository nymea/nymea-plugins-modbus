/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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
