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

quint16 SunSpecScheduleModel::modelId() const
{
    return m_modelId;
}
quint16 SunSpecScheduleModel::modelLength() const
{
    return m_modelLength;
}
SunSpecScheduleModel::ActschdFlags SunSpecScheduleModel::actSchd() const
{
    return m_actSchd;
}
SunSpecScheduleModel::ModenaFlags SunSpecScheduleModel::modEna() const
{
    return m_modEna;
}
quint16 SunSpecScheduleModel::nSchd() const
{
    return m_nSchd;
}
quint16 SunSpecScheduleModel::nPts() const
{
    return m_nPts;
}
quint16 SunSpecScheduleModel::pad() const
{
    return m_pad;
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

    SunSpecDataPoint actSchdDataPoint;
    actSchdDataPoint.setName("ActSchd");
    actSchdDataPoint.setLabel("ActSchd");
    actSchdDataPoint.setDescription("Bitfield of active schedules");
    actSchdDataPoint.setMandatory(true);
    actSchdDataPoint.setSize(2);
    actSchdDataPoint.setAddressOffset(2);
    actSchdDataPoint.setBlockOffset(0);
    actSchdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    actSchdDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(actSchdDataPoint.name(), actSchdDataPoint);

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("Is basic scheduling active.");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(4);
    modEnaDataPoint.setBlockOffset(2);
    modEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint nSchdDataPoint;
    nSchdDataPoint.setName("NSchd");
    nSchdDataPoint.setLabel("NSchd");
    nSchdDataPoint.setDescription("Number of schedules supported (recommend min. 4, max 32)");
    nSchdDataPoint.setMandatory(true);
    nSchdDataPoint.setSize(1);
    nSchdDataPoint.setAddressOffset(5);
    nSchdDataPoint.setBlockOffset(3);
    nSchdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nSchdDataPoint.name(), nSchdDataPoint);

    SunSpecDataPoint nPtsDataPoint;
    nPtsDataPoint.setName("NPts");
    nPtsDataPoint.setLabel("NPts");
    nPtsDataPoint.setDescription("Number of schedule entries supported (maximum of 10).");
    nPtsDataPoint.setMandatory(true);
    nPtsDataPoint.setSize(1);
    nPtsDataPoint.setAddressOffset(6);
    nPtsDataPoint.setBlockOffset(4);
    nPtsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nPtsDataPoint.name(), nPtsDataPoint);

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

