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

#include "sunspecschedulemodel.h"

SunSpecScheduleModel::SunSpecScheduleModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 133;
}

SunSpecScheduleModel::~SunSpecScheduleModel()
{

}

QString SunSpecScheduleModel::name() const
{
    return "schedule";
}

QString SunSpecScheduleModel::description() const
{
    return "Basic Scheduling ";
}

QString SunSpecScheduleModel::label() const
{
    return "Basic Scheduling";
}

void SunSpecScheduleModel::readModelHeader()
{

}

void SunSpecScheduleModel::readBlockData()
{

}

void SunSpecScheduleModel::initDataPoints()
{
    SunSpecDataPoint modelIdDataPoint;
    modelIdDataPoint.setName("ID");
    modelIdDataPoint.setLabel("Model ID");
    modelIdDataPoint.setDescription("Model identifier");
    modelIdDataPoint.setMandatory(true);
    modelIdDataPoint.setSize(1);
    modelIdDataPoint.setAddressOffset(0);
    modelIdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

    SunSpecDataPoint actschdDataPoint;
    actschdDataPoint.setName("ActSchd");
    actschdDataPoint.setLabel("ActSchd");
    actschdDataPoint.setDescription("Bitfield of active schedules");
    actschdDataPoint.setMandatory(true);
    actschdDataPoint.setSize(2);
    actschdDataPoint.setAddressOffset(2);
    actschdDataPoint.setBlockOffset(0);
    actschdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    actschdDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(actschdDataPoint.name(), actschdDataPoint);

    SunSpecDataPoint modenaDataPoint;
    modenaDataPoint.setName("ModEna");
    modenaDataPoint.setLabel("ModEna");
    modenaDataPoint.setDescription("Is basic scheduling active.");
    modenaDataPoint.setMandatory(true);
    modenaDataPoint.setSize(1);
    modenaDataPoint.setAddressOffset(4);
    modenaDataPoint.setBlockOffset(2);
    modenaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modenaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modenaDataPoint.name(), modenaDataPoint);

    SunSpecDataPoint nschdDataPoint;
    nschdDataPoint.setName("NSchd");
    nschdDataPoint.setLabel("NSchd");
    nschdDataPoint.setDescription("Number of schedules supported (recommend min. 4, max 32)");
    nschdDataPoint.setMandatory(true);
    nschdDataPoint.setSize(1);
    nschdDataPoint.setAddressOffset(5);
    nschdDataPoint.setBlockOffset(3);
    nschdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nschdDataPoint.name(), nschdDataPoint);

    SunSpecDataPoint nptsDataPoint;
    nptsDataPoint.setName("NPts");
    nptsDataPoint.setLabel("NPts");
    nptsDataPoint.setDescription("Number of schedule entries supported (maximum of 10).");
    nptsDataPoint.setMandatory(true);
    nptsDataPoint.setSize(1);
    nptsDataPoint.setAddressOffset(6);
    nptsDataPoint.setBlockOffset(4);
    nptsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nptsDataPoint.name(), nptsDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setLabel("Pad");
    padDataPoint.setDescription("Pad register.");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(7);
    padDataPoint.setBlockOffset(5);
    padDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

