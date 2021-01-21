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

class SunSpecMeter : public QObject
{
    Q_OBJECT
public:
    //Model 203 = Three phase meter
    enum MandatoryRegistersModel20X {
        TotalAcCurrent          = 2,
        PhaseACurrent           = 3,
        PhaseBCurrent           = 4,
        PhaseCCurrent           = 5,
        CurrentScaleFactor      = 6,
        VoltageLN               = 7,
        PhaseVoltageAN          = 8,
        PhaseVoltageBN          = 9,
        PhaseVoltageCN          = 10,
        VoltageLL               = 11,
        PhaseVoltageAB          = 12,
        PhaseVoltageBC          = 13,
        PhaseVoltageCA          = 14,
        VoltageScaleFactor      = 15,
        Frequency               = 16,
        TotalRealPower          = 18,
        RealPowerScaleFactor    = 22,
        TotalRealEnergyExported = 38,
        TotalRealEnergyImported = 46,
        RealEnergyScaleFactor   = 54,
        MeterEventFlags         = 105
    };

    struct MeterData {
        SunSpec::SunSpecEvent1 event;
        SunSpec::SunSpecOperatingState operatingState;
    };

    SunSpecMeter(SunSpec *sunspec, SunSpec::BlockId blockId, int modbusAddress);
    SunSpec::BlockId blockId();
    void init();
    void getMeterMap();

private:
    SunSpec *m_connection = nullptr;
    SunSpec::BlockId m_id = SunSpec::BlockIdDeltaConnectThreePhaseMeter;
    uint m_mapLength = 0;
    uint m_mapModbusStartRegister = 40000;
    bool m_initFinishedSuccess = false;

    void readMeterBlockHeader();

private slots:
   void onModbusMapReceived(SunSpec::BlockId mapId, uint mapLength, QVector<quint16> data);

signals:
    void initFinished(bool success);
    void meterDataReceived(const MeterData &data);
};

#endif // SUNSPECMETER_H
