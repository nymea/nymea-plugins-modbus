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

#ifndef SUNSPECFLOWBATTERYMODULEMODEL_H
#define SUNSPECFLOWBATTERYMODULEMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecFlowBatteryModuleModel;

class SunSpecFlowBatteryModuleModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    explicit SunSpecFlowBatteryModuleModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecFlowBatteryModuleModel *parent = nullptr);
    ~SunSpecFlowBatteryModuleModelRepeatingBlock() override = default;

    SunSpecFlowBatteryModuleModel *parentModel() const;

    QString name() const override;
    /* Stack Points To Be Determined */
    quint16 stackPointsToBeDetermined() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecFlowBatteryModuleModel *m_parentModel = nullptr;

    quint16 m_stackPointsToBeDetermined = 0;

};



class SunSpecFlowBatteryModuleModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecFlowBatteryModuleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecFlowBatteryModuleModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Module Points To Be Determined */
    quint16 modulePointsToBeDetermined() const;

protected:
    quint16 m_fixedBlockLength = 1;
    quint16 m_repeatingBlockLength = 1;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    quint16 m_modulePointsToBeDetermined = 0;


};

QDebug operator<<(QDebug debug, SunSpecFlowBatteryModuleModel *model);

#endif // SUNSPECFLOWBATTERYMODULEMODEL_H
