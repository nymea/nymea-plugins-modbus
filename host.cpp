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

#include "host.h"

Host::Host()
{
    qRegisterMetaType<Host>();
    qRegisterMetaType<QList<Host> >();
}

QString Host::macAddress() const
{
    return m_macAddress;
}

void Host::setMacAddress(const QString &macAddress)
{
    m_macAddress = macAddress;
}

QString Host::hostName() const
{
    return m_hostName;
}

void Host::setHostName(const QString &hostName)
{
    m_hostName = hostName;
}

QString Host::address() const
{
    return m_address;
}

void Host::setAddress(const QString &address)
{
    m_address = address;
}

void Host::seen()
{
    m_lastSeenTime = QDateTime::currentDateTime();
}

QDateTime Host::lastSeenTime() const
{
    return m_lastSeenTime;
}

bool Host::reachable() const
{
    return m_reachable;
}

void Host::setReachable(bool reachable)
{
    m_reachable = reachable;
}

QDebug operator<<(QDebug dbg, const Host &host)
{
    dbg.nospace() << "Host(" << host.macAddress() << "," << host.hostName() << ", " << host.address() << ", " << (host.reachable() ? "up" : "down") << ")";
    return dbg.space();
}
