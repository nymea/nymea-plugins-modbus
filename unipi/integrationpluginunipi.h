/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2024, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef INTEGRATIONPLUGINUNIPI_H
#define INTEGRATIONPLUGINUNIPI_H

#include "integrations/integrationplugin.h"
#include "plugintimer.h"

#include "unipi/unipi.h"
#include "neuron/neuron.h"
#include "neuronextension/neuronextension.h"
#include "neuronextension/neuronextensionbus.h"

#include <QTimer>
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
    Neuron *m_neuron = nullptr;
    QHash<ThingId, NeuronExtension *> m_neuronExtensions;
    QHash<QUuid, NeuronExtensionBus *> m_neuronExtensionBus;

    QTimer *m_reconnectTimer = nullptr;
    QHash<QUuid, ThingActionInfo *> m_asyncActions;
    QHash<ThingClassId, StateTypeId> m_connectionStateTypeIds;
    QHash<ThingClassId, QString> m_neuronMigration;
    QHash<ThingClassId, QString> m_neuronExtensionMigration;

private slots:
    void onNeuronRelayOutputStatusChanged(const QString &circuit, bool value);
    void onNeuronDigitalInputStatusChanged(const QString &circuit, bool value);
    void onNeuronDigitalOutputStatusChanged(const QString &circuit, bool value);
    void onNeuronAnalogInputStatusChanged(const QString &circuit, double value);
    void onNeuronAnalogOutputStatusChanged(const QString &circuit, double value);

    void onNeuronExtensionConnectionStateChanged(bool state);

    void onNeuronExtensionDigitalInputStatusChanged(const QString &circuit, bool value);
    void onNeuronExtensionDigitalOutputStatusChanged(const QString &circuit, bool value);
    void onNeuronExtensionRelayOutputStatusChanged(const QString &circuit, bool value);
    void onNeuronExtensionAnalogInputStatusChanged(const QString &circuit, double value);
    void onNeuronExtensionAnalogOutputStatusChanged(const QString &circuit,double value);

    void onUniPiDigitalInputStatusChanged(const QString &circuit, bool value);
    void onUniPiDigitalOutputStatusChanged(const QString &circuit, bool value);
    void onUniPiAnalogInputStatusChanged(const QString &circuit, double value);
    void onUniPiAnalogOutputStatusChanged(double value);
};

#endif // INTEGRATIONPLUGINUNIPI_H
