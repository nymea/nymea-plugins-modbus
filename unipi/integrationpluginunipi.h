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

#ifndef INTEGRATIONPLUGINUNIPI_H
#define INTEGRATIONPLUGINUNIPI_H

#include "integrations/integrationplugin.h"
#include "plugintimer.h"
#include "unipi.h"
#include "neuron.h"
#include "neuronextension.h"

#include <QTimer>
#include <QtSerialBus>
#include <QHostAddress>
#include <QUuid>

class IntegrationPluginUniPi : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginunipi.json")
    Q_INTERFACES(IntegrationPlugin)

public:

    explicit IntegrationPluginUniPi();
    void init() override;

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;
    void thingRemoved(Thing *thing) override;

private:
    UniPi *m_unipi = nullptr;
    QHash<ThingId, Neuron *> m_neurons;
    QHash<ThingId, NeuronExtension *> m_neuronExtensions;
    QModbusTcpClient *m_modbusTCPMaster = nullptr;
    QModbusRtuSerialMaster *m_modbusRTUMaster = nullptr;

    QHash<Thing *, QTimer *> m_unlatchTimer;
    QTimer *m_reconnectTimer = nullptr;
    QHash<QUuid, ThingActionInfo *> m_asyncActions;
    QHash<ThingClassId, StateTypeId> m_connectionStateTypeIds;

    bool neuronDeviceInit();
    bool neuronExtensionInterfaceInit();

private slots:
    void onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value);

    void onRequestExecuted(const QUuid &requestId, bool success);
    void onRequestError(const QUuid &requestId, const QString &error);

    void onNeuronConnectionStateChanged(bool state);
    void onNeuronDigitalInputStatusChanged(const QString &circuit, bool value);
    void onNeuronDigitalOutputStatusChanged(const QString &circuit, bool value);
    void onNeuronAnalogInputStatusChanged(const QString &circuit, double value);
    void onNeuronAnalogOutputStatusChanged(const QString &circuit,double value);
    void onNeuronUserLEDStatusChanged(const QString &circuit, bool value);

    void onNeuronExtensionConnectionStateChanged(bool state);
    void onNeuronExtensionDigitalInputStatusChanged(const QString &circuit, bool value);
    void onNeuronExtensionDigitalOutputStatusChanged(const QString &circuit, bool value);
    void onNeuronExtensionAnalogInputStatusChanged(const QString &circuit, double value);
    void onNeuronExtensionAnalogOutputStatusChanged(const QString &circuit,double value);
    void onNeuronExtensionUserLEDStatusChanged(const QString &circuit, bool value);

    void onReconnectTimer();

    void onModbusTCPStateChanged(QModbusDevice::State state);
    void onModbusRTUStateChanged(QModbusDevice::State state);

    void onUniPiDigitalInputStatusChanged(const QString &circuit, bool value);
    void onUniPiDigitalOutputStatusChanged(const QString &circuit, bool value);
    void onUniPiAnalogInputStatusChanged(const QString &circuit, double value);
    void onUniPiAnalogOutputStatusChanged(double value);
};

#endif // INTEGRATIONPLUGINUNIPI_H
