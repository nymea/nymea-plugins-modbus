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

#ifndef SUNSPECINVERTER_H
#define SUNSPECINVERTER_H

#include <QObject>
#include "sunspecdevice.h"

class SunSpecInverter : public SunSpecDevice
{
    Q_OBJECT
public:

    enum Model10X { // Mandatory register
        Model10XAcCurrent            = 0,
        Model10XPhaseACurrent        = 1,
        Model10XPhaseBCurrent        = 2,
        Model10XPhaseCCurrent        = 3,
        Model10XAmpereScaleFactor    = 4,
        Model10XPhaseVoltageAN       = 8,
        Model10XPhaseVoltageBN       = 9,
        Model10XPhaseVoltageCN       = 10,
        Model10XVoltageScaleFactor   = 11,
        Model10XACPower              = 12,
        Model10XWattScaleFactor      = 13,
        Model10XLineFrequency        = 14,
        Model10XHerzScaleFactor      = 15,
        Model10XAcEnergy             = 22,
        Model10XWattHoursScaleFactor = 24,
        Model10XCabinetTemperature   = 31,
        Model10XTemperatureScaleFactor = 35,
        Model10XOperatingState       = 36,
        Model10XEvent1               = 38
    };

    enum Model11X { // Mandatory register
        Model11XAcCurrent            = 0,
        Model11XPhaseACurrent        = 2,
        Model11XPhaseBCurrent        = 4,
        Model11XPhaseCCurrent        = 6,
        Model11XPhaseVoltageAN       = 14,
        Model11XPhaseVoltageBN       = 16,
        Model11XPhaseVoltageCN       = 18,
        Model11XACPower              = 20,
        Model11XLineFrequency        = 22,
        Model11XAcEnergy             = 30,
        Model11XCabinetTemperature   = 38,
        Model11XOperatingState       = 46,
        Model11XEvent1               = 48
    };

    enum Model10XOptional { // Optional register
        Model10XPhaseVoltageAB      = 5,
        Model10XPhaseVoltageBC      = 6,
        Model10XPhaseVoltageCA      = 7,
        Model10XACApparentPower     = 16,
        Model10XACApparentPowerSF   = 17,
        Model10XACReactivePower     = 18,
        Model10XACReactivePowerSF   = 19,
        Model10XACPowerFactor       = 20,
        Model10XACPowerFactorSF     = 21,
        Model10XDCCurrent           = 25,
        Model10XDCCurrentSF         = 26,
        Model10XDCVoltage           = 27,
        Model10XDCVoltageSF         = 28,
        Model10XDCPower             = 29,
        Model10XDCPowerSF           = 30,
        Model10XHeatSinkTemperature = 32,
        Model10XTransformerTemperature = 33,
        Model10XOtherTemperature       = 34,
        Model10XVendorOperatingState   = 37,
        Model10XVendorEventBitfield1   = 42,
        Model10XVendorEventBitfield2   = 44,
        Model10XVendorEventBitfield3   = 46,
        Model10XVendorEventBitfield4   = 48
    };

    enum Model11XOptional { // Optinal registers
        Model11XPhaseVoltageAB      = 8,
        Model11XPhaseVoltageBC      = 10,
        Model11XPhaseVoltageCA      = 12,
        Model11XACApparentPower     = 24,
        Model11XACReactivePower     = 26,
        Model11XACPowerFactor       = 28,
        Model11XDCCurrent           = 32,
        Model11XDCVoltage           = 34,
        Model11XDCPower             = 36,
        Model11XHeatSinkTemperature     = 40,
        Model11XTransformerTemperature  = 42,
        Model11XOtherTemperature        = 44,
        Model11XVendorOperatingState    = 47,
        Model11XVendorEventBitfield1    = 52,
        Model11XVendorEventBitfield2    = 54,
        Model11XVendorEventBitfield3    = 56,
        Model11XVendorEventBitfield4    = 58
    };

    struct SunSpecEvent1 {
        bool groundFault;
        bool dcOverVoltage;
        bool acDisconnect;
        bool dcDicconnect;
        bool gridDisconnect;
        bool cabinetOpen;
        bool manualShutdown;
        bool overTemperature;
        bool overFrequency;
        bool underFrequency;
        bool acOverVolt;
        bool acUnderVolt;
        bool blownStringFuse;
        bool underTemperature;
        bool memoryLoss;
        bool hwTestFailure;
    };

    struct InverterData {
        float acCurrent; //in ampere
        float phaseACurrent;
        float phaseBCurrent;
        float phaseCCurrent;
        float phaseVoltageAB;
        float phaseVoltageBC;
        float phaseVoltageCA;
        float phaseVoltageAN;
        float phaseVoltageBN;
        float phaseVoltageCN;
        float acPower;
        float lineFrequency;
        float acEnergy;
        float cabinetTemperature; // in degree Celsius
        SunSpecEvent1 event1;
        SunSpec::SunSpecOperatingState operatingState;
    };

    SunSpecInverter(SunSpec *sunspec, SunSpec::ModelId modelId, int modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecInverter() override = default;

    void init() override;
    void readModelHeader() override;
    void readBlockData() override;

private:
    SunSpecEvent1 bitfieldToSunSpecEvent1(quint16 register0, quint16 register1);
    void getInverterModelHeader();

private slots:
    void onModelDataBlockReceived(SunSpec::ModelId mapId, uint mapLength, QVector<quint16> data);

signals:
    void inverterDataReceived(InverterData data);
};

#endif // SUNSPECINVERTER_H
