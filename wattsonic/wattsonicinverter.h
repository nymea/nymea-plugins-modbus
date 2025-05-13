/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
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

#ifndef WATTSONICINVERTER_H
#define WATTSONICINVERTER_H

#include <QObject>

#include "wattsonicmodbusrtuconnection.h"

class WattsonicInverter : public WattsonicModbusRtuConnection
{
    Q_OBJECT
public:
    struct Info {
        QString type;
        QString model;
    };

    enum Generation {
        GenerationUnknwon,
        Generation2,
        Generation3
    };
    Q_ENUM(Generation)

    explicit WattsonicInverter(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent = nullptr);

    Generation generation() const;
    WattsonicInverter::Info inverterInfo() const;

    // Generation specific registers
    float batteryVoltageDc() const;
    BatteryMode batteryMode() const;
    qint32 batteryPower() const;
    float totalEnergyInjectedToGrid() const;
    float totalEnergyPurchasedFromGrid() const;
    float SOC() const;
    float SOH() const;

    bool update() override;

    static WattsonicInverter::Info getInverterInfo(quint16 equipmentInfo);

signals:
    void generationChanged();
    void customInitializationFinished(bool success);

private:
    WattsonicInverter::Info m_inverterInfo;
    Generation m_generation = GenerationUnknwon;

};

#endif // WATTSONICINVERTER_H
