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

#include "sunspecirradiancemodel.h"
#include "sunspecconnection.h"

SunSpecIrradianceModelRepeatingBlock::SunSpecIrradianceModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecIrradianceModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecIrradianceModelRepeatingBlock::name() const
{
    return "repeating";
}

SunSpecIrradianceModel *SunSpecIrradianceModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecIrradianceModelRepeatingBlock::ghi() const
{
    return m_ghi;
}
quint16 SunSpecIrradianceModelRepeatingBlock::poai() const
{
    return m_poai;
}
quint16 SunSpecIrradianceModelRepeatingBlock::dfi() const
{
    return m_dfi;
}
quint16 SunSpecIrradianceModelRepeatingBlock::dni() const
{
    return m_dni;
}
quint16 SunSpecIrradianceModelRepeatingBlock::oti() const
{
    return m_oti;
}

void SunSpecIrradianceModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint ghiDataPoint;
    ghiDataPoint.setName("GHI");
    ghiDataPoint.setLabel("GHI");
    ghiDataPoint.setDescription("Global Horizontal Irradiance");
    ghiDataPoint.setUnits("W/m2");
    ghiDataPoint.setSize(1);
    ghiDataPoint.setAddressOffset(0);
    ghiDataPoint.setSunSpecDataType("uint16");
    ghiDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ghiDataPoint.name(), ghiDataPoint);

    SunSpecDataPoint poaiDataPoint;
    poaiDataPoint.setName("POAI");
    poaiDataPoint.setLabel("POAI");
    poaiDataPoint.setDescription("Plane-of-Array Irradiance");
    poaiDataPoint.setUnits("W/m2");
    poaiDataPoint.setSize(1);
    poaiDataPoint.setAddressOffset(1);
    poaiDataPoint.setSunSpecDataType("uint16");
    poaiDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(poaiDataPoint.name(), poaiDataPoint);

    SunSpecDataPoint dfiDataPoint;
    dfiDataPoint.setName("DFI");
    dfiDataPoint.setLabel("DFI");
    dfiDataPoint.setDescription("Diffuse Irradiance");
    dfiDataPoint.setUnits("W/m2");
    dfiDataPoint.setSize(1);
    dfiDataPoint.setAddressOffset(2);
    dfiDataPoint.setBlockOffset(0);
    dfiDataPoint.setSunSpecDataType("uint16");
    dfiDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dfiDataPoint.name(), dfiDataPoint);

    SunSpecDataPoint dniDataPoint;
    dniDataPoint.setName("DNI");
    dniDataPoint.setLabel("DNI");
    dniDataPoint.setDescription("Direct Normal Irradiance");
    dniDataPoint.setUnits("W/m2");
    dniDataPoint.setSize(1);
    dniDataPoint.setAddressOffset(3);
    dniDataPoint.setBlockOffset(1);
    dniDataPoint.setSunSpecDataType("uint16");
    dniDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dniDataPoint.name(), dniDataPoint);

    SunSpecDataPoint otiDataPoint;
    otiDataPoint.setName("OTI");
    otiDataPoint.setLabel("OTI");
    otiDataPoint.setDescription("Other Irradiance");
    otiDataPoint.setUnits("W/m2");
    otiDataPoint.setSize(1);
    otiDataPoint.setAddressOffset(4);
    otiDataPoint.setBlockOffset(2);
    otiDataPoint.setSunSpecDataType("uint16");
    otiDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(otiDataPoint.name(), otiDataPoint);

}

void SunSpecIrradianceModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("GHI").isValid())
        m_ghi = m_dataPoints.value("GHI").toUInt16();

    if (m_dataPoints.value("POAI").isValid())
        m_poai = m_dataPoints.value("POAI").toUInt16();

    if (m_dataPoints.value("DFI").isValid())
        m_dfi = m_dataPoints.value("DFI").toUInt16();

    if (m_dataPoints.value("DNI").isValid())
        m_dni = m_dataPoints.value("DNI").toUInt16();

    if (m_dataPoints.value("OTI").isValid())
        m_oti = m_dataPoints.value("OTI").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecIrradianceModel::SunSpecIrradianceModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 302, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecIrradianceModel::~SunSpecIrradianceModel()
{

}

QString SunSpecIrradianceModel::name() const
{
    return "irradiance";
}

QString SunSpecIrradianceModel::description() const
{
    return "Include to support various irradiance measurements";
}

QString SunSpecIrradianceModel::label() const
{
    return "Irradiance Model";
}

void SunSpecIrradianceModel::initDataPoints()
{
    SunSpecDataPoint modelIdDataPoint;
    modelIdDataPoint.setName("ID");
    modelIdDataPoint.setLabel("Model ID");
    modelIdDataPoint.setDescription("Model identifier");
    modelIdDataPoint.setMandatory(true);
    modelIdDataPoint.setSize(1);
    modelIdDataPoint.setAddressOffset(0);
    modelIdDataPoint.setSunSpecDataType("uint16");
    modelIdDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setSunSpecDataType("uint16");
    modelLengthDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

}

void SunSpecIrradianceModel::processBlockData()
{
    // Update properties according to the data point type

    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecIrradianceModel *model)
{
    debug.nospace().noquote() << "SunSpecIrradianceModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";

    return debug.space().quote();
}
