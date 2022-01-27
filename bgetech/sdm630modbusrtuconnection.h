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

#ifndef SDM630MODBUSRTUCONNECTION_H
#define SDM630MODBUSRTUCONNECTION_H

#include <QObject>

#include "../modbus/modbusdatautils.h"
#include <hardware/modbus/modbusrtumaster.h>

class Sdm630ModbusRtuConnection : public QObject
{
    Q_OBJECT
public:
    explicit Sdm630ModbusRtuConnection(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent = nullptr);
    ~Sdm630ModbusRtuConnection() = default;

    ModbusRtuMaster *modbusRtuMaster() const;
    quint16 slaveId() const;

    /* Total system power [W] - Address: 52, Size: 2 */
    float totalCurrentPower() const;

    /* Voltage phase L1 [V] - Address: 0, Size: 2 */
    float voltagePhaseA() const;

    /* Voltage phase L2 [V] - Address: 2, Size: 2 */
    float voltagePhaseB() const;

    /* Voltage phase L3 [V] - Address: 4, Size: 2 */
    float voltagePhaseC() const;

    /* Current phase L1 [A] - Address: 6, Size: 2 */
    float currentPhaseA() const;

    /* Current phase L2 [A] - Address: 8, Size: 2 */
    float currentPhaseB() const;

    /* Current phase L3 [A] - Address: 10, Size: 2 */
    float currentPhaseC() const;

    /* Read block from start addess 0 with size of 12 registers containing following 6 properties:
      - Voltage phase L1 [V] - Address: 0, Size: 2
      - Voltage phase L2 [V] - Address: 2, Size: 2
      - Voltage phase L3 [V] - Address: 4, Size: 2
      - Current phase L1 [A] - Address: 6, Size: 2
      - Current phase L2 [A] - Address: 8, Size: 2
      - Current phase L3 [A] - Address: 10, Size: 2
    */ 
    void updatePhaseVoltageAndCurrentBlock();
    /* Power phase L1 [W] - Address: 12, Size: 2 */
    float powerPhaseA() const;

    /* Power phase L2 [W] - Address: 14, Size: 2 */
    float powerPhaseB() const;

    /* Power phase L3 [W] - Address: 16, Size: 2 */
    float powerPhaseC() const;

    /* Read block from start addess 12 with size of 6 registers containing following 3 properties:
      - Power phase L1 [W] - Address: 12, Size: 2
      - Power phase L2 [W] - Address: 14, Size: 2
      - Power phase L3 [W] - Address: 16, Size: 2
    */ 
    void updatePhasePowerBlock();
    /* Frequency [Hz] - Address: 70, Size: 2 */
    float frequency() const;

    /* Total energy consumed [kWh] - Address: 72, Size: 2 */
    float totalEnergyConsumed() const;

    /* Total energy produced [kWh] - Address: 74, Size: 2 */
    float totalEnergyProduced() const;

    /* Read block from start addess 70 with size of 6 registers containing following 3 properties:
      - Frequency [Hz] - Address: 70, Size: 2
      - Total energy consumed [kWh] - Address: 72, Size: 2
      - Total energy produced [kWh] - Address: 74, Size: 2
    */ 
    void updateFrequencyAndTotalEnergyBlock();
    /* Energy produced phase A [kWh] - Address: 346, Size: 2 */
    float energyProducedPhaseA() const;

    /* Energy produced phase B [kWh] - Address: 348, Size: 2 */
    float energyProducedPhaseB() const;

    /* Energy produced phase C [kWh] - Address: 350, Size: 2 */
    float energyProducedPhaseC() const;

    /* Energy consumed phase A [kWh] - Address: 352, Size: 2 */
    float energyConsumedPhaseA() const;

    /* Energy consumed phase B [kWh] - Address: 354, Size: 2 */
    float energyConsumedPhaseB() const;

    /* Energy consumed phase C [kWh] - Address: 356, Size: 2 */
    float energyConsumedPhaseC() const;

    /* Read block from start addess 346 with size of 12 registers containing following 6 properties:
      - Energy produced phase A [kWh] - Address: 346, Size: 2
      - Energy produced phase B [kWh] - Address: 348, Size: 2
      - Energy produced phase C [kWh] - Address: 350, Size: 2
      - Energy consumed phase A [kWh] - Address: 352, Size: 2
      - Energy consumed phase B [kWh] - Address: 354, Size: 2
      - Energy consumed phase C [kWh] - Address: 356, Size: 2
    */ 
    void updatePhaseEnergyEnergyBlock();

    void updateTotalCurrentPower();

    virtual void initialize();
    virtual void update();

signals:
    void initializationFinished();

    void totalCurrentPowerChanged(float totalCurrentPower);
    void voltagePhaseAChanged(float voltagePhaseA);
    void voltagePhaseBChanged(float voltagePhaseB);
    void voltagePhaseCChanged(float voltagePhaseC);
    void currentPhaseAChanged(float currentPhaseA);
    void currentPhaseBChanged(float currentPhaseB);
    void currentPhaseCChanged(float currentPhaseC);
    void powerPhaseAChanged(float powerPhaseA);
    void powerPhaseBChanged(float powerPhaseB);
    void powerPhaseCChanged(float powerPhaseC);
    void frequencyChanged(float frequency);
    void totalEnergyConsumedChanged(float totalEnergyConsumed);
    void totalEnergyProducedChanged(float totalEnergyProduced);
    void energyProducedPhaseAChanged(float energyProducedPhaseA);
    void energyProducedPhaseBChanged(float energyProducedPhaseB);
    void energyProducedPhaseCChanged(float energyProducedPhaseC);
    void energyConsumedPhaseAChanged(float energyConsumedPhaseA);
    void energyConsumedPhaseBChanged(float energyConsumedPhaseB);
    void energyConsumedPhaseCChanged(float energyConsumedPhaseC);

private:
    ModbusRtuMaster *m_modbusRtuMaster = nullptr;
    quint16 m_slaveId = 1;
    QVector<ModbusRtuReply *> m_pendingInitReplies;

    float m_totalCurrentPower = 0;
    float m_voltagePhaseA = 0;
    float m_voltagePhaseB = 0;
    float m_voltagePhaseC = 0;
    float m_currentPhaseA = 0;
    float m_currentPhaseB = 0;
    float m_currentPhaseC = 0;
    float m_powerPhaseA = 0;
    float m_powerPhaseB = 0;
    float m_powerPhaseC = 0;
    float m_frequency = 0;
    float m_totalEnergyConsumed = 0;
    float m_totalEnergyProduced = 0;
    float m_energyProducedPhaseA = 0;
    float m_energyProducedPhaseB = 0;
    float m_energyProducedPhaseC = 0;
    float m_energyConsumedPhaseA = 0;
    float m_energyConsumedPhaseB = 0;
    float m_energyConsumedPhaseC = 0;

    void verifyInitFinished();

    ModbusRtuReply *readTotalCurrentPower();


};

QDebug operator<<(QDebug debug, Sdm630ModbusRtuConnection *sdm630ModbusRtuConnection);

#endif // SDM630MODBUSRTUCONNECTION_H
