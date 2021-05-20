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

#include "mtec.h"
#include "extern-plugininfo.h"
#include "mtechelpers.h"

#include <cstring>

MTec::MTec(const QHostAddress &address, QObject *parent) :
    QObject(parent),
    m_hostAddress(address)
{
    m_modbusMaster = new ModbusTCPMaster(address, 502, this);

    qCDebug(dcMTec()) << "created ModbusTCPMaster";

    if (m_modbusMaster->connectDevice()) {
        emit connectedChanged(MTecHelpers::ConnectionState::Connecting);
    }

    connect(m_modbusMaster, &ModbusTCPMaster::receivedHoldingRegister, this, &MTec::onReceivedHoldingRegister);
    connect(m_modbusMaster, &ModbusTCPMaster::readRequestError, this, &MTec::onModbusError);
    connect(m_modbusMaster, &ModbusTCPMaster::writeRequestError, this, &MTec::onModbusError);
}

MTec::~MTec()
{
}

void MTec::onModbusError()
{
    qCWarning(dcMTec()) << "MTec: Received modbus error";

    /* Only emit connected changed signal, if a soft connection
     * had already been established.
     * This avoids a series of possibly fake connection state changes,
     * while the modbus server in the heatpump is starting up. */
    if (m_softConnection) {
        m_softConnection = false;
        emit connectedChanged(MTecHelpers::ConnectionState::Offline);
    }
}

void MTec::onRequestStatus()
{
    if ((m_softConnection) ||
            ((m_connected) && (m_requestsSent < MTec::ConnectionRetries))) {

        if (m_requestsSent < MTec::ConnectionRetries) {
            m_firstTimeout = QDateTime::currentDateTime();
            m_firstTimeout = m_firstTimeout.addSecs(MTec::FirstConnectionTimeout);

            /* Save original modbus timeout, will be set again
             * after first response is received */
            m_modbusTimeout = m_modbusMaster->timeout();

            /* The M-Tec heatpump requires a longer timeout to
             * start-up the modbus communication */
            m_modbusMaster->setTimeout(MTec::FirstConnectionTimeout);
        } else {
            /* Set back original modbus timeout */
            m_modbusMaster->setTimeout(m_modbusTimeout);
        }
        
    } else {
        qCDebug(dcMTec()) << "Max number of modbus connects reached, giving up";
        return;
    }

    m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, MTec::ActualPowerConsumption, 1);
    m_requestsSent ++;
}

void MTec::onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value)
{
    Q_UNUSED(slaveAddress);

    /* Some response was received, so the modbus communication is
     * established. */
    m_softConnection = true;
    emit connectedChanged(MTecHelpers::ConnectionState::Online);

    switch (modbusRegister) {
    case ActualPowerConsumption:
        if (value.length() == 1) {
            m_info.actualPowerConsumption = MTecHelpers::convertRegisterToFloat(value[0]);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, MTec::ActualExcessEnergySmartHome, 1);
        break;
    case ActualExcessEnergySmartHome:
        if (value.length() == 1) {
            m_info.actualExcessEnergySmartHome = MTecHelpers::convertRegisterToFloat(value[0]);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, MTec::ActualExcessEnergyElectricityMeter, 1);
        break;
    case ActualExcessEnergyElectricityMeter:
        if (value.length() == 1) {
            m_info.actualExcessEnergyElectricityMeter = MTecHelpers::convertRegisterToFloat(value[0]);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, MTec::ExternalSetValueScaling, 1);
        break;
    case ExternalSetValueScaling:
        if (value.length() == 1) {
            m_info.externalSetValueScaling = MTecHelpers::convertRegisterToFloat(value[0]);
        }
        m_modbusMaster->readHoldingRegister(MTec::ModbusUnitID, MTec::RequestExternalHeatSource, 1);
        break;
    case RequestExternalHeatSource:
        if (value.length() == 1) {
            m_info.requestExternalHeatSource = MTecHelpers::externalHeatSourceRequestToString(value[0]);

            /* Everything read without errors
             * -> update status in plugin */
            emit statusUpdated(m_info);
        }
        break;
    }
}

