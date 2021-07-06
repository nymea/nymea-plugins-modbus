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
#include "sunspecconnection.h"

SunSpecScheduleModel::SunSpecScheduleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 133, 6, parent)
{
    //Q_ASSERT_X(length == 6,  "SunSpecScheduleModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
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

SunSpecScheduleModel::ActschdFlags SunSpecScheduleModel::actSchd() const
{
    return m_actSchd;
}

QModbusReply *SunSpecScheduleModel::setActSchd(ActschdFlags actSchd)
{
    Q_UNUSED(actSchd)
    return nullptr;
}
SunSpecScheduleModel::ModenaFlags SunSpecScheduleModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecScheduleModel::setModEna(ModenaFlags modEna)
{
    Q_UNUSED(modEna)
    return nullptr;
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
void SunSpecScheduleModel::processBlockData()
{
    // Update properties according to the data point type
    m_actSchd = static_cast<ActschdFlags>(m_dataPoints.value("ActSchd").toUInt32());
    m_modEna = static_cast<ModenaFlags>(m_dataPoints.value("ModEna").toUInt16());
    m_nSchd = m_dataPoints.value("NSchd").toUInt16();
    m_nPts = m_dataPoints.value("NPts").toUInt16();
    m_pad = m_dataPoints.value("Pad").toUInt16();

    qCDebug(dcSunSpec()) << this;
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

    SunSpecDataPoint actSchdDataPoint;
    actSchdDataPoint.setName("ActSchd");
    actSchdDataPoint.setLabel("ActSchd");
    actSchdDataPoint.setDescription("Bitfield of active schedules");
    actSchdDataPoint.setMandatory(true);
    actSchdDataPoint.setSize(2);
    actSchdDataPoint.setAddressOffset(2);
    actSchdDataPoint.setBlockOffset(0);
    actSchdDataPoint.setSunSpecDataType("bitfield32");
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
    modEnaDataPoint.setSunSpecDataType("bitfield16");
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
    nSchdDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(nSchdDataPoint.name(), nSchdDataPoint);

    SunSpecDataPoint nPtsDataPoint;
    nPtsDataPoint.setName("NPts");
    nPtsDataPoint.setLabel("NPts");
    nPtsDataPoint.setDescription("Number of schedule entries supported (maximum of 10).");
    nPtsDataPoint.setMandatory(true);
    nPtsDataPoint.setSize(1);
    nPtsDataPoint.setAddressOffset(6);
    nPtsDataPoint.setBlockOffset(4);
    nPtsDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(nPtsDataPoint.name(), nPtsDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setLabel("Pad");
    padDataPoint.setDescription("Pad register.");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(7);
    padDataPoint.setBlockOffset(5);
    padDataPoint.setSunSpecDataType("pad");
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

QDebug operator<<(QDebug debug, SunSpecScheduleModel *model)
{
    debug.nospace().noquote() << "SunSpecScheduleModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    if (model->dataPoints().value("ActSchd").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ActSchd") << "--> " << model->actSchd() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ActSchd") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ModEna").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModEna") << "--> " << model->modEna() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModEna") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("NSchd").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NSchd") << "--> " << model->nSchd() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NSchd") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("NPts").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NPts") << "--> " << model->nPts() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NPts") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Pad").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad") << "--> " << model->pad() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad") << "--> NaN" << endl;
    }


    return debug.space().quote();
}
