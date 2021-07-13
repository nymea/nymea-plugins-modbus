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
#include "sunspecconnection.h"

SunSpecAggregatorModel::SunSpecAggregatorModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 2, 14, parent)
{
    //Q_ASSERT_X(length == 14,  "SunSpecAggregatorModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
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
void SunSpecAggregatorModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("AID").isValid())
        m_aid = m_dataPoints.value("AID").toUInt16();

    if (m_dataPoints.value("N").isValid())
        m_n = m_dataPoints.value("N").toUInt16();

    if (m_dataPoints.value("UN").isValid())
        m_un = m_dataPoints.value("UN").toUInt16();

    if (m_dataPoints.value("St").isValid())
        m_status = static_cast<St>(m_dataPoints.value("St").toUInt16());

    if (m_dataPoints.value("StVnd").isValid())
        m_vendorStatus = m_dataPoints.value("StVnd").toUInt16();

    if (m_dataPoints.value("Evt").isValid())
        m_eventCode = static_cast<EvtFlags>(m_dataPoints.value("Evt").toUInt32());

    if (m_dataPoints.value("EvtVnd").isValid())
        m_vendorEventCode = m_dataPoints.value("EvtVnd").toUInt32();

    if (m_dataPoints.value("Ctl").isValid())
        m_control = static_cast<Ctl>(m_dataPoints.value("Ctl").toUInt16());

    if (m_dataPoints.value("CtlVnd").isValid())
        m_vendorControl = m_dataPoints.value("CtlVnd").toUInt32();

    if (m_dataPoints.value("CtlVl").isValid())
        m_controlValue = m_dataPoints.value("CtlVl").toUInt32();


    qCDebug(dcSunSpecModelData()) << this;
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
    modelIdDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

    SunSpecDataPoint aidDataPoint;
    aidDataPoint.setName("AID");
    aidDataPoint.setLabel("AID");
    aidDataPoint.setDescription("Aggregated model id");
    aidDataPoint.setMandatory(true);
    aidDataPoint.setSize(1);
    aidDataPoint.setAddressOffset(2);
    aidDataPoint.setBlockOffset(0);
    aidDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(aidDataPoint.name(), aidDataPoint);

    SunSpecDataPoint nDataPoint;
    nDataPoint.setName("N");
    nDataPoint.setLabel("N");
    nDataPoint.setDescription("Number of aggregated models");
    nDataPoint.setMandatory(true);
    nDataPoint.setSize(1);
    nDataPoint.setAddressOffset(3);
    nDataPoint.setBlockOffset(1);
    nDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(nDataPoint.name(), nDataPoint);

    SunSpecDataPoint unDataPoint;
    unDataPoint.setName("UN");
    unDataPoint.setLabel("UN");
    unDataPoint.setDescription("Update Number.  Incrementing number each time the mapping is changed.  If the number is not changed from the last reading the direct access to a specific offset will result in reading the same logical model as before.  Otherwise the entire model must be read to refresh the changes");
    unDataPoint.setMandatory(true);
    unDataPoint.setSize(1);
    unDataPoint.setAddressOffset(4);
    unDataPoint.setBlockOffset(2);
    unDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(unDataPoint.name(), unDataPoint);

    SunSpecDataPoint statusDataPoint;
    statusDataPoint.setName("St");
    statusDataPoint.setLabel("Status");
    statusDataPoint.setDescription("Enumerated status code");
    statusDataPoint.setMandatory(true);
    statusDataPoint.setSize(1);
    statusDataPoint.setAddressOffset(5);
    statusDataPoint.setBlockOffset(3);
    statusDataPoint.setSunSpecDataType("enum16");
    m_dataPoints.insert(statusDataPoint.name(), statusDataPoint);

    SunSpecDataPoint vendorStatusDataPoint;
    vendorStatusDataPoint.setName("StVnd");
    vendorStatusDataPoint.setLabel("Vendor Status");
    vendorStatusDataPoint.setDescription("Vendor specific status code");
    vendorStatusDataPoint.setSize(1);
    vendorStatusDataPoint.setAddressOffset(6);
    vendorStatusDataPoint.setBlockOffset(4);
    vendorStatusDataPoint.setSunSpecDataType("enum16");
    m_dataPoints.insert(vendorStatusDataPoint.name(), vendorStatusDataPoint);

    SunSpecDataPoint eventCodeDataPoint;
    eventCodeDataPoint.setName("Evt");
    eventCodeDataPoint.setLabel("Event Code");
    eventCodeDataPoint.setDescription("Bitmask event code");
    eventCodeDataPoint.setMandatory(true);
    eventCodeDataPoint.setSize(2);
    eventCodeDataPoint.setAddressOffset(7);
    eventCodeDataPoint.setBlockOffset(5);
    eventCodeDataPoint.setSunSpecDataType("bitfield32");
    m_dataPoints.insert(eventCodeDataPoint.name(), eventCodeDataPoint);

    SunSpecDataPoint vendorEventCodeDataPoint;
    vendorEventCodeDataPoint.setName("EvtVnd");
    vendorEventCodeDataPoint.setLabel("Vendor Event Code");
    vendorEventCodeDataPoint.setDescription("Vendor specific event code");
    vendorEventCodeDataPoint.setSize(2);
    vendorEventCodeDataPoint.setAddressOffset(9);
    vendorEventCodeDataPoint.setBlockOffset(7);
    vendorEventCodeDataPoint.setSunSpecDataType("bitfield32");
    m_dataPoints.insert(vendorEventCodeDataPoint.name(), vendorEventCodeDataPoint);

    SunSpecDataPoint controlDataPoint;
    controlDataPoint.setName("Ctl");
    controlDataPoint.setLabel("Control");
    controlDataPoint.setDescription("Control register for all aggregated devices");
    controlDataPoint.setSize(1);
    controlDataPoint.setAddressOffset(11);
    controlDataPoint.setBlockOffset(9);
    controlDataPoint.setSunSpecDataType("enum16");
    m_dataPoints.insert(controlDataPoint.name(), controlDataPoint);

    SunSpecDataPoint vendorControlDataPoint;
    vendorControlDataPoint.setName("CtlVnd");
    vendorControlDataPoint.setLabel("Vendor Control");
    vendorControlDataPoint.setDescription("Vendor control register for all aggregated devices");
    vendorControlDataPoint.setSize(2);
    vendorControlDataPoint.setAddressOffset(12);
    vendorControlDataPoint.setBlockOffset(10);
    vendorControlDataPoint.setSunSpecDataType("enum32");
    m_dataPoints.insert(vendorControlDataPoint.name(), vendorControlDataPoint);

    SunSpecDataPoint controlValueDataPoint;
    controlValueDataPoint.setName("CtlVl");
    controlValueDataPoint.setLabel("Control Value");
    controlValueDataPoint.setDescription("Numerical value used as a parameter to the control");
    controlValueDataPoint.setSize(2);
    controlValueDataPoint.setAddressOffset(14);
    controlValueDataPoint.setBlockOffset(12);
    controlValueDataPoint.setSunSpecDataType("enum32");
    m_dataPoints.insert(controlValueDataPoint.name(), controlValueDataPoint);

}

QDebug operator<<(QDebug debug, SunSpecAggregatorModel *model)
{
    debug.nospace().noquote() << "SunSpecAggregatorModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    if (model->dataPoints().value("AID").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("AID") << "--> " << model->aid() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("AID") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("N").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("N") << "--> " << model->n() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("N") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("UN").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("UN") << "--> " << model->un() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("UN") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("St").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("St") << "--> " << model->status() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("St") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StVnd").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StVnd") << "--> " << model->vendorStatus() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StVnd") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Evt").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "--> " << model->eventCode() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("EvtVnd").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd") << "--> " << model->vendorEventCode() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Ctl").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Ctl") << "--> " << model->control() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Ctl") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("CtlVnd").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("CtlVnd") << "--> " << model->vendorControl() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("CtlVnd") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("CtlVl").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("CtlVl") << "--> " << model->controlValue() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("CtlVl") << "--> NaN" << endl;
    }


    return debug.space().quote();
}
