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

SunSpecModel *SunSpecModelFactory::createModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder)
{
    switch(modelId) {
    case ModelIdCommon: {
        SunSpecCommonModel *model = new SunSpecCommonModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdAggregator: {
        SunSpecAggregatorModel *model = new SunSpecAggregatorModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdInverterSinglePhase: {
        SunSpecInverterSinglePhaseModel *model = new SunSpecInverterSinglePhaseModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdInverterSplitPhase: {
        SunSpecInverterSplitPhaseModel *model = new SunSpecInverterSplitPhaseModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdInverterThreePhase: {
        SunSpecInverterThreePhaseModel *model = new SunSpecInverterThreePhaseModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdInverterSinglePhaseFloat: {
        SunSpecInverterSinglePhaseFloatModel *model = new SunSpecInverterSinglePhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdInverterSplitPhaseFloat: {
        SunSpecInverterSplitPhaseFloatModel *model = new SunSpecInverterSplitPhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdInverterThreePhaseFloat: {
        SunSpecInverterThreePhaseFloatModel *model = new SunSpecInverterThreePhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdNameplate: {
        SunSpecNameplateModel *model = new SunSpecNameplateModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdSettings: {
        SunSpecSettingsModel *model = new SunSpecSettingsModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdStatus: {
        SunSpecStatusModel *model = new SunSpecStatusModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdControls: {
        SunSpecControlsModel *model = new SunSpecControlsModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdStorage: {
        SunSpecStorageModel *model = new SunSpecStorageModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdEnergyStorageBaseDeprecated: {
        SunSpecEnergyStorageBaseModelDeprecatedModel *model = new SunSpecEnergyStorageBaseModelDeprecatedModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdPricing: {
        SunSpecPricingModel *model = new SunSpecPricingModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdVoltVar: {
        SunSpecVoltVarModel *model = new SunSpecVoltVarModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdFreqWattParam: {
        SunSpecFreqWattParamModel *model = new SunSpecFreqWattParamModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdReactiveCurrent: {
        SunSpecReactiveCurrentModel *model = new SunSpecReactiveCurrentModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLvrt: {
        SunSpecLvrtModel *model = new SunSpecLvrtModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdHvrt: {
        SunSpecHvrtModel *model = new SunSpecHvrtModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdWattPf: {
        SunSpecWattPfModel *model = new SunSpecWattPfModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdVoltWatt: {
        SunSpecVoltWattModel *model = new SunSpecVoltWattModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdSchedule: {
        SunSpecScheduleModel *model = new SunSpecScheduleModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdFreqWatt: {
        SunSpecFreqWattModel *model = new SunSpecFreqWattModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLfrt: {
        SunSpecLfrtModel *model = new SunSpecLfrtModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdHfrt: {
        SunSpecHfrtModel *model = new SunSpecHfrtModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLvrtc: {
        SunSpecLvrtcModel *model = new SunSpecLvrtcModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdHvrtc: {
        SunSpecHvrtcModel *model = new SunSpecHvrtcModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLvrtx: {
        SunSpecLvrtxModel *model = new SunSpecLvrtxModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdHvrtx: {
        SunSpecHvrtxModel *model = new SunSpecHvrtxModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLfrtc: {
        SunSpecLfrtcModel *model = new SunSpecLfrtcModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdHfrtc: {
        SunSpecHfrtcModel *model = new SunSpecHfrtcModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLfrtx: {
        SunSpecLfrtxModel *model = new SunSpecLfrtxModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdHfrtx: {
        SunSpecHfrtxModel *model = new SunSpecHfrtxModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdExtSettings: {
        SunSpecExtSettingsModel *model = new SunSpecExtSettingsModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdMppt: {
        SunSpecMpptModel *model = new SunSpecMpptModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdMeterSinglePhase: {
        SunSpecMeterSinglePhaseModel *model = new SunSpecMeterSinglePhaseModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdMeterSplitSinglePhaseAbn: {
        SunSpecMeterSplitSinglePhaseAbnModel *model = new SunSpecMeterSplitSinglePhaseAbnModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdMeterThreePhase: {
        SunSpecMeterThreePhaseModel *model = new SunSpecMeterThreePhaseModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdMeterThreePhaseDeltaConnect: {
        SunSpecMeterThreePhaseDeltaConnectModel *model = new SunSpecMeterThreePhaseDeltaConnectModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdMeterSinglePhaseFloat: {
        SunSpecMeterSinglePhaseFloatModel *model = new SunSpecMeterSinglePhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdMeterSplitSinglePhaseFloat: {
        SunSpecMeterSplitSinglePhaseFloatModel *model = new SunSpecMeterSplitSinglePhaseFloatModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdMeterThreePhaseWyeConnect: {
        SunSpecMeterThreePhaseWyeConnectModel *model = new SunSpecMeterThreePhaseWyeConnectModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdDeltaConnectThreePhaseAbcMeter: {
        SunSpecDeltaConnectThreePhaseAbcMeterModel *model = new SunSpecDeltaConnectThreePhaseAbcMeterModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdSecureAcMeterSelectedReadings: {
        SunSpecSecureAcMeterSelectedReadingsModel *model = new SunSpecSecureAcMeterSelectedReadingsModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdIrradiance: {
        SunSpecIrradianceModel *model = new SunSpecIrradianceModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLocation: {
        SunSpecLocationModel *model = new SunSpecLocationModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdRefPoint: {
        SunSpecRefPointModel *model = new SunSpecRefPointModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdBaseMet: {
        SunSpecBaseMetModel *model = new SunSpecBaseMetModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdMiniMet: {
        SunSpecMiniMetModel *model = new SunSpecMiniMetModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdStringCombinerCurrent: {
        SunSpecStringCombinerCurrentModel *model = new SunSpecStringCombinerCurrentModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdStringCombinerAdvanced: {
        SunSpecStringCombinerAdvancedModel *model = new SunSpecStringCombinerAdvancedModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdSolarModule: {
        SunSpecSolarModuleModel *model = new SunSpecSolarModuleModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdTrackerController: {
        SunSpecTrackerControllerModel *model = new SunSpecTrackerControllerModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdBattery: {
        SunSpecBatteryModel *model = new SunSpecBatteryModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLithiumIonBank: {
        SunSpecLithiumIonBankModel *model = new SunSpecLithiumIonBankModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLithiumIonString: {
        SunSpecLithiumIonStringModel *model = new SunSpecLithiumIonStringModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdLithiumIonModule: {
        SunSpecLithiumIonModuleModel *model = new SunSpecLithiumIonModuleModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdFlowBattery: {
        SunSpecFlowBatteryModel *model = new SunSpecFlowBatteryModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdFlowBatteryString: {
        SunSpecFlowBatteryStringModel *model = new SunSpecFlowBatteryStringModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdFlowBatteryModule: {
        SunSpecFlowBatteryModuleModel *model = new SunSpecFlowBatteryModuleModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    case ModelIdFlowBatteryStack: {
        SunSpecFlowBatteryStackModel *model = new SunSpecFlowBatteryStackModel(connection, modbusStartRegister, modelLength, connection);
        model->setByteOrder(byteOrder);
        return model;
    };
    default:
        return nullptr;
    }
}

