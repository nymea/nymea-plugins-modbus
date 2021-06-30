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

#include "sunspecaggregatormodel.h"

SunSpecAggregatorModel::SunSpecAggregatorModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 2;
}

SunSpecAggregatorModel::~SunSpecAggregatorModel()
{

}

QString SunSpecAggregatorModel::name() const
{
    return "aggregator";
}

QString SunSpecAggregatorModel::description() const
{
    return "Aggregates a collection of models for a given model id";
}

QString SunSpecAggregatorModel::label() const
{
    return "Basic Aggregator";
}

quint16 SunSpecAggregatorModel::modelId() const
{
    return m_modelId;
}
quint16 SunSpecAggregatorModel::modelLength() const
{
    return m_modelLength;
}
quint16 SunSpecAggregatorModel::aid() const
{
    return m_aid;
}
quint16 SunSpecAggregatorModel::n() const
{
    return m_n;
}
quint16 SunSpecAggregatorModel::un() const
{
    return m_un;
}
SunSpecAggregatorModel::St SunSpecAggregatorModel::status() const
{
    return m_status;
}
quint16 SunSpecAggregatorModel::vendorStatus() const
{
    return m_vendorStatus;
}
SunSpecAggregatorModel::EvtFlags SunSpecAggregatorModel::eventCode() const
{
    return m_eventCode;
}
quint32 SunSpecAggregatorModel::vendorEventCode() const
{
    return m_vendorEventCode;
}
SunSpecAggregatorModel::Ctl SunSpecAggregatorModel::control() const
{
    return m_control;
}
quint32 SunSpecAggregatorModel::vendorControl() const
{
    return m_vendorControl;
}
quint32 SunSpecAggregatorModel::controlValue() const
{
    return m_controlValue;
}
void SunSpecAggregatorModel::initDataPoints()
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

    SunSpecDataPoint aidDataPoint;
    aidDataPoint.setName("AID");
    aidDataPoint.setLabel("AID");
    aidDataPoint.setDescription("Aggregated model id");
    aidDataPoint.setMandatory(true);
    aidDataPoint.setSize(1);
    aidDataPoint.setAddressOffset(2);
    aidDataPoint.setBlockOffset(0);
    aidDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(aidDataPoint.name(), aidDataPoint);

    SunSpecDataPoint nDataPoint;
    nDataPoint.setName("N");
    nDataPoint.setLabel("N");
    nDataPoint.setDescription("Number of aggregated models");
    nDataPoint.setMandatory(true);
    nDataPoint.setSize(1);
    nDataPoint.setAddressOffset(3);
    nDataPoint.setBlockOffset(1);
    nDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(nDataPoint.name(), nDataPoint);

    SunSpecDataPoint unDataPoint;
    unDataPoint.setName("UN");
    unDataPoint.setLabel("UN");
    unDataPoint.setDescription("Update Number.  Incrementing number each time the mapping is changed.  If the number is not changed from the last reading the direct access to a specific offset will result in reading the same logical model as before.  Otherwise the entire model must be read to refresh the changes");
    unDataPoint.setMandatory(true);
    unDataPoint.setSize(1);
    unDataPoint.setAddressOffset(4);
    unDataPoint.setBlockOffset(2);
    unDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(unDataPoint.name(), unDataPoint);

    SunSpecDataPoint statusDataPoint;
    statusDataPoint.setName("St");
    statusDataPoint.setLabel("Status");
    statusDataPoint.setDescription("Enumerated status code");
    statusDataPoint.setMandatory(true);
    statusDataPoint.setSize(1);
    statusDataPoint.setAddressOffset(5);
    statusDataPoint.setBlockOffset(3);
    statusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(statusDataPoint.name(), statusDataPoint);

    SunSpecDataPoint vendorStatusDataPoint;
    vendorStatusDataPoint.setName("StVnd");
    vendorStatusDataPoint.setLabel("Vendor Status");
    vendorStatusDataPoint.setDescription("Vendor specific status code");
    vendorStatusDataPoint.setSize(1);
    vendorStatusDataPoint.setAddressOffset(6);
    vendorStatusDataPoint.setBlockOffset(4);
    vendorStatusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(vendorStatusDataPoint.name(), vendorStatusDataPoint);

    SunSpecDataPoint eventCodeDataPoint;
    eventCodeDataPoint.setName("Evt");
    eventCodeDataPoint.setLabel("Event Code");
    eventCodeDataPoint.setDescription("Bitmask event code");
    eventCodeDataPoint.setMandatory(true);
    eventCodeDataPoint.setSize(2);
    eventCodeDataPoint.setAddressOffset(7);
    eventCodeDataPoint.setBlockOffset(5);
    eventCodeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(eventCodeDataPoint.name(), eventCodeDataPoint);

    SunSpecDataPoint vendorEventCodeDataPoint;
    vendorEventCodeDataPoint.setName("EvtVnd");
    vendorEventCodeDataPoint.setLabel("Vendor Event Code");
    vendorEventCodeDataPoint.setDescription("Vendor specific event code");
    vendorEventCodeDataPoint.setSize(2);
    vendorEventCodeDataPoint.setAddressOffset(9);
    vendorEventCodeDataPoint.setBlockOffset(7);
    vendorEventCodeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventCodeDataPoint.name(), vendorEventCodeDataPoint);

    SunSpecDataPoint controlDataPoint;
    controlDataPoint.setName("Ctl");
    controlDataPoint.setLabel("Control");
    controlDataPoint.setDescription("Control register for all aggregated devices");
    controlDataPoint.setSize(1);
    controlDataPoint.setAddressOffset(11);
    controlDataPoint.setBlockOffset(9);
    controlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(controlDataPoint.name(), controlDataPoint);

    SunSpecDataPoint vendorControlDataPoint;
    vendorControlDataPoint.setName("CtlVnd");
    vendorControlDataPoint.setLabel("Vendor Control");
    vendorControlDataPoint.setDescription("Vendor control register for all aggregated devices");
    vendorControlDataPoint.setSize(2);
    vendorControlDataPoint.setAddressOffset(12);
    vendorControlDataPoint.setBlockOffset(10);
    vendorControlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum32"));
    m_dataPoints.insert(vendorControlDataPoint.name(), vendorControlDataPoint);

    SunSpecDataPoint controlValueDataPoint;
    controlValueDataPoint.setName("CtlVl");
    controlValueDataPoint.setLabel("Control Value");
    controlValueDataPoint.setDescription("Numerical value used as a parameter to the control");
    controlValueDataPoint.setSize(2);
    controlValueDataPoint.setAddressOffset(14);
    controlValueDataPoint.setBlockOffset(12);
    controlValueDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum32"));
    m_dataPoints.insert(controlValueDataPoint.name(), controlValueDataPoint);

}

