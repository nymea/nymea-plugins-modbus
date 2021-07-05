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

SunSpecModel *SunSpecModelFactory::createModel(SunSpecConnection *connection, quint16 modelId, quint16 modelLength)
{
    switch(modelId) {
    case 1:
        return new SunSpecCommonModel(connection, modelId, modelLength, connection);
    case 2:
        return new SunSpecAggregatorModel(connection, modelId, modelLength, connection);
    case 101:
        return new SunSpecInverterSinglePhaseModel(connection, modelId, modelLength, connection);
    case 102:
        return new SunSpecInverterSplitPhaseModel(connection, modelId, modelLength, connection);
    case 103:
        return new SunSpecInverterThreePhaseModel(connection, modelId, modelLength, connection);
    case 111:
        return new SunSpecInverterSinglePhaseFloatModel(connection, modelId, modelLength, connection);
    case 112:
        return new SunSpecInverterSplitPhaseFloatModel(connection, modelId, modelLength, connection);
    case 113:
        return new SunSpecInverterThreePhaseFloatModel(connection, modelId, modelLength, connection);
    case 120:
        return new SunSpecNameplateModel(connection, modelId, modelLength, connection);
    case 121:
        return new SunSpecSettingsModel(connection, modelId, modelLength, connection);
    case 122:
        return new SunSpecStatusModel(connection, modelId, modelLength, connection);
    case 123:
        return new SunSpecControlsModel(connection, modelId, modelLength, connection);
    case 124:
        return new SunSpecStorageModel(connection, modelId, modelLength, connection);
    case 801:
        return new SunSpecEnergyStorageBaseModelDeprecatedModel(connection, modelId, modelLength, connection);
    case 125:
        return new SunSpecPricingModel(connection, modelId, modelLength, connection);
    case 126:
        return new SunSpecVoltVarModel(connection, modelId, modelLength, connection);
    case 127:
        return new SunSpecFreqWattParamModel(connection, modelId, modelLength, connection);
    case 128:
        return new SunSpecReactiveCurrentModel(connection, modelId, modelLength, connection);
    case 129:
        return new SunSpecLvrtModel(connection, modelId, modelLength, connection);
    case 130:
        return new SunSpecHvrtModel(connection, modelId, modelLength, connection);
    case 131:
        return new SunSpecWattPfModel(connection, modelId, modelLength, connection);
    case 132:
        return new SunSpecVoltWattModel(connection, modelId, modelLength, connection);
    case 133:
        return new SunSpecScheduleModel(connection, modelId, modelLength, connection);
    case 134:
        return new SunSpecFreqWattModel(connection, modelId, modelLength, connection);
    case 135:
        return new SunSpecLfrtModel(connection, modelId, modelLength, connection);
    case 136:
        return new SunSpecHfrtModel(connection, modelId, modelLength, connection);
    case 137:
        return new SunSpecLvrtcModel(connection, modelId, modelLength, connection);
    case 138:
        return new SunSpecHvrtcModel(connection, modelId, modelLength, connection);
    case 139:
        return new SunSpecLvrtxModel(connection, modelId, modelLength, connection);
    case 140:
        return new SunSpecHvrtxModel(connection, modelId, modelLength, connection);
    case 141:
        return new SunSpecLfrtcModel(connection, modelId, modelLength, connection);
    case 142:
        return new SunSpecHfrtcModel(connection, modelId, modelLength, connection);
    case 143:
        return new SunSpecLfrtxModel(connection, modelId, modelLength, connection);
    case 144:
        return new SunSpecHfrtxModel(connection, modelId, modelLength, connection);
    case 145:
        return new SunSpecExtSettingsModel(connection, modelId, modelLength, connection);
    case 160:
        return new SunSpecMpptModel(connection, modelId, modelLength, connection);
    case 201:
        return new SunSpecMeterSinglePhaseModel(connection, modelId, modelLength, connection);
    case 202:
        return new SunSpecMeterSplitSinglePhaseAbnModel(connection, modelId, modelLength, connection);
    case 203:
        return new SunSpecMeterThreePhaseModel(connection, modelId, modelLength, connection);
    case 204:
        return new SunSpecMeterThreePhaseDeltaConnectModel(connection, modelId, modelLength, connection);
    case 211:
        return new SunSpecMeterSinglePhaseFloatModel(connection, modelId, modelLength, connection);
    case 212:
        return new SunSpecMeterSplitSinglePhaseFloatModel(connection, modelId, modelLength, connection);
    case 213:
        return new SunSpecMeterThreePhaseWyeConnectModel(connection, modelId, modelLength, connection);
    case 214:
        return new SunSpecDeltaConnectThreePhaseAbcMeterModel(connection, modelId, modelLength, connection);
    case 220:
        return new SunSpecSecureAcMeterSelectedReadingsModel(connection, modelId, modelLength, connection);
    case 302:
        return new SunSpecIrradianceModel(connection, modelId, modelLength, connection);
    case 305:
        return new SunSpecLocationModel(connection, modelId, modelLength, connection);
    case 306:
        return new SunSpecRefPointModel(connection, modelId, modelLength, connection);
    case 307:
        return new SunSpecBaseMetModel(connection, modelId, modelLength, connection);
    case 308:
        return new SunSpecMiniMetModel(connection, modelId, modelLength, connection);
    case 403:
        return new SunSpecStringCombinerCurrentModel(connection, modelId, modelLength, connection);
    case 404:
        return new SunSpecStringCombinerAdvancedModel(connection, modelId, modelLength, connection);
    case 502:
        return new SunSpecSolarModuleModel(connection, modelId, modelLength, connection);
    case 601:
        return new SunSpecTrackerControllerModel(connection, modelId, modelLength, connection);
    case 802:
        return new SunSpecBatteryModel(connection, modelId, modelLength, connection);
    case 803:
        return new SunSpecLithiumIonBankModel(connection, modelId, modelLength, connection);
    case 804:
        return new SunSpecLithiumIonStringModel(connection, modelId, modelLength, connection);
    case 805:
        return new SunSpecLithiumIonModuleModel(connection, modelId, modelLength, connection);
    case 806:
        return new SunSpecFlowBatteryModel(connection, modelId, modelLength, connection);
    case 807:
        return new SunSpecFlowBatteryStringModel(connection, modelId, modelLength, connection);
    case 808:
        return new SunSpecFlowBatteryModuleModel(connection, modelId, modelLength, connection);
    case 809:
        return new SunSpecFlowBatteryStackModel(connection, modelId, modelLength, connection);
    default:
        return nullptr;
    }
}

