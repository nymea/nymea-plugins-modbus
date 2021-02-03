/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
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

SunSpecMeter::SunSpecMeter(SunSpec *sunspec, SunSpec::ModelId modelId, int modbusAddress) :
    QObject(sunspec),
    m_connection(sunspec),
    m_id(modelId),
    m_modelModbusStartRegister(modbusAddress)
{
    qCDebug(dcSunSpec()) << "SunSpecMeter: Setting up meter";
    connect(m_connection, &SunSpec::modelDataBlockReceived, this, &SunSpecMeter::onModelDataBlockReceived);
}

SunSpec::ModelId SunSpecMeter::modelId()
{

    return m_id;
}

void SunSpecMeter::init()
{
    qCDebug(dcSunSpec()) << "SunSpecMeter: Init";
    m_connection->readModelHeader(m_modelModbusStartRegister);
    connect(m_connection, &SunSpec::modelHeaderReceived, this, [this] (uint modbusAddress, SunSpec::ModelId modelId, uint length) {
        if (modelId == m_id) {
            qCDebug(dcSunSpec()) << "SunSpecMeter: Model Header received, modbus address:" << modbusAddress << "model Id:" << modelId << "length:" << length;
            m_modelLength = length;
            emit initFinished(true);
            m_initFinishedSuccess = true;
        }
    });
    QTimer::singleShot(10000, this,[this] {
        if (!m_initFinishedSuccess) {
            emit initFinished(false);
        }
    });
}

void SunSpecMeter::getMeterModelDataBlock()
{
    qCDebug(dcSunSpec()) << "SunSpecMeter: get meter model data block, modbus register" << m_modelModbusStartRegister << "length" << m_modelLength;
    m_connection->readModelDataBlock(m_modelModbusStartRegister, m_modelLength);
}

void SunSpecMeter::getMeterModelHeader()
{
    qCDebug(dcSunSpec()) << "SunSpecMeter: get meter model header, modbus register" << m_modelModbusStartRegister << "length" << m_modelLength;
    m_connection->readModelHeader(m_modelModbusStartRegister);
}

void SunSpecMeter::onModelDataBlockReceived(SunSpec::ModelId modelId, uint length, QVector<quint16> data)
{
    if (modelId != m_id) {
        return;
    }

    if (length < m_modelLength) {
        qCDebug(dcSunSpec()) << "SunSpecMeter: on model data block received, model length is too short" << length;
        return;
    }

    qCDebug(dcSunSpec()) << "SunSpecMeter: Received" << modelId;
    switch (modelId) {
    case SunSpec::ModelIdSinglePhaseMeter:
    case SunSpec::ModelIdSplitSinglePhaseMeter:
    case SunSpec::ModelIdDeltaConnectThreePhaseMeter:
    case SunSpec::ModelIdWyeConnectThreePhaseMeter: {

        MeterData meterData;
        quint16 currentScaleFactor = data[Model20XCurrentScaleFactor];
        meterData.totalAcCurrent = m_connection->convertValueWithSSF(data[Model20XTotalAcCurrent], currentScaleFactor);
        meterData.phaseACurrent  = m_connection->convertValueWithSSF(data[Model20XPhaseACurrent], currentScaleFactor);
        meterData.phaseBCurrent  = m_connection->convertValueWithSSF(data[Model20XPhaseBCurrent], currentScaleFactor);
        meterData.phaseCCurrent  = m_connection->convertValueWithSSF(data[Model20XPhaseCCurrent], currentScaleFactor);
        quint16 voltageScaleFactor = data[Model20XVoltageScaleFactor];
        meterData.voltageLN = m_connection->convertValueWithSSF(data[Model20XVoltageLN], voltageScaleFactor);
        meterData.phaseVoltageAN = m_connection->convertValueWithSSF(data[Model20XPhaseVoltageAN], voltageScaleFactor);
        meterData.phaseVoltageBN = m_connection->convertValueWithSSF(data[Model20XPhaseVoltageBN], voltageScaleFactor);
        meterData.phaseVoltageCN = m_connection->convertValueWithSSF(data[Model20XPhaseVoltageCN], voltageScaleFactor);
        meterData.voltageLL      = m_connection->convertValueWithSSF(data[Model20XVoltageLL], voltageScaleFactor);
        meterData.phaseVoltageAB = m_connection->convertValueWithSSF(data[Model20XPhaseVoltageAB], voltageScaleFactor);
        meterData.phaseVoltageBC = m_connection->convertValueWithSSF(data[Model20XPhaseVoltageBC], voltageScaleFactor);
        meterData.phaseVoltageCA = m_connection->convertValueWithSSF(data[Model20XPhaseVoltageCA], voltageScaleFactor);
        meterData.frequency      = m_connection->convertValueWithSSF(data[Model20XFrequency], data[Model20XFrequencyScaleFactor]);
        meterData.totalRealPower = m_connection->convertValueWithSSF(data[Model20XTotalRealPower], data[Model20XRealPowerScaleFactor]);
        quint16 energyScaleFactor = data[Model20XRealEnergyScaleFactor];
        meterData.totalRealEnergyExported = m_connection->convertValueWithSSF(data[Model20XTotalRealEnergyExported], energyScaleFactor);
        meterData.totalRealEnergyImported = m_connection->convertValueWithSSF(data[Model20XTotalRealEnergyImported], energyScaleFactor);;
        meterData.meterEventFlags = (static_cast<quint32>(data[Model20XMeterEventFlags]) << 16) | data[Model20XMeterEventFlags+1];
        emit meterDataReceived(meterData);

    } break;
    case SunSpec::ModelIdSinglePhaseMeterFloat:
    case SunSpec::ModelIdSplitSinglePhaseMeterFloat:
    case SunSpec::ModelIdDeltaConnectThreePhaseMeterFloat:
    case SunSpec::ModelIdWyeConnectThreePhaseMeterFloat: {

        MeterData meterData;
        meterData.totalAcCurrent = m_connection->convertFloatValues(data[Model21XTotalAcCurrent], data[Model21XTotalAcCurrent+1]);
        meterData.phaseACurrent  = m_connection->convertFloatValues(data[Model21XPhaseACurrent], data[Model21XPhaseACurrent+1]);
        meterData.phaseBCurrent  = m_connection->convertFloatValues(data[Model21XPhaseBCurrent], data[Model21XPhaseBCurrent+1]);
        meterData.phaseCCurrent  = m_connection->convertFloatValues(data[Model21XPhaseCCurrent], data[Model21XPhaseCCurrent+1]);
        meterData.voltageLN      = m_connection->convertFloatValues(data[Model21XVoltageLN], data[Model21XVoltageLN+1]);
        meterData.phaseVoltageAN = m_connection->convertFloatValues(data[Model21XPhaseVoltageAN], data[Model21XPhaseVoltageAN+1]);
        meterData.phaseVoltageBN = m_connection->convertFloatValues(data[Model21XPhaseVoltageBN], data[Model21XPhaseVoltageBN+1]);
        meterData.phaseVoltageCN = m_connection->convertFloatValues(data[Model21XPhaseVoltageCN], data[Model21XPhaseVoltageCN+1]);
        meterData.voltageLL      = m_connection->convertFloatValues(data[Model21XVoltageLL], data[Model21XVoltageLL+1]);
        meterData.phaseVoltageAB = m_connection->convertFloatValues(data[Model21XPhaseVoltageAB], data[Model21XPhaseVoltageAB+1]);
        meterData.phaseVoltageBC = m_connection->convertFloatValues(data[Model21XPhaseVoltageBC], data[Model21XPhaseVoltageBC+1]);
        meterData.phaseVoltageCA = m_connection->convertFloatValues(data[Model21XPhaseVoltageCA], data[Model21XPhaseVoltageCA+1]);
        meterData.frequency      = m_connection->convertFloatValues(data[Model21XFrequency], data[Model21XFrequency+1]);
        meterData.totalRealPower = m_connection->convertFloatValues(data[Model21XTotalRealPower], data[Model21XTotalRealPower+1]);
        meterData.totalRealEnergyExported = m_connection->convertFloatValues(data[Model21XTotalRealEnergyExported], data[Model21XTotalRealEnergyExported+1]);
        meterData.totalRealEnergyImported = m_connection->convertFloatValues(data[Model21XTotalRealEnergyImported], data[Model21XTotalRealEnergyImported+1]);
        meterData.meterEventFlags = ((static_cast<quint32>(data[Model21XMeterEventFlags]) << 16) | data[Model21XMeterEventFlags+1]);
        emit meterDataReceived(meterData);

    } break;
    default:
        break;
    }
}
