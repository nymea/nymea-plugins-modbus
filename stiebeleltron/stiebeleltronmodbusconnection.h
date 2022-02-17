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

#ifndef STIEBELELTRONMODBUSCONNECTION_H
#define STIEBELELTRONMODBUSCONNECTION_H

#include <QObject>

#include "../modbus/modbusdatautils.h"
#include "../modbus/modbustcpmaster.h"

class StiebelEltronModbusConnection : public ModbusTCPMaster
{
    Q_OBJECT
public:
    enum Registers {
        RegisterRoomTemperatureFEK = 502,
        RegisterOutdoorTemperature = 506,
        RegisterFlowTemperature = 514,
        RegisterReturnTemperature = 515,
        RegisterStorageTankTemperature = 517,
        RegisterHotWaterTemperature = 521,
        RegisterSolarCollectorTemperature = 527,
        RegisterSolarStorageTankTemperature = 528,
        RegisterExternalHeatSourceTemperature = 530,
        RegisterHotGasTemperature1 = 543,
        RegisterHotGasTemperature2 = 550,
        RegisterSourceTemperature = 562,
        RegisterOperatingMode = 1500,
        RegisterSystemStatus = 2500,
        RegisterHeatingEnergy = 3501,
        RegisterHotWaterEnergy = 3504,
        RegisterConsumedEnergyHeating = 3511,
        RegisterConsumedEnergyHotWater = 3514,
        RegisterSgReadyActive = 4000,
        RegisterSgReadyState = 4001,
        RegisterSgReadyStateRO = 5000
    };
    Q_ENUM(Registers)

    enum OperatingMode {
        OperatingModeEmergency = 0,
        OperatingModeStandby = 1,
        OperatingModeProgram = 2,
        OperatingModeComfort = 3,
        OperatingModeEco = 4,
        OperatingModeHotWater = 5
    };
    Q_ENUM(OperatingMode)

    enum SmartGridState {
        SmartGridStateModeOne = 1,
        SmartGridStateModeTwo = 0,
        SmartGridStateModeThree = 65536,
        SmartGridStateModeFour = 65537
    };
    Q_ENUM(SmartGridState)

    explicit StiebelEltronModbusConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    ~StiebelEltronModbusConnection() = default;

    /* Outdoor temperature [°C] - Address: 506, Size: 1 */
    float outdoorTemperature() const;

    /* Flow temperature [°C] - Address: 514, Size: 1 */
    float flowTemperature() const;

    /* Hot water temperature [°C] - Address: 521, Size: 1 */
    float hotWaterTemperature() const;

    /* Hot gas temperature HP 1 [°C] - Address: 543, Size: 1 */
    float hotGasTemperature1() const;

    /* Hot gas temperature HP 2 [°C] - Address: 550, Size: 1 */
    float hotGasTemperature2() const;

    /* Source temperature [°C] - Address: 562, Size: 1 */
    float SourceTemperature() const;

    /* Room temperature FEK [°C] - Address: 502, Size: 1 */
    float roomTemperatureFEK() const;

    /* Return temperature [°C] - Address: 515, Size: 1 */
    float returnTemperature() const;

    /* Solar collector temperature [°C] - Address: 527, Size: 1 */
    float solarCollectorTemperature() const;

    /* Solar storage tank temperature [°C] - Address: 528, Size: 1 */
    float solarStorageTankTemperature() const;

    /* Storage tank temperature [°C] - Address: 517, Size: 1 */
    float storageTankTemperature() const;

    /* External heat source temperature [°C] - Address: 530, Size: 1 */
    float externalHeatSourceTemperature() const;

    /* Heating energy [kWh] - Address: 3501, Size: 2 */
    quint32 heatingEnergy() const;

    /* Hot water energy [kWh] - Address: 3504, Size: 2 */
    quint32 hotWaterEnergy() const;

    /* Consumed energy heating [kWh] - Address: 3511, Size: 2 */
    quint32 consumedEnergyHeating() const;

    /* Consumed energy hot water [kWh] - Address: 3514, Size: 2 */
    quint32 consumedEnergyHotWater() const;

    /* Operating mode - Address: 1500, Size: 1 */
    OperatingMode operatingMode() const;

    /* System status - Address: 2500, Size: 1 */
    quint16 systemStatus() const;

    /* Smart grid status - Address: 5000, Size: 1 */
    quint16 sgReadyStateRO() const;

    /* SG ready active - Address: 4000, Size: 1 */
    quint16 sgReadyActive() const;
    QModbusReply *setSgReadyActive(quint16 sgReadyActive);

    /* SG Ready mode - Address: 4001, Size: 2 */
    SmartGridState sgReadyState() const;
    QModbusReply *setSgReadyState(SmartGridState sgReadyState);

    virtual void initialize();
    virtual void update();

    void updateOutdoorTemperature();
    void updateFlowTemperature();
    void updateHotWaterTemperature();
    void updateHotGasTemperature1();
    void updateHotGasTemperature2();
    void updateSourceTemperature();
    void updateRoomTemperatureFEK();
    void updateReturnTemperature();
    void updateSolarCollectorTemperature();
    void updateSolarStorageTankTemperature();
    void updateStorageTankTemperature();
    void updateExternalHeatSourceTemperature();
    void updateHeatingEnergy();
    void updateHotWaterEnergy();
    void updateConsumedEnergyHeating();
    void updateConsumedEnergyHotWater();
    void updateOperatingMode();
    void updateSystemStatus();
    void updateSgReadyStateRO();
    void updateSgReadyActive();
    void updateSgReadyState();

signals:
    void initializationFinished();

    void outdoorTemperatureChanged(float outdoorTemperature);
    void flowTemperatureChanged(float flowTemperature);
    void hotWaterTemperatureChanged(float hotWaterTemperature);
    void hotGasTemperature1Changed(float hotGasTemperature1);
    void hotGasTemperature2Changed(float hotGasTemperature2);
    void SourceTemperatureChanged(float SourceTemperature);
    void roomTemperatureFEKChanged(float roomTemperatureFEK);
    void returnTemperatureChanged(float returnTemperature);
    void solarCollectorTemperatureChanged(float solarCollectorTemperature);
    void solarStorageTankTemperatureChanged(float solarStorageTankTemperature);
    void storageTankTemperatureChanged(float storageTankTemperature);
    void externalHeatSourceTemperatureChanged(float externalHeatSourceTemperature);
    void heatingEnergyChanged(quint32 heatingEnergy);
    void hotWaterEnergyChanged(quint32 hotWaterEnergy);
    void consumedEnergyHeatingChanged(quint32 consumedEnergyHeating);
    void consumedEnergyHotWaterChanged(quint32 consumedEnergyHotWater);
    void operatingModeChanged(OperatingMode operatingMode);
    void systemStatusChanged(quint16 systemStatus);
    void sgReadyStateROChanged(quint16 sgReadyStateRO);
    void sgReadyActiveChanged(quint16 sgReadyActive);
    void sgReadyStateChanged(SmartGridState sgReadyState);

protected:
    QModbusReply *readOutdoorTemperature();
    QModbusReply *readFlowTemperature();
    QModbusReply *readHotWaterTemperature();
    QModbusReply *readHotGasTemperature1();
    QModbusReply *readHotGasTemperature2();
    QModbusReply *readSourceTemperature();
    QModbusReply *readRoomTemperatureFEK();
    QModbusReply *readReturnTemperature();
    QModbusReply *readSolarCollectorTemperature();
    QModbusReply *readSolarStorageTankTemperature();
    QModbusReply *readStorageTankTemperature();
    QModbusReply *readExternalHeatSourceTemperature();
    QModbusReply *readHeatingEnergy();
    QModbusReply *readHotWaterEnergy();
    QModbusReply *readConsumedEnergyHeating();
    QModbusReply *readConsumedEnergyHotWater();
    QModbusReply *readOperatingMode();
    QModbusReply *readSystemStatus();
    QModbusReply *readSgReadyStateRO();
    QModbusReply *readSgReadyActive();
    QModbusReply *readSgReadyState();

    float m_outdoorTemperature = 0;
    float m_flowTemperature = 0;
    float m_hotWaterTemperature = 0;
    float m_hotGasTemperature1 = 0;
    float m_hotGasTemperature2 = 0;
    float m_SourceTemperature = 0;
    float m_roomTemperatureFEK = 0;
    float m_returnTemperature = 0;
    float m_solarCollectorTemperature = 0;
    float m_solarStorageTankTemperature = 0;
    float m_storageTankTemperature = 0;
    float m_externalHeatSourceTemperature = 0;
    quint32 m_heatingEnergy = 0;
    quint32 m_hotWaterEnergy = 0;
    quint32 m_consumedEnergyHeating = 0;
    quint32 m_consumedEnergyHotWater = 0;
    OperatingMode m_operatingMode = OperatingModeStandby;
    quint16 m_systemStatus = 0;
    quint16 m_sgReadyStateRO = 3;
    quint16 m_sgReadyActive = 0;
    SmartGridState m_sgReadyState = SmartGridStateModeThree;

private:
    quint16 m_slaveId = 1;
    QVector<QModbusReply *> m_pendingInitReplies;

    void verifyInitFinished();


};

QDebug operator<<(QDebug debug, StiebelEltronModbusConnection *stiebelEltronModbusConnection);

#endif // STIEBELELTRONMODBUSCONNECTION_H
