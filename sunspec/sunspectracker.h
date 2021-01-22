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

#ifndef SUNSPECTRACKER_H
#define SUNSPECTRACKER_H

#include <QObject>
#include "sunspec.h"

class SunSpecTracker : public SunSpec
{
    Q_OBJECT
public:

    enum TrackerType {
        Unknown = 0,
        Fixed   = 1,
        Horizontal = 2,
        Tilted  = 3,
        Azimuth = 4,
        Dual    = 5,
        Other = 99
    };
    Q_ENUM(TrackerType)

    struct TrackerData {
        SunSpecEvent1 event;
        SunSpecOperatingState operatingState;
    };

    SunSpecTracker(const QHostAddress &hostAddress, uint port = 502, QObject *parent = 0);

    void getTrackerMap();

private:
    ModelId m_id = ModelIdTrackerController;
    uint m_mapLength = 0;
    uint m_mapModbusStartRegister = 40000;

    void readTrackerBlockHeader();

private slots:
    //void onModbusMapReceived(BlockId mapId, uint mapLength, QVector<quint16> data);

signals:
    void initFinished();
    void trackerDataReceived(const TrackerData &data);
};

#endif // SUNSPECTRACKER_H
