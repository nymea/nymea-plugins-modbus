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

#ifndef IDM_H
#define IDM_H

#include <QObject>

#include <modbustcpmaster.h>

#include "idminfo.h"

/*
 * Functionality:
 *       The current version allows read access to selected
 *       modbus registers:
 *       + Room temperature (HK A)
 *       + Water temperature
 *       + Outside air temperature
 *       + Target room temperature eco mode (HK A)
 *       + Target water temperature
 *       + Current power consumption
 *       + Operation mode
 *       + Error
 *      
 *       At present there is no write access for target
 *       room temperature and target water temperature. These
 *       result in an "invalid data access" error from the
 *       device.
 */

/* Note: It would be desirable to read the modbus registers
 *       of the Idm heat pump in groups to minimize the number
 *       of read requests. However, a maximum of 6 registers 
 *       can be read simultaneously. With the given set of
 *       addresses it is not possible to reasonably group the
 *       registers, therefore they are read individually.
 */

class Idm : public QObject
{
    Q_OBJECT
public:
    explicit Idm(const QHostAddress &address, QObject *parent = nullptr);
    ~Idm();

    bool connectDevice();
    QHostAddress address() const;
    QUuid setTargetTemperature(double targetTemperature);
    void getStatus();

private:
    /** Modbus Unit ID of Idm device */
    static const quint16 modbusUnitID = 1;

    enum IscModus {
        KeineAbwarme = 0,
        Heizung      = 1,
        Warmwasser   = 4,
        Warmequelle  = 8,
    };

    /** System operation modes according to manual p. 13 */
    enum IdmSysMode {
        IdmSysModeStandby           = 0,
        IdmSysModeAutomatic         = 1,
        IdmSysModeAway              = 2,
        IdmSysModeOnlyHotwater      = 4,
        IdmSysModeOnlyRoomHeating   = 5
    };

    /** Heat pump operation modes according to manual p. 14 */
    enum IdmHeatPumpMode {
        IdmHeatPumpModeOff          = 0,
        IdmHeatPumpModeHeating      = 1,
        IdmHeatPumpModeCooling      = 2,
        IdmHeatPumpModeHotWater     = 4,
        IdmHeatPumpModeDefrost      = 8
    };

    /** The following modbus addresses are according to the manual
     *  Modbus TCP Navigatorregelung 2.0 pages 13-31.
     *  Comments at the end of each line give their original name
     *  in the German manual. */
    enum RegisterList {
        OutsideTemperature              = 1000, // Außentemperatur (B31)
        MeanOutsideTemperature          = 1002, // Gemittelte Außentemperature
        CurrentFaultNumber              = 1004, // Aktuelle Störungsnummer
        OperationModeSystem             = 1005, // Betriebsart System
        SmartGridStatus                 = 1006, // Smart Grid Status
        HeatStorageTemperature          = 1008, // Wärmespeichertemperatur (B38)
        ColdStorageTemperature          = 1010, // Kältespeichertemperatur (B40)
        DrinkingWaterHeaterTempBottom   = 1012, // Trinkwassererwärmertmp. unten (B41)
        DrinkingWaterHeaterTempTop      = 1014, // Trinkwassererwärmertmp. oben (B48)
        HotWaterTapTemperature          = 1030, // Warmwasserzapftemperatur (B42)
        TargetHotWaterTemperature       = 1032, // Warmwasser-Solltemperatur
        HeatPumpOperatingMode           = 1090, // Betriebsart Wärmepumpe
        SummationFaultHeatPump          = 1099, // Summenstörung Wärepumpe
        RoomTemperatureHKA              = 1364, // Heizkreis A Raumtemperature (B61)
        HumiditySensor                  = 1392, // Feuchtesensor
        RoomTemperatureTargetHeatingHKA = 1401, // Raumsolltemperatur Heizen Normal HK A
        RoomTemperatureTargetHeatingEcoHKA = 1415, // Raumsolltemperatur Heizen Eco HK A
        ExternalOutsideTemperature      = 1690, // Externe Außentemperatur
        ExternalHumidity                = 1692, // Externe Feuchte
        ExternalRequestTemperatureHeating = 1694, // Externe Anforderungstemperatur Heizen
        ExternalRequestTemperatureCooling = 1695, // Externe Anforderungstemperatur Kühlen
        HeatingRequirement              = 1710, // Anforderung Heizen
        CoolingRequirement              = 1711, // Anforderung Kühlen
        HotWaterChargingRequirement     = 1712, // Anforderung Warmwasserladung
        HeatQuantityHeating             = 1750, // Wärmemenge Heizen
        HeatQuantityCooling             = 1752, // Wärmemenge Kühlen
        HeatQuantityHotWater            = 1754, // Wärmemenge Warmwasser
        HeatQuantityDefrosting          = 1756, // Wärmemenge Abtauung
        HeatQuantityPassiveCooling      = 1758, // Wärmemenge Passive Kühlung,
        HeatQuantityPhotovolatics       = 1760, // Wärmemenge Solar
        HeatQuantityHeatingElemetn      = 1762, // Wärmemenge Elektroheizeinsatz,
        CurrentPower                    = 1790, // Momentanleistung
        CurrentPowerSolar               = 1792, // MomentanleistungSolar
        SolarCollectorTemperature       = 1850, // SolarKollektortemperatur (B73)
        SolarCollectorReturnTemperature = 1852, // SolarKollektorruecklauftemperatur (B75)
        SolarChargeTemperature          = 1854, // SolarLadetemperatur (B74)
        SolarOperatingMode              = 1856, // Betriebsart Solar
        ISCMode                         = 1875, // ISCModus
        AcknowledgeFaultMessages        = 1999, // Störmeldungen quittieren
        TargetRoomTemperatureZ1R1       = 2004, // Zonenmodul 1 Raumsolltemperatur Raum 1
        CurrentPhotovoltaicsSurplus     =   74, // Aktueller PV-Überschuss
        CurrentPhotovoltaicsProduction  =   78, // Aktueller PV Produktion
        CurrentPowerConsumptionHeatPump = 4122, // Aktuelle Leistungsaufnahme Wärmepumpe
    };

    /* Note: This class only requires one IP address and one
     *       TCP Modbus connection. Multiple devices are managed
     *       within the IntegrationPluginIdm class. */
    QHostAddress m_hostAddress;

    /** Pointer to ModbusTCPMaster object, responsible for low-level communicaiton */
    ModbusTCPMaster *m_modbusMaster = nullptr;

    /** This structure is allocated within onRequestStatus and filled
     * by the receivedStatusGroupx functions */
    IdmInfo m_idmInfo;

    /** Converts a heat pump operation mode code to a string (according to manual p. 14) */
    QString heatPumpOperationModeToString(IdmHeatPumpMode mode);

signals:
    void statusUpdated(const IdmInfo &info);
    void targetRoomTemperatureChanged();
    void writeRequestExecuted(const QUuid &requestId, bool success);

private slots:
    void onModbusError();
    void onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value);
};

#endif // IDM_H
