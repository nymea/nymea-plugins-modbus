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
#include "sunspec.h"

class SunSpecInverter : public QObject
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
        SunSpec::SunSpecEvent1 event;
        SunSpec::SunSpecOperatingState operatingState;
    };

    SunSpecInverter(SunSpec *sunspec, SunSpec::BlockId blockId, int modbusAddress);
    SunSpec::BlockId blockId();
    void init();
    void getInverterMap();

private:
    SunSpec *m_connection = nullptr;
    SunSpec::BlockId m_id; //e.g. 103 for three phase inverter, 113 for three phase inverter with floating point representation
    uint m_mapLength = 0;
    uint m_mapModbusStartRegister = 40000;
    bool m_initFinishedSuccess = false;

    void readInverterBlockHeader();

private slots:
    void onModbusMapReceived(SunSpec::BlockId mapId, uint mapLength, QVector<quint16> data);

signals:
    void initFinished(bool success);
    void inverterDataReceived(InverterData data);
};

#endif // SUNSPECINVERTER_H
