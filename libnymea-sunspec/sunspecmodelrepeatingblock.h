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

#ifndef SUNSPECMODELREPEATINGBLOCK_H
#define SUNSPECMODELREPEATINGBLOCK_H

#include <QObject>

#include "sunspecdatapoint.h"

class SunSpecModel;

class SunSpecModelRepeatingBlock : public QObject
{
    Q_OBJECT

    friend class SunSpecModel;

public:
    explicit SunSpecModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecModel *parent = nullptr);
    virtual ~SunSpecModelRepeatingBlock() = default;

    virtual QString name() const = 0;

    quint16 blockIndex() const;
    quint16 blockSize() const;

    virtual void processBlockData() = 0;

protected:
    quint16 m_blockIndex;
    quint16 m_blockSize;
    quint16 m_modbusStartRegister;

    SunSpecDataPoint::ByteOrder m_byteOrder = SunSpecDataPoint::ByteOrderLittleEndian;
    QHash<QString, SunSpecDataPoint> m_dataPoints;

    virtual void initDataPoints() = 0;

};

QDebug operator<<(QDebug debug, SunSpecModelRepeatingBlock *repeatingBlock);


#endif // SUNSPECMODELREPEATINGBLOCK_H
