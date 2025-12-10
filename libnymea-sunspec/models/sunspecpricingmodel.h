// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-sunspec.
*
* libnymea-sunspec is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-sunspec is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-sunspec. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SUNSPECPRICINGMODEL_H
#define SUNSPECPRICINGMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecPricingModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Sigtype {
        SigtypeUnknown = 0,
        SigtypeAbsolute = 1,
        SigtypeRelative = 2,
        SigtypeMultiplier = 3,
        SigtypeLevel = 4
    };
    Q_ENUM(Sigtype)

    enum Modena {
        ModenaEnable = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecPricingModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecPricingModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Is price-based charge/discharge mode active? */
    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    /* Meaning of the pricing signal. When a Price schedule is used, type must match the schedule range variable description. */
    Sigtype sigType() const;
    QModbusReply *setSigType(Sigtype sigType);

    /* Utility/ESP specific pricing signal. Content depends on pricing signal type. When H/M/L type is specified. Low=0; Med=1; High=2. */
    float sig() const;
    QModbusReply *setSig(float sig);

    /* Time window for charge/discharge pricing change. [Secs] */
    quint16 winTms() const;
    QModbusReply *setWinTms(quint16 winTms);

    /* Timeout period for charge/discharge pricing change. [Secs] */
    quint16 rvtTms() const;
    QModbusReply *setRvtTms(quint16 rvtTms);

    /* Ramp time for moving from current charge or discharge level to new level. [Secs] */
    quint16 rmpTms() const;
    QModbusReply *setRmpTms(quint16 rmpTms);

    /* Pricing signal scale factor. */
    qint16 sigSf() const;
    quint16 pad() const;

protected:
    quint16 m_fixedBlockLength = 8;

    void initDataPoints();
    void processBlockData() override;

private:
    ModenaFlags m_modEna;
    Sigtype m_sigType;
    float m_sig = 0;
    quint16 m_winTms = 0;
    quint16 m_rvtTms = 0;
    quint16 m_rmpTms = 0;
    qint16 m_sigSf = 0;
    quint16 m_pad = 0;


};

QDebug operator<<(QDebug debug, SunSpecPricingModel *model);

#endif // SUNSPECPRICINGMODEL_H
