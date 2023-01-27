/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2023, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
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

#ifndef MODBUSDEGISTERDEFINITION
#define MODBUSDEGISTERDEFINITION

#endif // MODBUSDEGISTERDEFINITION

#include <QObject>

enum ModbusRegisterX2 {
    AbsenkungderLuefterstufe1 = 5328,
    AktiveLuefterstufe = 1066,
    AnforderungDerVentilatorenDurchWaermepumpe = 1292,
    AnforderungDerVentilatorenDurchZonenregelung = 1336,
    AnforderungDisbalance = 1338,
    AnforderungBeschattung = 1218,
    AnforderungBrauchwasserheizungElektoheizstab = 1038,
    AnforderungBrauchwasserheizungWaermepumpe = 1036,
    AnforderungBypassklappe = 1216,
    AnforderungFrostschutz = 1146,
    AnforderungLST3EXT = 228,
    AnforderungRaumHeizstufe1 = 1032,
    AnforderungRaumHeizstufe2 = 1034,
    AnforderungSoleFuerAuszenlufterwaermung = 1198,
    AnforderungSoleFuerWaermepumpe = 1196,
    AnforderungSoleKuehlung = 1320,
    Auszenluft = 202,
    AuszenluftfuehlerVorhanden = 5304 ,
    Beschattungsfunktion = 5336,
    Betriebsart = 5002,
    BetriebsstundenAbluftventilator = 902,
    BetriebsstundenBeschattung = 964,
    BetriebsstundenBypassklappe = 932,
    BetriebsstundenFeinstaub = 928,
    BetriebsstundenFrostschutzeinrichtung = 940,
    BetriebsstundenGrobstaub = 926,
    BetriebsstundenHeizstufe1 = 912,
    BetriebsstundenHeizstufe2 = 914,
    BetriebsstundenHeizung = 938,
    BetriebsstundenLuefterstufe0 = 962,
    BetriebsstundenLuefterstufe1 = 904,
    BetriebsstundenLuefterstufe2 = 906,
    BetriebsstundenLuefterstufe3 = 908,
    BetriebsstundenReduzierteLuefterstufe = 944,
    BetriebsstundenSoleKreis = 934,
    BetriebsstundenSoleKreisAuszenluft = 970,
    BetriebsstundenZuluftventilator = 900,
    Brandmeldealarm = 838,
    BrandmeldeanlageVorhanden = 5068,
    CO2 = 230,
    CO2Messung = 1048,
    CO2SensorVorhanden = 5054,
    Datum = 5210,
    DrehzahlAbluftventilator = 1094,
    DrehzahlZuluftventilator = 1092,
    ErhoehungDerLuefterstufe3 = 5330,
    FeinstaubfilterVorhanden = 5034,
    FeinstaubfilterWechseln = 7004,
    FeinstaubfilterStandzeit = 5032,
    FunktionHeizungPlus = 5492,
    Geraetetyp = 5000,
    GesamtBefoerderteKubikmeter = 946,
    GesamtBefoerderteLuftmengeSeitFilterwechsel = 960,
    GrobstaubfilterVorhanden = 5154,
    GrobstaubfilterWechseln = 7002,
    GrobstaubfilterBetriebsartFiltereberwachung = 5164,
    GrobstaubfilterMaximalesFoerdervolumen = 5166,
    GrobstaubfilterStandzeit = 5030,
    IstDrehzahlAbluftventilator = 1186,
    IstDrehzahlZuluftventilator = 1184,
    KontaktBrandmeldealarm = 238,
    KuehlungVorhanden = 5192,
    MaximalZulaessigeDrehzahlAbluftventilator = 5270,
    MaximalZulaessigeDrehzahlZuluftventilator = 5268,
    PelletofenVorhanden = 5168,
    PelletofenAnlaufverzoegerung = 5282,
    PelletofenMindestlaufzeit = 5284,
    Raum = 200,
    RaumSoll = 5016,
    RaumtemperaturBeschattung = 5338,
    RelaiskontaktEXT = 252,
    Revisionstuere = 226,
    SollVolumenstromAbluft = 1084,
    SollVolumenstromLuefterstufe = 5060,
    SollVolumenstromZuluft = 1082,
    SollwertErhoehungFunktionHeizungPlus = 5496,
    StoerungAbluftventilator = 826,
    StoerungBoilerfuehlerElektroheizstab = 828,
    StoerungBoilerfuehlerSolar = 844,
    StoerungBoilerfuehlerWaermepumpe = 830,
    StoerungBoileruebertemperatur = 810,
    StoerungCO2Sensor = 832,
    StoerungDruckverlustAbluftZuGrosz = 854,
    StoerungDruckverlustZuluftZuGrosz = 852,
    StoerungDurchflussmengeHeizgkreis = 848,
    StoerungDurchflussmengeSolekreis = 846,
    StoerungTeilnehmerNichtErreichbar = 856,
    StoerungTemperaturfuehlerAuszenluft = 806,
    StoerungTemperaturfuehlerHeizkreisVorlauf = 850,
    StoerungTemperaturfuehlerRaum = 804,
    StoerungTemperaturfuehlerSolarkollektor = 842,
    StoerungTemperaturfuehlerSole = 812,
    StoerungTemperaturfuehlerSoleAuszenluft = 7504,
    StoerungWaermepumpeHochdruck = 818,
    StoerungWaermepumpeNiederdruck = 820,
    StoerungWertNichtZulaessig = 840,
    StoerungZuluftventilator = 824,
    Summenstoerung = 800,
    Summenstoerung2 = 7500,
    SoftwareVersion = 1156,
    Tag = 1174,
    TemperaturAuszenluftBeschattung = 5340,
    TemperaturAuszenluftBypassKuehlungAus = 5452,
    TemperaturAuszenluftBypassKuehlungEin = 5450,
    TemperaturAuszenluftBypassOeffnen = 5084,
    TemperaturAuszenluftBypassSchlieszen = 5086,
    TemperaturAuszenluftFrostschutz= 5206,
    TemperaturAuszenluftFrostschutzAus = 5090,
    TemperaturAuszenluftFrostschutzEin = 5088,
    TemperaturAuszenluftKuehlungAus = 5200,
    TemperaturAuszenluftKuelungEin = 5198,
    TemperaturAuszenluftReduktionLuftmenge10Prozent = 5422,
    TemperaturAuszenluftReduktionLuftmenge20Prozent = 5424,
    TemperaturSole = 216,
    TemperaturSoleNachAuszenluftvorwaermung = 206,
    TemperaturschwelleAuszenluftHeizenKuehlen = 5186,
    TimeOutSolltemperaturRaum = 7508,
    TimeOutTemperaturfuehlerAuszenluft = 7510,
    TimeoutTemperaturfuehlerHeizkreisVorlauf = 7512,
    TimeoutTemperaturfuehlerRaum = 7506,
    Uhrzeit = 5212,
    UhrzeitundDatumVerschicken = 5442,
    VolumenstrombalanceZuluftAbluft = 5026,
    VorlaufNiedertemperaturHeizkreis = 240,
    ZeitspanneFunktionHeizung =  5494,
    ZeitspanneFunktionParty = 5038,
    HochdruckWaermepumpe = 222,
    NiederdruckWaermepumpe = 224, //nicht vorhanden, vorhanden
    HystereseSolareRaumheizungAus = 5302,
    KontaktEVU = 232,
    MaximalZulaessigeVorlauftemperaturHeizung = 5202,
    MinimalZulaessigeRuecklauftemperaturPassiveKuehlung = 5204,
    MinimaleBrauchwassertemperaturSolareRaumheizungEin = 5300,
    RaumtemperaturPassiveKuehlungEin = 5182,
    RelaiskontaktExt = 262,
    SolaranlageVorhanden = 5190,
    SolaranlageAusschaltschwelleSolarpumpe = 5180,
    SolaranlageEinschaltschwelleSolarpumpe  =5178,
    SolaranlageMaximaleLadetemperatur = 5176,
    SolareRaumheizungVorhanden = 5306,
    StatusWaermepumpe = 1314,
    StatusWaermepumpeRestzeit = 1316, // in seconds
    EVUAnlageVorhanden = 5146,
    EVUSperreBrauchwasserAktiv = 1270,
    EVUSperreElektroheizstabAktiv = 1274,
    EVUSperreRaumheizungAktiv = 1272,
    BrauchwasserSolltermperatur = 5064,
    BrauchwassertemperaturRaumheizungssperre = 5130,
    FunktionBadPlus = 5036,
    ElektroheizstabVorhanden = 5126,
    EnergieKompressor = 4500,
    EnergieLuftvorerwarrmung = 4506,
    EnergieRaumheizung = 4502,
    EnergieWarmwasser = 4504,
    LeistungKompressor = 4000,
    LeistungLuftvorwaermung = 4006 ,
    LeistungRaumheizung = 4002,
    LeistungWarmwasser = 4004,
    TemperaturBoilerSolar = 244,
    TemperaturSolarkollektor = 242,
    TemperaturWarmwasserspeicherOben = 212,
    TemperaturWarmwasserspeicherUnten = 214,
    Waermepumpe = 1044,
    ZentralgeraetAdresse = 5436
};


enum DeviceType {
    NotDefined,
    AerosilentPrimus = 1,
    AerosilentTopo,
    AerosilentMicro,
    AerosmartS,
    AerosmartM,
    AerosmartL,
    AerosmartXls,
    AerosilentCentro,
    TermosmartSc,
    X2_LU = 25,
    X2_WP = 26,
    AerosmartMono,
    Vbox120,
    AerosilentBianco,
    X2Plus,
    AerosilentBusiness,
    CentralDevice,
    AerosilentStratos,
    ZoneControl,
    Vbox300
};

enum HeatPumpStatus {
    Wiedereinschaltsperre,
    Waermepumpeaus,
    VorlaufzeitSolepumpe,
    VorlaufzeitVentilatoren,
    OeffnenHeissgasventil,
    OeffnenFluessiggasventil,
    StartKompressor,
    MindestlaufzeitWaermepumpe,
    WaermepumpeEin,
    KaeltemittleAbsaugen,
    BeimAbtauenAbsaugen,
    AbtauenEin,
    NachAbtauenAbtropfen
};

enum VentilationMode {
    ManuellStufe0 = 0,
    ManuellStufe1,
    ManuellStufe2,
    ManuellStufe3,
    Automatikbetrieb,
    Party
};
