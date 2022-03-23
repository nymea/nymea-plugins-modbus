/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#ifndef PRO380MODBUSRTUCONNECTION_H
#define PRO380MODBUSRTUCONNECTION_H

#include <QObject>

#include "../modbus/modbusdatautils.h"
#include <hardware/modbus/modbusrtumaster.h>

class Pro380ModbusRtuConnection : public QObject
{
    Q_OBJECT
public:
    explicit Pro380ModbusRtuConnection(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent = nullptr);
    ~Pro380ModbusRtuConnection() = default;

    ModbusRtuMaster *modbusRtuMaster() const;
    quint16 slaveId() const;

    /* Frequency [Hz] - Address: 20488, Size: 2 */
    float frequency() const;

    /* Total energy consumed (Forward active energy) [kWh] - Address: 24588, Size: 2 */
    float totalEnergyConsumed() const;

    /* Total energy produced (Reverse active energy) [kWh] - Address: 24600, Size: 2 */
    float totalEnergyProduced() const;

    /* Voltage phase L1 [V] - Address: 20482, Size: 2 */
    float voltagePhaseA() const;

    /* Voltage phase L2 [V] - Address: 20484, Size: 2 */
    float voltagePhaseB() const;

    /* Voltage phase L3 [V] - Address: 20486, Size: 2 */
    float voltagePhaseC() const;

    /* Read block from start addess 20482 with size of 6 registers containing following 3 properties:
      - Voltage phase L1 [V] - Address: 20482, Size: 2
      - Voltage phase L2 [V] - Address: 20484, Size: 2
      - Voltage phase L3 [V] - Address: 20486, Size: 2
    */ 
    void updatePhasesVoltageBlock();
    /* Current phase L1 [A] - Address: 20492, Size: 2 */
    float currentPhaseA() const;

    /* Current phase L2 [A] - Address: 20494, Size: 2 */
    float currentPhaseB() const;

    /* Current phase L3 [A] - Address: 20496, Size: 2 */
    float currentPhaseC() const;

    /* Read block from start addess 20492 with size of 6 registers containing following 3 properties:
      - Current phase L1 [A] - Address: 20492, Size: 2
      - Current phase L2 [A] - Address: 20494, Size: 2
      - Current phase L3 [A] - Address: 20496, Size: 2
    */ 
    void updatePhasesCurrentBlock();
    /* Total system power [kW] - Address: 20498, Size: 2 */
    float totalCurrentPower() const;

    /* Power phase L1 [kW] - Address: 20500, Size: 2 */
    float powerPhaseA() const;

    /* Power phase L2 [kW] - Address: 20502, Size: 2 */
    float powerPhaseB() const;

    /* Power phase L3 [kW] - Address: 20504, Size: 2 */
    float powerPhaseC() const;

    /* Read block from start addess 20498 with size of 8 registers containing following 4 properties:
      - Total system power [kW] - Address: 20498, Size: 2
      - Power phase L1 [kW] - Address: 20500, Size: 2
      - Power phase L2 [kW] - Address: 20502, Size: 2
      - Power phase L3 [kW] - Address: 20504, Size: 2
    */ 
    void updateCurrentPowerBlock();
    /* Energy consumed phase A [kWh] - Address: 24594, Size: 2 */
    float energyConsumedPhaseA() const;

    /* Energy consumed phase B [kWh] - Address: 24596, Size: 2 */
    float energyConsumedPhaseB() const;

    /* Energy consumed phase C [kWh] - Address: 24598, Size: 2 */
    float energyConsumedPhaseC() const;

    /* Read block from start addess 24594 with size of 6 registers containing following 3 properties:
      - Energy consumed phase A [kWh] - Address: 24594, Size: 2
      - Energy consumed phase B [kWh] - Address: 24596, Size: 2
      - Energy consumed phase C [kWh] - Address: 24598, Size: 2
    */ 
    void updatePhasesEnergyConsumedBlock();
    /* Energy produced phase A [kWh] - Address: 24606, Size: 2 */
    float energyProducedPhaseA() const;

    /* Energy produced phase B [kWh] - Address: 24608, Size: 2 */
    float energyProducedPhaseB() const;

    /* Energy produced phase C [kWh] - Address: 24610, Size: 2 */
    float energyProducedPhaseC() const;

    /* Read block from start addess 24606 with size of 6 registers containing following 3 properties:
      - Energy produced phase A [kWh] - Address: 24606, Size: 2
      - Energy produced phase B [kWh] - Address: 24608, Size: 2
      - Energy produced phase C [kWh] - Address: 24610, Size: 2
    */ 
    void updatePhasesEnergyProducedBlock();

    void updateFrequency();
    void updateTotalEnergyConsumed();
    void updateTotalEnergyProduced();

    virtual void initialize();
    virtual void update();

signals:
    void initializationFinished();

    void frequencyChanged(float frequency);
    void totalEnergyConsumedChanged(float totalEnergyConsumed);
    void totalEnergyProducedChanged(float totalEnergyProduced);
    void voltagePhaseAChanged(float voltagePhaseA);
    void voltagePhaseBChanged(float voltagePhaseB);
    void voltagePhaseCChanged(float voltagePhaseC);
    void currentPhaseAChanged(float currentPhaseA);
    void currentPhaseBChanged(float currentPhaseB);
    void currentPhaseCChanged(float currentPhaseC);
    void totalCurrentPowerChanged(float totalCurrentPower);
    void powerPhaseAChanged(float powerPhaseA);
    void powerPhaseBChanged(float powerPhaseB);
    void powerPhaseCChanged(float powerPhaseC);
    void energyConsumedPhaseAChanged(float energyConsumedPhaseA);
    void energyConsumedPhaseBChanged(float energyConsumedPhaseB);
    void energyConsumedPhaseCChanged(float energyConsumedPhaseC);
    void energyProducedPhaseAChanged(float energyProducedPhaseA);
    void energyProducedPhaseBChanged(float energyProducedPhaseB);
    void energyProducedPhaseCChanged(float energyProducedPhaseC);

private:
    ModbusRtuMaster *m_modbusRtuMaster = nullptr;
    quint16 m_slaveId = 1;
    QVector<ModbusRtuReply *> m_pendingInitReplies;

    float m_frequency = 0;
    float m_totalEnergyConsumed = 0;
    float m_totalEnergyProduced = 0;
    float m_voltagePhaseA = 0;
    float m_voltagePhaseB = 0;
    float m_voltagePhaseC = 0;
    float m_currentPhaseA = 0;
    float m_currentPhaseB = 0;
    float m_currentPhaseC = 0;
    float m_totalCurrentPower = 0;
    float m_powerPhaseA = 0;
    float m_powerPhaseB = 0;
    float m_powerPhaseC = 0;
    float m_energyConsumedPhaseA = 0;
    float m_energyConsumedPhaseB = 0;
    float m_energyConsumedPhaseC = 0;
    float m_energyProducedPhaseA = 0;
    float m_energyProducedPhaseB = 0;
    float m_energyProducedPhaseC = 0;

    void verifyInitFinished();

    ModbusRtuReply *readFrequency();
    ModbusRtuReply *readTotalEnergyConsumed();
    ModbusRtuReply *readTotalEnergyProduced();


};

QDebug operator<<(QDebug debug, Pro380ModbusRtuConnection *pro380ModbusRtuConnection);

#endif // PRO380MODBUSRTUCONNECTION_H
