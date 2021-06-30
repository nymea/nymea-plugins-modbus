/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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

#ifndef SUNSPECBATTERYMODEL_H
#define SUNSPECBATTERYMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecBatteryModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum ChaSt {
        ChaStOff = 1,
        ChaStEmpty = 2,
        ChaStDischarging = 3,
        ChaStCharging = 4,
        ChaStFull = 5,
        ChaStHolding = 6,
        ChaStTesting = 7
    };
    Q_ENUM(ChaSt)

    enum LocRemCtl {
        LocRemCtlRemote = 0,
        LocRemCtlLocal = 1
    };
    Q_ENUM(LocRemCtl)

    enum Typ {
        TypNotApplicableUnknown = 0,
        TypLeadAcid = 1,
        TypNickelMetalHydrate = 2,
        TypNickelCadmium = 3,
        TypLithiumIon = 4,
        TypCarbonZinc = 5,
        TypZincChloride = 6,
        TypAlkaline = 7,
        TypRechargeableAlkaline = 8,
        TypSodiumSulfur = 9,
        TypFlow = 10,
        TypOther = 99
    };
    Q_ENUM(Typ)

    enum State {
        StateDisconnected = 1,
        StateInitializing = 2,
        StateConnected = 3,
        StateStandby = 4,
        StateSocProtection = 5,
        StateSuspending = 6,
        StateFault = 99
    };
    Q_ENUM(State)

    enum ReqInvState {
        ReqInvStateNoRequest = 0,
        ReqInvStateStart = 1,
        ReqInvStateStop = 2
    };
    Q_ENUM(ReqInvState)

    enum SetOp {
        SetOpConnect = 1,
        SetOpDisconnect = 2
    };
    Q_ENUM(SetOp)

    enum SetInvState {
        SetInvStateInverterStopped = 1,
        SetInvStateInverterStandby = 2,
        SetInvStateInverterStarted = 3
    };
    Q_ENUM(SetInvState)

    explicit SunSpecBatteryModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecBatteryModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    void readModelHeader() override;
    void readBlockData() override;

private:
    void initDataPoints();

};

#endif // SUNSPECBATTERYMODEL_H
