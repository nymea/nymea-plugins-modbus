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

#include "sunspecfreqwattparammodel.h"

SunSpecFreqWattParamModel::SunSpecFreqWattParamModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 127;
}

SunSpecFreqWattParamModel::~SunSpecFreqWattParamModel()
{

}

QString SunSpecFreqWattParamModel::name() const
{
    return "freq_watt_param";
}

QString SunSpecFreqWattParamModel::description() const
{
    return "Parameterized Frequency-Watt ";
}

QString SunSpecFreqWattParamModel::label() const
{
    return "Freq-Watt Param";
}

quint16 SunSpecFreqWattParamModel::modelId() const
{
    return m_modelId;
}
quint16 SunSpecFreqWattParamModel::modelLength() const
{
    return m_modelLength;
}
float SunSpecFreqWattParamModel::wGra() const
{
    return m_wGra;
}
qint16 SunSpecFreqWattParamModel::hzStr() const
{
    return m_hzStr;
}
qint16 SunSpecFreqWattParamModel::hzStop() const
{
    return m_hzStop;
}
SunSpecFreqWattParamModel::HysenaFlags SunSpecFreqWattParamModel::hysEna() const
{
    return m_hysEna;
}
SunSpecFreqWattParamModel::ModenaFlags SunSpecFreqWattParamModel::modEna() const
{
    return m_modEna;
}
float SunSpecFreqWattParamModel::hzStopWGra() const
{
    return m_hzStopWGra;
}
quint16 SunSpecFreqWattParamModel::pad() const
{
    return m_pad;
}
void SunSpecFreqWattParamModel::initDataPoints()
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

    SunSpecDataPoint wGraDataPoint;
    wGraDataPoint.setName("WGra");
    wGraDataPoint.setLabel("WGra");
    wGraDataPoint.setDescription("The slope of the reduction in the maximum allowed watts output as a function of frequency.");
    wGraDataPoint.setUnits("% PM/Hz");
    wGraDataPoint.setMandatory(true);
    wGraDataPoint.setSize(1);
    wGraDataPoint.setAddressOffset(2);
    wGraDataPoint.setBlockOffset(0);
    wGraDataPoint.setScaleFactorName("WGra_SF");
    wGraDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wGraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(wGraDataPoint.name(), wGraDataPoint);

    SunSpecDataPoint hzStrDataPoint;
    hzStrDataPoint.setName("HzStr");
    hzStrDataPoint.setLabel("HzStr");
    hzStrDataPoint.setDescription("The frequency deviation from nominal frequency (ECPNomHz) at which a snapshot of the instantaneous power output is taken to act as the CAPPED power level (PM) and above which reduction in power output occurs.");
    hzStrDataPoint.setUnits("Hz");
    hzStrDataPoint.setMandatory(true);
    hzStrDataPoint.setSize(1);
    hzStrDataPoint.setAddressOffset(3);
    hzStrDataPoint.setBlockOffset(1);
    hzStrDataPoint.setScaleFactorName("HzStrStop_SF");
    hzStrDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    hzStrDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hzStrDataPoint.name(), hzStrDataPoint);

    SunSpecDataPoint hzStopDataPoint;
    hzStopDataPoint.setName("HzStop");
    hzStopDataPoint.setLabel("HzStop");
    hzStopDataPoint.setDescription("The frequency deviation from nominal frequency (ECPNomHz) at which curtailed power output may return to normal and the cap on the power level value is removed.");
    hzStopDataPoint.setUnits("Hz");
    hzStopDataPoint.setMandatory(true);
    hzStopDataPoint.setSize(1);
    hzStopDataPoint.setAddressOffset(4);
    hzStopDataPoint.setBlockOffset(2);
    hzStopDataPoint.setScaleFactorName("HzStrStop_SF");
    hzStopDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    hzStopDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hzStopDataPoint.name(), hzStopDataPoint);

    SunSpecDataPoint hysEnaDataPoint;
    hysEnaDataPoint.setName("HysEna");
    hysEnaDataPoint.setLabel("HysEna");
    hysEnaDataPoint.setDescription("Enable hysteresis");
    hysEnaDataPoint.setMandatory(true);
    hysEnaDataPoint.setSize(1);
    hysEnaDataPoint.setAddressOffset(5);
    hysEnaDataPoint.setBlockOffset(3);
    hysEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    hysEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hysEnaDataPoint.name(), hysEnaDataPoint);

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("Is Parameterized Frequency-Watt control active.");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(6);
    modEnaDataPoint.setBlockOffset(4);
    modEnaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint hzStopWGraDataPoint;
    hzStopWGraDataPoint.setName("HzStopWGra");
    hzStopWGraDataPoint.setLabel("HzStopWGra");
    hzStopWGraDataPoint.setDescription("The maximum time-based rate of change at which power output returns to normal after having been capped by an over frequency event.");
    hzStopWGraDataPoint.setUnits("% WMax/min");
    hzStopWGraDataPoint.setSize(1);
    hzStopWGraDataPoint.setAddressOffset(7);
    hzStopWGraDataPoint.setBlockOffset(5);
    hzStopWGraDataPoint.setScaleFactorName("RmpIncDec_SF");
    hzStopWGraDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    hzStopWGraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hzStopWGraDataPoint.name(), hzStopWGraDataPoint);

    SunSpecDataPoint wGraSfDataPoint;
    wGraSfDataPoint.setName("WGra_SF");
    wGraSfDataPoint.setLabel("WGra_SF");
    wGraSfDataPoint.setDescription("Scale factor for output gradient.");
    wGraSfDataPoint.setSize(1);
    wGraSfDataPoint.setAddressOffset(8);
    wGraSfDataPoint.setBlockOffset(6);
    wGraSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(wGraSfDataPoint.name(), wGraSfDataPoint);

    SunSpecDataPoint hzStrStopSfDataPoint;
    hzStrStopSfDataPoint.setName("HzStrStop_SF");
    hzStrStopSfDataPoint.setLabel("HzStrStop_SF");
    hzStrStopSfDataPoint.setDescription("Scale factor for frequency deviations.");
    hzStrStopSfDataPoint.setSize(1);
    hzStrStopSfDataPoint.setAddressOffset(9);
    hzStrStopSfDataPoint.setBlockOffset(7);
    hzStrStopSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(hzStrStopSfDataPoint.name(), hzStrStopSfDataPoint);

    SunSpecDataPoint rmpIncDecSfDataPoint;
    rmpIncDecSfDataPoint.setName("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setLabel("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setDescription("Scale factor for increment and decrement ramps.");
    rmpIncDecSfDataPoint.setSize(1);
    rmpIncDecSfDataPoint.setAddressOffset(10);
    rmpIncDecSfDataPoint.setBlockOffset(8);
    rmpIncDecSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(rmpIncDecSfDataPoint.name(), rmpIncDecSfDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(11);
    padDataPoint.setBlockOffset(9);
    padDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

