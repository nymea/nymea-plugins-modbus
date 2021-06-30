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

void SunSpecFreqWattParamModel::readModelHeader()
{

}

void SunSpecFreqWattParamModel::readBlockData()
{

}

void SunSpecFreqWattParamModel::initDataPoints()
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

    SunSpecDataPoint wgraDataPoint;
    wgraDataPoint.setName("WGra");
    wgraDataPoint.setLabel("WGra");
    wgraDataPoint.setDescription("The slope of the reduction in the maximum allowed watts output as a function of frequency.");
    wgraDataPoint.setUnits("% PM/Hz");
    wgraDataPoint.setMandatory(true);
    wgraDataPoint.setSize(1);
    wgraDataPoint.setScaleFactorName("WGra_SF");
    wgraDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wgraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wgraDataPoint;

    SunSpecDataPoint hzstrDataPoint;
    hzstrDataPoint.setName("HzStr");
    hzstrDataPoint.setLabel("HzStr");
    hzstrDataPoint.setDescription("The frequency deviation from nominal frequency (ECPNomHz) at which a snapshot of the instantaneous power output is taken to act as the CAPPED power level (PM) and above which reduction in power output occurs.");
    hzstrDataPoint.setUnits("Hz");
    hzstrDataPoint.setMandatory(true);
    hzstrDataPoint.setSize(1);
    hzstrDataPoint.setScaleFactorName("HzStrStop_SF");
    hzstrDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    hzstrDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << hzstrDataPoint;

    SunSpecDataPoint hzstopDataPoint;
    hzstopDataPoint.setName("HzStop");
    hzstopDataPoint.setLabel("HzStop");
    hzstopDataPoint.setDescription("The frequency deviation from nominal frequency (ECPNomHz) at which curtailed power output may return to normal and the cap on the power level value is removed.");
    hzstopDataPoint.setUnits("Hz");
    hzstopDataPoint.setMandatory(true);
    hzstopDataPoint.setSize(1);
    hzstopDataPoint.setScaleFactorName("HzStrStop_SF");
    hzstopDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    hzstopDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << hzstopDataPoint;

    SunSpecDataPoint hysenaDataPoint;
    hysenaDataPoint.setName("HysEna");
    hysenaDataPoint.setLabel("HysEna");
    hysenaDataPoint.setDescription("Enable hysteresis");
    hysenaDataPoint.setMandatory(true);
    hysenaDataPoint.setSize(1);
    hysenaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    hysenaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << hysenaDataPoint;

    SunSpecDataPoint modenaDataPoint;
    modenaDataPoint.setName("ModEna");
    modenaDataPoint.setLabel("ModEna");
    modenaDataPoint.setDescription("Is Parameterized Frequency-Watt control active.");
    modenaDataPoint.setMandatory(true);
    modenaDataPoint.setSize(1);
    modenaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modenaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << modenaDataPoint;

    SunSpecDataPoint hzstopwgraDataPoint;
    hzstopwgraDataPoint.setName("HzStopWGra");
    hzstopwgraDataPoint.setLabel("HzStopWGra");
    hzstopwgraDataPoint.setDescription("The maximum time-based rate of change at which power output returns to normal after having been capped by an over frequency event.");
    hzstopwgraDataPoint.setUnits("% WMax/min");
    hzstopwgraDataPoint.setSize(1);
    hzstopwgraDataPoint.setScaleFactorName("RmpIncDec_SF");
    hzstopwgraDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    hzstopwgraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << hzstopwgraDataPoint;

    SunSpecDataPoint wgraSfDataPoint;
    wgraSfDataPoint.setName("WGra_SF");
    wgraSfDataPoint.setLabel("WGra_SF");
    wgraSfDataPoint.setDescription("Scale factor for output gradient.");
    wgraSfDataPoint.setSize(1);
    wgraSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << wgraSfDataPoint;

    SunSpecDataPoint hzstrstopSfDataPoint;
    hzstrstopSfDataPoint.setName("HzStrStop_SF");
    hzstrstopSfDataPoint.setLabel("HzStrStop_SF");
    hzstrstopSfDataPoint.setDescription("Scale factor for frequency deviations.");
    hzstrstopSfDataPoint.setSize(1);
    hzstrstopSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << hzstrstopSfDataPoint;

    SunSpecDataPoint rmpincdecSfDataPoint;
    rmpincdecSfDataPoint.setName("RmpIncDec_SF");
    rmpincdecSfDataPoint.setLabel("RmpIncDec_SF");
    rmpincdecSfDataPoint.setDescription("Scale factor for increment and decrement ramps.");
    rmpincdecSfDataPoint.setSize(1);
    rmpincdecSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << rmpincdecSfDataPoint;

    SunSpecDataPoint PadDataPoint;
    PadDataPoint.setName("Pad");
    PadDataPoint.setSize(1);
    PadDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints << PadDataPoint;

}

