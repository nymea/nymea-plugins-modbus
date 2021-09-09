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
#include "sunspecaggregatormodel.h"
#include "sunspecinvertersinglephasemodel.h"
#include "sunspecinvertersplitphasemodel.h"
#include "sunspecinverterthreephasemodel.h"
#include "sunspecinvertersinglephasefloatmodel.h"
#include "sunspecinvertersplitphasefloatmodel.h"
#include "sunspecinverterthreephasefloatmodel.h"
#include "sunspecnameplatemodel.h"
#include "sunspecsettingsmodel.h"
#include "sunspecstatusmodel.h"
#include "sunspeccontrolsmodel.h"
#include "sunspecstoragemodel.h"
#include "sunspecenergystoragebasemodeldeprecatedmodel.h"
#include "sunspecpricingmodel.h"
#include "sunspecvoltvarmodel.h"
#include "sunspecfreqwattparammodel.h"
#include "sunspecreactivecurrentmodel.h"
#include "sunspeclvrtmodel.h"
#include "sunspechvrtmodel.h"
#include "sunspecwattpfmodel.h"
#include "sunspecvoltwattmodel.h"
#include "sunspecschedulemodel.h"
#include "sunspecfreqwattmodel.h"
#include "sunspeclfrtmodel.h"
#include "sunspechfrtmodel.h"
#include "sunspeclvrtcmodel.h"
#include "sunspechvrtcmodel.h"
#include "sunspeclvrtxmodel.h"
#include "sunspechvrtxmodel.h"
#include "sunspeclfrtcmodel.h"
#include "sunspechfrtcmodel.h"
#include "sunspeclfrtxmodel.h"
#include "sunspechfrtxmodel.h"
#include "sunspecextsettingsmodel.h"
#include "sunspecmpptmodel.h"
#include "sunspecmetersinglephasemodel.h"
#include "sunspecmetersplitsinglephaseabnmodel.h"
#include "sunspecmeterthreephasemodel.h"
#include "sunspecmeterthreephasedeltaconnectmodel.h"
#include "sunspecmetersinglephasefloatmodel.h"
#include "sunspecmetersplitsinglephasefloatmodel.h"
#include "sunspecmeterthreephasewyeconnectmodel.h"
#include "sunspecdeltaconnectthreephaseabcmetermodel.h"
#include "sunspecsecureacmeterselectedreadingsmodel.h"
#include "sunspecirradiancemodel.h"
#include "sunspeclocationmodel.h"
#include "sunspecrefpointmodel.h"
#include "sunspecbasemetmodel.h"
#include "sunspecminimetmodel.h"
#include "sunspecstringcombinercurrentmodel.h"
#include "sunspecstringcombineradvancedmodel.h"
#include "sunspecsolarmodulemodel.h"
#include "sunspectrackercontrollermodel.h"
#include "sunspecbatterymodel.h"
#include "sunspeclithiumionbankmodel.h"
#include "sunspeclithiumionstringmodel.h"
#include "sunspeclithiumionmodulemodel.h"
#include "sunspecflowbatterymodel.h"
#include "sunspecflowbatterystringmodel.h"
#include "sunspecflowbatterymodulemodel.h"
#include "sunspecflowbatterystackmodel.h"

SunSpecModelFactory::SunSpecModelFactory(QObject *parent) :
    QObject(parent)
{

}

SunSpecModel *SunSpecModelFactory::createModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength)
{
    switch(modelId) {
    case ModelIdCommon:
        return new SunSpecCommonModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdAggregator:
        return new SunSpecAggregatorModel(connection, modbusStartRegister, modelLength, connection);
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
    case ModelIdNameplate:
        return new SunSpecNameplateModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdSettings:
        return new SunSpecSettingsModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdStatus:
        return new SunSpecStatusModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdControls:
        return new SunSpecControlsModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdStorage:
        return new SunSpecStorageModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdEnergyStorageBaseDeprecated:
        return new SunSpecEnergyStorageBaseModelDeprecatedModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdPricing:
        return new SunSpecPricingModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdVoltVar:
        return new SunSpecVoltVarModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdFreqWattParam:
        return new SunSpecFreqWattParamModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdReactiveCurrent:
        return new SunSpecReactiveCurrentModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLvrt:
        return new SunSpecLvrtModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdHvrt:
        return new SunSpecHvrtModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdWattPf:
        return new SunSpecWattPfModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdVoltWatt:
        return new SunSpecVoltWattModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdSchedule:
        return new SunSpecScheduleModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdFreqWatt:
        return new SunSpecFreqWattModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLfrt:
        return new SunSpecLfrtModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdHfrt:
        return new SunSpecHfrtModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLvrtc:
        return new SunSpecLvrtcModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdHvrtc:
        return new SunSpecHvrtcModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLvrtx:
        return new SunSpecLvrtxModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdHvrtx:
        return new SunSpecHvrtxModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLfrtc:
        return new SunSpecLfrtcModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdHfrtc:
        return new SunSpecHfrtcModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLfrtx:
        return new SunSpecLfrtxModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdHfrtx:
        return new SunSpecHfrtxModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdExtSettings:
        return new SunSpecExtSettingsModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdMppt:
        return new SunSpecMpptModel(connection, modbusStartRegister, modelLength, connection);
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
    case ModelIdSecureAcMeterSelectedReadings:
        return new SunSpecSecureAcMeterSelectedReadingsModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdIrradiance:
        return new SunSpecIrradianceModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLocation:
        return new SunSpecLocationModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdRefPoint:
        return new SunSpecRefPointModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdBaseMet:
        return new SunSpecBaseMetModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdMiniMet:
        return new SunSpecMiniMetModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdStringCombinerCurrent:
        return new SunSpecStringCombinerCurrentModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdStringCombinerAdvanced:
        return new SunSpecStringCombinerAdvancedModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdSolarModule:
        return new SunSpecSolarModuleModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdTrackerController:
        return new SunSpecTrackerControllerModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdBattery:
        return new SunSpecBatteryModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLithiumIonBank:
        return new SunSpecLithiumIonBankModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLithiumIonString:
        return new SunSpecLithiumIonStringModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdLithiumIonModule:
        return new SunSpecLithiumIonModuleModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdFlowBattery:
        return new SunSpecFlowBatteryModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdFlowBatteryString:
        return new SunSpecFlowBatteryStringModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdFlowBatteryModule:
        return new SunSpecFlowBatteryModuleModel(connection, modbusStartRegister, modelLength, connection);
    case ModelIdFlowBatteryStack:
        return new SunSpecFlowBatteryStackModel(connection, modbusStartRegister, modelLength, connection);
    default:
        return nullptr;
    }
}

