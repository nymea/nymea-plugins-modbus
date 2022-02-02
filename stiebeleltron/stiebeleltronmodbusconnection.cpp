/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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


#include "stiebeleltronmodbusconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcStiebelEltronModbusConnection, "StiebelEltronModbusConnection")

StiebelEltronModbusConnection::StiebelEltronModbusConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    ModbusTCPMaster(hostAddress, port, parent),
    m_slaveId(slaveId)
{
    
}

float StiebelEltronModbusConnection::outdoorTemperature() const
{
    return m_outdoorTemperature;
}

void StiebelEltronModbusConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void StiebelEltronModbusConnection::update()
{
    updateOutdoorTemperature();
}

void StiebelEltronModbusConnection::updateOutdoorTemperature()
{
    // Update registers from Flow
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Flow\" register:" << 507 << "size:" << 1;
    QModbusReply *reply = readOutdoorTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Flow\" register" << 507 << "size:" << 1 << values;
                    float receivedOutdoorTemperature = ModbusDataUtils::convertToInt16(values) * 1.0 * pow(10, -1);
                    if (m_outdoorTemperature != receivedOutdoorTemperature) {
                        m_outdoorTemperature = receivedOutdoorTemperature;
                        emit outdoorTemperatureChanged(m_outdoorTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Flow\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Flow\" registers from" << hostAddress().toString() << errorString();
    }
}

QModbusReply *StiebelEltronModbusConnection::readOutdoorTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 507, 1);
    return sendReadRequest(request, m_slaveId);
}

void StiebelEltronModbusConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcStiebelEltronModbusConnection()) << "Initialization finished of StiebelEltronModbusConnection" << hostAddress().toString();
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, StiebelEltronModbusConnection *stiebelEltronModbusConnection)
{
    debug.nospace().noquote() << "StiebelEltronModbusConnection(" << stiebelEltronModbusConnection->hostAddress().toString() << ":" << stiebelEltronModbusConnection->port() << ")" << "\n";
    debug.nospace().noquote() << "    - Flow:" << stiebelEltronModbusConnection->outdoorTemperature() << " [°C]" << "\n";
    return debug.quote().space();
}

