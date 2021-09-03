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

#include "sunspecmodelfactory.h"
#include "sunspecconnection.h"

#include "sunspeccommonmodel.h"
#include "sunspecinvertersinglephasemodel.h"
#include "sunspecinvertersplitphasemodel.h"
#include "sunspecinverterthreephasemodel.h"
#include "sunspecinvertersinglephasefloatmodel.h"
#include "sunspecinvertersplitphasefloatmodel.h"
#include "sunspecinverterthreephasefloatmodel.h"
#include "sunspecstoragemodel.h"
#include "sunspecmetersinglephasemodel.h"
#include "sunspecmetersplitsinglephaseabnmodel.h"
#include "sunspecmeterthreephasemodel.h"
#include "sunspecmeterthreephasedeltaconnectmodel.h"
#include "sunspecmetersinglephasefloatmodel.h"
#include "sunspecmetersplitsinglephasefloatmodel.h"
#include "sunspecmeterthreephasewyeconnectmodel.h"
#include "sunspecdeltaconnectthreephaseabcmetermodel.h"
#include "sunspecbatterymodel.h"

SunSpecModelFactory::SunSpecModelFactory(QObject *parent) :
    QObject(parent)
{

}

SunSpecModel *SunSpecModelFactory::createModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength)
{
    switch(modelId) {
    case ModelIdCommon:
        return new SunSpecCommonModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdInverterSinglePhase:
        return new SunSpecInverterSinglePhaseModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdInverterSplitPhase:
        return new SunSpecInverterSplitPhaseModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdInverterThreePhase:
        return new SunSpecInverterThreePhaseModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdInverterSinglePhaseFloat:
        return new SunSpecInverterSinglePhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdInverterSplitPhaseFloat:
        return new SunSpecInverterSplitPhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdInverterThreePhaseFloat:
        return new SunSpecInverterThreePhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdStorage:
        return new SunSpecStorageModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdMeterSinglePhase:
        return new SunSpecMeterSinglePhaseModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdMeterSplitSinglePhaseAbn:
        return new SunSpecMeterSplitSinglePhaseAbnModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdMeterThreePhase:
        return new SunSpecMeterThreePhaseModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdMeterThreePhaseDeltaConnect:
        return new SunSpecMeterThreePhaseDeltaConnectModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdMeterSinglePhaseFloat:
        return new SunSpecMeterSinglePhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdMeterSplitSinglePhaseFloat:
        return new SunSpecMeterSplitSinglePhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdMeterThreePhaseWyeConnect:
        return new SunSpecMeterThreePhaseWyeConnectModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdDeltaConnectThreePhaseAbcMeter:
        return new SunSpecDeltaConnectThreePhaseAbcMeterModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdBattery:
        return new SunSpecBatteryModel(connection, modbusStartRegister, modelLength, connection);
    default:
        return nullptr;
    }
}

