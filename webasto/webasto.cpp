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

#include "webasto.h"
#include "extern-plugininfo.h"

Webasto::Webasto(const QHostAddress &address, uint port, QObject *parent) :
    QObject(parent)
{
    m_modbusConnection = new QModbusTcpClient(this);
    m_modbusConnection->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    m_modbusConnection->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address.toString());
    m_modbusConnection->setNumberOfRetries(3);
    m_modbusConnection->setTimeout(1000);
}

void Webasto::setAddress(const QHostAddress &address)
{
    qCDebug(dcWebasto()) << "Webasto: set address" << address;
    m_modbusConnection->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address.toString());
}

QHostAddress Webasto::address() const
{
    return QHostAddress(m_modbusConnection->connectionParameter(QModbusDevice::NetworkAddressParameter).toString());
}

bool Webasto::connected()
{
    return (m_modbusConnection->state() == QModbusTcpClient::State::ConnectedState);
}

void Webasto::getBasicInformation()
{

}

void Webasto::getUserId()
{

}

void Webasto::getSessionInformation()
{

}

void Webasto::setLiveBit()
{

}

QUuid Webasto::writeHoldingRegister()
{
    QUuid request = QUuid::createUuid();


    return request;
}
