/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#include "sunspecmeter.h"
#include "extern-plugininfo.h"

#include <models/sunspecmodelfactory.h>
#include <models/sunspecmeterthreephasemodel.h>
#include <models/sunspecmetersinglephasemodel.h>
#include <models/sunspecmetersinglephasefloatmodel.h>
#include <models/sunspecmetersplitsinglephaseabnmodel.h>
#include <models/sunspecmetersplitsinglephasefloatmodel.h>
#include <models/sunspecmeterthreephasewyeconnectmodel.h>
#include <models/sunspecmeterthreephasedeltaconnectmodel.h>
#include <models/sunspecdeltaconnectthreephaseabcmetermodel.h>


SunSpecMeter::SunSpecMeter(Thing *thing, SunSpecModel *model, QObject *parent) :
    SunSpecThing(thing, model, parent)
{
    connect(m_model, &SunSpecModel::blockUpdated, this, &SunSpecMeter::onBlockDataUpdated);
}

void SunSpecMeter::readBlockData()
{
    m_model->readBlockData();
}

void SunSpecMeter::onBlockDataUpdated()
{
    switch (m_model->modelId()) {
    case SunSpecModelFactory::ModelIdMeterSinglePhase: {
        SunSpecMeterSinglePhaseModel *meter = qobject_cast<SunSpecMeterSinglePhaseModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << meter;
        m_thing->setStateValue(sunspecSinglePhaseMeterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecSinglePhaseMeterPhaseACurrentStateTypeId, meter->ampsPhaseA());
        m_thing->setStateValue(sunspecSinglePhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        m_thing->setStateValue(sunspecSinglePhaseMeterPhaseANVoltageStateTypeId, meter->phaseVoltageAn());
        m_thing->setStateValue(sunspecSinglePhaseMeterFrequencyStateTypeId, meter->hz());
        m_thing->setStateValue(sunspecSinglePhaseMeterCurrentPowerStateTypeId, meter->watts());
        m_thing->setStateValue(sunspecSinglePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        m_thing->setStateValue(sunspecSinglePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterSinglePhaseFloat: {
        SunSpecMeterSinglePhaseFloatModel *meter = qobject_cast<SunSpecMeterSinglePhaseFloatModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << meter;
        m_thing->setStateValue(sunspecSinglePhaseMeterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecSinglePhaseMeterPhaseACurrentStateTypeId, meter->ampsPhaseA());
        m_thing->setStateValue(sunspecSinglePhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        m_thing->setStateValue(sunspecSinglePhaseMeterPhaseANVoltageStateTypeId, meter->phaseVoltageAn());
        m_thing->setStateValue(sunspecSinglePhaseMeterFrequencyStateTypeId, meter->hz());
        m_thing->setStateValue(sunspecSinglePhaseMeterCurrentPowerStateTypeId, meter->watts());
        m_thing->setStateValue(sunspecSinglePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        m_thing->setStateValue(sunspecSinglePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterSplitSinglePhaseAbn: {
        SunSpecMeterSplitSinglePhaseAbnModel *meter = qobject_cast<SunSpecMeterSplitSinglePhaseAbnModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << meter;
        m_thing->setStateValue(sunspecSplitPhaseMeterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecSplitPhaseMeterTotalCurrentStateTypeId, meter->amps());
        m_thing->setStateValue(sunspecSplitPhaseMeterPhaseACurrentStateTypeId, meter->ampsPhaseA());
        m_thing->setStateValue(sunspecSplitPhaseMeterPhaseBCurrentStateTypeId, meter->ampsPhaseB());
        m_thing->setStateValue(sunspecSplitPhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        m_thing->setStateValue(sunspecSplitPhaseMeterPhaseANVoltageStateTypeId, meter->phaseVoltageAn());
        m_thing->setStateValue(sunspecSplitPhaseMeterPhaseBNVoltageStateTypeId, meter->phaseVoltageBn());
        m_thing->setStateValue(sunspecSplitPhaseMeterFrequencyStateTypeId, meter->hz());
        m_thing->setStateValue(sunspecSplitPhaseMeterCurrentPowerStateTypeId, meter->watts());
        m_thing->setStateValue(sunspecSplitPhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        m_thing->setStateValue(sunspecSplitPhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterSplitSinglePhaseFloat: {
        SunSpecMeterSplitSinglePhaseFloatModel *meter = qobject_cast<SunSpecMeterSplitSinglePhaseFloatModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << meter;
        m_thing->setStateValue(sunspecSplitPhaseMeterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecSplitPhaseMeterTotalCurrentStateTypeId, meter->amps());
        m_thing->setStateValue(sunspecSplitPhaseMeterPhaseACurrentStateTypeId, meter->ampsPhaseA());
        m_thing->setStateValue(sunspecSplitPhaseMeterPhaseBCurrentStateTypeId, meter->ampsPhaseB());
        m_thing->setStateValue(sunspecSplitPhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        m_thing->setStateValue(sunspecSplitPhaseMeterPhaseANVoltageStateTypeId, meter->phaseVoltageAn());
        m_thing->setStateValue(sunspecSplitPhaseMeterPhaseBNVoltageStateTypeId, meter->phaseVoltageBn());
        m_thing->setStateValue(sunspecSplitPhaseMeterFrequencyStateTypeId, meter->hz());
        m_thing->setStateValue(sunspecSplitPhaseMeterCurrentPowerStateTypeId, meter->watts());
        m_thing->setStateValue(sunspecSplitPhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        m_thing->setStateValue(sunspecSplitPhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterThreePhase: {
        SunSpecMeterThreePhaseModel *meter = qobject_cast<SunSpecMeterThreePhaseModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << meter;
        m_thing->setStateValue(sunspecThreePhaseMeterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecThreePhaseMeterTotalCurrentStateTypeId, meter->amps());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseACurrentStateTypeId, meter->ampsPhaseA());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseBCurrentStateTypeId, meter->ampsPhaseB());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseCCurrentStateTypeId, meter->ampsPhaseC());
        m_thing->setStateValue(sunspecThreePhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseANVoltageStateTypeId, meter->phaseVoltageAn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseBNVoltageStateTypeId, meter->phaseVoltageBn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseCNVoltageStateTypeId, meter->phaseVoltageCn());
        m_thing->setStateValue(sunspecThreePhaseMeterFrequencyStateTypeId, meter->hz());
        m_thing->setStateValue(sunspecThreePhaseMeterCurrentPowerStateTypeId, meter->watts());
        m_thing->setStateValue(sunspecThreePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        m_thing->setStateValue(sunspecThreePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        break;
    }
    case SunSpecModelFactory::ModelIdDeltaConnectThreePhaseAbcMeter: {
        SunSpecDeltaConnectThreePhaseAbcMeterModel *meter = qobject_cast<SunSpecDeltaConnectThreePhaseAbcMeterModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << meter;
        m_thing->setStateValue(sunspecThreePhaseMeterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecThreePhaseMeterTotalCurrentStateTypeId, meter->amps());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseACurrentStateTypeId, meter->ampsPhaseA());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseBCurrentStateTypeId, meter->ampsPhaseB());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseCCurrentStateTypeId, meter->ampsPhaseC());
        m_thing->setStateValue(sunspecThreePhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseANVoltageStateTypeId, meter->phaseVoltageAn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseBNVoltageStateTypeId, meter->phaseVoltageBn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseCNVoltageStateTypeId, meter->phaseVoltageCn());
        m_thing->setStateValue(sunspecThreePhaseMeterFrequencyStateTypeId, meter->hz());
        m_thing->setStateValue(sunspecThreePhaseMeterCurrentPowerStateTypeId, meter->watts());
        m_thing->setStateValue(sunspecThreePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        m_thing->setStateValue(sunspecThreePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterThreePhaseWyeConnect: {
        SunSpecMeterThreePhaseWyeConnectModel *meter = qobject_cast<SunSpecMeterThreePhaseWyeConnectModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << meter;
        m_thing->setStateValue(sunspecThreePhaseMeterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecThreePhaseMeterTotalCurrentStateTypeId, meter->amps());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseACurrentStateTypeId, meter->ampsPhaseA());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseBCurrentStateTypeId, meter->ampsPhaseB());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseCCurrentStateTypeId, meter->ampsPhaseC());
        m_thing->setStateValue(sunspecThreePhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseANVoltageStateTypeId, meter->phaseVoltageAn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseBNVoltageStateTypeId, meter->phaseVoltageBn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseCNVoltageStateTypeId, meter->phaseVoltageCn());
        m_thing->setStateValue(sunspecThreePhaseMeterFrequencyStateTypeId, meter->hz());
        m_thing->setStateValue(sunspecThreePhaseMeterCurrentPowerStateTypeId, meter->watts());
        m_thing->setStateValue(sunspecThreePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        m_thing->setStateValue(sunspecThreePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterThreePhaseDeltaConnect: {
        SunSpecMeterThreePhaseDeltaConnectModel *meter = qobject_cast<SunSpecMeterThreePhaseDeltaConnectModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << meter;
        m_thing->setStateValue(sunspecThreePhaseMeterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecThreePhaseMeterTotalCurrentStateTypeId, meter->amps());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseACurrentStateTypeId, meter->ampsPhaseA());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseBCurrentStateTypeId, meter->ampsPhaseB());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseCCurrentStateTypeId, meter->ampsPhaseC());
        m_thing->setStateValue(sunspecThreePhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseANVoltageStateTypeId, meter->phaseVoltageAn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseBNVoltageStateTypeId, meter->phaseVoltageBn());
        m_thing->setStateValue(sunspecThreePhaseMeterPhaseCNVoltageStateTypeId, meter->phaseVoltageCn());
        m_thing->setStateValue(sunspecThreePhaseMeterFrequencyStateTypeId, meter->hz());
        m_thing->setStateValue(sunspecThreePhaseMeterCurrentPowerStateTypeId, meter->watts());
        m_thing->setStateValue(sunspecThreePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        m_thing->setStateValue(sunspecThreePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        break;
    }
    default:
        qCWarning(dcSunSpec()) << "Received block data from unhandled model" << m_model;
        break;
    }
}
