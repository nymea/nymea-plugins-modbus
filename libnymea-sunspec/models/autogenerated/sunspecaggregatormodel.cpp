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

void SunSpecAggregatorModel::readModelHeader()
{

}

void SunSpecAggregatorModel::readBlockData()
{

}

void SunSpecAggregatorModel::initDataPoints()
{
    SunSpecDataPoint modelIdDataPoint;
    modelIdDataPoint.setName("ID");
    modelIdDataPoint.setLabel("Model ID");
    modelIdDataPoint.setDescription("Model identifier");
    modelIdDataPoint.setMandatory(true);
    modelIdDataPoint.setSize(1);
    modelIdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << modelIdDataPoint;

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << modelLengthDataPoint;

    SunSpecDataPoint aidDataPoint;
    aidDataPoint.setName("AID");
    aidDataPoint.setLabel("AID");
    aidDataPoint.setDescription("Aggregated model id");
    aidDataPoint.setMandatory(true);
    aidDataPoint.setSize(1);
    aidDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << aidDataPoint;

    SunSpecDataPoint nDataPoint;
    nDataPoint.setName("N");
    nDataPoint.setLabel("N");
    nDataPoint.setDescription("Number of aggregated models");
    nDataPoint.setMandatory(true);
    nDataPoint.setSize(1);
    nDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << nDataPoint;

    SunSpecDataPoint unDataPoint;
    unDataPoint.setName("UN");
    unDataPoint.setLabel("UN");
    unDataPoint.setDescription("Update Number.  Incrementing number each time the mapping is changed.  If the number is not changed from the last reading the direct access to a specific offset will result in reading the same logical model as before.  Otherwise the entire model must be read to refresh the changes");
    unDataPoint.setMandatory(true);
    unDataPoint.setSize(1);
    unDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << unDataPoint;

    SunSpecDataPoint statusDataPoint;
    statusDataPoint.setName("St");
    statusDataPoint.setLabel("Status");
    statusDataPoint.setDescription("Enumerated status code");
    statusDataPoint.setMandatory(true);
    statusDataPoint.setSize(1);
    statusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints << statusDataPoint;

    SunSpecDataPoint vendorStatusDataPoint;
    vendorStatusDataPoint.setName("StVnd");
    vendorStatusDataPoint.setLabel("Vendor Status");
    vendorStatusDataPoint.setDescription("Vendor specific status code");
    vendorStatusDataPoint.setSize(1);
    vendorStatusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints << vendorStatusDataPoint;

    SunSpecDataPoint eventCodeDataPoint;
    eventCodeDataPoint.setName("Evt");
    eventCodeDataPoint.setLabel("Event Code");
    eventCodeDataPoint.setDescription("Bitmask event code");
    eventCodeDataPoint.setMandatory(true);
    eventCodeDataPoint.setSize(2);
    eventCodeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << eventCodeDataPoint;

    SunSpecDataPoint vendorEventCodeDataPoint;
    vendorEventCodeDataPoint.setName("EvtVnd");
    vendorEventCodeDataPoint.setLabel("Vendor Event Code");
    vendorEventCodeDataPoint.setDescription("Vendor specific event code");
    vendorEventCodeDataPoint.setSize(2);
    vendorEventCodeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << vendorEventCodeDataPoint;

    SunSpecDataPoint controlDataPoint;
    controlDataPoint.setName("Ctl");
    controlDataPoint.setLabel("Control");
    controlDataPoint.setDescription("Control register for all aggregated devices");
    controlDataPoint.setSize(1);
    controlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints << controlDataPoint;

    SunSpecDataPoint vendorControlDataPoint;
    vendorControlDataPoint.setName("CtlVnd");
    vendorControlDataPoint.setLabel("Vendor Control");
    vendorControlDataPoint.setDescription("Vendor control register for all aggregated devices");
    vendorControlDataPoint.setSize(2);
    vendorControlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum32"));
    m_dataPoints << vendorControlDataPoint;

    SunSpecDataPoint controlValueDataPoint;
    controlValueDataPoint.setName("CtlVl");
    controlValueDataPoint.setLabel("Control Value");
    controlValueDataPoint.setDescription("Numerical value used as a parameter to the control");
    controlValueDataPoint.setSize(2);
    controlValueDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum32"));
    m_dataPoints << controlValueDataPoint;

}

