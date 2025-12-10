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

#include "sunspecmodelrepeatingblock.h"
#include "sunspecmodel.h"

SunSpecModelRepeatingBlock::SunSpecModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecModel *parent) :
    QObject(parent),
    m_blockIndex(blockIndex),
    m_blockSize(blockSize),
    m_modbusStartRegister(modbusStartRegister)
{

}

quint16 SunSpecModelRepeatingBlock::blockIndex() const
{
    return m_blockIndex;
}

quint16 SunSpecModelRepeatingBlock::blockSize() const
{
    return m_blockSize;
}

QDebug operator<<(QDebug debug, SunSpecModelRepeatingBlock *repeatingBlock)
{
    debug.nospace().noquote() << "SunSpecModelRepeatingBlock(Name: " << repeatingBlock->name();
    debug.nospace().noquote() << ", Index: " << repeatingBlock->blockIndex() << ", Length: " << repeatingBlock->blockSize() << ")";
    return debug.space().quote();
}
