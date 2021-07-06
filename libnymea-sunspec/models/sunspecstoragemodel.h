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

    explicit SunSpecStorageModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent = nullptr);
    ~SunSpecStorageModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    float wChaMax() const;
    QModbusReply *setWChaMax(float wChaMax);

    float wChaGra() const;
    QModbusReply *setWChaGra(float wChaGra);

    float wDisChaGra() const;
    QModbusReply *setWDisChaGra(float wDisChaGra);

    Storctl_modFlags storCtlMod() const;
    QModbusReply *setStorCtlMod(Storctl_modFlags storCtlMod);

    float vaChaMax() const;
    QModbusReply *setVaChaMax(float vaChaMax);

    float minRsvPct() const;
    QModbusReply *setMinRsvPct(float minRsvPct);

    float chaState() const;
    float storAval() const;
    float inBatV() const;
    Chast chaSt() const;
    float outWRte() const;
    QModbusReply *setOutWRte(float outWRte);

    float inWRte() const;
    QModbusReply *setInWRte(float inWRte);

    quint16 inOutWRteWinTms() const;
    QModbusReply *setInOutWRteWinTms(quint16 inOutWRteWinTms);

    quint16 inOutWRteRvrtTms() const;
    QModbusReply *setInOutWRteRvrtTms(quint16 inOutWRteRvrtTms);

    quint16 inOutWRteRmpTms() const;
    QModbusReply *setInOutWRteRmpTms(quint16 inOutWRteRmpTms);

    Chagriset chaGriSet() const;
    QModbusReply *setChaGriSet(Chagriset chaGriSet);


protected:
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

    void initDataPoints();

};

QDebug operator<<(QDebug debug, SunSpecStorageModel *model);

#endif // SUNSPECSTORAGEMODEL_H
