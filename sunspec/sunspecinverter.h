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

class SunSpecInverter : public SunSpec
{
    Q_OBJECT
public:
    SunSpecInverter(const QHostAddress &hostAddress, uint port = 502, QObject *parent = 0);

    enum Model10X { // Mandatory register
        Model10XAcCurrent            = 2,
        Model10XPhaseACurrent        = 3,
        Model10XPhaseBCurrent        = 4,
        Model10XPhaseCCurrent        = 5,
        Model10XAmpereScaleFactor    = 6,
        Model10XPhaseVoltageAN       = 10,
        Model10XPhaseVoltageBN       = 11,
        Model10XPhaseVoltageCN       = 12,
        Model10XVoltageScaleFactor   = 13,
        Model10XACPower              = 14,
        Model10XWattScaleFactor      = 15,
        Model10XLineFrequency        = 16,
        Model10XHerzScaleFactor      = 17,
        Model10XAcEnergy             = 24,
        Model10XWattHoursScaleFactor = 25,
        Model10XCabinetTemperature   = 33,
        Model10XTemperatureScaleFactor = 37,
        Model10XOperatingState       = 38,
        Model10XEvent1               = 40
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
        SunSpecEvent1 event;
        SunSpecOperatingState operatingState;
    };

    void getInverterMap();

private:
    BlockId m_id = BlockIdInverterThreePhase; //e.g. 103 for three phase inverter, 113 for three phase inverter with floating point representation
    uint m_mapLength = 0;
    uint m_mapModbusStartRegister = 40000;

    void readInverterBlockHeader();

private slots:
    void onModbusMapReceived(BlockId mapId, uint mapLength, QVector<quint16> data);

signals:
    void initFinished();
    void inverterDataReceived(InverterData data);
};

#endif // SUNSPECINVERTER_H
