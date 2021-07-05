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
    explicit SunSpecModelFactory(QObject *parent = nullptr);
    ~SunSpecModelFactory() = default;

    SunSpecModel *createModel(SunSpecConnection *connection, quint16 modelId, quint16 modelLength);

private:


};

#endif // SUNSPECMODELFACTORY_H
