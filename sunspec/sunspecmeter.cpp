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
        qCDebug(dcSunSpec()) << "SunSpecMeter: Model Header received, modbus address:" << modbusAddress << "model Id:" << modelId << "length:" << length;
        m_modelLength = length;
        emit initFinished(true);
        m_initFinishedSuccess = true;
    });
    QTimer::singleShot(10000, this,[this] {
       if (!m_initFinishedSuccess) {
           emit initFinished(false);
       }
    });
}

void SunSpecMeter::getMeterModelDataBlock()
{
   m_connection->readModelDataBlock(m_modelModbusStartRegister, m_modelLength);
}

void SunSpecMeter::getMeterModelHeader()
{

}

void SunSpecMeter::onModelDataBlockReceived(SunSpec::ModelId modelId, uint length, QVector<quint16> data)
{
    if (modelId != m_id) {
        return;
    }
    Q_UNUSED(length)
    Q_UNUSED(data)
    switch (modelId) {

    case SunSpec::ModelIdSinglePhaseMeter:
    case SunSpec::ModelIdSinglePhaseMeterFloat: {

    } break;
    case SunSpec::ModelIdSplitSinglePhaseMeter:
    case SunSpec::ModelIdSplitSinglePhaseMeterFloat: {

    } break;
    case SunSpec::ModelIdWyeConnectThreePhaseMeterFloat:
    case SunSpec::ModelIdDeltaConnectThreePhaseMeterFloat: {

    } break;
    default:
        break;
    }
}
