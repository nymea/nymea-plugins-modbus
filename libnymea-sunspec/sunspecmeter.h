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

#ifndef SUNSPECMETER_H
#define SUNSPECMETER_H

#include <QObject>
#include "sunspec.h"
#include "sunspecdevice.h"

class SunSpecMeter : public SunSpecDevice
{
    Q_OBJECT

public:
    enum MeterEventFlags {
        MeterEventPowerFailure = 2,
        MeterEventUnderVoltage,
        MeterEventLowPF,
        MeterEventOverCurrent,
        MeterEventOverVoltage,
        MeterEventMissing_Sensor,
        MeterEventReserved1,
        MeterEventReserved2,
        MeterEventReserved3,
        MeterEventReserved4,
        MeterEventReserved5,
        MeterEventReserved6,
        MeterEventReserved7,
        MeterEventReserved8,
        MeterEventOEM01,
        MeterEventOEM02,
        MeterEventOEM03,
        MeterEventOEM04,
        MeterEventOEM05,
        MeterEventOEM06,
        MeterEventOEM07,
        MeterEventOEM08,
        MeterEventOEM09,
        MeterEventOEM10,
        MeterEventOEM11,
        MeterEventOEM12,
        MeterEventOEM13,
        MeterEventOEM14,
        MeterEventOEM15
    };

    //Model 201 = Single phase meter SF
    //Model 202 = Split phase meter SF
    //Model 203 = Three phase meter SF
    //Note: For example single phase inverters, Phase B current is optional then.
    enum Model20X {
        Model20XTotalAcCurrent          = 0,
        Model20XPhaseACurrent           = 1,
        Model20XPhaseBCurrent           = 2,
        Model20XPhaseCCurrent           = 3,
        Model20XCurrentScaleFactor      = 4,
        Model20XVoltageLN               = 5,
        Model20XPhaseVoltageAN          = 6,
        Model20XPhaseVoltageBN          = 7,
        Model20XPhaseVoltageCN          = 8,
        Model20XVoltageLL               = 9,
        Model20XPhaseVoltageAB          = 10,
        Model20XPhaseVoltageBC          = 11,
        Model20XPhaseVoltageCA          = 12,
        Model20XVoltageScaleFactor      = 13,
        Model20XFrequency               = 14,
        Model20XFrequencyScaleFactor    = 15,
        Model20XTotalRealPower          = 16,
        Model20XRealPowerScaleFactor    = 20,
        Model20XTotalRealEnergyExported = 36,
        Model20XTotalRealEnergyImported = 44,
        Model20XRealEnergyScaleFactor   = 52,
        Model20XMeterEventFlags         = 103
    };

    //Model 211 = Single phase meter float
    //Model 212 = Split phase meter float
    //Model 213 = Three phase meter float
    enum Model21X {
        Model21XTotalAcCurrent          = 0,
        Model21XPhaseACurrent           = 2,
        Model21XPhaseBCurrent           = 4,
        Model21XPhaseCCurrent           = 6,
        Model21XVoltageLN               = 8,
        Model21XPhaseVoltageAN          = 10,
        Model21XPhaseVoltageBN          = 12,
        Model21XPhaseVoltageCN          = 14,
        Model21XVoltageLL               = 16,
        Model21XPhaseVoltageAB          = 18,
        Model21XPhaseVoltageBC          = 20,
        Model21XPhaseVoltageCA          = 22,
        Model21XFrequency               = 24,
        Model21XTotalRealPower          = 26,
        Model21XTotalRealEnergyExported = 58,
        Model21XTotalRealEnergyImported = 66,
        Model21XMeterEventFlags         = 122
    };

    struct MeterData {
        double totalAcCurrent; // [A]
        double phaseACurrent;  // [A]
        double phaseBCurrent;  // [A]
        double phaseCCurrent;  // [A]
        double voltageLN; // [V]
        double phaseVoltageAN; // [V]
        double phaseVoltageBN; // [V]
        double phaseVoltageCN; // [V]
        double voltageLL;      // [V]
        double phaseVoltageAB; // [V]
        double phaseVoltageBC; // [V]
        double phaseVoltageCA; // [V]
        double frequency;      // [Hz]
        double totalRealPower; // [W]
        double totalRealEnergyExported; // [kWh]
        double totalRealEnergyImported; // [kWh]
        quint32 meterEventFlags;        // MeterEventFlags
    };

    SunSpecMeter(SunSpec *connection, SunSpec::ModelId modelId, int modbusStartAddress, QObject *parent = nullptr);
    ~SunSpecMeter() override = default;

    void init() override;
    void readModelHeader() override;
    void readBlockData() override;

private slots:
   void onModelDataBlockReceived(SunSpec::ModelId modelId, uint length, QVector<quint16> data);

signals:
    void meterDataReceived(const MeterData &data);
};

#endif // SUNSPECMETER_H
