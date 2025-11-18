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

#ifndef SUNSPECCOMMONMODEL_H
#define SUNSPECCOMMONMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecCommonModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecCommonModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecCommonModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Well known value registered with SunSpec for compliance */
    QString manufacturer() const;
    /* Manufacturer specific value (32 chars) */
    QString model() const;
    /* Manufacturer specific value (16 chars) */
    QString options() const;
    /* Manufacturer specific value (16 chars) */
    QString version() const;
    /* Manufacturer specific value (32 chars) */
    QString serialNumber() const;

    /* Modbus device address */
    quint16 deviceAddress() const;
    QModbusReply *setDeviceAddress(quint16 deviceAddress);

    /* Force even alignment */
    quint16 pad() const;

protected:
    quint16 m_fixedBlockLength = 66;

    void initDataPoints();
    void processBlockData() override;

private:
    QString m_manufacturer;
    QString m_model;
    QString m_options;
    QString m_version;
    QString m_serialNumber;
    quint16 m_deviceAddress = 0;
    quint16 m_pad = 0;


};

QDebug operator<<(QDebug debug, SunSpecCommonModel *model);

#endif // SUNSPECCOMMONMODEL_H
