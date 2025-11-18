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

#ifndef SUNSPECEXTSETTINGSMODEL_H
#define SUNSPECEXTSETTINGSMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecExtSettingsModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecExtSettingsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecExtSettingsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Ramp up rate as a percentage of max current. [Pct] */
    float rampUpRate() const;
    QModbusReply *setRampUpRate(float rampUpRate);

    /* Ramp down rate as a percentage of max current. [Pct] */
    float nomRmpDnRte() const;
    QModbusReply *setNomRmpDnRte(float nomRmpDnRte);

    /* Emergency ramp up rate as a percentage of max current. [Pct] */
    float emergencyRampUpRate() const;
    QModbusReply *setEmergencyRampUpRate(float emergencyRampUpRate);

    /* Emergency ramp down rate as a percentage of max current. [Pct] */
    float emergencyRampDownRate() const;
    QModbusReply *setEmergencyRampDownRate(float emergencyRampDownRate);

    /* Connect ramp up rate as a percentage of max current. [Pct] */
    float connectRampUpRate() const;
    QModbusReply *setConnectRampUpRate(float connectRampUpRate);

    /* Connect ramp down rate as a percentage of max current. [Pct] */
    float connectRampDownRate() const;
    QModbusReply *setConnectRampDownRate(float connectRampDownRate);

    /* Ramp rate specified in percent of max current. [Pct] */
    float defaultRampRate() const;
    QModbusReply *setDefaultRampRate(float defaultRampRate);

    /* Ramp Rate Scale Factor */
    qint16 rampRateScaleFactor() const;

protected:
    quint16 m_fixedBlockLength = 8;

    void initDataPoints();
    void processBlockData() override;

private:
    float m_rampUpRate = 0;
    float m_nomRmpDnRte = 0;
    float m_emergencyRampUpRate = 0;
    float m_emergencyRampDownRate = 0;
    float m_connectRampUpRate = 0;
    float m_connectRampDownRate = 0;
    float m_defaultRampRate = 0;
    qint16 m_rampRateScaleFactor = 0;


};

QDebug operator<<(QDebug debug, SunSpecExtSettingsModel *model);

#endif // SUNSPECEXTSETTINGSMODEL_H
