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

#ifndef SUNSPECMODELFACTORY_H
#define SUNSPECMODELFACTORY_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecCommonModel;
class SunSpecInverterSinglePhaseModel;
class SunSpecInverterSplitPhaseModel;
class SunSpecInverterThreePhaseModel;
class SunSpecInverterSinglePhaseFloatModel;
class SunSpecInverterSplitPhaseFloatModel;
class SunSpecInverterThreePhaseFloatModel;
class SunSpecStorageModel;
class SunSpecMeterSinglePhaseModel;
class SunSpecMeterSplitSinglePhaseAbnModel;
class SunSpecMeterThreePhaseModel;
class SunSpecMeterThreePhaseDeltaConnectModel;
class SunSpecMeterSinglePhaseFloatModel;
class SunSpecMeterSplitSinglePhaseFloatModel;
class SunSpecMeterThreePhaseWyeConnectModel;
class SunSpecDeltaConnectThreePhaseAbcMeterModel;
class SunSpecBatteryModel;

class SunSpecModelFactory : public QObject
{
    Q_OBJECT
public:
    enum ModelId {
        ModelIdCommon = 1,
        ModelIdInverterSinglePhase = 101,
        ModelIdInverterSplitPhase = 102,
        ModelIdInverterThreePhase = 103,
        ModelIdInverterSinglePhaseFloat = 111,
        ModelIdInverterSplitPhaseFloat = 112,
        ModelIdInverterThreePhaseFloat = 113,
        ModelIdStorage = 124,
        ModelIdMeterSinglePhase = 201,
        ModelIdMeterSplitSinglePhaseAbn = 202,
        ModelIdMeterThreePhase = 203,
        ModelIdMeterThreePhaseDeltaConnect = 204,
        ModelIdMeterSinglePhaseFloat = 211,
        ModelIdMeterSplitSinglePhaseFloat = 212,
        ModelIdMeterThreePhaseWyeConnect = 213,
        ModelIdDeltaConnectThreePhaseAbcMeter = 214,
        ModelIdBattery = 802
    };
    Q_ENUM(ModelId)

    explicit SunSpecModelFactory(QObject *parent = nullptr);
    ~SunSpecModelFactory() = default;

    SunSpecModel *createModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength);

private:


};

#endif // SUNSPECMODELFACTORY_H
