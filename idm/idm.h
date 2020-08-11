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

#ifndef IDM_H
#define IDM_H

#include <QObject>

#include "../modbus/modbustcpmaster.h"

class Idm : public QObject
{
    Q_OBJECT
public:
    explicit Idm(const QHostAddress &address, QObject *parent = nullptr);
    void getOutsideTemperature();
    void getCurrentFaultNumber();


private:

    enum IscModus {
        KeineAbwarme = 0,
        Heizung      = 1,
        Warmwasser   = 4,
        Warmequelle  = 8,
    };

    enum RegisterList {
        OutsideTemperature              = 1000,
        MeanOutsideTemperature          = 1002,
        CurrentFaultNumber              = 1004,
        OperationModeSystem             = 1005,
        SmartGridStatus                 = 1006,
        HeatStorageTemperature          = 1008,
        ColdStorageTemperature          = 1010,
        DrinkingWaterHeaterTempBelow    = 1012,
        DrinkingWaterHeaterTempAbove    = 1014,
        HotWaterTapTemperature          = 1030,
        TargetHotWaterTemperature       = 1032,
        HeatPumpOperatingMode           = 1090,
        SummationFaultHeatPump          = 1099,
        Humiditysensor                  = 1392,
        ExternalOutsideTemperature      = 1690,
        ExternalHumidity                = 1692,
        ExternalRequestTemperatureHeating = 1694, //Externe Anforderungstemperatur Heizen
        ExternalRequestTemperatureCooling = 1695, // Externe Anforderungstemperatur Kühlen
        HeatingRequirement              = 1710
        CoolingRequirement              = 1711,
        HotWaterChargingRequirement     = 1712, // Anforderung Warmwasserladung
        //Wärmemenge Heizen
        //Wärmemenge Kühlen,
        //Wärmemenge Warmwasser,
        //Wärmemenge Abtauung,
        //Wärmemenge Passive Kühlung,
        //Wärmemenge Solar,
        //Wärmemenge Elektroheizeinsatz,
        Momentanleistung
        SolarKollektortemperatur
        SolarKollektorrücklauftemperatur
        SolarLadetemperatur
        MomentanleistungSolar,
        SolarOperatingMode          =
        ISCModus                    = 1874,
        AcknowledgeFaultMessages    = 1999, // Störmeldungen quittieren
        Aktueller PV-Überschuss
        Aktueller PV Produktion
        Aktuelle Leistungsaufnahme Wärmepumpe
    };

    QHostAddress m_hostAddress;
    ModbusTCPMaster *m_modbusMaster = nullptr;

signals:

};

#endif // IDM_H
