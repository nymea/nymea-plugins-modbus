/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
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
#include "sunspecdatapoint.h"

class SunSpecConnection;

class SunSpecCommonModel;
class SunSpecAggregatorModel;
class SunSpecInverterSinglePhaseModel;
class SunSpecInverterSplitPhaseModel;
class SunSpecInverterThreePhaseModel;
class SunSpecInverterSinglePhaseFloatModel;
class SunSpecInverterSplitPhaseFloatModel;
class SunSpecInverterThreePhaseFloatModel;
class SunSpecNameplateModel;
class SunSpecSettingsModel;
class SunSpecStatusModel;
class SunSpecControlsModel;
class SunSpecStorageModel;
class SunSpecEnergyStorageBaseModelDeprecatedModel;
class SunSpecPricingModel;
class SunSpecVoltVarModel;
class SunSpecFreqWattParamModel;
class SunSpecReactiveCurrentModel;
class SunSpecLvrtModel;
class SunSpecHvrtModel;
class SunSpecWattPfModel;
class SunSpecVoltWattModel;
class SunSpecScheduleModel;
class SunSpecFreqWattModel;
class SunSpecLfrtModel;
class SunSpecHfrtModel;
class SunSpecLvrtcModel;
class SunSpecHvrtcModel;
class SunSpecLvrtxModel;
class SunSpecHvrtxModel;
class SunSpecLfrtcModel;
class SunSpecHfrtcModel;
class SunSpecLfrtxModel;
class SunSpecHfrtxModel;
class SunSpecExtSettingsModel;
class SunSpecMpptModel;
class SunSpecMeterSinglePhaseModel;
class SunSpecMeterSplitSinglePhaseAbnModel;
class SunSpecMeterThreePhaseModel;
class SunSpecMeterThreePhaseDeltaConnectModel;
class SunSpecMeterSinglePhaseFloatModel;
class SunSpecMeterSplitSinglePhaseFloatModel;
class SunSpecMeterThreePhaseWyeConnectModel;
class SunSpecDeltaConnectThreePhaseAbcMeterModel;
class SunSpecSecureAcMeterSelectedReadingsModel;
class SunSpecIrradianceModel;
class SunSpecLocationModel;
class SunSpecRefPointModel;
class SunSpecBaseMetModel;
class SunSpecMiniMetModel;
class SunSpecStringCombinerCurrentModel;
class SunSpecStringCombinerAdvancedModel;
class SunSpecSolarModuleModel;
class SunSpecTrackerControllerModel;
class SunSpecBatteryModel;
class SunSpecLithiumIonBankModel;
class SunSpecLithiumIonStringModel;
class SunSpecLithiumIonModuleModel;
class SunSpecFlowBatteryModel;
class SunSpecFlowBatteryStringModel;
class SunSpecFlowBatteryModuleModel;
class SunSpecFlowBatteryStackModel;

class SunSpecModelFactory : public QObject
{
    Q_OBJECT
public:
    enum ModelId {
        ModelIdCommon = 1,
        ModelIdAggregator = 2,
        ModelIdInverterSinglePhase = 101,
        ModelIdInverterSplitPhase = 102,
        ModelIdInverterThreePhase = 103,
        ModelIdInverterSinglePhaseFloat = 111,
        ModelIdInverterSplitPhaseFloat = 112,
        ModelIdInverterThreePhaseFloat = 113,
        ModelIdNameplate = 120,
        ModelIdSettings = 121,
        ModelIdStatus = 122,
        ModelIdControls = 123,
        ModelIdStorage = 124,
        ModelIdEnergyStorageBaseDeprecated = 801,
        ModelIdPricing = 125,
        ModelIdVoltVar = 126,
        ModelIdFreqWattParam = 127,
        ModelIdReactiveCurrent = 128,
        ModelIdLvrt = 129,
        ModelIdHvrt = 130,
        ModelIdWattPf = 131,
        ModelIdVoltWatt = 132,
        ModelIdSchedule = 133,
        ModelIdFreqWatt = 134,
        ModelIdLfrt = 135,
        ModelIdHfrt = 136,
        ModelIdLvrtc = 137,
        ModelIdHvrtc = 138,
        ModelIdLvrtx = 139,
        ModelIdHvrtx = 140,
        ModelIdLfrtc = 141,
        ModelIdHfrtc = 142,
        ModelIdLfrtx = 143,
        ModelIdHfrtx = 144,
        ModelIdExtSettings = 145,
        ModelIdMppt = 160,
        ModelIdMeterSinglePhase = 201,
        ModelIdMeterSplitSinglePhaseAbn = 202,
        ModelIdMeterThreePhase = 203,
        ModelIdMeterThreePhaseDeltaConnect = 204,
        ModelIdMeterSinglePhaseFloat = 211,
        ModelIdMeterSplitSinglePhaseFloat = 212,
        ModelIdMeterThreePhaseWyeConnect = 213,
        ModelIdDeltaConnectThreePhaseAbcMeter = 214,
        ModelIdSecureAcMeterSelectedReadings = 220,
        ModelIdIrradiance = 302,
        ModelIdLocation = 305,
        ModelIdRefPoint = 306,
        ModelIdBaseMet = 307,
        ModelIdMiniMet = 308,
        ModelIdStringCombinerCurrent = 403,
        ModelIdStringCombinerAdvanced = 404,
        ModelIdSolarModule = 502,
        ModelIdTrackerController = 601,
        ModelIdBattery = 802,
        ModelIdLithiumIonBank = 803,
        ModelIdLithiumIonString = 804,
        ModelIdLithiumIonModule = 805,
        ModelIdFlowBattery = 806,
        ModelIdFlowBatteryString = 807,
        ModelIdFlowBatteryModule = 808,
        ModelIdFlowBatteryStack = 809
    };
    Q_ENUM(ModelId)

    explicit SunSpecModelFactory(QObject *parent = nullptr);
    ~SunSpecModelFactory() = default;

    SunSpecModel *createModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder);

private:


};

#endif // SUNSPECMODELFACTORY_H
