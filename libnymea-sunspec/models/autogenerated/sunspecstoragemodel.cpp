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

#include "sunspecstoragemodel.h"

SunSpecStorageModel::SunSpecStorageModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecStorageModel::~SunSpecStorageModel()
{

}

QString SunSpecStorageModel::name() const
{
    switch (m_modelId) {
    case 124:
        return "storage";
    case 801:
        return "storage";
    default:
        return QString();
    }
}

QString SunSpecStorageModel::description() const
{
    switch (m_modelId) {
    case 124:
        return "Basic Storage Controls ";
    case 801:
        return "This model has been deprecated.";
    default:
        return QString();
    }
}

QString SunSpecStorageModel::label() const
{
    switch (m_modelId) {
    case 124:
        return "Storage";
    case 801:
        return "Energy Storage Base Model (DEPRECATED)";
    default:
        return QString();
    }
}

void SunSpecStorageModel::readModelHeader()
{

}

void SunSpecStorageModel::readBlockData()
{

}

void SunSpecStorageModel::initDataPoints()
{
    switch (m_modelId) {
    case 124: {
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

        SunSpecDataPoint wchamaxDataPoint;
        wchamaxDataPoint.setName("WChaMax");
        wchamaxDataPoint.setLabel("WChaMax");
        wchamaxDataPoint.setDescription("Setpoint for maximum charge.");
        wchamaxDataPoint.setUnits("W");
        wchamaxDataPoint.setMandatory(true);
        wchamaxDataPoint.setSize(1);
        wchamaxDataPoint.setScaleFactorName("WChaMax_SF");
        wchamaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        wchamaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << wchamaxDataPoint;

        SunSpecDataPoint wchagraDataPoint;
        wchagraDataPoint.setName("WChaGra");
        wchagraDataPoint.setLabel("WChaGra");
        wchagraDataPoint.setDescription("Setpoint for maximum charging rate. Default is MaxChaRte.");
        wchagraDataPoint.setUnits("% WChaMax/sec");
        wchagraDataPoint.setMandatory(true);
        wchagraDataPoint.setSize(1);
        wchagraDataPoint.setScaleFactorName("WChaDisChaGra_SF");
        wchagraDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        wchagraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << wchagraDataPoint;

        SunSpecDataPoint wdischagraDataPoint;
        wdischagraDataPoint.setName("WDisChaGra");
        wdischagraDataPoint.setLabel("WDisChaGra");
        wdischagraDataPoint.setDescription("Setpoint for maximum discharge rate. Default is MaxDisChaRte.");
        wdischagraDataPoint.setUnits("% WChaMax/sec");
        wdischagraDataPoint.setMandatory(true);
        wdischagraDataPoint.setSize(1);
        wdischagraDataPoint.setScaleFactorName("WChaDisChaGra_SF");
        wdischagraDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        wdischagraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << wdischagraDataPoint;

        SunSpecDataPoint storctlModDataPoint;
        storctlModDataPoint.setName("StorCtl_Mod");
        storctlModDataPoint.setLabel("StorCtl_Mod");
        storctlModDataPoint.setDescription("Activate hold/discharge/charge storage control mode. Bitfield value.");
        storctlModDataPoint.setMandatory(true);
        storctlModDataPoint.setSize(1);
        storctlModDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
        storctlModDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << storctlModDataPoint;

        SunSpecDataPoint vachamaxDataPoint;
        vachamaxDataPoint.setName("VAChaMax");
        vachamaxDataPoint.setLabel("VAChaMax");
        vachamaxDataPoint.setDescription("Setpoint for maximum charging VA.");
        vachamaxDataPoint.setUnits("VA");
        vachamaxDataPoint.setSize(1);
        vachamaxDataPoint.setScaleFactorName("VAChaMax_SF");
        vachamaxDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        vachamaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << vachamaxDataPoint;

        SunSpecDataPoint minrsvpctDataPoint;
        minrsvpctDataPoint.setName("MinRsvPct");
        minrsvpctDataPoint.setLabel("MinRsvPct");
        minrsvpctDataPoint.setDescription("Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage.");
        minrsvpctDataPoint.setUnits("% WChaMax");
        minrsvpctDataPoint.setSize(1);
        minrsvpctDataPoint.setScaleFactorName("MinRsvPct_SF");
        minrsvpctDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        minrsvpctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << minrsvpctDataPoint;

        SunSpecDataPoint chastateDataPoint;
        chastateDataPoint.setName("ChaState");
        chastateDataPoint.setLabel("ChaState");
        chastateDataPoint.setDescription("Currently available energy as a percent of the capacity rating.");
        chastateDataPoint.setUnits("% AhrRtg");
        chastateDataPoint.setSize(1);
        chastateDataPoint.setScaleFactorName("ChaState_SF");
        chastateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << chastateDataPoint;

        SunSpecDataPoint storavalDataPoint;
        storavalDataPoint.setName("StorAval");
        storavalDataPoint.setLabel("StorAval");
        storavalDataPoint.setDescription("State of charge (ChaState) minus storage reserve (MinRsvPct) times capacity rating (AhrRtg).");
        storavalDataPoint.setUnits("AH");
        storavalDataPoint.setSize(1);
        storavalDataPoint.setScaleFactorName("StorAval_SF");
        storavalDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << storavalDataPoint;

        SunSpecDataPoint inbatvDataPoint;
        inbatvDataPoint.setName("InBatV");
        inbatvDataPoint.setLabel("InBatV");
        inbatvDataPoint.setDescription("Internal battery voltage.");
        inbatvDataPoint.setUnits("V");
        inbatvDataPoint.setSize(1);
        inbatvDataPoint.setScaleFactorName("InBatV_SF");
        inbatvDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << inbatvDataPoint;

        SunSpecDataPoint chastDataPoint;
        chastDataPoint.setName("ChaSt");
        chastDataPoint.setLabel("ChaSt");
        chastDataPoint.setDescription("Charge status of storage device. Enumerated value.");
        chastDataPoint.setSize(1);
        chastDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << chastDataPoint;

        SunSpecDataPoint outwrteDataPoint;
        outwrteDataPoint.setName("OutWRte");
        outwrteDataPoint.setLabel("OutWRte");
        outwrteDataPoint.setDescription("Percent of max discharge rate.");
        outwrteDataPoint.setUnits("% WDisChaMax");
        outwrteDataPoint.setSize(1);
        outwrteDataPoint.setScaleFactorName("InOutWRte_SF");
        outwrteDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        outwrteDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << outwrteDataPoint;

        SunSpecDataPoint inwrteDataPoint;
        inwrteDataPoint.setName("InWRte");
        inwrteDataPoint.setLabel("InWRte");
        inwrteDataPoint.setDescription("Percent of max charging rate.");
        inwrteDataPoint.setUnits(" % WChaMax");
        inwrteDataPoint.setSize(1);
        inwrteDataPoint.setScaleFactorName("InOutWRte_SF");
        inwrteDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        inwrteDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << inwrteDataPoint;

        SunSpecDataPoint inoutwrteWintmsDataPoint;
        inoutwrteWintmsDataPoint.setName("InOutWRte_WinTms");
        inoutwrteWintmsDataPoint.setLabel("InOutWRte_WinTms");
        inoutwrteWintmsDataPoint.setDescription("Time window for charge/discharge rate change.");
        inoutwrteWintmsDataPoint.setUnits("Secs");
        inoutwrteWintmsDataPoint.setSize(1);
        inoutwrteWintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        inoutwrteWintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << inoutwrteWintmsDataPoint;

        SunSpecDataPoint inoutwrteRvrttmsDataPoint;
        inoutwrteRvrttmsDataPoint.setName("InOutWRte_RvrtTms");
        inoutwrteRvrttmsDataPoint.setLabel("InOutWRte_RvrtTms");
        inoutwrteRvrttmsDataPoint.setDescription("Timeout period for charge/discharge rate.");
        inoutwrteRvrttmsDataPoint.setUnits("Secs");
        inoutwrteRvrttmsDataPoint.setSize(1);
        inoutwrteRvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        inoutwrteRvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << inoutwrteRvrttmsDataPoint;

        SunSpecDataPoint inoutwrteRmptmsDataPoint;
        inoutwrteRmptmsDataPoint.setName("InOutWRte_RmpTms");
        inoutwrteRmptmsDataPoint.setLabel("InOutWRte_RmpTms");
        inoutwrteRmptmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
        inoutwrteRmptmsDataPoint.setUnits("Secs");
        inoutwrteRmptmsDataPoint.setSize(1);
        inoutwrteRmptmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        inoutwrteRmptmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << inoutwrteRmptmsDataPoint;

        SunSpecDataPoint ChaGriSetDataPoint;
        ChaGriSetDataPoint.setName("ChaGriSet");
        ChaGriSetDataPoint.setSize(1);
        ChaGriSetDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        ChaGriSetDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
        m_dataPoints << ChaGriSetDataPoint;

        SunSpecDataPoint wchamaxSfDataPoint;
        wchamaxSfDataPoint.setName("WChaMax_SF");
        wchamaxSfDataPoint.setLabel("WChaMax_SF");
        wchamaxSfDataPoint.setDescription("Scale factor for maximum charge.");
        wchamaxSfDataPoint.setMandatory(true);
        wchamaxSfDataPoint.setSize(1);
        wchamaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << wchamaxSfDataPoint;

        SunSpecDataPoint wchadischagraSfDataPoint;
        wchadischagraSfDataPoint.setName("WChaDisChaGra_SF");
        wchadischagraSfDataPoint.setLabel("WChaDisChaGra_SF");
        wchadischagraSfDataPoint.setDescription("Scale factor for maximum charge and discharge rate.");
        wchadischagraSfDataPoint.setMandatory(true);
        wchadischagraSfDataPoint.setSize(1);
        wchadischagraSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << wchadischagraSfDataPoint;

        SunSpecDataPoint vachamaxSfDataPoint;
        vachamaxSfDataPoint.setName("VAChaMax_SF");
        vachamaxSfDataPoint.setLabel("VAChaMax_SF");
        vachamaxSfDataPoint.setDescription("Scale factor for maximum charging VA.");
        vachamaxSfDataPoint.setSize(1);
        vachamaxSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << vachamaxSfDataPoint;

        SunSpecDataPoint minrsvpctSfDataPoint;
        minrsvpctSfDataPoint.setName("MinRsvPct_SF");
        minrsvpctSfDataPoint.setLabel("MinRsvPct_SF");
        minrsvpctSfDataPoint.setDescription("Scale factor for minimum reserve percentage.");
        minrsvpctSfDataPoint.setSize(1);
        minrsvpctSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << minrsvpctSfDataPoint;

        SunSpecDataPoint chastateSfDataPoint;
        chastateSfDataPoint.setName("ChaState_SF");
        chastateSfDataPoint.setLabel("ChaState_SF");
        chastateSfDataPoint.setDescription("Scale factor for available energy percent.");
        chastateSfDataPoint.setSize(1);
        chastateSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << chastateSfDataPoint;

        SunSpecDataPoint storavalSfDataPoint;
        storavalSfDataPoint.setName("StorAval_SF");
        storavalSfDataPoint.setLabel("StorAval_SF");
        storavalSfDataPoint.setDescription("Scale factor for state of charge.");
        storavalSfDataPoint.setSize(1);
        storavalSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << storavalSfDataPoint;

        SunSpecDataPoint inbatvSfDataPoint;
        inbatvSfDataPoint.setName("InBatV_SF");
        inbatvSfDataPoint.setLabel("InBatV_SF");
        inbatvSfDataPoint.setDescription("Scale factor for battery voltage.");
        inbatvSfDataPoint.setSize(1);
        inbatvSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << inbatvSfDataPoint;

        SunSpecDataPoint inoutwrteSfDataPoint;
        inoutwrteSfDataPoint.setName("InOutWRte_SF");
        inoutwrteSfDataPoint.setLabel("InOutWRte_SF");
        inoutwrteSfDataPoint.setDescription("Scale factor for percent charge/discharge rate.");
        inoutwrteSfDataPoint.setSize(1);
        inoutwrteSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << inoutwrteSfDataPoint;

        break;
    }
    case 801: {
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

        SunSpecDataPoint deprecatedModelDataPoint;
        deprecatedModelDataPoint.setName("DEPRECATED");
        deprecatedModelDataPoint.setLabel("Deprecated Model");
        deprecatedModelDataPoint.setDescription("This model has been deprecated.");
        deprecatedModelDataPoint.setMandatory(true);
        deprecatedModelDataPoint.setSize(1);
        deprecatedModelDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << deprecatedModelDataPoint;

        break;
    }
    default:
        break;
    }
}

