// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef WATTSONICDISCOVERY_H
#define WATTSONICDISCOVERY_H

#include <QObject>

#include <hardware/modbus/modbusrtuhardwareresource.h>

#include "wattsonicinverter.h"

class WattsonicDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit WattsonicDiscovery(ModbusRtuHardwareResource *modbusRtuResource, QObject *parent = nullptr);

    struct Result {
        QUuid modbusRtuMasterId;
        QString serialNumber;
        quint16 slaveId;
        WattsonicInverter::Generation generation;
        WattsonicInverter::Info inverterInfo;
    };

    void startDiscovery(quint16 slaveId);

    QList<Result> discoveryResults() const;


signals:
    void discoveryFinished(bool modbusRtuMasterAvailable);

private slots:
    void tryConnect(ModbusRtuMaster *master, quint16 slaveId);

private:
    ModbusRtuHardwareResource *m_modbusRtuResource = nullptr;
    QList<ModbusRtuMaster *> m_candidateMasters;
    QList<ModbusRtuMaster *> m_verifiedMasters;
    QList<Result> m_discoveryResults;
};

#endif // WATTSONICDISCOVERY_H
