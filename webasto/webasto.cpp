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
    qCDebug(dcWebasto()) << "Webasto: Webasto connection created" << address.toString() << port;
    m_modbusConnection = new ModbusTcpMaster(address, port, this);
    m_modbusConnection->setNumberOfRetries(3);
    m_modbusConnection->setTimeout(1000);

    connect(m_modbusConnection, &ModbusTcpMaster::receivedHoldingRegister, this, &Webasto::onReceivedHoldingRegister);
    connect(m_modbusConnection, &ModbusTcpMaster::writeRequestExecuted, this, &Webasto::writeRequestExecuted);
    connect(m_modbusConnection, &ModbusTcpMaster::writeRequestError, this, &Webasto::writeRequestError);

    m_lifeBitTimer = new QTimer(this);
    m_lifeBitTimer->start(10000);
    connect(m_lifeBitTimer, &QTimer::timeout, this, [this] {
        setLiveBit();
    });
}

ModbusTcpMaster *Webasto::modbusTcpMaster() const
{
    return m_modbusConnection;
}

bool Webasto::connected()
{
    return m_modbusConnection->connected();
}

bool Webasto::connectDevice()
{
    return m_modbusConnection->connectDevice();
}

void Webasto::setLivebitInterval(uint seconds)
{
    qCDebug(dcWebasto()) << "Webasto: Live bit interval set to" << seconds << "[s]";
    m_lifeBitTimer->setInterval(seconds*1000);
}

void Webasto::getRegister(Webasto::TqModbusRegister modbusRegister, uint length)
{
    qCDebug(dcWebasto()) << "Webasto: Get register" << modbusRegister << length;
    if (length < 1 || length > 10) {
        qCWarning(dcWebasto()) << "Invalide register length, allowed values [1,10]";
        return;
    }

    m_modbusConnection->readHoldingRegister(m_unitId, modbusRegister, length);
}

QUuid Webasto::setSafeCurrent(quint16 ampere) const
{
    return m_modbusConnection->writeHoldingRegister(m_unitId, TqSafeCurrent, ampere);
}

QUuid Webasto::seComTimeout(quint16 seconds) const
{
    return m_modbusConnection->writeHoldingRegister(m_unitId, TqComTimeout, seconds);
}

QUuid Webasto::setChargePower(quint32 watt) const
{
    QVector<quint16> data;
    data.append(watt>>16);
    data.append(watt&0xff);
    return m_modbusConnection->writeHoldingRegisters(m_unitId, TqChargePower, data);
}

QUuid Webasto::setChargeCurrent(quint16 ampere) const
{
   return m_modbusConnection->writeHoldingRegister(m_unitId, TqChargeCurrent, ampere);
}

void Webasto::setLiveBit()
{
    qCDebug(dcWebasto()) << "Webasto: Set live bit";
    m_modbusConnection->writeHoldingRegister(m_unitId, TqLifeBit, 0x0001);
    if (m_awaitingLiveBitResponse) {
        // Live bit response has not been received, setting connection as disconnected
        // The live bit acts as heartbeat for both sides, client and server
        if (m_connected) {
            m_connected = false;
            emit connectionStateChanged(false);
        }
    } else {
        m_awaitingLiveBitResponse = true;
    }
}

void Webasto::onReceivedHoldingRegister(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values)
{
    Q_UNUSED(slaveAddress)
    if (modbusRegister == TqLifeBit) {
        m_awaitingLiveBitResponse = false;
        if (!m_connected) {
            m_connected = true;
            emit connectionStateChanged(true);
        }
    }
    emit receivedRegister(TqModbusRegister(modbusRegister), values);
}
