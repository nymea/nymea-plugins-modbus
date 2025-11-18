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

#ifndef NEURON_H
#define NEURON_H

#include "neuroncommon.h"

#include <QObject>
#include <QHash>
#include <QTimer>
#include <QUuid>

class Neuron : public NeuronCommon
{
    Q_OBJECT
public:

    enum NeuronTypes {
        S103,
        M103,
        M203,
        M303,
        M403,
        M503,
        M523,
        L203,
        L303,
        L403,
        L503,
        L513,
        L523,
        L533
    };
    Q_ENUM(NeuronTypes)

    explicit Neuron(NeuronTypes neuronType, QModbusClient *modbusInterface, QObject *parent = nullptr);
    ~Neuron();
    QString type();

private:
    NeuronTypes m_neuronType = NeuronTypes::S103;
    bool loadModbusMap() override;
};
#endif // NEURON_H
