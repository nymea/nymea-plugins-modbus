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

#include "sunspecinverter.h"

#include <models/sunspecmodelfactory.h>
#include <models/sunspecinvertersinglephasemodel.h>
#include <models/sunspecinvertersinglephasefloatmodel.h>
#include <models/sunspecinvertersplitphasemodel.h>
#include <models/sunspecinvertersplitphasefloatmodel.h>
#include <models/sunspecinverterthreephasemodel.h>
#include <models/sunspecinverterthreephasefloatmodel.h>

SunSpecInverter::SunSpecInverter(Thing *thing, SunSpecModel *model, QObject *parent) :
    SunSpecThing(thing, model, parent)
{
    connect(m_model, &SunSpecModel::blockUpdated, this, &SunSpecInverter::onBlockDataUpdated);
}

void SunSpecInverter::readBlockData()
{
    m_model->readBlockData();
}

void SunSpecInverter::onBlockDataUpdated()
{
    switch (m_model->modelId()) {
    case SunSpecModelFactory::ModelIdInverterSinglePhase: {
        SunSpecInverterSinglePhaseModel *inverter = qobject_cast<SunSpecInverterSinglePhaseModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << inverter;
        m_thing->setStateValue(sunspecSinglePhaseInverterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecSinglePhaseInverterTotalCurrentStateTypeId, inverter->amps());
        m_thing->setStateValue(sunspecSinglePhaseInverterCurrentPowerStateTypeId, inverter->watts());
        m_thing->setStateValue(sunspecSinglePhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        m_thing->setStateValue(sunspecSinglePhaseInverterFrequencyStateTypeId, inverter->hz());
        m_thing->setStateValue(sunspecSinglePhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());

        m_thing->setStateValue(sunspecSinglePhaseInverterPhaseVoltageStateTypeId, inverter->phaseVoltageAn());

        m_thing->setStateValue(sunspecSinglePhaseInverterOperatingStateStateTypeId, getStateString(inverter->operatingState()));
        m_thing->setStateValue(sunspecSinglePhaseInverterErrorStateTypeId, getErrorString(inverter->event1()));

        break;
    }
    case SunSpecModelFactory::ModelIdInverterSinglePhaseFloat: {
        SunSpecInverterSinglePhaseFloatModel *inverter = qobject_cast<SunSpecInverterSinglePhaseFloatModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << inverter;
        m_thing->setStateValue(sunspecSinglePhaseInverterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecSinglePhaseInverterTotalCurrentStateTypeId, inverter->amps());
        m_thing->setStateValue(sunspecSinglePhaseInverterCurrentPowerStateTypeId, inverter->watts());
        m_thing->setStateValue(sunspecSinglePhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        m_thing->setStateValue(sunspecSinglePhaseInverterFrequencyStateTypeId, inverter->hz());
        m_thing->setStateValue(sunspecSinglePhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());

        m_thing->setStateValue(sunspecSinglePhaseInverterPhaseVoltageStateTypeId, inverter->phaseVoltageAn());

        m_thing->setStateValue(sunspecSinglePhaseInverterOperatingStateStateTypeId, getStateString(inverter->operatingState()));

        m_thing->setStateValue(sunspecSinglePhaseInverterErrorStateTypeId, getErrorString(inverter->event1()));

        break;
    }
    case SunSpecModelFactory::ModelIdInverterSplitPhase: {
        SunSpecInverterSplitPhaseModel *inverter = qobject_cast<SunSpecInverterSplitPhaseModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << inverter;
        m_thing->setStateValue(sunspecSplitPhaseInverterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecSplitPhaseInverterTotalCurrentStateTypeId, inverter->amps());
        m_thing->setStateValue(sunspecSplitPhaseInverterCurrentPowerStateTypeId, inverter->watts());
        m_thing->setStateValue(sunspecSplitPhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        m_thing->setStateValue(sunspecSplitPhaseInverterFrequencyStateTypeId, inverter->hz());
        m_thing->setStateValue(sunspecSplitPhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());

        m_thing->setStateValue(sunspecSplitPhaseInverterPhaseANVoltageStateTypeId, inverter->phaseVoltageAn());
        m_thing->setStateValue(sunspecSplitPhaseInverterPhaseBNVoltageStateTypeId, inverter->phaseVoltageBn());

        m_thing->setStateValue(sunspecSplitPhaseInverterPhaseACurrentStateTypeId, inverter->ampsPhaseA());
        m_thing->setStateValue(sunspecSplitPhaseInverterPhaseBCurrentStateTypeId, inverter->ampsPhaseB());

        m_thing->setStateValue(sunspecSplitPhaseInverterOperatingStateStateTypeId, getStateString(inverter->operatingState()));
        m_thing->setStateValue(sunspecSplitPhaseInverterErrorStateTypeId, getErrorString(inverter->event1()));

        break;
    }
    case SunSpecModelFactory::ModelIdInverterSplitPhaseFloat: {
        SunSpecInverterSplitPhaseFloatModel *inverter = qobject_cast<SunSpecInverterSplitPhaseFloatModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << inverter;
        m_thing->setStateValue(sunspecSplitPhaseInverterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecSplitPhaseInverterTotalCurrentStateTypeId, inverter->amps());
        m_thing->setStateValue(sunspecSplitPhaseInverterCurrentPowerStateTypeId, inverter->watts());
        m_thing->setStateValue(sunspecSplitPhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        m_thing->setStateValue(sunspecSplitPhaseInverterFrequencyStateTypeId, inverter->hz());
        m_thing->setStateValue(sunspecSplitPhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());

        m_thing->setStateValue(sunspecSplitPhaseInverterPhaseANVoltageStateTypeId, inverter->phaseVoltageAn());
        m_thing->setStateValue(sunspecSplitPhaseInverterPhaseBNVoltageStateTypeId, inverter->phaseVoltageBn());

        m_thing->setStateValue(sunspecSplitPhaseInverterPhaseACurrentStateTypeId, inverter->ampsPhaseA());
        m_thing->setStateValue(sunspecSplitPhaseInverterPhaseBCurrentStateTypeId, inverter->ampsPhaseB());

        m_thing->setStateValue(sunspecSplitPhaseInverterOperatingStateStateTypeId, getStateString(inverter->operatingState()));
        m_thing->setStateValue(sunspecSplitPhaseInverterErrorStateTypeId, getErrorString(inverter->event1()));
        break;
    }
    case SunSpecModelFactory::ModelIdInverterThreePhase: {
        SunSpecInverterThreePhaseModel *inverter = qobject_cast<SunSpecInverterThreePhaseModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << inverter;
        m_thing->setStateValue(sunspecThreePhaseInverterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecThreePhaseInverterTotalCurrentStateTypeId, inverter->amps());
        m_thing->setStateValue(sunspecThreePhaseInverterCurrentPowerStateTypeId, inverter->watts());
        m_thing->setStateValue(sunspecThreePhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        m_thing->setStateValue(sunspecThreePhaseInverterFrequencyStateTypeId, inverter->hz());
        m_thing->setStateValue(sunspecThreePhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());

        m_thing->setStateValue(sunspecThreePhaseInverterPhaseANVoltageStateTypeId, inverter->phaseVoltageAn());
        m_thing->setStateValue(sunspecThreePhaseInverterPhaseBNVoltageStateTypeId, inverter->phaseVoltageBn());
        m_thing->setStateValue(sunspecThreePhaseInverterPhaseCNVoltageStateTypeId, inverter->phaseVoltageCn());

        m_thing->setStateValue(sunspecThreePhaseInverterPhaseACurrentStateTypeId, inverter->ampsPhaseA());
        m_thing->setStateValue(sunspecThreePhaseInverterPhaseBCurrentStateTypeId, inverter->ampsPhaseB());
        m_thing->setStateValue(sunspecThreePhaseInverterPhaseCCurrentStateTypeId, inverter->ampsPhaseC());

        m_thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, getStateString(inverter->operatingState()));
        m_thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, getErrorString(inverter->event1()));
        break;
    }
    case SunSpecModelFactory::ModelIdInverterThreePhaseFloat: {
        SunSpecInverterThreePhaseFloatModel *inverter = qobject_cast<SunSpecInverterThreePhaseFloatModel *>(m_model);
        qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << inverter;
        m_thing->setStateValue(sunspecThreePhaseInverterConnectedStateTypeId, true);
        m_thing->setStateValue(sunspecThreePhaseInverterTotalCurrentStateTypeId, inverter->amps());
        m_thing->setStateValue(sunspecThreePhaseInverterCurrentPowerStateTypeId, inverter->watts());
        m_thing->setStateValue(sunspecThreePhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        m_thing->setStateValue(sunspecThreePhaseInverterFrequencyStateTypeId, inverter->hz());
        m_thing->setStateValue(sunspecThreePhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());

        m_thing->setStateValue(sunspecThreePhaseInverterPhaseANVoltageStateTypeId, inverter->phaseVoltageAn());
        m_thing->setStateValue(sunspecThreePhaseInverterPhaseBNVoltageStateTypeId, inverter->phaseVoltageBn());
        m_thing->setStateValue(sunspecThreePhaseInverterPhaseCNVoltageStateTypeId, inverter->phaseVoltageCn());

        m_thing->setStateValue(sunspecThreePhaseInverterPhaseACurrentStateTypeId, inverter->ampsPhaseA());
        m_thing->setStateValue(sunspecThreePhaseInverterPhaseBCurrentStateTypeId, inverter->ampsPhaseB());
        m_thing->setStateValue(sunspecThreePhaseInverterPhaseCCurrentStateTypeId, inverter->ampsPhaseC());

        m_thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, getStateString(inverter->operatingState()));
        m_thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, getErrorString(inverter->event1()));
        break;
    }
    default:
        qCWarning(dcSunSpec()) << "Received block data from unhandled model" << m_model;
        break;
    }
}

QString SunSpecInverter::getStateString(quint16 state)
{
    // Note: this works because the models have all the same value
    QString stateString;
    switch (state) {
    case SunSpecInverterSinglePhaseModel::StOff:
        stateString = "Off";
        break;
    case SunSpecInverterSinglePhaseModel::StSleeping:
        stateString = "Sleeping";
        break;
    case SunSpecInverterSinglePhaseModel::StStarting:
        stateString = "Starting";
        break;
    case SunSpecInverterSinglePhaseModel::StMppt:
        stateString = "MPPT";
        break;
    case SunSpecInverterSinglePhaseModel::StThrottled:
        stateString = "Throttled";
        break;
    case SunSpecInverterSinglePhaseModel::StShuttingDown:
        stateString = "Shutting down";
        break;
    case SunSpecInverterSinglePhaseModel::StFault:
        stateString = "Fault";
        break;
    case SunSpecInverterSinglePhaseModel::StStandby:
        stateString = "Standby";
        break;
    }
    return stateString;
}

QString SunSpecInverter::getErrorString(quint32 flag)
{
    // Note: this works because the models have all the same value
    QStringList errorStrings;
    SunSpecInverterSinglePhaseModel::Evt1Flags event1Flag = static_cast<SunSpecInverterSinglePhaseModel::Evt1Flags>(flag);
    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1GroundFault)) {
        errorStrings.append(QT_TR_NOOP("Ground fault"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1DcOverVolt)) {
        errorStrings.append(QT_TR_NOOP("DC over voltage"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1AcDisconnect)) {
        errorStrings.append(QT_TR_NOOP("AC disconnect"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1DcDisconnect)) {
        errorStrings.append(QT_TR_NOOP("DC disconnect"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1GridDisconnect)) {
        errorStrings.append(QT_TR_NOOP("Grid disconnect"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1CabinetOpen)) {
        errorStrings.append(QT_TR_NOOP("Cabinet open"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1ManualShutdown)) {
        errorStrings.append(QT_TR_NOOP("Manual shutdown"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1OverTemp)) {
        errorStrings.append(QT_TR_NOOP("Over temperature"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1OverFrequency)) {
        errorStrings.append(QT_TR_NOOP("Over frequency"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1UnderFrequency)) {
        errorStrings.append(QT_TR_NOOP("Under frequency"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1AcOverVolt)) {
        errorStrings.append(QT_TR_NOOP("AC over voltage"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1AcUnderVolt)) {
        errorStrings.append(QT_TR_NOOP("AC under voltage"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1BlownStringFuse)) {
        errorStrings.append(QT_TR_NOOP("Blown string fuse"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1UnderTemp)) {
        errorStrings.append(QT_TR_NOOP("Under temperature"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1MemoryLoss)) {
        errorStrings.append(QT_TR_NOOP("Memory loss"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1HwTestFailure)) {
        errorStrings.append(QT_TR_NOOP("Hardware test failure"));
    }

    if (errorStrings.isEmpty()) {
        return QT_TR_NOOP("No error");
    } else {
        return errorStrings.join(", ");
    }

}
