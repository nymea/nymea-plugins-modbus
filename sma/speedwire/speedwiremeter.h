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

#ifndef SPEEDWIREMETER_H
#define SPEEDWIREMETER_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

#include "speedwireinterface.h"

class SpeedwireMeter : public QObject
{
    Q_OBJECT
public:
    explicit SpeedwireMeter(SpeedwireInterface *speedwireInterface, quint16 modelId, quint32 serialNumber, QObject *parent = nullptr);

    bool reachable() const;

    double currentPower() const;
    double totalEnergyProduced() const;
    double totalEnergyConsumed() const;

    double energyConsumedPhaseA() const;
    double energyConsumedPhaseB() const;
    double energyConsumedPhaseC() const;

    double energyProducedPhaseA() const;
    double energyProducedPhaseB() const;
    double energyProducedPhaseC() const;

    double currentPowerPhaseA() const;
    double currentPowerPhaseB() const;
    double currentPowerPhaseC() const;

    double voltagePhaseA() const;
    double voltagePhaseB() const;
    double voltagePhaseC() const;

    double amperePhaseA() const;
    double amperePhaseB() const;
    double amperePhaseC() const;

    QString softwareVersion() const;

signals:
    void reachableChanged(bool reachable);
    void valuesUpdated();

private:
    SpeedwireInterface *m_speedwireInterface = nullptr;
    QHostAddress m_address;
    bool m_initialized = false;
    quint16 m_modelId = 0;
    quint32 m_serialNumber = 0;

    QTimer m_timer;
    bool m_reachable = false;
    qint64 m_lastSeenTimestamp = 0;

    double m_currentPower = 0;
    double m_totalEnergyProduced = 0;
    double m_totalEnergyConsumed = 0;

    double m_energyConsumedPhaseA = 0;
    double m_energyConsumedPhaseB = 0;
    double m_energyConsumedPhaseC = 0;

    double m_energyProducedPhaseA = 0;
    double m_energyProducedPhaseB = 0;
    double m_energyProducedPhaseC = 0;

    double m_currentPowerPhaseA = 0;
    double m_currentPowerPhaseB = 0;
    double m_currentPowerPhaseC = 0;

    double m_voltagePhaseA = 0;
    double m_voltagePhaseB = 0;
    double m_voltagePhaseC = 0;

    double m_amperePhaseA = 0;
    double m_amperePhaseB = 0;
    double m_amperePhaseC = 0;

    QString m_softwareVersion;


private slots:
    void evaluateReachable();
    void processData(const QHostAddress &senderAddress, quint16 senderPort, const QByteArray &data, bool multicast);

};

#endif // SPEEDWIREMETER_H
