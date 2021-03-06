/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
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

#include "integrationpluginunipi.h"
#include "plugininfo.h"
#include "hardware/i2c/i2cmanager.h"
#include "hardwaremanager.h"
#include "hardware/modbus/modbusrtumaster.h"
#include "hardware/modbus/modbusrtuhardwareresource.h"

#include <QJsonDocument>
#include <QTimer>
#include <QSerialPort>

IntegrationPluginUniPi::IntegrationPluginUniPi()
{
}


void IntegrationPluginUniPi::init()
{
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = false"));

    m_connectionStateTypeIds.insert(uniPi1ThingClassId, uniPi1ConnectedStateTypeId);
    m_connectionStateTypeIds.insert(uniPi1LiteThingClassId, uniPi1LiteConnectedStateTypeId);
    m_connectionStateTypeIds.insert(neuronThingClassId, neuronConnectedStateTypeId);
    m_connectionStateTypeIds.insert(neuronExtensionThingClassId, neuronExtensionConnectedStateTypeId);

    m_neuronMigration.insert(ThingClassId("c28dc232-6797-4aff-83dd-8d811d485482"), "S103");
    m_neuronMigration.insert(ThingClassId("58632506-8d06-4ad0-98d0-275ef8a88955"), "M103");
    m_neuronMigration.insert(ThingClassId("d254dbed-4cf0-4fe3-96da-9adfd8d1c912"), "M203");
    m_neuronMigration.insert(ThingClassId("6ee92fd5-c169-4050-aa3c-d8289bee20ec"), "M303");
    m_neuronMigration.insert(ThingClassId("0621f5f1-8531-4d44-800e-52f926c58b17"), "M403");
    m_neuronMigration.insert(ThingClassId("0d5a09de-daf8-469f-803c-520f763167df"), "M503");
    m_neuronMigration.insert(ThingClassId("366bac45-c602-4c45-9812-1d3d04b2c3db"), "L203");
    m_neuronMigration.insert(ThingClassId("fc1a9aab-8573-42fe-8ed4-4876795870c2"), "L303");
    m_neuronMigration.insert(ThingClassId("72637166-6730-4b2c-9b5d-4d2066c2fea8"), "L403");
    m_neuronMigration.insert(ThingClassId("58d33a7f-30ac-4d79-94a1-2013dc50743e"), "L503");
    m_neuronMigration.insert(ThingClassId("4b57e164-5f62-4518-afc1-15a659a2c402"), "L513");

    m_neuronExtensionMigration.insert(ThingClassId("9bfe46d0-5dbd-432c-877f-1ff47faf6e17"), "xS10");
    m_neuronExtensionMigration.insert(ThingClassId("a1ec57b1-fcf9-4540-80c9-40f78cddc85f"), "xS30");
    m_neuronExtensionMigration.insert(ThingClassId("05c78946-48f4-4e1b-9d45-90fbd66c71c0"), "xS40");
    m_neuronExtensionMigration.insert(ThingClassId("c31f5e8c-a27c-49db-b5a8-dd065336b79a"), "xS50");
    m_neuronExtensionMigration.insert(ThingClassId("f59bfba2-0455-49f2-b92d-badfec5dcc01"), "xS11");
    m_neuronExtensionMigration.insert(ThingClassId("0e11fbd3-8d4a-4fd8-aeeb-25ee2d134a17"), "xS51");
}

void IntegrationPluginUniPi::discoverThings(ThingDiscoveryInfo *info)
{
    ThingClassId thingClassId = info->thingClassId();

    if (thingClassId == digitalInputThingClassId) {
        qCDebug(dcUniPi()) << "Discovering digital inputs";
        foreach(Thing *parent, myThings()) {
            if (((parent->thingClassId() == uniPi1ThingClassId) || (parent->thingClassId() == uniPi1LiteThingClassId)) && m_unipi) {
                foreach (QString circuit, m_unipi->digitalInputs()) {
                    ThingDescriptor thingDescriptor(digitalInputThingClassId, QString("Digital input %1").arg(circuit), "UniPi 1", parent->id());
                    foreach(Thing *thing, myThings().filterByParentId(parent->id())) {
                        if (thing->paramValue(digitalInputThingCircuitParamTypeId) == circuit) {
                            qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit;
                            thingDescriptor.setThingId(thing->id());
                            break;
                        }
                    }
                    ParamList params;
                    params.append(Param(digitalInputThingCircuitParamTypeId, circuit));
                    thingDescriptor.setParams(params);
                    info->addThingDescriptor(thingDescriptor);
                }
                break;
            }
        }

        foreach (NeuronExtension *neuronExtension, m_neuronExtensions) {
            ThingId parentId = m_neuronExtensions.key(neuronExtension);
            foreach (QString circuit, neuronExtension->digitalInputs()) {
                ThingDescriptor ThingDescriptor(digitalInputThingClassId, QString("Digital input %1").arg(circuit), QString("Neuron extension %1, slave address %2").arg(neuronExtension->type()).arg(QString::number(neuronExtension->slaveAddress())), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(digitalInputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(digitalInputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }

        if (!myThings().filterByThingClassId(neuronThingClassId).isEmpty()) {
            ThingId parentId = myThings().filterByThingClassId(neuronThingClassId).first()->id();
            foreach (QString circuit, m_neuron->digitalInputs()) {
                ThingDescriptor ThingDescriptor(digitalInputThingClassId, QString("Digital input %1").arg(circuit), QString("Neuron %1").arg(myThings().findById(parentId)->stateValue(neuronThingModelParamTypeId).toString()), parentId);
                foreach (Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(digitalInputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(digitalInputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }
        return info->finish(Thing::ThingErrorNoError);

    } else if (thingClassId == relayOutputThingClassId) {
        qCDebug(dcUniPi()) << "Discovering relay outputs";

        foreach (NeuronExtension *neuronExtension, m_neuronExtensions) {
            ThingId parentId = m_neuronExtensions.key(neuronExtension);
            foreach (QString circuit, neuronExtension->relayOutputs()) {
                ThingDescriptor ThingDescriptor(relayOutputThingClassId, QString("Relay output %1").arg(circuit), QString("Neuron extension %1, Slave address %2").arg(neuronExtension->type()).arg(QString::number(neuronExtension->slaveAddress())), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(relayOutputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(relayOutputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }

        if (!myThings().filterByThingClassId(neuronThingClassId).isEmpty()) {
            ThingId parentId = myThings().filterByThingClassId(neuronThingClassId).first()->id();
            foreach (QString circuit, m_neuron->relayOutputs()) {
                ThingDescriptor ThingDescriptor(relayOutputThingClassId, QString("Relay output %1").arg(circuit), QString("Neuron %1").arg(myThings().findById(parentId)->stateValue(neuronThingModelParamTypeId).toString()), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(relayOutputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added relay:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(relayOutputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }
        return info->finish(Thing::ThingErrorNoError);

    } else if (thingClassId == digitalOutputThingClassId) {
        qCDebug(dcUniPi()) << "Discovering digital outputs";
        foreach(Thing *parent, myThings()) {
            if (((parent->thingClassId() == uniPi1ThingClassId) || (parent->thingClassId() == uniPi1LiteThingClassId)) && m_unipi) {
                foreach (QString circuit, m_unipi->digitalOutputs()) {
                    ThingDescriptor ThingDescriptor(digitalOutputThingClassId, QString("Digital output %1").arg(circuit), "UniPi 1", parent->id());
                    foreach(Thing *thing, myThings().filterByParentId(parent->id())) {
                        if (thing->paramValue(digitalOutputThingCircuitParamTypeId) == circuit) {
                            qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parent->id();
                            ThingDescriptor.setThingId(thing->id());
                            break;
                        }
                    }
                    ParamList params;
                    params.append(Param(digitalOutputThingCircuitParamTypeId, circuit));
                    ThingDescriptor.setParams(params);
                    info->addThingDescriptor(ThingDescriptor);
                }
                break;
            }
        }

        foreach (NeuronExtension *neuronExtension, m_neuronExtensions) {
            ThingId parentId = m_neuronExtensions.key(neuronExtension);
            foreach (QString circuit, neuronExtension->digitalOutputs()) {
                ThingDescriptor ThingDescriptor(digitalOutputThingClassId, QString("Digital output %1").arg(circuit), QString("Neuron extension %1, Slave address %2").arg(neuronExtension->type()).arg(QString::number(neuronExtension->slaveAddress())), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(digitalOutputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(digitalOutputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }
        if (!myThings().filterByThingClassId(neuronThingClassId).isEmpty()) {
            ThingId parentId = myThings().filterByThingClassId(neuronThingClassId).first()->id();
            foreach (QString circuit, m_neuron->digitalOutputs()) {
                ThingDescriptor ThingDescriptor(digitalOutputThingClassId, QString("Digital output %1").arg(circuit), QString("Neuron %1").arg(myThings().findById(parentId)->stateValue(neuronThingModelParamTypeId).toString()), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(digitalOutputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(digitalOutputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }
        return info->finish(Thing::ThingErrorNoError);
    } else if (thingClassId == analogInputThingClassId) {
        qCDebug(dcUniPi()) << "Discovering analog inputs";
        foreach(Thing *parent, myThings()) {
            if (((parent->thingClassId() == uniPi1ThingClassId) || (parent->thingClassId() == uniPi1LiteThingClassId)) && m_unipi) {
                foreach (QString circuit, m_unipi->analogInputs()) {
                    ThingDescriptor ThingDescriptor(analogInputThingClassId, QString("Analog input %1").arg(circuit), "UniPi", parent->id());
                    foreach(Thing *thing, myThings().filterByParentId(parent->id())) {
                        if (thing->paramValue(analogInputThingCircuitParamTypeId) == circuit) {
                            qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parent->id();
                            ThingDescriptor.setThingId(thing->id());
                            break;
                        }
                    }
                    ParamList params;
                    params.append(Param(analogInputThingCircuitParamTypeId, circuit));
                    ThingDescriptor.setParams(params);
                    info->addThingDescriptor(ThingDescriptor);
                }
                break;
            }
        }

        foreach (NeuronExtension *neuronExtension, m_neuronExtensions) {
            ThingId parentId = m_neuronExtensions.key(neuronExtension);
            foreach (QString circuit, neuronExtension->analogInputs()) {
                ThingDescriptor ThingDescriptor(analogInputThingClassId, QString("Analog input %1").arg(circuit), QString("Neuron extension %1, Slave address %2").arg(neuronExtension->type()).arg(QString::number(neuronExtension->slaveAddress())), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(analogInputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(analogInputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }
        if (!myThings().filterByThingClassId(neuronThingClassId).isEmpty()) {
            ThingId parentId = myThings().filterByThingClassId(neuronThingClassId).first()->id();
            foreach (QString circuit, m_neuron->analogInputs()) {
                ThingDescriptor ThingDescriptor(analogInputThingClassId, QString("Analog input %1").arg(circuit), QString("Neuron %1").arg(myThings().findById(parentId)->stateValue(neuronThingModelParamTypeId).toString()), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(analogInputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(analogInputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }
        return info->finish(Thing::ThingErrorNoError);
    } else if (thingClassId == analogOutputThingClassId) {
        qCDebug(dcUniPi()) << "Discovering analog outputs";
        foreach(Thing *parent, myThings()) {
            if (((parent->thingClassId() == uniPi1ThingClassId) || (parent->thingClassId() == uniPi1LiteThingClassId)) && m_unipi) {
                foreach (QString circuit, m_unipi->analogOutputs()) {
                    ThingDescriptor ThingDescriptor(analogOutputThingClassId, QString("Analog Output %1").arg(circuit), "UniPi", parent->id());
                    foreach(Thing *thing, myThings().filterByParentId(parent->id())) {
                        if (thing->paramValue(analogOutputThingCircuitParamTypeId) == circuit) {
                            qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parent->id();
                            ThingDescriptor.setThingId(thing->id());
                            break;
                        }
                    }
                    ParamList params;
                    params.append(Param(analogOutputThingCircuitParamTypeId, circuit));
                    ThingDescriptor.setParams(params);
                    info->addThingDescriptor(ThingDescriptor);
                }
                break;
            }
        }

        foreach (NeuronExtension *neuronExtension, m_neuronExtensions) {
            ThingId parentId = m_neuronExtensions.key(neuronExtension);
            foreach (QString circuit, neuronExtension->analogOutputs()) {
                ThingDescriptor ThingDescriptor(analogOutputThingClassId, QString("Analog output %1").arg(circuit), QString("Neuron extension %1, Slave address %2").arg(neuronExtension->type()).arg(QString::number(neuronExtension->slaveAddress())), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(analogOutputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(analogOutputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }
        if (!myThings().filterByThingClassId(neuronThingClassId).isEmpty()) {
            ThingId parentId = myThings().filterByThingClassId(neuronThingClassId).first()->id();
            foreach (QString circuit, m_neuron->analogOutputs()) {
                ThingDescriptor ThingDescriptor(analogOutputThingClassId, QString("Analog output %1").arg(circuit), QString("Neuron %1").arg(myThings().findById(parentId)->stateValue(neuronThingModelParamTypeId).toString()), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(analogOutputThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(analogOutputThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }
        return info->finish(Thing::ThingErrorNoError);
    } else if (thingClassId == neuronExtensionBusThingClassId) {

        //TODO
        return info->finish(Thing::ThingErrorNoError);

    } else if (thingClassId == neuronExtensionThingClassId) {

        for (NeuronExtensionBus *neuronExtensionBus: m_neuronExtensionBus.values()) {

            NeuronExtensionDiscovery *extensionDiscovery = new NeuronExtensionDiscovery(neuronExtensionBus);
            connect(info, &ThingDiscoveryInfo::destroyed, extensionDiscovery, &NeuronExtensionDiscovery::deleteLater);
            connect(extensionDiscovery, &NeuronExtensionDiscovery::finished, extensionDiscovery, &NeuronExtensionDiscovery::deleteLater);
            connect(extensionDiscovery, &NeuronExtensionDiscovery::deviceFound, info, [info, neuronExtensionBus, this] (int address, NeuronExtension::ExtensionTypes model) {

                QString modelString = NeuronExtension::stringFromType(model);
                qCDebug(dcUniPi()) << "Neuron extension discovered" << model << "address" << address;

                ParamList params;
                QString descriptionString = modelString;
                descriptionString.append(QT_TR_NOOP(", address "));
                descriptionString.append(QString::number(address));
                if (!neuronExtensionBus->serialPort().isEmpty()) {
                    descriptionString.append(QT_TR_NOOP(", serial port "));
                    descriptionString.append(neuronExtensionBus->serialPort());
                }
                ThingDescriptor thingDescriptor(neuronExtensionThingClassId, "Neuron extension", descriptionString);
                params.append(Param(neuronExtensionThingNeuronExtensionBusUuidParamTypeId, m_neuronExtensionBus.key(neuronExtensionBus).toString(QUuid::WithoutBraces)));
                params.append(Param(neuronExtensionThingModelParamTypeId, modelString));
                params.append(Param(neuronExtensionThingSlaveAddressParamTypeId, address));
                if (myThings().findByParams(params)){
                    // Reconfiguration
                    //TODO doesnt work
                    thingDescriptor.setThingId(myThings().findByParams(params)->id());
                }
                thingDescriptor.setParams(params);
                info->addThingDescriptor(thingDescriptor);
            });
            extensionDiscovery->startDiscovery();
        }
        QTimer::singleShot(9000, info, [info] {
            qCDebug(dcUniPi()) << "Discovery finished";
            info->finish(Thing::ThingErrorNoError);
        });
        return;
    } else {
        qCWarning(dcUniPi()) << "Unhandled Thing class in discoverThing" << thingClassId;
        return info->finish(Thing::ThingErrorThingClassNotFound);
    }
}


void IntegrationPluginUniPi::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    if (m_neuronMigration.contains(thing->thingClassId())) {
        qCWarning(dcUniPi()) << "UniPi plugin was upgraded from V2 to V3, and the devices cannot be migrated, please readd your Neuron";
        info->finish(Thing::ThingErrorSetupFailed, tr("Could not migrate thing to new plugin version"));
        return;
    }
    if (m_neuronExtensionMigration.contains(thing->thingClassId())) {
        qCWarning(dcUniPi()) << "UniPi plugin was upgraded from V2 to V3, and the devices cannot be migrated, please readd your Neuron extensions";
        info->finish(Thing::ThingErrorSetupFailed, tr("Could not migrate thing to new plugin version"));
        return;
    }

    if(thing->thingClassId() == uniPi1ThingClassId ||
            thing->thingClassId() == uniPi1LiteThingClassId) {

        qCDebug(dcUniPi()) << "Setting up UniPi 1 thing" << thing->name();

        if (m_unipi) {
            qCWarning(dcUniPi()) << "UniPi 1 already setted up, only one thing allowed";
            return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("There is already a UniPi gateway in the system.")); //only one parent Thing allowed
        }

        if(thing->thingClassId() == uniPi1ThingClassId) {
            m_unipi = new UniPi(hardwareManager()->i2cManager(), UniPi::UniPiType::UniPi1, this);
        } else if (thing->thingClassId() == uniPi1LiteThingClassId) {
            m_unipi = new UniPi(hardwareManager()->i2cManager(), UniPi::UniPiType::UniPi1Lite, this);
        }

        if (!m_unipi->init()) {
            qCWarning(dcUniPi()) << "Could not setup UniPi";
            m_unipi->deleteLater();
            m_unipi = nullptr;
            return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Error setting up UniPi."));
        }
        connect(m_unipi, &UniPi::digitalInputStatusChanged, this, &IntegrationPluginUniPi::onUniPiDigitalInputStatusChanged);
        connect(m_unipi, &UniPi::digitalOutputStatusChanged, this, &IntegrationPluginUniPi::onUniPiDigitalOutputStatusChanged);
        connect(m_unipi, &UniPi::analogInputStatusChanged, this, &IntegrationPluginUniPi::onUniPiAnalogInputStatusChanged);
        connect(m_unipi, &UniPi::analogOutputStatusChanged, this, &IntegrationPluginUniPi::onUniPiAnalogOutputStatusChanged);
        thing->setStateValue(m_connectionStateTypeIds.value(thing->thingClassId()), true);
        return info->finish(Thing::ThingErrorNoError);

    } else if(thing->thingClassId() == neuronThingClassId) {
        qCDebug(dcUniPi()) << "Setting up Neuron thing" << thing->name();

        foreach (Thing *existingThing, myThings().filterByThingClassId(neuronThingClassId)) {
            if (existingThing->id() != thing->id()) {
                qCWarning(dcUniPi()) << "Only one neuron device is allowed";
                return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Neuron is already setted up."));
            }
        }

        if (m_neuron) {
            qCDebug(dcUniPi()) << "Setup after reconnection, cleaning up ...";
            m_neuron->deleteLater();
            m_neuron = nullptr;
        }
        m_neuron = new Neuron(this);

        if (!m_neuron->init()) {
            qCWarning(dcUniPi()) << "Could not init Neuron";
            m_neuron->deleteLater();
            m_neuron = nullptr;
            return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Error setting up Neuron Thing."));
        }

        thing->setStateValue(neuronConnectedStateTypeId, true);
        connect(m_neuron, &Neuron::digitalInputStatusChanged, this, &IntegrationPluginUniPi::onNeuronDigitalInputStatusChanged);
        connect(m_neuron, &Neuron::relayOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronRelayOutputStatusChanged);
        connect(m_neuron, &Neuron::digitalOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronDigitalOutputStatusChanged);
        connect(m_neuron, &Neuron::analogInputStatusChanged, this, &IntegrationPluginUniPi::onNeuronAnalogInputStatusChanged);
        connect(m_neuron, &Neuron::analogOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronAnalogOutputStatusChanged);

        return info->finish(Thing::ThingErrorNoError);

    } else if (thing->thingClassId() == neuronExtensionBusThingClassId) {

        QString port = thing->paramValue(neuronExtensionBusThingSerialPortParamTypeId).toString();
        int baudrate = thing->paramValue(neuronExtensionBusThingBaudrateParamTypeId).toInt();
        QString parity = thing->paramValue(neuronExtensionBusThingParityParamTypeId).toString();

        auto *bus = new NeuronExtensionBus(port, baudrate, parity.at(0).toUpper().toLatin1(), this);
        if (!bus->init()) {
            bus->deleteLater();
            return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Error setting up Neuron extension bus."));
        }
        m_neuronExtensionBus.insert(thing->id(), bus);
        return info->finish(Thing::ThingErrorNoError);

    } else if(thing->thingClassId() == neuronExtensionThingClassId) {
        qCDebug(dcUniPi()) << "Setting up Neuron extension thing" << thing->name();

        if (m_neuronExtensions.contains(thing->id())) {
            qCDebug(dcUniPi) << "Setup after reconfiguration, cleaning up ...";
            m_neuronExtensions.take(thing->id())->deleteLater();
        }

        int slaveAddress = thing->paramValue(neuronExtensionThingSlaveAddressParamTypeId).toInt();
        ThingId neuronExtensionBusThingId = ThingId(thing->paramValue(neuronExtensionThingNeuronExtensionBusUuidParamTypeId).toUuid());
        QString modelString = thing->paramValue(neuronExtensionThingModelParamTypeId).toString();

        NeuronExtension::ExtensionTypes type;
        if (modelString == "xS10") {
            type = NeuronExtension::xS10;
        } else if (modelString == "xS30") {
            type = NeuronExtension::xS30;
        } else if (modelString == "xS40") {
            type = NeuronExtension::xS40;
        } else if (modelString == "xS50") {
            type = NeuronExtension::xS50;
        } else if (modelString == "xS11") {
            type = NeuronExtension::xS11;
        } else if (modelString == "xS51") {
            type = NeuronExtension::xS51;
        } else {
            qCDebug(dcUniPi()) << "Unkown Neuron extension type" << modelString;
            return info->finish(Thing::ThingErrorSetupFailed, tr("Neuron extension type not supported"));
        }
        if (!m_neuronExtensionBus.contains(neuronExtensionBusThingId)) {
            // Probably not yet setup
            return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Error setting up Neuron extension Thing."));
        }
        NeuronExtensionBus *bus = m_neuronExtensionBus.value(neuronExtensionBusThingId);

        NeuronExtension *neuronExtension = new NeuronExtension(type, bus, slaveAddress, this);
        if (!neuronExtension->init()) {
            qCWarning(dcUniPi()) << "Could not load the modbus map";
            neuronExtension->deleteLater();
            return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Error setting up Neuron extension Thing."));
        }

        connect(neuronExtension, &NeuronExtension::connectionStateChanged, this, &IntegrationPluginUniPi::onNeuronExtensionConnectionStateChanged);

        connect(neuronExtension, &NeuronExtension::digitalInputStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionDigitalInputStatusChanged);
        connect(neuronExtension, &NeuronExtension::relayOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionRelayOutputStatusChanged);
        connect(neuronExtension, &NeuronExtension::digitalOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionDigitalOutputStatusChanged);
        connect(neuronExtension, &NeuronExtension::analogInputStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionAnalogInputStatusChanged);
        connect(neuronExtension, &NeuronExtension::analogOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionAnalogOutputStatusChanged);

        connect(info, &ThingSetupInfo::aborted, neuronExtension, &NeuronExtension::deleteLater);
        connect(neuronExtension, &NeuronExtension::deviceInformationReceived, info, [info, neuronExtension, this] {
            qCDebug(dcUniPi()) << "Neuron extension device information received";
            m_neuronExtensions.insert(info->thing()->id(), neuronExtension);
            info->thing()->setStateValue(neuronExtensionConnectedStateTypeId, true);
            return info->finish(Thing::ThingErrorNoError);
        });
        neuronExtension->getDeviceInformation();
        return;

    } else if ((thing->thingClassId() == relayOutputThingClassId) ||
               (thing->thingClassId() == digitalOutputThingClassId) ||
               (thing->thingClassId() == digitalInputThingClassId)  ||
               (thing->thingClassId() == analogInputThingClassId)   ||
               (thing->thingClassId() == analogOutputThingClassId)) {
        // Wait for the parent thing to complete setup
        Thing *parent = myThings().findById(thing->parentId());
        if (parent->setupComplete()) {
            return info->finish(Thing::ThingErrorNoError);
        } else {
            connect(parent, &Thing::setupStatusChanged, info, [info] {
                return info->finish(Thing::ThingErrorNoError);
            });
        }
    } else {
        qCWarning(dcUniPi()) << "Unhandled Thing class in setupThing:" << thing->thingClassId();
        return info->finish(Thing::ThingErrorThingClassNotFound);
    }
}

void IntegrationPluginUniPi::postSetupThing(Thing *thing)
{
    qCDebug(dcUniPi()) << "Post setup" << thing->name();

    if (thing->thingClassId() == neuronThingClassId) {
        if(!m_neuron)  {
            return;
        }
        m_neuron->start(); //Start thread
        connect(m_neuron, &Neuron::finished, this, [] {
           qCWarning(dcUniPi()) << "Neuron thread has finished";
        });
    } else if (thing->thingClassId() == neuronExtensionBusThingClassId) {
        thing->setStateValue(neuronExtensionBusConnectedStateTypeId, true);
    } else if (thing->thingClassId() == neuronExtensionThingClassId) {
        NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->id());
        if (!neuronExtension) {
            return;
        }

    } else if (thing->thingClassId() == relayOutputThingClassId) {
        Thing *parentThing = myThings().findById(thing->parentId());
        QString circuit = thing->paramValue(relayOutputThingCircuitParamTypeId).toString();
        bool value = false;
        if (parentThing->thingClassId() == neuronThingClassId) {
            if (!m_neuron) {
                return;
            }
            //value = m_neuron->relayOutputValue(circuit);

        } else {
            NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->id());
            if (!neuronExtension) {
                return;
            }
            value = neuronExtension->relayOutputValue(circuit);
        }
        thing->setStateValue(relayOutputPowerStateTypeId, value);
    } else if (thing->thingClassId() == digitalOutputThingClassId) {
        Thing *parentThing = myThings().findById(thing->parentId());
        QString circuit = thing->paramValue(digitalOutputThingCircuitParamTypeId).toString();
        bool value = false;
        if (parentThing->thingClassId() == neuronThingClassId) {
            if (!m_neuron) {
                return;
            }
            //value = m_neuron->digitalOutputValue(circuit);

        } else {
            NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->id());
            if (!neuronExtension) {
                return;
            }
            value = neuronExtension->digitalOutputValue(circuit);
        }
        thing->setStateValue(digitalOutputPowerStateTypeId, value);

    } else if (thing->thingClassId() == digitalInputThingClassId) {
        Thing *parentThing = myThings().findById(thing->parentId());
        QString circuit = thing->paramValue(digitalInputThingCircuitParamTypeId).toString();
        bool value = false;
        if (parentThing->thingClassId() == neuronThingClassId) {
            if (!m_neuron) {
                return;
            }
            //value = m_neuron->digitalInputValue(circuit);

        } else {
            NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->id());
            if (!neuronExtension) {
                return;
            }
            value = neuronExtension->digitalInputValue(circuit);
        }
        thing->setStateValue(digitalInputInputStatusStateTypeId, value);

    } else if (thing->thingClassId() == analogInputThingClassId) {
        Thing *parentThing = myThings().findById(thing->parentId());
        QString circuit = thing->paramValue(analogInputThingCircuitParamTypeId).toString();
        double value = 0.f;
        if (parentThing->thingClassId() == neuronThingClassId) {
            if (!m_neuron) {
                return;
            }
            //value = m_neuron->analogInputValue(circuit);
        } else {
            NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->id());
            if (!neuronExtension) {
                return;
            }
            value = neuronExtension->analogInputValue(circuit);
        }
        thing->setStateValue(analogInputPercentageStateTypeId, value);

        connect(thing, &Thing::settingChanged, thing, [parentThing, circuit, thing, this] (const ParamTypeId &paramTypeId, const QVariant &value) {
            if (paramTypeId == analogInputSettingsConfigurationParamTypeId) {
                if (parentThing->thingClassId() == neuronThingClassId) {
                    if (!m_neuron) {
                        return;
                    }
                    if (value.toString() == "Voltage") {
                        //TODO m_neuron->setAnalogInputConfiguration(circuit, NeuronCommon::AnalogIOConfiguration::Voltage);
                    } else if (value.toString() == "Current") {
                        //TODO m_neuron->setAnalogInputConfiguration(circuit, NeuronCommon::AnalogIOConfiguration::Current);
                    }
                    return;
                }
                if (parentThing->thingClassId() == neuronExtensionThingClassId) {
                    NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->parentId());
                    if (!neuronExtension) {
                        return;
                    }
                    if (value.toString() == "Voltage") {
                        //TODO m_neuron->setAnalogInputConfiguration(circuit, NeuronCommon::AnalogIOConfiguration::Voltage);
                    } else if (value.toString() == "Current") {
                        //TODO m_neuron->setAnalogInputConfiguration(circuit, NeuronCommon::AnalogIOConfiguration::Current);
                    }
                    return;
                }
            }
        });
    } else if (thing->thingClassId() == analogOutputThingClassId) {
        Thing *parentThing = myThings().findById(thing->parentId());
        QString circuit = thing->paramValue(analogOutputThingCircuitParamTypeId).toString();
        double value = 0.f;
        if (parentThing->thingClassId() == neuronThingClassId) {
            //if (!m_neuron) {
            //    return;
            //}
            //value = m_neuron->analogOutputValue(circuit);
        } else {
            NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->id());
            if (!neuronExtension) {
                return;
            }
            value = neuronExtension->analogOutputValue(circuit);
        }
        thing->setStateValue(analogOutputPercentageStateTypeId, value);
    }
}


void IntegrationPluginUniPi::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == relayOutputThingClassId)  {
        if (action.actionTypeId() == relayOutputPowerActionTypeId) {
            QString relayOutputNumber = thing->paramValue(relayOutputThingCircuitParamTypeId).toString();
            bool stateValue = action.param(relayOutputPowerActionPowerParamTypeId).value().toBool();
            ThingClassId parentThingClassId = myThings().findById(thing->parentId())->thingClassId();

            if (parentThingClassId == neuronThingClassId) {
                if (!m_neuron) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                if (!m_neuron->setRelayOutput(relayOutputNumber, stateValue)) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                } else {
                    info->finish(Thing::ThingErrorNoError);
                }
                return;
            } else if (parentThingClassId == neuronExtensionThingClassId) {
                NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->parentId());
                if (!neuronExtension) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                neuronExtension->setRelayOutput(relayOutputNumber, stateValue);
                return info->finish(Thing::ThingErrorNoError);
            }
        } else {
            qCWarning(dcUniPi()) << "Unhandled ActionTypeId" << action.actionTypeId();
            return info->finish(Thing::ThingErrorActionTypeNotFound);
        }
    } else if (thing->thingClassId() == digitalOutputThingClassId)  {

        if (action.actionTypeId() == digitalOutputPowerActionTypeId) {
            QString digitalOutputNumber = thing->paramValue(digitalOutputThingCircuitParamTypeId).toString();
            bool stateValue = action.param(digitalOutputPowerActionPowerParamTypeId).value().toBool();
            ThingClassId parentThingClassId = myThings().findById(thing->parentId())->thingClassId();

            //TODO remove unipi only Relays
            if (parentThingClassId == uniPi1ThingClassId ||
                    parentThingClassId == uniPi1LiteThingClassId) {
                if (!m_unipi) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                if(m_unipi->setDigitalOutput(digitalOutputNumber, stateValue)) {
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            } else if (parentThingClassId == neuronThingClassId) {
                if (!m_neuron) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                if (!m_neuron->setDigitalOutput(digitalOutputNumber, stateValue)) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                } else {
                    info->finish(Thing::ThingErrorNoError);
                }
                return;
            } else if (parentThingClassId == neuronExtensionThingClassId) {
                NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->parentId());
                if (!neuronExtension) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                neuronExtension->setDigitalOutput(digitalOutputNumber, stateValue);
                return info->finish(Thing::ThingErrorNoError);
            }
        } else {
            qCWarning(dcUniPi()) << "Unhandled ActionTypeId" << action.actionTypeId();
            return info->finish(Thing::ThingErrorActionTypeNotFound);
        }
    } else if (thing->thingClassId() == analogOutputThingClassId) {

        if (action.actionTypeId() == analogOutputOutputValueActionTypeId) {
            QString analogOutputNumber = thing->paramValue(analogOutputThingCircuitParamTypeId).toString();
            double analogValue = action.param(analogOutputOutputValueActionOutputValueParamTypeId).value().toDouble();
            ThingClassId parentThingClassId = myThings().findById(thing->parentId())->thingClassId();

            if (parentThingClassId == uniPi1ThingClassId ||
                    parentThingClassId == uniPi1LiteThingClassId) {
                if (!m_unipi) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                if(m_unipi->setAnalogOutput(analogValue)) {
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            } else if (parentThingClassId == neuronThingClassId) {
                if (!m_neuron) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                if(!m_neuron->setAnalogOutput(analogOutputNumber, analogValue)) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                } else {
                    info->finish(Thing::ThingErrorNoError);
                }
                return;
            } else if (parentThingClassId == neuronExtensionThingClassId) {
                NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->parentId());
                if (!neuronExtension) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                neuronExtension->setAnalogOutput(analogOutputNumber, analogValue);
                return;
            }
        } else {
            qCWarning(dcUniPi()) << "Unhandled ActionTypeId" << action.actionTypeId();
            return info->finish(Thing::ThingErrorActionTypeNotFound);
        }
    } else {
        qCWarning(dcUniPi()) << "Unhandled Thing class in executeAction" << thing->thingClassId();
        info->finish(Thing::ThingErrorThingClassNotFound);
    }
}


void IntegrationPluginUniPi::thingRemoved(Thing *thing)
{
    qCDebug(dcUniPi()) << "Deleting thing" << thing->name();
    if(m_neuronExtensions.contains(thing->id())) {
        NeuronExtension *neuronExtension = m_neuronExtensions.take(thing->id());
        neuronExtension->deleteLater();
    } else if (thing->thingClassId() == neuronThingClassId) {
        m_neuron->requestInterruption();
        if (!m_neuron->wait(200)) {
            qCWarning(dcUniPi()) << "neuron wait for finish timeout";
        }
        m_neuron->deleteLater();
        m_neuron = nullptr;
    } else if ((thing->thingClassId() == uniPi1ThingClassId) || (thing->thingClassId() == uniPi1LiteThingClassId)) {
        if(m_unipi) {
            m_unipi->deleteLater();
            m_unipi = nullptr;
        }
    }
}


void IntegrationPluginUniPi::onNeuronRelayOutputStatusChanged(const QString &circuit, bool value)
{
    Thing *neuronThing = myThings().filterByThingClassId(neuronThingClassId).first();
    if (!neuronThing) {
        qCWarning(dcUniPi()) << "Could not find any Thing associated to Neuron object";
        return;
    }

    foreach(Thing *thing, myThings().filterByParentId(neuronThing->id())) {
        if (thing->thingClassId() == relayOutputThingClassId) {
            if (thing->paramValue(relayOutputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(relayOutputPowerStateTypeId, value);
                return;
            }
        }
    }
}


void IntegrationPluginUniPi::onNeuronDigitalInputStatusChanged(const QString &circuit, bool value)
{
    Thing *neuronThing = myThings().filterByThingClassId(neuronThingClassId).first();
    if (!neuronThing) {
        qCWarning(dcUniPi()) << "Could not find any Thing associated to Neuron obejct";
        return;
    }

    foreach(Thing *thing, myThings().filterByParentId(neuronThing->id())) {
        if (thing->thingClassId() == digitalInputThingClassId) {
            if (thing->paramValue(digitalInputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(digitalInputInputStatusStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onNeuronDigitalOutputStatusChanged(const QString &circuit, bool value)
{
    Thing *neuronThing = myThings().filterByThingClassId(neuronThingClassId).first();
    if (!neuronThing) {
        qCWarning(dcUniPi()) << "Could not find any Thing associated to Neuron obejct";
        return;
    }

    foreach(Thing *thing, myThings().filterByParentId(neuronThing->id())) {
        if (thing->thingClassId() == digitalOutputThingClassId) {
            if (thing->paramValue(digitalOutputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(digitalOutputPowerStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onNeuronAnalogInputStatusChanged(const QString &circuit, double value)
{
    Thing *neuronThing = myThings().filterByThingClassId(neuronThingClassId).first();
    if (!neuronThing) {
        qCWarning(dcUniPi()) << "Could not find any Thing associated to Neuron obejct";
        return;
    }

    foreach(Thing *thing, myThings().filterByParentId(neuronThing->id())) {
        if (thing->thingClassId() == analogInputThingClassId) {
            if (thing->paramValue(analogInputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(analogInputInputValueStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onNeuronAnalogOutputStatusChanged(const QString &circuit, double value)
{
    Thing *neuronThing = myThings().filterByThingClassId(neuronThingClassId).first();
    if (!neuronThing) {
        qCWarning(dcUniPi()) << "Could not find any Thing associated to Neuron obejct";
        return;
    }

    foreach(Thing *thing, myThings().filterByParentId(neuronThing->id())) {
        if (thing->thingClassId() == analogOutputThingClassId) {
            if (thing->paramValue(analogOutputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(analogOutputOutputValueStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onNeuronExtensionDigitalInputStatusChanged(const QString &circuit, bool value)
{
    NeuronExtension *neuronExtension = static_cast<NeuronExtension *>(sender());

    foreach(Thing *thing, myThings().filterByParentId(m_neuronExtensions.key(neuronExtension))) {
        if (thing->thingClassId() == digitalInputThingClassId) {
            if (thing->paramValue(digitalInputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(digitalInputInputStatusStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onNeuronExtensionDigitalOutputStatusChanged(const QString &circuit, bool value)
{
    NeuronExtension *neuronExtension = static_cast<NeuronExtension *>(sender());

    foreach(Thing *thing, myThings().filterByParentId(m_neuronExtensions.key(neuronExtension))) {
        if (thing->thingClassId() == digitalOutputThingClassId) {
            if (thing->paramValue(digitalOutputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(digitalOutputPowerStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onNeuronExtensionRelayOutputStatusChanged(const QString &circuit, bool value)
{
    NeuronExtension *neuronExtension = static_cast<NeuronExtension *>(sender());

    foreach(Thing *thing, myThings().filterByParentId(m_neuronExtensions.key(neuronExtension))) {
        if (thing->thingClassId() == relayOutputThingClassId) {
            if (thing->paramValue(relayOutputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(relayOutputPowerStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onNeuronExtensionAnalogInputStatusChanged(const QString &circuit, double value)
{
    NeuronExtension *neuronExtension = static_cast<NeuronExtension *>(sender());

    foreach(Thing *thing, myThings().filterByParentId(m_neuronExtensions.key(neuronExtension))) {
        if (thing->thingClassId() == analogInputThingClassId) {
            if (thing->paramValue(analogInputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(analogInputInputValueStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onNeuronExtensionAnalogOutputStatusChanged(const QString &circuit, double value)
{
    NeuronExtension *neuronExtension = static_cast<NeuronExtension *>(sender());

    foreach(Thing *thing, myThings().filterByParentId(m_neuronExtensions.key(neuronExtension))) {
        if (thing->thingClassId() == analogOutputThingClassId) {
            if (thing->paramValue(analogOutputThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(analogOutputOutputValueStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onNeuronExtensionConnectionStateChanged(bool state)
{
    NeuronExtension *neuron = static_cast<NeuronExtension *>(sender());
    Thing *thing = myThings().findById(m_neuronExtensions.key(neuron));
    if (!thing) {
        qCWarning(dcUniPi()) << "Could not find any Thing associated to NeuronExtension obejct";
        return;
    }
    qCDebug(dcUniPi()) << "Neuron extension connection state changed" << thing->name() << state;
    thing->setStateValue(m_connectionStateTypeIds.value(thing->thingClassId()), state);
}

void IntegrationPluginUniPi::onUniPiDigitalInputStatusChanged(const QString &circuit, bool value)
{
    qDebug(dcUniPi) << "Digital Input changed" << circuit << value;
    Q_FOREACH (Thing *thing, myThings().filterByThingClassId(digitalInputThingClassId)) {
        if (thing->paramValue(digitalInputThingCircuitParamTypeId).toString() == circuit) {

            thing->setStateValue(digitalInputInputStatusStateTypeId, value);
            return;
        }
    }
}

void IntegrationPluginUniPi::onUniPiDigitalOutputStatusChanged(const QString &circuit, bool value)
{
    qDebug(dcUniPi) << "Digital Output changed" << circuit << value;
    Q_FOREACH (Thing *thing, myThings().filterByThingClassId(digitalOutputThingClassId)) {
        if (thing->paramValue(digitalOutputThingCircuitParamTypeId).toString() == circuit) {
            thing->setStateValue(digitalOutputPowerStateTypeId, value);
            return;
        }
    }
}

void IntegrationPluginUniPi::onUniPiAnalogInputStatusChanged(const QString &circuit, double value)
{
    qDebug(dcUniPi) << "Analog Input changed" << circuit << value;
    Q_FOREACH (Thing *thing, myThings().filterByThingClassId(analogInputThingClassId)) {
        if (thing->paramValue(analogInputThingCircuitParamTypeId).toString() == circuit) {
            thing->setStateValue(analogInputInputValueStateTypeId, value);
            return;
        }
    }
}

void IntegrationPluginUniPi::onUniPiAnalogOutputStatusChanged(double value)
{
    qDebug(dcUniPi) << "Analog output changed" << value;
    Q_FOREACH (Thing *thing, myThings().filterByThingClassId(analogOutputThingClassId)) {
        thing->setStateValue(analogOutputOutputValueStateTypeId, value);
        return;
    }
}
