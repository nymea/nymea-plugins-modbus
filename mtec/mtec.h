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

#ifndef MTEC_H
#define MTEC_H

#include <QDateTime>
#include <QObject>

#include "../modbus/modbustcpmaster.h"

class MTec : public QObject
{
    Q_OBJECT
public:
    enum HeatpumpState {
        HeatpumpStateStandby = 0,
        HeatpumpStatePreRun = 1,
        HeatpumpStateAutomaticHeat = 2,
        HeatpumpStateDefrost = 3,
        HeatpumpStateAutomaticCool = 4,
        HeatpumpStatePostRun = 5,
        HeatpumpStateSaftyShutdown = 7,
        HeatpumpStateError = 8
    };
    Q_ENUM(HeatpumpState)

    explicit MTec(const QHostAddress &address, QObject *parent = nullptr);
    ~MTec();

    inline QHostAddress hostAddress() const { return m_hostAddress; };

    bool connectDevice();
    void disconnectDevice();

    void updateValues();

private:
    /** Modbus Unit ID (undocumented, guessing 1 for now) */
    static const quint16 ModbusUnitID = 1;

    /** The following modbus addresses can be read: */
    enum Register {
        /* APPL.CtrlAppl.sParam.hotWaterTank[0].topTemp.values.actValue
         * Hot water tank top temperature [1/10°C]. */
        RegisterHotWaterTankTemperature = 401,

        /* APPL.CtrlAppl.sParam.bufferTank[0].topTemp.values.actValue
         * Buffer Actual top temperature [1/10°C]. */
        RegisterBufferTankTemperature = 601,

        /* APPL.CtrlAppl.sStatisticalData.heatpump[0].consumption.electricalenergy
         * Total accumulated electrical energy [kWh] */
        RegisterTotalAccumulatedElectricalEnergy = 702,

        /* APPL.CtrlAppl.sParam.heatpump[0].values.heatpumpState */
        RegisterHeatpumpState = 703,

        /* APPL.CtrlAppl.sParam.heatpump[0].HeatMeter.values.power
         * Actual power consumtion [W] */
        RegisterHeatMeterPowerConsumption = 706,

        /* APPL.CtrlAppl.sParam.heatpump[0].ElectricEnergyMeter.values.power
         * Actual power consumtion [W] */
        RegisterEnergyMeterPowerConsumption = 707,

        /* APPL.CtrlAppl.sIOModule.Virt[0].param.sensor[0]
         * Acutal excess energy given from Smart home System [W] */
        RegisterActualExcessEnergySmartHome = 1000,

        /* APPL.CtrlAppl.sParam.outdoorTemp.values.actValue
         * Actual exterior temperature [°C] */
        RegisterActualOutdoorTemperature = 1502,

    };

    QHostAddress m_hostAddress;
    ModbusTCPMaster *m_modbusMaster = nullptr;

    double m_waterTankTemperature = 0;
    double m_bufferTankTemperature = 0;
    double m_totalAccumulatedElectricalEnergy = 0;
    HeatpumpState m_heatPumpState = HeatpumpStateStandby;
    double m_heatMeterPowerConsumption = 0;
    double m_energyMeterPowerConsumption = 0;
    double m_actualExcessEnergySmartHome = 0;
    double m_actualOutdoorTemperature = 0;

signals:
    void connectedChanged(bool connected);

    void waterTankTemperatureChanged(double waterTankTemperature);
    void bufferTankTemperatureChanged(double bufferTankTemperature);
    void totalAccumulatedElectricalEnergyChanged(double totalAccumulatedElectricalEnergy);
    void heatPumpStateChanged(HeatpumpState heatPumpState);
    void heatMeterPowerConsumptionChanged(double heatMeterPowerConsumption);
    void energyMeterPowerConsumptionChanged(double energyMeterPowerConsumption);
    void actualExcessEnergySmartHomeChanged(double actualExcessEnergySmartHome);
    void actualOutdoorTemperatureChanged(double actualOutdoorTemperature);

private slots:
    void onModbusError();
    void onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value);

};

#endif // MTEC_H

