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

#include "mtecinfo.h"
#include "mtechelpers.h"

class MTec : public QObject
{
    Q_OBJECT
public:
    /** Constructor */
    explicit MTec(const QHostAddress &address, QObject *parent = nullptr);

    /** Destructor */
    ~MTec();

    inline QHostAddress hostAddress() const { return m_hostAddress; };

    bool connectDevice();
    void disconnectDevice();

    void requestStatus();

private:
    /** Modbus Unit ID (undocumented, guessing 1 for now) */
    static const quint16 ModbusUnitID = 1;

    /** The following modbus addresses can be read: */
    enum RegisterList {
        /**
         * APPL.CtrlAppl.sParam.heatpump[0].ElectricEnergyMeter.values.power
         * Actual power consumtion [W]
         */
        ActualPowerConsumption              = 707,

        /**
         * APPL.CtrlAppl.sIOModule.Virt[0].param.sensor[0]
         * Acutal excess energy given from Smart home System [W]
         */
        ActualExcessEnergySmartHome         = 1000,

        /**
         * APPL.CtrlAppl.sParam.photovoltaics.ElectricEnergyMeter.values.power
         * Acutal excess energy given from Electricity Meter [W]
         */
        ActualExcessEnergyElectricityMeter  = 1002,

        /**
         * APPL.CtrlAppl.sParam.extHeatSource[0].param.externalSetvalueScaled
         * Control of the heat source by an external control [100%]
         */
        ExternalSetValueScaling             = 1600,

        /**
         * APPL.CtrlAppl.sParam.extHeatSource[0].values.requestExtHeatsource
         * 0…no request, external heat source must be turned off.
         * 1…external heat source is released and can be switched on.
         * 2…external heat source is required and must be turned on.
         */
        RequestExternalHeatSource           = 1601
    };

    /* Note: This class only requires one IP address and one
     *       TCP Modbus connection. Multiple devices are managed
     *       within the IntegrationPluginMTec class. */
    QHostAddress m_hostAddress;

    /** Pointer to ModbusTCPMaster object, responseible for low-level communicaiton */
    ModbusTCPMaster *m_modbusMaster = nullptr;

    /** This structure is filled by the receivedStatus... functions */
    MTecInfo m_info ;


signals:
    void connectedChanged(bool connected);
    void statusUpdated(const MTecInfo &info);

private slots:
    void onModbusError();
    void onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value);

};

#endif // MTEC_H

