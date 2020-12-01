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

#ifndef SUNSPECSTRINGCOMBINER_H
#define SUNSPECSTRINGCOMBINER_H

#include <QObject>
#include "sunspec.h"

class SunSpecStringCombiner : public SunSpec
{
    Q_OBJECT
public:

    //Map401 length: 14 + (RB Count * 8)
    //Map403 length: 16 + (RB Count * 8)
    enum Map401 {
        Map401CurrentScaleFactor           = 0,
        Map401AmpHourScaleFactor           = 1,
        Map401VoltageScaleFactor           = 2,
        Map401MaximumDCCurrentRating       = 3,
        Map401NumberOfInputs               = 4,
        Map401Events                       = 5,
        Map401VendorDefniedEvents          = 7,
        Map401TotalMeasuredCurrent         = 9,
        Map401TotalMeteredAmpHours         = 10,
        Map401OutputVoltage                = 12,
        Map401InternalOperatingTemperature = 13
    };

    enum Map402 {
        Map402CurrentScaleFactor,
        Map402AmpHourScaleFactor,
        Map402VoltageScaleFactor,
        Map402PowerScaleFactor,
        Map402EnergyScaleFactor,
        Map402MaximumDCCurrentRating,
        Map402NumberOfInputs,
        Map402BitmaskValueEvents,
        Map402BitmaskvalueVendorDefniedEvents,
        Map402TotalMeasuredCurrent,
        Map402TotalMeteredAmpHours,
        Map402OutputVoltage
    };

    enum Map401RB { //Repeating block
        ID          = 0,
        Event       = 1,
        VendorEvent = 3,
        Amps        = 5,
        AmpHours    = 6
    };

    enum StringCombinerEvent {
        LowVoltage = 0,
        LowPower,
        LowEfficiency,
        Current,
        Voltage,
        Power,
        Pr,
        Disconnected,
        FuseFault,
        CombinerFuseFault,
        CombinerCabinetOpen,
        Temp,
        Groundfault,
        ReversedPolarity,
        Incompatible,
        CommunicationError,
        InternalError,
        Theft,
        ArcDetected
    };
    Q_ENUM(StringCombinerEvent)

    struct StringCombinerData {
        SunSpecEvent1 event;
        SunSpecOperatingState operatingState;
    };

    SunSpecStringCombiner(const QHostAddress &hostAddress, uint port = 502, QObject *parent = 0);

    void getStringCombinerMap();

private:
    BlockId m_id = BlockIdStringCombiner;
    uint m_mapLength = 0;
    uint m_mapModbusStartRegister = 40000;

    void readStringCombinerMapHeader();

private slots:
    void onModbusMapReceived(BlockId mapId, uint mapLength, QVector<quint16> data);

signals:
    void initFinished();
    void stringCombinerDataReceived(const StringCombinerData &data);
};

#endif // SUNSPECSTRINGCOMBINER_H
