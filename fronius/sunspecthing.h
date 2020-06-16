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

#ifndef SUNSPECTHING_H
#define SUNSPECTHING_H

#include "integrations/thing.h"

#include <QObject>
#include <QHostAddress>
#include <QByteArray>
#include <QBitArray>

#include <modbus/modbus.h>
#include <modbus/modbus-tcp.h>

class SunspecThing : public QObject
{
    Q_OBJECT
public:
    enum StorageState {
        Off = 1,
        Empty = 2,
        Discharging = 3,
        Charging = 4,
        Full = 5,
        Holding = 6,
        Testing = 7
    };
    Q_ENUM(StorageState)

    explicit SunspecThing(Thing *thing, QObject *parent = nullptr);
    ~SunspecThing();

private:
    Thing *m_thing;
    modbus_t *m_modbus;
    int m_slaveId = 1;
    bool m_floatingPointRepresentation = false;

    void destroyModbus();

    void readCommonBlock();
    void readStorageBlock();

    QByteArray convertModbusRegister(const uint16_t &modbusData);
    QBitArray convertModbusRegisterBits(const uint16_t &modbusData);
    QByteArray convertModbusRegisters(uint16_t *modbusData, const int &offset, const int &size);

    QString storageStateToString(const StorageState &state);

public slots:
    bool connectModbus();
    void disconnectModbus();

    bool setGridCharging(const bool &charging);
    bool setDischargingRate(const int &charging);
    bool setChargingRate(const int &charging);
    bool setStorageControlMode(const int &charging);

    void update();
};

#endif // SUNSPECTHING_H
