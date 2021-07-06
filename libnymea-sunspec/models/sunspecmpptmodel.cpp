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

#include "sunspecmpptmodel.h"
#include "sunspecconnection.h"

SunSpecMpptModel::SunSpecMpptModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 160, 8, parent)
{
    //Q_ASSERT_X(length == 8,  "SunSpecMpptModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecMpptModel::~SunSpecMpptModel()
{

}

QString SunSpecMpptModel::name() const
{
    return "mppt";
}

QString SunSpecMpptModel::description() const
{
    return QString();
}

QString SunSpecMpptModel::label() const
{
    return "Multiple MPPT Inverter Extension Model";
}

SunSpecMpptModel::EvtFlags SunSpecMpptModel::globalEvents() const
{
    return m_globalEvents;
}
int SunSpecMpptModel::numberOfModules() const
{
    return m_numberOfModules;
}
quint16 SunSpecMpptModel::timestampPeriod() const
{
    return m_timestampPeriod;
}
void SunSpecMpptModel::processBlockData()
{
    // Scale factors
    m_currentScaleFactor = m_dataPoints.value("DCA_SF").toInt16();
    m_voltageScaleFactor = m_dataPoints.value("DCV_SF").toInt16();
    m_powerScaleFactor = m_dataPoints.value("DCW_SF").toInt16();
    m_energyScaleFactor = m_dataPoints.value("DCWH_SF").toInt16();

    // Update properties according to the data point type
    m_globalEvents = static_cast<EvtFlags>(m_dataPoints.value("Evt").toUInt32());
    m_numberOfModules = m_dataPoints.value("N").toUInt16();
    m_timestampPeriod = m_dataPoints.value("TmsPer").toUInt16();

    qCDebug(dcSunSpec()) << this;
}

void SunSpecMpptModel::initDataPoints()
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

    SunSpecDataPoint currentScaleFactorDataPoint;
    currentScaleFactorDataPoint.setName("DCA_SF");
    currentScaleFactorDataPoint.setLabel("Current Scale Factor");
    currentScaleFactorDataPoint.setSize(1);
    currentScaleFactorDataPoint.setAddressOffset(2);
    currentScaleFactorDataPoint.setBlockOffset(0);
    currentScaleFactorDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(currentScaleFactorDataPoint.name(), currentScaleFactorDataPoint);

    SunSpecDataPoint voltageScaleFactorDataPoint;
    voltageScaleFactorDataPoint.setName("DCV_SF");
    voltageScaleFactorDataPoint.setLabel("Voltage Scale Factor");
    voltageScaleFactorDataPoint.setSize(1);
    voltageScaleFactorDataPoint.setAddressOffset(3);
    voltageScaleFactorDataPoint.setBlockOffset(1);
    voltageScaleFactorDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(voltageScaleFactorDataPoint.name(), voltageScaleFactorDataPoint);

    SunSpecDataPoint powerScaleFactorDataPoint;
    powerScaleFactorDataPoint.setName("DCW_SF");
    powerScaleFactorDataPoint.setLabel("Power Scale Factor");
    powerScaleFactorDataPoint.setSize(1);
    powerScaleFactorDataPoint.setAddressOffset(4);
    powerScaleFactorDataPoint.setBlockOffset(2);
    powerScaleFactorDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(powerScaleFactorDataPoint.name(), powerScaleFactorDataPoint);

    SunSpecDataPoint energyScaleFactorDataPoint;
    energyScaleFactorDataPoint.setName("DCWH_SF");
    energyScaleFactorDataPoint.setLabel("Energy Scale Factor");
    energyScaleFactorDataPoint.setSize(1);
    energyScaleFactorDataPoint.setAddressOffset(5);
    energyScaleFactorDataPoint.setBlockOffset(3);
    energyScaleFactorDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(energyScaleFactorDataPoint.name(), energyScaleFactorDataPoint);

    SunSpecDataPoint globalEventsDataPoint;
    globalEventsDataPoint.setName("Evt");
    globalEventsDataPoint.setLabel("Global Events");
    globalEventsDataPoint.setSize(2);
    globalEventsDataPoint.setAddressOffset(6);
    globalEventsDataPoint.setBlockOffset(4);
    globalEventsDataPoint.setSunSpecDataType("bitfield32");
    m_dataPoints.insert(globalEventsDataPoint.name(), globalEventsDataPoint);

    SunSpecDataPoint numberOfModulesDataPoint;
    numberOfModulesDataPoint.setName("N");
    numberOfModulesDataPoint.setLabel("Number of Modules");
    numberOfModulesDataPoint.setSize(1);
    numberOfModulesDataPoint.setAddressOffset(8);
    numberOfModulesDataPoint.setBlockOffset(6);
    numberOfModulesDataPoint.setSunSpecDataType("count");
    m_dataPoints.insert(numberOfModulesDataPoint.name(), numberOfModulesDataPoint);

    SunSpecDataPoint timestampPeriodDataPoint;
    timestampPeriodDataPoint.setName("TmsPer");
    timestampPeriodDataPoint.setLabel("Timestamp Period");
    timestampPeriodDataPoint.setSize(1);
    timestampPeriodDataPoint.setAddressOffset(9);
    timestampPeriodDataPoint.setBlockOffset(7);
    timestampPeriodDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(timestampPeriodDataPoint.name(), timestampPeriodDataPoint);

}

QDebug operator<<(QDebug debug, SunSpecMpptModel *model)
{
    debug.nospace().noquote() << "SunSpecMpptModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    if (model->dataPoints().value("Evt").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "--> " << model->globalEvents() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("N").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("N") << "--> " << model->numberOfModules() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("N") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("TmsPer").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TmsPer") << "--> " << model->timestampPeriod() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TmsPer") << "--> NaN" << endl;
    }


    return debug.space().quote();
}
