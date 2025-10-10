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

#ifndef SUNSPECSTORAGEMODEL_H
#define SUNSPECSTORAGEMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecStorageModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Chast {
        ChastOff = 1,
        ChastEmpty = 2,
        ChastDischarging = 3,
        ChastCharging = 4,
        ChastFull = 5,
        ChastHolding = 6,
        ChastTesting = 7
    };
    Q_ENUM(Chast)

    enum Chagriset {
        ChagrisetPv = 0,
        ChagrisetGrid = 1
    };
    Q_ENUM(Chagriset)

    enum Storctl_mod {
        Storctl_modCharge = 0x1,
        Storctl_modDiScharge = 0x2
    };
    Q_DECLARE_FLAGS(Storctl_modFlags, Storctl_mod)
    Q_FLAG(Storctl_mod)

    explicit SunSpecStorageModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecStorageModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Setpoint for maximum charge. [W] */
    float wChaMax() const;
    QModbusReply *setWChaMax(float wChaMax);

    /* Setpoint for maximum charging rate. Default is MaxChaRte. [% WChaMax/sec] */
    float wChaGra() const;
    QModbusReply *setWChaGra(float wChaGra);

    /* Setpoint for maximum discharge rate. Default is MaxDisChaRte. [% WChaMax/sec] */
    float wDisChaGra() const;
    QModbusReply *setWDisChaGra(float wDisChaGra);

    /* Activate hold/discharge/charge storage control mode. Bitfield value. */
    Storctl_modFlags storCtlMod() const;
    QModbusReply *setStorCtlMod(Storctl_modFlags storCtlMod);

    /* Setpoint for maximum charging VA. [VA] */
    float vaChaMax() const;
    QModbusReply *setVaChaMax(float vaChaMax);

    /* Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage. [% WChaMax] */
    float minRsvPct() const;
    QModbusReply *setMinRsvPct(float minRsvPct);

    /* Currently available energy as a percent of the capacity rating. [% AhrRtg] */
    float chaState() const;
    /* State of charge (ChaState) minus storage reserve (MinRsvPct) times capacity rating (AhrRtg). [AH] */
    float storAval() const;
    /* Internal battery voltage. [V] */
    float inBatV() const;
    /* Charge status of storage device. Enumerated value. */
    Chast chaSt() const;
    /* Percent of max discharge rate. [% WDisChaMax] */
    float outWRte() const;
    QModbusReply *setOutWRte(float outWRte);

    /* Percent of max charging rate. [ % WChaMax] */
    float inWRte() const;
    QModbusReply *setInWRte(float inWRte);

    /* Time window for charge/discharge rate change. [Secs] */
    quint16 inOutWRteWinTms() const;
    QModbusReply *setInOutWRteWinTms(quint16 inOutWRteWinTms);

    /* Timeout period for charge/discharge rate. [Secs] */
    quint16 inOutWRteRvrtTms() const;
    QModbusReply *setInOutWRteRvrtTms(quint16 inOutWRteRvrtTms);

    /* Ramp time for moving from current setpoint to new setpoint. [Secs] */
    quint16 inOutWRteRmpTms() const;
    QModbusReply *setInOutWRteRmpTms(quint16 inOutWRteRmpTms);

    Chagriset chaGriSet() const;
    QModbusReply *setChaGriSet(Chagriset chaGriSet);

    /* Scale factor for maximum charge. */
    qint16 wChaMaxSf() const;
    /* Scale factor for maximum charge and discharge rate. */
    qint16 wChaDisChaGraSf() const;
    /* Scale factor for maximum charging VA. */
    qint16 vaChaMaxSf() const;
    /* Scale factor for minimum reserve percentage. */
    qint16 minRsvPctSf() const;
    /* Scale factor for available energy percent. */
    qint16 chaStateSf() const;
    /* Scale factor for state of charge. */
    qint16 storAvalSf() const;
    /* Scale factor for battery voltage. */
    qint16 inBatVSf() const;
    /* Scale factor for percent charge/discharge rate. */
    qint16 inOutWRteSf() const;

protected:
    quint16 m_fixedBlockLength = 24;

    void initDataPoints();
    void processBlockData() override;

private:
    float m_wChaMax = 0;
    float m_wChaGra = 0;
    float m_wDisChaGra = 0;
    Storctl_modFlags m_storCtlMod;
    float m_vaChaMax = 0;
    float m_minRsvPct = 0;
    float m_chaState = 0;
    float m_storAval = 0;
    float m_inBatV = 0;
    Chast m_chaSt;
    float m_outWRte = 0;
    float m_inWRte = 0;
    quint16 m_inOutWRteWinTms = 0;
    quint16 m_inOutWRteRvrtTms = 0;
    quint16 m_inOutWRteRmpTms = 0;
    Chagriset m_chaGriSet;
    qint16 m_wChaMaxSf = 0;
    qint16 m_wChaDisChaGraSf = 0;
    qint16 m_vaChaMaxSf = 0;
    qint16 m_minRsvPctSf = 0;
    qint16 m_chaStateSf = 0;
    qint16 m_storAvalSf = 0;
    qint16 m_inBatVSf = 0;
    qint16 m_inOutWRteSf = 0;


};

QDebug operator<<(QDebug debug, SunSpecStorageModel *model);

#endif // SUNSPECSTORAGEMODEL_H
