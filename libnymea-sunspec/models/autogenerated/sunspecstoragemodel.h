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

#include "sunspec.h"
#include "sunspecmodel.h"

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

    explicit SunSpecStorageModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecStorageModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    float wChaMax() const;
    float wChaGra() const;
    float wDisChaGra() const;
    Storctl_modFlags storCtlMod() const;
    float vaChaMax() const;
    float minRsvPct() const;
    float chaState() const;
    float storAval() const;
    float inBatV() const;
    Chast chaSt() const;
    qint16 outWRte() const;
    qint16 inWRte() const;
    quint16 inOutWRteWinTms() const;
    quint16 inOutWRteRvrtTms() const;
    quint16 inOutWRteRmpTms() const;
    Chagriset chaGriSet() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    float m_wChaMax;
    float m_wChaGra;
    float m_wDisChaGra;
    Storctl_modFlags m_storCtlMod;
    float m_vaChaMax;
    float m_minRsvPct;
    float m_chaState;
    float m_storAval;
    float m_inBatV;
    Chast m_chaSt;
    qint16 m_outWRte;
    qint16 m_inWRte;
    quint16 m_inOutWRteWinTms;
    quint16 m_inOutWRteRvrtTms;
    quint16 m_inOutWRteRmpTms;
    Chagriset m_chaGriSet;
    qint16 m_wChaMaxSf;
    qint16 m_wChaDisChaGraSf;
    qint16 m_vaChaMaxSf;
    qint16 m_minRsvPctSf;
    qint16 m_chaStateSf;
    qint16 m_storAvalSf;
    qint16 m_inBatVSf;
    qint16 m_inOutWRteSf;

    void initDataPoints();

};

#endif // SUNSPECSTORAGEMODEL_H
