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

#ifndef ALPHACONNECTMODBUSTCPCONNECTION_H
#define ALPHACONNECTMODBUSTCPCONNECTION_H

#include <QObject>

#include "../modbus/modbusdatautils.h"
#include "../modbus/modbustcpmaster.h"

class AlphaConnectModbusTcpConnection : public ModbusTCPMaster
{
    Q_OBJECT
public:
    enum SystemStatus {
        SystemStatusHeatingMode = 0,
        SystemStatusDomesticHotWater = 1,
        SystemStatusSwimmingPool = 2,
        SystemStatusEVUOff = 3,
        SystemStatusDefrost = 4,
        SystemStatusOff = 5,
        SystemStatusExternalEnergySource = 6,
        SystemStatusCoolingMode = 7
    };
    Q_ENUM(SystemStatus)

    enum SmartGridState {
        SmartGridStateOff = 0,
        SmartGridStateLow = 1,
        SmartGridStateStandard = 2,
        SmartGridStateHigh = 3
    };
    Q_ENUM(SmartGridState)

    explicit AlphaConnectModbusTcpConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    ~AlphaConnectModbusTcpConnection() = default;

    /* Flow [°C] - Address: 1, Size: 1 */
    float flowTemperature() const;

    /* Return [°C] - Address: 2, Size: 1 */
    float returnTemperature() const;

    /* External return [°C] - Address: 3, Size: 1 */
    float externalReturnTemperature() const;

    /* Hot water temperature [°C] - Address: 4, Size: 1 */
    float hotWaterTemperature() const;

    /* Hot gas temperature [°C] - Address: 8, Size: 1 */
    float hotGasTemperature() const;

    /* Heat source inlet temperature [°C] - Address: 9, Size: 1 */
    float heatSourceInletTemperature() const;

    /* Heat source outlet temperature [°C] - Address: 10, Size: 1 */
    float heatSourceOutletTemperature() const;

    /* Room remote adjuster 1 temperature [°C] - Address: 11, Size: 1 */
    float roomTemperature1() const;

    /* Room remote adjuster 2 temperature [°C] - Address: 12, Size: 1 */
    float roomTemperature2() const;

    /* Room remote adjuster 3 temperature [°C] - Address: 13, Size: 1 */
    float roomTemperature3() const;

    /* Solar collector temperature [°C] - Address: 14, Size: 1 */
    float solarCollectorTemperature() const;

    /* Solar storage tank temperature [°C] - Address: 15, Size: 1 */
    float solarStorageTankTemperature() const;

    /* External energy source temperature [°C] - Address: 16, Size: 1 */
    float externalEnergySourceTemperature() const;

    /* Supply air temperature [°C] - Address: 17, Size: 1 */
    float supplyAirTemperature() const;

    /* External air temperature [°C] - Address: 18, Size: 1 */
    float externalAirTemperature() const;

    /* RBE actual room temperature [°C] - Address: 24, Size: 1 */
    float rbeRoomActualTemperature() const;

    /* RBE room temperature setpoint [°C] - Address: 24, Size: 1 */
    float rbeRoomSetpointTemperature() const;

    /* Heating pump operating hours [h] - Address: 33, Size: 1 */
    quint16 heatingPumpOperatingHours() const;

    /* System status - Address: 37, Size: 1 */
    SystemStatus systemStatus() const;

    /* Heating energy [kWh] - Address: 38, Size: 2 */
    float heatingEnergy() const;

    /* Water heat energy [kWh] - Address: 40, Size: 2 */
    float waterHeatEnergy() const;

    /* Total energy [kWh] - Address: 44, Size: 2 */
    float totalHeatEnergy() const;

    /* Outdoor temperature [°C] - Address: 0, Size: 1 */
    float outdoorTemperature() const;
    QModbusReply *setOutdoorTemperature(float outdoorTemperature);

    /* Return setpoint temperature [°C] - Address: 1, Size: 1 */
    float returnSetpointTemperature() const;
    QModbusReply *setReturnSetpointTemperature(float returnSetpointTemperature);

    /* Hot water setpoint temperature [°C] - Address: 5, Size: 1 */
    float hotWaterSetpointTemperature() const;
    QModbusReply *setHotWaterSetpointTemperature(float hotWaterSetpointTemperature);

    /* Smart grid control - Address: 14, Size: 1 */
    SmartGridState smartGrid() const;
    QModbusReply *setSmartGrid(SmartGridState smartGrid);

    virtual void initialize();
    virtual void update();

    void updateFlowTemperature();
    void updateReturnTemperature();
    void updateExternalReturnTemperature();
    void updateHotWaterTemperature();
    void updateHotGasTemperature();
    void updateHeatSourceInletTemperature();
    void updateHeatSourceOutletTemperature();
    void updateRoomTemperature1();
    void updateRoomTemperature2();
    void updateRoomTemperature3();
    void updateSolarCollectorTemperature();
    void updateSolarStorageTankTemperature();
    void updateExternalEnergySourceTemperature();
    void updateSupplyAirTemperature();
    void updateExternalAirTemperature();
    void updateRbeRoomActualTemperature();
    void updateRbeRoomSetpointTemperature();
    void updateHeatingPumpOperatingHours();
    void updateSystemStatus();
    void updateHeatingEnergy();
    void updateWaterHeatEnergy();
    void updateTotalHeatEnergy();
    void updateOutdoorTemperature();
    void updateReturnSetpointTemperature();
    void updateHotWaterSetpointTemperature();
    void updateSmartGrid();

signals:
    void initializationFinished();

    void flowTemperatureChanged(float flowTemperature);
    void returnTemperatureChanged(float returnTemperature);
    void externalReturnTemperatureChanged(float externalReturnTemperature);
    void hotWaterTemperatureChanged(float hotWaterTemperature);
    void hotGasTemperatureChanged(float hotGasTemperature);
    void heatSourceInletTemperatureChanged(float heatSourceInletTemperature);
    void heatSourceOutletTemperatureChanged(float heatSourceOutletTemperature);
    void roomTemperature1Changed(float roomTemperature1);
    void roomTemperature2Changed(float roomTemperature2);
    void roomTemperature3Changed(float roomTemperature3);
    void solarCollectorTemperatureChanged(float solarCollectorTemperature);
    void solarStorageTankTemperatureChanged(float solarStorageTankTemperature);
    void externalEnergySourceTemperatureChanged(float externalEnergySourceTemperature);
    void supplyAirTemperatureChanged(float supplyAirTemperature);
    void externalAirTemperatureChanged(float externalAirTemperature);
    void rbeRoomActualTemperatureChanged(float rbeRoomActualTemperature);
    void rbeRoomSetpointTemperatureChanged(float rbeRoomSetpointTemperature);
    void heatingPumpOperatingHoursChanged(quint16 heatingPumpOperatingHours);
    void systemStatusChanged(SystemStatus systemStatus);
    void heatingEnergyChanged(float heatingEnergy);
    void waterHeatEnergyChanged(float waterHeatEnergy);
    void totalHeatEnergyChanged(float totalHeatEnergy);
    void outdoorTemperatureChanged(float outdoorTemperature);
    void returnSetpointTemperatureChanged(float returnSetpointTemperature);
    void hotWaterSetpointTemperatureChanged(float hotWaterSetpointTemperature);
    void smartGridChanged(SmartGridState smartGrid);

private:
    quint16 m_slaveId = 1;
    QVector<QModbusReply *> m_pendingInitReplies;

    float m_flowTemperature = 0;
    float m_returnTemperature = 0;
    float m_externalReturnTemperature = 0;
    float m_hotWaterTemperature = 0;
    float m_hotGasTemperature = 0;
    float m_heatSourceInletTemperature = 0;
    float m_heatSourceOutletTemperature = 0;
    float m_roomTemperature1 = 0;
    float m_roomTemperature2 = 0;
    float m_roomTemperature3 = 0;
    float m_solarCollectorTemperature = 0;
    float m_solarStorageTankTemperature = 0;
    float m_externalEnergySourceTemperature = 0;
    float m_supplyAirTemperature = 0;
    float m_externalAirTemperature = 0;
    float m_rbeRoomActualTemperature = 0;
    float m_rbeRoomSetpointTemperature = 0;
    quint16 m_heatingPumpOperatingHours = 0;
    SystemStatus m_systemStatus = SystemStatusHeatingMode;
    float m_heatingEnergy = 0;
    float m_waterHeatEnergy = 0;
    float m_totalHeatEnergy = 0;
    float m_outdoorTemperature = 0;
    float m_returnSetpointTemperature = 0;
    float m_hotWaterSetpointTemperature = 0;
    SmartGridState m_smartGrid = SmartGridStateStandard;

    void verifyInitFinished();

    QModbusReply *readFlowTemperature();
    QModbusReply *readReturnTemperature();
    QModbusReply *readExternalReturnTemperature();
    QModbusReply *readHotWaterTemperature();
    QModbusReply *readHotGasTemperature();
    QModbusReply *readHeatSourceInletTemperature();
    QModbusReply *readHeatSourceOutletTemperature();
    QModbusReply *readRoomTemperature1();
    QModbusReply *readRoomTemperature2();
    QModbusReply *readRoomTemperature3();
    QModbusReply *readSolarCollectorTemperature();
    QModbusReply *readSolarStorageTankTemperature();
    QModbusReply *readExternalEnergySourceTemperature();
    QModbusReply *readSupplyAirTemperature();
    QModbusReply *readExternalAirTemperature();
    QModbusReply *readRbeRoomActualTemperature();
    QModbusReply *readRbeRoomSetpointTemperature();
    QModbusReply *readHeatingPumpOperatingHours();
    QModbusReply *readSystemStatus();
    QModbusReply *readHeatingEnergy();
    QModbusReply *readWaterHeatEnergy();
    QModbusReply *readTotalHeatEnergy();
    QModbusReply *readOutdoorTemperature();
    QModbusReply *readReturnSetpointTemperature();
    QModbusReply *readHotWaterSetpointTemperature();
    QModbusReply *readSmartGrid();


};

QDebug operator<<(QDebug debug, AlphaConnectModbusTcpConnection *alphaConnectModbusTcpConnection);

#endif // ALPHACONNECTMODBUSTCPCONNECTION_H
