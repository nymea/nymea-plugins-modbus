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

#ifndef SUNSPECIRRADIANCEMODEL_H
#define SUNSPECIRRADIANCEMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecIrradianceModel;

class SunSpecIrradianceModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    explicit SunSpecIrradianceModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecIrradianceModel *parent = nullptr);
    ~SunSpecIrradianceModelRepeatingBlock() override = default;

    SunSpecIrradianceModel *parentModel() const;

    QString name() const override;
    /* Global Horizontal Irradiance [W/m2] */
    quint16 ghi() const;
    /* Plane-of-Array Irradiance [W/m2] */
    quint16 poai() const;
    /* Diffuse Irradiance [W/m2] */
    quint16 dfi() const;
    /* Direct Normal Irradiance [W/m2] */
    quint16 dni() const;
    /* Other Irradiance [W/m2] */
    quint16 oti() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecIrradianceModel *m_parentModel = nullptr;

    quint16 m_ghi = 0;
    quint16 m_poai = 0;
    quint16 m_dfi = 0;
    quint16 m_dni = 0;
    quint16 m_oti = 0;

};



class SunSpecIrradianceModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecIrradianceModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecIrradianceModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


protected:
    quint16 m_fixedBlockLength = 0;
    quint16 m_repeatingBlockLength = 5;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();



};

QDebug operator<<(QDebug debug, SunSpecIrradianceModel *model);

#endif // SUNSPECIRRADIANCEMODEL_H
