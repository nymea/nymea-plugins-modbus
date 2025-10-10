/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
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

#include "sunspecnameplatemodel.h"
#include "sunspecconnection.h"

SunSpecNameplateModel::SunSpecNameplateModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 120, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecNameplateModel::~SunSpecNameplateModel()
{

}

QString SunSpecNameplateModel::name() const
{
    return "nameplate";
}

QString SunSpecNameplateModel::description() const
{
    return "Inverter Controls Nameplate Ratings ";
}

QString SunSpecNameplateModel::label() const
{
    return "Nameplate";
}

SunSpecNameplateModel::Dertyp SunSpecNameplateModel::derTyp() const
{
    return m_derTyp;
}
float SunSpecNameplateModel::wRtg() const
{
    return m_wRtg;
}
qint16 SunSpecNameplateModel::wRtgSf() const
{
    return m_wRtgSf;
}
float SunSpecNameplateModel::vaRtg() const
{
    return m_vaRtg;
}
qint16 SunSpecNameplateModel::vaRtgSf() const
{
    return m_vaRtgSf;
}
float SunSpecNameplateModel::vArRtgQ1() const
{
    return m_vArRtgQ1;
}
float SunSpecNameplateModel::vArRtgQ2() const
{
    return m_vArRtgQ2;
}
float SunSpecNameplateModel::vArRtgQ3() const
{
    return m_vArRtgQ3;
}
float SunSpecNameplateModel::vArRtgQ4() const
{
    return m_vArRtgQ4;
}
qint16 SunSpecNameplateModel::vArRtgSf() const
{
    return m_vArRtgSf;
}
float SunSpecNameplateModel::aRtg() const
{
    return m_aRtg;
}
qint16 SunSpecNameplateModel::aRtgSf() const
{
    return m_aRtgSf;
}
float SunSpecNameplateModel::pfRtgQ1() const
{
    return m_pfRtgQ1;
}
float SunSpecNameplateModel::pfRtgQ2() const
{
    return m_pfRtgQ2;
}
float SunSpecNameplateModel::pfRtgQ3() const
{
    return m_pfRtgQ3;
}
float SunSpecNameplateModel::pfRtgQ4() const
{
    return m_pfRtgQ4;
}
qint16 SunSpecNameplateModel::pfRtgSf() const
{
    return m_pfRtgSf;
}
float SunSpecNameplateModel::whRtg() const
{
    return m_whRtg;
}
qint16 SunSpecNameplateModel::whRtgSf() const
{
    return m_whRtgSf;
}
float SunSpecNameplateModel::ahrRtg() const
{
    return m_ahrRtg;
}
qint16 SunSpecNameplateModel::ahrRtgSf() const
{
    return m_ahrRtgSf;
}
float SunSpecNameplateModel::maxChaRte() const
{
    return m_maxChaRte;
}
qint16 SunSpecNameplateModel::maxChaRteSf() const
{
    return m_maxChaRteSf;
}
float SunSpecNameplateModel::maxDisChaRte() const
{
    return m_maxDisChaRte;
}
qint16 SunSpecNameplateModel::maxDisChaRteSf() const
{
    return m_maxDisChaRteSf;
}
quint16 SunSpecNameplateModel::pad() const
{
    return m_pad;
}
void SunSpecNameplateModel::initDataPoints()
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

    SunSpecDataPoint derTypDataPoint;
    derTypDataPoint.setName("DERTyp");
    derTypDataPoint.setLabel("DERTyp");
    derTypDataPoint.setDescription("Type of DER device. Default value is 4 to indicate PV device.");
    derTypDataPoint.setMandatory(true);
    derTypDataPoint.setSize(1);
    derTypDataPoint.setAddressOffset(2);
    derTypDataPoint.setBlockOffset(0);
    derTypDataPoint.setSunSpecDataType("enum16");
    derTypDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(derTypDataPoint.name(), derTypDataPoint);

    SunSpecDataPoint wRtgDataPoint;
    wRtgDataPoint.setName("WRtg");
    wRtgDataPoint.setLabel("WRtg");
    wRtgDataPoint.setDescription("Continuous power output capability of the inverter.");
    wRtgDataPoint.setUnits("W");
    wRtgDataPoint.setMandatory(true);
    wRtgDataPoint.setSize(1);
    wRtgDataPoint.setAddressOffset(3);
    wRtgDataPoint.setBlockOffset(1);
    wRtgDataPoint.setScaleFactorName("WRtg_SF");
    wRtgDataPoint.setSunSpecDataType("uint16");
    wRtgDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wRtgDataPoint.name(), wRtgDataPoint);

    SunSpecDataPoint wRtgSfDataPoint;
    wRtgSfDataPoint.setName("WRtg_SF");
    wRtgSfDataPoint.setLabel("WRtg_SF");
    wRtgSfDataPoint.setDescription("Scale factor");
    wRtgSfDataPoint.setMandatory(true);
    wRtgSfDataPoint.setSize(1);
    wRtgSfDataPoint.setAddressOffset(4);
    wRtgSfDataPoint.setBlockOffset(2);
    wRtgSfDataPoint.setSunSpecDataType("sunssf");
    wRtgSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wRtgSfDataPoint.name(), wRtgSfDataPoint);

    SunSpecDataPoint vaRtgDataPoint;
    vaRtgDataPoint.setName("VARtg");
    vaRtgDataPoint.setLabel("VARtg");
    vaRtgDataPoint.setDescription("Continuous Volt-Ampere capability of the inverter.");
    vaRtgDataPoint.setUnits("VA");
    vaRtgDataPoint.setMandatory(true);
    vaRtgDataPoint.setSize(1);
    vaRtgDataPoint.setAddressOffset(5);
    vaRtgDataPoint.setBlockOffset(3);
    vaRtgDataPoint.setScaleFactorName("VARtg_SF");
    vaRtgDataPoint.setSunSpecDataType("uint16");
    vaRtgDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vaRtgDataPoint.name(), vaRtgDataPoint);

    SunSpecDataPoint vaRtgSfDataPoint;
    vaRtgSfDataPoint.setName("VARtg_SF");
    vaRtgSfDataPoint.setLabel("VARtg_SF");
    vaRtgSfDataPoint.setDescription("Scale factor");
    vaRtgSfDataPoint.setMandatory(true);
    vaRtgSfDataPoint.setSize(1);
    vaRtgSfDataPoint.setAddressOffset(6);
    vaRtgSfDataPoint.setBlockOffset(4);
    vaRtgSfDataPoint.setSunSpecDataType("sunssf");
    vaRtgSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vaRtgSfDataPoint.name(), vaRtgSfDataPoint);

    SunSpecDataPoint vArRtgQ1DataPoint;
    vArRtgQ1DataPoint.setName("VArRtgQ1");
    vArRtgQ1DataPoint.setLabel("VArRtgQ1");
    vArRtgQ1DataPoint.setDescription("Continuous VAR capability of the inverter in quadrant 1.");
    vArRtgQ1DataPoint.setUnits("var");
    vArRtgQ1DataPoint.setMandatory(true);
    vArRtgQ1DataPoint.setSize(1);
    vArRtgQ1DataPoint.setAddressOffset(7);
    vArRtgQ1DataPoint.setBlockOffset(5);
    vArRtgQ1DataPoint.setScaleFactorName("VArRtg_SF");
    vArRtgQ1DataPoint.setSunSpecDataType("int16");
    vArRtgQ1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArRtgQ1DataPoint.name(), vArRtgQ1DataPoint);

    SunSpecDataPoint vArRtgQ2DataPoint;
    vArRtgQ2DataPoint.setName("VArRtgQ2");
    vArRtgQ2DataPoint.setLabel("VArRtgQ2");
    vArRtgQ2DataPoint.setDescription("Continuous VAR capability of the inverter in quadrant 2.");
    vArRtgQ2DataPoint.setUnits("var");
    vArRtgQ2DataPoint.setMandatory(true);
    vArRtgQ2DataPoint.setSize(1);
    vArRtgQ2DataPoint.setAddressOffset(8);
    vArRtgQ2DataPoint.setBlockOffset(6);
    vArRtgQ2DataPoint.setScaleFactorName("VArRtg_SF");
    vArRtgQ2DataPoint.setSunSpecDataType("int16");
    vArRtgQ2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArRtgQ2DataPoint.name(), vArRtgQ2DataPoint);

    SunSpecDataPoint vArRtgQ3DataPoint;
    vArRtgQ3DataPoint.setName("VArRtgQ3");
    vArRtgQ3DataPoint.setLabel("VArRtgQ3");
    vArRtgQ3DataPoint.setDescription("Continuous VAR capability of the inverter in quadrant 3.");
    vArRtgQ3DataPoint.setUnits("var");
    vArRtgQ3DataPoint.setMandatory(true);
    vArRtgQ3DataPoint.setSize(1);
    vArRtgQ3DataPoint.setAddressOffset(9);
    vArRtgQ3DataPoint.setBlockOffset(7);
    vArRtgQ3DataPoint.setScaleFactorName("VArRtg_SF");
    vArRtgQ3DataPoint.setSunSpecDataType("int16");
    vArRtgQ3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArRtgQ3DataPoint.name(), vArRtgQ3DataPoint);

    SunSpecDataPoint vArRtgQ4DataPoint;
    vArRtgQ4DataPoint.setName("VArRtgQ4");
    vArRtgQ4DataPoint.setLabel("VArRtgQ4");
    vArRtgQ4DataPoint.setDescription("Continuous VAR capability of the inverter in quadrant 4.");
    vArRtgQ4DataPoint.setUnits("var");
    vArRtgQ4DataPoint.setMandatory(true);
    vArRtgQ4DataPoint.setSize(1);
    vArRtgQ4DataPoint.setAddressOffset(10);
    vArRtgQ4DataPoint.setBlockOffset(8);
    vArRtgQ4DataPoint.setScaleFactorName("VArRtg_SF");
    vArRtgQ4DataPoint.setSunSpecDataType("int16");
    vArRtgQ4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArRtgQ4DataPoint.name(), vArRtgQ4DataPoint);

    SunSpecDataPoint vArRtgSfDataPoint;
    vArRtgSfDataPoint.setName("VArRtg_SF");
    vArRtgSfDataPoint.setLabel("VArRtg_SF");
    vArRtgSfDataPoint.setDescription("Scale factor");
    vArRtgSfDataPoint.setMandatory(true);
    vArRtgSfDataPoint.setSize(1);
    vArRtgSfDataPoint.setAddressOffset(11);
    vArRtgSfDataPoint.setBlockOffset(9);
    vArRtgSfDataPoint.setSunSpecDataType("sunssf");
    vArRtgSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArRtgSfDataPoint.name(), vArRtgSfDataPoint);

    SunSpecDataPoint aRtgDataPoint;
    aRtgDataPoint.setName("ARtg");
    aRtgDataPoint.setLabel("ARtg");
    aRtgDataPoint.setDescription("Maximum RMS AC current level capability of the inverter.");
    aRtgDataPoint.setUnits("A");
    aRtgDataPoint.setMandatory(true);
    aRtgDataPoint.setSize(1);
    aRtgDataPoint.setAddressOffset(12);
    aRtgDataPoint.setBlockOffset(10);
    aRtgDataPoint.setScaleFactorName("ARtg_SF");
    aRtgDataPoint.setSunSpecDataType("uint16");
    aRtgDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(aRtgDataPoint.name(), aRtgDataPoint);

    SunSpecDataPoint aRtgSfDataPoint;
    aRtgSfDataPoint.setName("ARtg_SF");
    aRtgSfDataPoint.setLabel("ARtg_SF");
    aRtgSfDataPoint.setDescription("Scale factor");
    aRtgSfDataPoint.setMandatory(true);
    aRtgSfDataPoint.setSize(1);
    aRtgSfDataPoint.setAddressOffset(13);
    aRtgSfDataPoint.setBlockOffset(11);
    aRtgSfDataPoint.setSunSpecDataType("sunssf");
    aRtgSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(aRtgSfDataPoint.name(), aRtgSfDataPoint);

    SunSpecDataPoint pfRtgQ1DataPoint;
    pfRtgQ1DataPoint.setName("PFRtgQ1");
    pfRtgQ1DataPoint.setLabel("PFRtgQ1");
    pfRtgQ1DataPoint.setDescription("Minimum power factor capability of the inverter in quadrant 1.");
    pfRtgQ1DataPoint.setUnits("cos()");
    pfRtgQ1DataPoint.setMandatory(true);
    pfRtgQ1DataPoint.setSize(1);
    pfRtgQ1DataPoint.setAddressOffset(14);
    pfRtgQ1DataPoint.setBlockOffset(12);
    pfRtgQ1DataPoint.setScaleFactorName("PFRtg_SF");
    pfRtgQ1DataPoint.setSunSpecDataType("int16");
    pfRtgQ1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfRtgQ1DataPoint.name(), pfRtgQ1DataPoint);

    SunSpecDataPoint pfRtgQ2DataPoint;
    pfRtgQ2DataPoint.setName("PFRtgQ2");
    pfRtgQ2DataPoint.setLabel("PFRtgQ2");
    pfRtgQ2DataPoint.setDescription("Minimum power factor capability of the inverter in quadrant 2.");
    pfRtgQ2DataPoint.setUnits("cos()");
    pfRtgQ2DataPoint.setMandatory(true);
    pfRtgQ2DataPoint.setSize(1);
    pfRtgQ2DataPoint.setAddressOffset(15);
    pfRtgQ2DataPoint.setBlockOffset(13);
    pfRtgQ2DataPoint.setScaleFactorName("PFRtg_SF");
    pfRtgQ2DataPoint.setSunSpecDataType("int16");
    pfRtgQ2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfRtgQ2DataPoint.name(), pfRtgQ2DataPoint);

    SunSpecDataPoint pfRtgQ3DataPoint;
    pfRtgQ3DataPoint.setName("PFRtgQ3");
    pfRtgQ3DataPoint.setLabel("PFRtgQ3");
    pfRtgQ3DataPoint.setDescription("Minimum power factor capability of the inverter in quadrant 3.");
    pfRtgQ3DataPoint.setUnits("cos()");
    pfRtgQ3DataPoint.setMandatory(true);
    pfRtgQ3DataPoint.setSize(1);
    pfRtgQ3DataPoint.setAddressOffset(16);
    pfRtgQ3DataPoint.setBlockOffset(14);
    pfRtgQ3DataPoint.setScaleFactorName("PFRtg_SF");
    pfRtgQ3DataPoint.setSunSpecDataType("int16");
    pfRtgQ3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfRtgQ3DataPoint.name(), pfRtgQ3DataPoint);

    SunSpecDataPoint pfRtgQ4DataPoint;
    pfRtgQ4DataPoint.setName("PFRtgQ4");
    pfRtgQ4DataPoint.setLabel("PFRtgQ4");
    pfRtgQ4DataPoint.setDescription("Minimum power factor capability of the inverter in quadrant 4.");
    pfRtgQ4DataPoint.setUnits("cos()");
    pfRtgQ4DataPoint.setMandatory(true);
    pfRtgQ4DataPoint.setSize(1);
    pfRtgQ4DataPoint.setAddressOffset(17);
    pfRtgQ4DataPoint.setBlockOffset(15);
    pfRtgQ4DataPoint.setScaleFactorName("PFRtg_SF");
    pfRtgQ4DataPoint.setSunSpecDataType("int16");
    pfRtgQ4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfRtgQ4DataPoint.name(), pfRtgQ4DataPoint);

    SunSpecDataPoint pfRtgSfDataPoint;
    pfRtgSfDataPoint.setName("PFRtg_SF");
    pfRtgSfDataPoint.setLabel("PFRtg_SF");
    pfRtgSfDataPoint.setDescription("Scale factor");
    pfRtgSfDataPoint.setMandatory(true);
    pfRtgSfDataPoint.setSize(1);
    pfRtgSfDataPoint.setAddressOffset(18);
    pfRtgSfDataPoint.setBlockOffset(16);
    pfRtgSfDataPoint.setSunSpecDataType("sunssf");
    pfRtgSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfRtgSfDataPoint.name(), pfRtgSfDataPoint);

    SunSpecDataPoint whRtgDataPoint;
    whRtgDataPoint.setName("WHRtg");
    whRtgDataPoint.setLabel("WHRtg");
    whRtgDataPoint.setDescription("Nominal energy rating of storage device.");
    whRtgDataPoint.setUnits("Wh");
    whRtgDataPoint.setSize(1);
    whRtgDataPoint.setAddressOffset(19);
    whRtgDataPoint.setBlockOffset(17);
    whRtgDataPoint.setScaleFactorName("WHRtg_SF");
    whRtgDataPoint.setSunSpecDataType("uint16");
    whRtgDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(whRtgDataPoint.name(), whRtgDataPoint);

    SunSpecDataPoint whRtgSfDataPoint;
    whRtgSfDataPoint.setName("WHRtg_SF");
    whRtgSfDataPoint.setLabel("WHRtg_SF");
    whRtgSfDataPoint.setDescription("Scale factor");
    whRtgSfDataPoint.setSize(1);
    whRtgSfDataPoint.setAddressOffset(20);
    whRtgSfDataPoint.setBlockOffset(18);
    whRtgSfDataPoint.setSunSpecDataType("sunssf");
    whRtgSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(whRtgSfDataPoint.name(), whRtgSfDataPoint);

    SunSpecDataPoint ahrRtgDataPoint;
    ahrRtgDataPoint.setName("AhrRtg");
    ahrRtgDataPoint.setLabel("AhrRtg");
    ahrRtgDataPoint.setDescription("The usable capacity of the battery.  Maximum charge minus minimum charge from a technology capability perspective (Amp-hour capacity rating).");
    ahrRtgDataPoint.setUnits("AH");
    ahrRtgDataPoint.setSize(1);
    ahrRtgDataPoint.setAddressOffset(21);
    ahrRtgDataPoint.setBlockOffset(19);
    ahrRtgDataPoint.setScaleFactorName("AhrRtg_SF");
    ahrRtgDataPoint.setSunSpecDataType("uint16");
    ahrRtgDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ahrRtgDataPoint.name(), ahrRtgDataPoint);

    SunSpecDataPoint ahrRtgSfDataPoint;
    ahrRtgSfDataPoint.setName("AhrRtg_SF");
    ahrRtgSfDataPoint.setLabel("AhrRtg_SF");
    ahrRtgSfDataPoint.setDescription("Scale factor for amp-hour rating.");
    ahrRtgSfDataPoint.setSize(1);
    ahrRtgSfDataPoint.setAddressOffset(22);
    ahrRtgSfDataPoint.setBlockOffset(20);
    ahrRtgSfDataPoint.setSunSpecDataType("sunssf");
    ahrRtgSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ahrRtgSfDataPoint.name(), ahrRtgSfDataPoint);

    SunSpecDataPoint maxChaRteDataPoint;
    maxChaRteDataPoint.setName("MaxChaRte");
    maxChaRteDataPoint.setLabel("MaxChaRte");
    maxChaRteDataPoint.setDescription("Maximum rate of energy transfer into the storage device.");
    maxChaRteDataPoint.setUnits("W");
    maxChaRteDataPoint.setSize(1);
    maxChaRteDataPoint.setAddressOffset(23);
    maxChaRteDataPoint.setBlockOffset(21);
    maxChaRteDataPoint.setScaleFactorName("MaxChaRte_SF");
    maxChaRteDataPoint.setSunSpecDataType("uint16");
    maxChaRteDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxChaRteDataPoint.name(), maxChaRteDataPoint);

    SunSpecDataPoint maxChaRteSfDataPoint;
    maxChaRteSfDataPoint.setName("MaxChaRte_SF");
    maxChaRteSfDataPoint.setLabel("MaxChaRte_SF");
    maxChaRteSfDataPoint.setDescription("Scale factor");
    maxChaRteSfDataPoint.setSize(1);
    maxChaRteSfDataPoint.setAddressOffset(24);
    maxChaRteSfDataPoint.setBlockOffset(22);
    maxChaRteSfDataPoint.setSunSpecDataType("sunssf");
    maxChaRteSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxChaRteSfDataPoint.name(), maxChaRteSfDataPoint);

    SunSpecDataPoint maxDisChaRteDataPoint;
    maxDisChaRteDataPoint.setName("MaxDisChaRte");
    maxDisChaRteDataPoint.setLabel("MaxDisChaRte");
    maxDisChaRteDataPoint.setDescription("Maximum rate of energy transfer out of the storage device.");
    maxDisChaRteDataPoint.setUnits("W");
    maxDisChaRteDataPoint.setSize(1);
    maxDisChaRteDataPoint.setAddressOffset(25);
    maxDisChaRteDataPoint.setBlockOffset(23);
    maxDisChaRteDataPoint.setScaleFactorName("MaxDisChaRte_SF");
    maxDisChaRteDataPoint.setSunSpecDataType("uint16");
    maxDisChaRteDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxDisChaRteDataPoint.name(), maxDisChaRteDataPoint);

    SunSpecDataPoint maxDisChaRteSfDataPoint;
    maxDisChaRteSfDataPoint.setName("MaxDisChaRte_SF");
    maxDisChaRteSfDataPoint.setLabel("MaxDisChaRte_SF");
    maxDisChaRteSfDataPoint.setDescription("Scale factor");
    maxDisChaRteSfDataPoint.setSize(1);
    maxDisChaRteSfDataPoint.setAddressOffset(26);
    maxDisChaRteSfDataPoint.setBlockOffset(24);
    maxDisChaRteSfDataPoint.setSunSpecDataType("sunssf");
    maxDisChaRteSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxDisChaRteSfDataPoint.name(), maxDisChaRteSfDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setLabel("Pad");
    padDataPoint.setDescription("Pad register.");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(27);
    padDataPoint.setBlockOffset(25);
    padDataPoint.setSunSpecDataType("pad");
    padDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

void SunSpecNameplateModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("WRtg_SF").isValid())
        m_wRtgSf = m_dataPoints.value("WRtg_SF").toInt16();

    if (m_dataPoints.value("VARtg_SF").isValid())
        m_vaRtgSf = m_dataPoints.value("VARtg_SF").toInt16();

    if (m_dataPoints.value("VArRtg_SF").isValid())
        m_vArRtgSf = m_dataPoints.value("VArRtg_SF").toInt16();

    if (m_dataPoints.value("ARtg_SF").isValid())
        m_aRtgSf = m_dataPoints.value("ARtg_SF").toInt16();

    if (m_dataPoints.value("PFRtg_SF").isValid())
        m_pfRtgSf = m_dataPoints.value("PFRtg_SF").toInt16();

    if (m_dataPoints.value("WHRtg_SF").isValid())
        m_whRtgSf = m_dataPoints.value("WHRtg_SF").toInt16();

    if (m_dataPoints.value("AhrRtg_SF").isValid())
        m_ahrRtgSf = m_dataPoints.value("AhrRtg_SF").toInt16();

    if (m_dataPoints.value("MaxChaRte_SF").isValid())
        m_maxChaRteSf = m_dataPoints.value("MaxChaRte_SF").toInt16();

    if (m_dataPoints.value("MaxDisChaRte_SF").isValid())
        m_maxDisChaRteSf = m_dataPoints.value("MaxDisChaRte_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("DERTyp").isValid())
        m_derTyp = static_cast<Dertyp>(m_dataPoints.value("DERTyp").toUInt16());

    if (m_dataPoints.value("WRtg").isValid())
        m_wRtg = m_dataPoints.value("WRtg").toFloatWithSSF(m_wRtgSf);

    if (m_dataPoints.value("WRtg_SF").isValid())
        m_wRtgSf = m_dataPoints.value("WRtg_SF").toInt16();

    if (m_dataPoints.value("VARtg").isValid())
        m_vaRtg = m_dataPoints.value("VARtg").toFloatWithSSF(m_vaRtgSf);

    if (m_dataPoints.value("VARtg_SF").isValid())
        m_vaRtgSf = m_dataPoints.value("VARtg_SF").toInt16();

    if (m_dataPoints.value("VArRtgQ1").isValid())
        m_vArRtgQ1 = m_dataPoints.value("VArRtgQ1").toFloatWithSSF(m_vArRtgSf);

    if (m_dataPoints.value("VArRtgQ2").isValid())
        m_vArRtgQ2 = m_dataPoints.value("VArRtgQ2").toFloatWithSSF(m_vArRtgSf);

    if (m_dataPoints.value("VArRtgQ3").isValid())
        m_vArRtgQ3 = m_dataPoints.value("VArRtgQ3").toFloatWithSSF(m_vArRtgSf);

    if (m_dataPoints.value("VArRtgQ4").isValid())
        m_vArRtgQ4 = m_dataPoints.value("VArRtgQ4").toFloatWithSSF(m_vArRtgSf);

    if (m_dataPoints.value("VArRtg_SF").isValid())
        m_vArRtgSf = m_dataPoints.value("VArRtg_SF").toInt16();

    if (m_dataPoints.value("ARtg").isValid())
        m_aRtg = m_dataPoints.value("ARtg").toFloatWithSSF(m_aRtgSf);

    if (m_dataPoints.value("ARtg_SF").isValid())
        m_aRtgSf = m_dataPoints.value("ARtg_SF").toInt16();

    if (m_dataPoints.value("PFRtgQ1").isValid())
        m_pfRtgQ1 = m_dataPoints.value("PFRtgQ1").toFloatWithSSF(m_pfRtgSf);

    if (m_dataPoints.value("PFRtgQ2").isValid())
        m_pfRtgQ2 = m_dataPoints.value("PFRtgQ2").toFloatWithSSF(m_pfRtgSf);

    if (m_dataPoints.value("PFRtgQ3").isValid())
        m_pfRtgQ3 = m_dataPoints.value("PFRtgQ3").toFloatWithSSF(m_pfRtgSf);

    if (m_dataPoints.value("PFRtgQ4").isValid())
        m_pfRtgQ4 = m_dataPoints.value("PFRtgQ4").toFloatWithSSF(m_pfRtgSf);

    if (m_dataPoints.value("PFRtg_SF").isValid())
        m_pfRtgSf = m_dataPoints.value("PFRtg_SF").toInt16();

    if (m_dataPoints.value("WHRtg").isValid())
        m_whRtg = m_dataPoints.value("WHRtg").toFloatWithSSF(m_whRtgSf);

    if (m_dataPoints.value("WHRtg_SF").isValid())
        m_whRtgSf = m_dataPoints.value("WHRtg_SF").toInt16();

    if (m_dataPoints.value("AhrRtg").isValid())
        m_ahrRtg = m_dataPoints.value("AhrRtg").toFloatWithSSF(m_ahrRtgSf);

    if (m_dataPoints.value("AhrRtg_SF").isValid())
        m_ahrRtgSf = m_dataPoints.value("AhrRtg_SF").toInt16();

    if (m_dataPoints.value("MaxChaRte").isValid())
        m_maxChaRte = m_dataPoints.value("MaxChaRte").toFloatWithSSF(m_maxChaRteSf);

    if (m_dataPoints.value("MaxChaRte_SF").isValid())
        m_maxChaRteSf = m_dataPoints.value("MaxChaRte_SF").toInt16();

    if (m_dataPoints.value("MaxDisChaRte").isValid())
        m_maxDisChaRte = m_dataPoints.value("MaxDisChaRte").toFloatWithSSF(m_maxDisChaRteSf);

    if (m_dataPoints.value("MaxDisChaRte_SF").isValid())
        m_maxDisChaRteSf = m_dataPoints.value("MaxDisChaRte_SF").toInt16();

    if (m_dataPoints.value("Pad").isValid())
        m_pad = m_dataPoints.value("Pad").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecNameplateModel *model)
{
    debug.nospace().noquote() << "SunSpecNameplateModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("DERTyp") << "-->";
    if (model->dataPoints().value("DERTyp").isValid()) {
        debug.nospace().noquote() << model->derTyp() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WRtg") << "-->";
    if (model->dataPoints().value("WRtg").isValid()) {
        debug.nospace().noquote() << model->wRtg() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VARtg") << "-->";
    if (model->dataPoints().value("VARtg").isValid()) {
        debug.nospace().noquote() << model->vaRtg() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArRtgQ1") << "-->";
    if (model->dataPoints().value("VArRtgQ1").isValid()) {
        debug.nospace().noquote() << model->vArRtgQ1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArRtgQ2") << "-->";
    if (model->dataPoints().value("VArRtgQ2").isValid()) {
        debug.nospace().noquote() << model->vArRtgQ2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArRtgQ3") << "-->";
    if (model->dataPoints().value("VArRtgQ3").isValid()) {
        debug.nospace().noquote() << model->vArRtgQ3() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArRtgQ4") << "-->";
    if (model->dataPoints().value("VArRtgQ4").isValid()) {
        debug.nospace().noquote() << model->vArRtgQ4() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ARtg") << "-->";
    if (model->dataPoints().value("ARtg").isValid()) {
        debug.nospace().noquote() << model->aRtg() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PFRtgQ1") << "-->";
    if (model->dataPoints().value("PFRtgQ1").isValid()) {
        debug.nospace().noquote() << model->pfRtgQ1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PFRtgQ2") << "-->";
    if (model->dataPoints().value("PFRtgQ2").isValid()) {
        debug.nospace().noquote() << model->pfRtgQ2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PFRtgQ3") << "-->";
    if (model->dataPoints().value("PFRtgQ3").isValid()) {
        debug.nospace().noquote() << model->pfRtgQ3() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PFRtgQ4") << "-->";
    if (model->dataPoints().value("PFRtgQ4").isValid()) {
        debug.nospace().noquote() << model->pfRtgQ4() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WHRtg") << "-->";
    if (model->dataPoints().value("WHRtg").isValid()) {
        debug.nospace().noquote() << model->whRtg() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("AhrRtg") << "-->";
    if (model->dataPoints().value("AhrRtg").isValid()) {
        debug.nospace().noquote() << model->ahrRtg() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("MaxChaRte") << "-->";
    if (model->dataPoints().value("MaxChaRte").isValid()) {
        debug.nospace().noquote() << model->maxChaRte() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("MaxDisChaRte") << "-->";
    if (model->dataPoints().value("MaxDisChaRte").isValid()) {
        debug.nospace().noquote() << model->maxDisChaRte() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad") << "-->";
    if (model->dataPoints().value("Pad").isValid()) {
        debug.nospace().noquote() << model->pad() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
