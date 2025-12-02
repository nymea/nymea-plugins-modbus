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

#ifndef SUNSPECENERGYSTORAGEBASEMODELDEPRECATEDMODEL_H
#define SUNSPECENERGYSTORAGEBASEMODELDEPRECATEDMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecEnergyStorageBaseModelDeprecatedModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecEnergyStorageBaseModelDeprecatedModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecEnergyStorageBaseModelDeprecatedModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* This model has been deprecated. */
    quint16 deprecatedModel() const;

protected:
    quint16 m_fixedBlockLength = 1;

    void initDataPoints();
    void processBlockData() override;

private:
    quint16 m_deprecatedModel = 0;


};

QDebug operator<<(QDebug debug, SunSpecEnergyStorageBaseModelDeprecatedModel *model);

#endif // SUNSPECENERGYSTORAGEBASEMODELDEPRECATEDMODEL_H
