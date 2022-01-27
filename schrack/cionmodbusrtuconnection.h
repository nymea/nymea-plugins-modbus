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

#ifndef CIONMODBUSRTUCONNECTION_H
#define CIONMODBUSRTUCONNECTION_H

#include <QObject>

#include "../modbus/modbusdatautils.h"
#include <hardware/modbus/modbusrtumaster.h>

class CionModbusRtuConnection : public QObject
{
    Q_OBJECT
public:
    enum Registers {
        RegisterChargingEnabled = 100,
        RegisterChargingCurrentSetpoint = 101,
        RegisterStatusBits = 121,
        RegisterCurrentChargingCurrentE3 = 126,
        RegisterMaxChargingCurrentE3 = 127,
        RegisterChargingDuration = 151,
        RegisterPluggedInDuration = 153,
        RegisterU1Voltage = 167,
        RegisterGridVoltage = 302,
        RegisterMinChargingCurrent = 507
    };
    Q_ENUM(Registers)

    explicit CionModbusRtuConnection(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent = nullptr);
    ~CionModbusRtuConnection() = default;

    ModbusRtuMaster *modbusRtuMaster() const;
    quint16 slaveId() const;

    /* Charging enabled - Address: 100, Size: 1 */
    quint16 chargingEnabled() const;
    ModbusRtuReply *setChargingEnabled(quint16 chargingEnabled);

    /* Allowed charging current [A] - Address: 101, Size: 1 */
    quint16 chargingCurrentSetpoint() const;
    ModbusRtuReply *setChargingCurrentSetpoint(quint16 chargingCurrentSetpoint);

    /* Maximum charging current - Address: 121, Size: 1 */
    quint16 statusBits() const;

    /* Charging duration [ms] - Address: 151, Size: 2 */
    quint32 chargingDuration() const;

    /* Plugged in duration [ms] - Address: 153, Size: 2 */
    quint32 pluggedInDuration() const;

    /* U1 voltage [V] - Address: 167, Size: 1 */
    quint16 u1Voltage() const;

    /* Voltage of the power supply grid [V] - Address: 302, Size: 1 */
    quint16 gridVoltage() const;

    /* Minimum charging current [A] - Address: 507, Size: 1 */
    quint16 minChargingCurrent() const;

    /* Current charging Ampere [A] - Address: 126, Size: 1 */
    quint16 currentChargingCurrentE3() const;

    /* Maximum charging current of connected cable [A] - Address: 127, Size: 1 */
    quint16 maxChargingCurrentE3() const;

    /* Read block from start addess 126 with size of 2 registers containing following 2 properties:
      - Current charging Ampere [A] - Address: 126, Size: 1
      - Maximum charging current of connected cable [A] - Address: 127, Size: 1
    */ 
    void updateE3Block();

    void updateChargingEnabled();
    void updateChargingCurrentSetpoint();
    void updateStatusBits();
    void updateChargingDuration();
    void updatePluggedInDuration();
    void updateU1Voltage();
    void updateGridVoltage();
    void updateMinChargingCurrent();

    virtual void initialize();
    virtual void update();

signals:
    void initializationFinished();

    void chargingEnabledChanged(quint16 chargingEnabled);
    void chargingCurrentSetpointChanged(quint16 chargingCurrentSetpoint);
    void statusBitsChanged(quint16 statusBits);
    void chargingDurationChanged(quint32 chargingDuration);
    void pluggedInDurationChanged(quint32 pluggedInDuration);
    void u1VoltageChanged(quint16 u1Voltage);
    void gridVoltageChanged(quint16 gridVoltage);
    void minChargingCurrentChanged(quint16 minChargingCurrent);
    void currentChargingCurrentE3Changed(quint16 currentChargingCurrentE3);
    void maxChargingCurrentE3Changed(quint16 maxChargingCurrentE3);

protected:
    ModbusRtuReply *readChargingEnabled();
    ModbusRtuReply *readChargingCurrentSetpoint();
    ModbusRtuReply *readStatusBits();
    ModbusRtuReply *readChargingDuration();
    ModbusRtuReply *readPluggedInDuration();
    ModbusRtuReply *readU1Voltage();
    ModbusRtuReply *readGridVoltage();
    ModbusRtuReply *readMinChargingCurrent();

    quint16 m_chargingEnabled = 0;
    quint16 m_chargingCurrentSetpoint = 6;
    quint16 m_statusBits = 0;
    quint32 m_chargingDuration = 6;
    quint32 m_pluggedInDuration = 6;
    quint16 m_u1Voltage = 32;
    quint16 m_gridVoltage = 0;
    quint16 m_minChargingCurrent = 6;
    quint16 m_currentChargingCurrentE3 = 6;
    quint16 m_maxChargingCurrentE3 = 32;

private:
    ModbusRtuMaster *m_modbusRtuMaster = nullptr;
    quint16 m_slaveId = 1;
    QVector<ModbusRtuReply *> m_pendingInitReplies;

    void verifyInitFinished();


};

QDebug operator<<(QDebug debug, CionModbusRtuConnection *cionModbusRtuConnection);

#endif // CIONMODBUSRTUCONNECTION_H
