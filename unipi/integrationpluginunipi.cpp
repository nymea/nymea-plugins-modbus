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
#include <QModbusTcpClient>

IntegrationPluginUniPi::IntegrationPluginUniPi()
{
}


void IntegrationPluginUniPi::init()
{
    connect(this, &IntegrationPluginUniPi::configValueChanged, this, &IntegrationPluginUniPi::onPluginConfigurationChanged);
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
    m_neuronExtensionMigration.insert(ThingClassId("9e2e13bb-c18c-438f-989f-52363561ce85"), "xS20");
    m_neuronExtensionMigration.insert(ThingClassId("a1ec57b1-fcf9-4540-80c9-40f78cddc85f"), "xS30");
    m_neuronExtensionMigration.insert(ThingClassId("05c78946-48f4-4e1b-9d45-90fbd66c71c0"), "xS40");
    m_neuronExtensionMigration.insert(ThingClassId("c31f5e8c-a27c-49db-b5a8-dd065336b79a"), "xS50");
    m_neuronExtensionMigration.insert(ThingClassId("f59bfba2-0455-49f2-b92d-badfec5dcc01"), "xS11");
    m_neuronExtensionMigration.insert(ThingClassId("0e11fbd3-8d4a-4fd8-aeeb-25ee2d134a17"), "xS51");

    // Modbus RTU hardware resource
    connect(hardwareManager()->modbusRtuResource(), &ModbusRtuHardwareResource::modbusRtuMasterRemoved, this, [=](const QUuid &modbusUuid){
        qCDebug(dcUniPi()) << "Modbus RTU master has been removed" << modbusUuid.toString();

        // Check if there is any device using this resource
        foreach (Thing *thing, m_modbusRtuMasters.keys()) {
            if (m_modbusRtuMasters.value(thing)->modbusUuid() == modbusUuid) {
                qCWarning(dcUniPi()) << "Hardware resource removed for" << thing << ". The thing will not be functional any more until a new resource has been configured for it.";
                m_modbusRtuMasters.remove(thing);
                thing->setStateValue(m_connectionStateTypeIds[thing->thingClassId()], false);

                // Set all child things disconnected
                foreach (Thing *childThing, myThings()) {
                    if (childThing->parentId() == thing->id()) {
                        thing->setStateValue(m_connectionStateTypeIds[childThing->thingClassId()], false);
                    }
                }
            }
        }
    });
}

void IntegrationPluginUniPi::discoverThings(ThingDiscoveryInfo *info)
{
    ThingClassId ThingClassId = info->thingClassId();

    if (ThingClassId == digitalInputThingClassId) {
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
                ThingDescriptor ThingDescriptor(digitalInputThingClassId, QString("Digital input %1").arg(circuit), QString("Neuron %1").arg(m_neuron->type()), parentId);
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
    } else if (ThingClassId == digitalOutputThingClassId) {
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
                ThingDescriptor ThingDescriptor(digitalOutputThingClassId, QString("Digital output %1").arg(circuit), QString("Neuron %1").arg(m_neuron->type()), parentId);
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
    } else if (ThingClassId == analogInputThingClassId) {
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
                ThingDescriptor ThingDescriptor(analogInputThingClassId, QString("Analog input %1").arg(circuit), QString("Neuron %1").arg(m_neuron->type()), parentId);
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
    } else if (ThingClassId == analogOutputThingClassId) {
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
                ThingDescriptor ThingDescriptor(analogOutputThingClassId, QString("Analog output %1").arg(circuit), QString("Neuron %1").arg(m_neuron->type()), parentId);
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
    } else if (ThingClassId == userLEDThingClassId) {
        qCDebug(dcUniPi()) << "Discovering user LEDs";
        QList<ThingDescriptor> ThingDescriptors;
        foreach (NeuronExtension *neuronExtension, m_neuronExtensions) {
            ThingId parentId = m_neuronExtensions.key(neuronExtension);
            foreach (QString circuit, neuronExtension->userLEDs()) {
                ThingDescriptor ThingDescriptor(userLEDThingClassId, QString("User programmable LED %1").arg(circuit), QString("Neuron extension %1, Slave address %2").arg(neuronExtension->type()).arg(neuronExtension->slaveAddress()), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(userLEDThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(userLEDThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }

        if (!myThings().filterByThingClassId(neuronThingClassId).isEmpty()) {
            ThingId parentId = myThings().filterByThingClassId(neuronThingClassId).first()->id();
            foreach (QString circuit, m_neuron->userLEDs()) {
                ThingDescriptor ThingDescriptor(userLEDThingClassId, QString("User programmable LED %1").arg(circuit), QString("Neuron %1").arg(m_neuron->type()), parentId);
                foreach(Thing *thing, myThings().filterByParentId(parentId)) {
                    if (thing->paramValue(userLEDThingCircuitParamTypeId) == circuit) {
                        qCDebug(dcUniPi()) << "Found already added Circuit:" << circuit << parentId;
                        ThingDescriptor.setThingId(thing->id());
                        break;
                    }
                }
                ParamList params;
                params.append(Param(userLEDThingCircuitParamTypeId, circuit));
                ThingDescriptor.setParams(params);
                info->addThingDescriptor(ThingDescriptor);
            }
        }
        return info->finish(Thing::ThingErrorNoError);

    } else if (ThingClassId == neuronExtensionThingClassId) {
        if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
            info->finish(Thing::ThingErrorHardwareNotAvailable, tr("No Modbus RTU interface available."));
        }
        foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
            qCDebug(dcUniPi()) << "Found RTU master resource" << modbusMaster;
            if (modbusMaster->connected()) {

                for (int i = 0; i <= 16; i++) {
                    ModbusRtuReply *reply = modbusMaster->readHoldingRegister(i, 1000, 7);
                    connect(reply, &ModbusRtuReply::finished, reply, &ModbusRtuReply::deleteLater);
                    connect(reply, &ModbusRtuReply::finished, this, [info, modbusMaster, reply] {

                        if (reply->error() != ModbusRtuReply::Error::NoError) {
                            return;
                        }
                        QVector<quint16> result = reply->result();
                        if (result.length() < 7) {
                            return;
                        }
                        qCDebug(dcUniPi()) << "Found Neuron Extension";
                        qCDebug(dcUniPi()) << "     - Slave Address" << reply->slaveAddress();
                        qCDebug(dcUniPi()) << "     - Firmware version" << result[0];
                        qCDebug(dcUniPi()) << "     - Number of IOs" << result[1];
                        qCDebug(dcUniPi()) << "     - Number of peripherals" << result[2];
                        qCDebug(dcUniPi()) << "     - Firmware Id" << result[3];
                        qCDebug(dcUniPi()) << "     - Hardware Id" << result[4];
                        qCDebug(dcUniPi()) << "     - Serial number" << (static_cast<quint32>(result[6])<<16 | result[5]);

                        ParamList parameters;
                        ThingDescriptor thingDescriptor(neuronExtensionThingClassId, "Neuron extension", modbusMaster->serialPort());
                        parameters.append(Param(neuronExtensionThingModbusMasterUuidParamTypeId, modbusMaster->modbusUuid()));
                        parameters.append(Param(neuronExtensionThingModelIdParamTypeId, result[4]));
                        thingDescriptor.setParams(parameters);
                        info->addThingDescriptor(thingDescriptor);
                    });
                }
            } else {
                qCWarning(dcUniPi()) << "Found configured resource" << modbusMaster << "but it is not connected. Skipping.";
            }
        }
        QTimer::singleShot(5000, info, [info] {info->finish(Thing::ThingErrorNoError);});
    } else {
        qCWarning(dcUniPi()) << "Unhandled Thing class in discoverThing" << ThingClassId;
        return info->finish(Thing::ThingErrorThingClassNotFound);
    }
}

void IntegrationPluginUniPi::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    if (m_neuronMigration.contains(thing->thingClassId())) {
        qCDebug(dcUniPi()) << "Start migration";
        //TODO
    }
    if (m_neuronExtensionMigration.contains(thing->thingClassId())) {
        qCDebug(dcUniPi()) << "Start migration";
        //TODO
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

        if (m_modbusTCPMaster) {
            m_modbusTCPMaster->deleteLater();
            m_modbusTCPMaster = nullptr;
        }

        int port = configValue(uniPiPluginPortParamTypeId).toInt();;
        QHostAddress ipAddress = QHostAddress(configValue(uniPiPluginAddressParamTypeId).toString());

        qCDebug(dcUniPi()) << "Creating Modbus TCP Master";
        m_modbusTCPMaster = new QModbusTcpClient(this);
        m_modbusTCPMaster->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
        m_modbusTCPMaster->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ipAddress.toString());
        m_modbusTCPMaster->setTimeout(200);
        m_modbusTCPMaster->setNumberOfRetries(2);


        if (!m_modbusTCPMaster->connectDevice()) {
            qCWarning(dcUniPi()) << "Connect failed:" << m_modbusTCPMaster->errorString();
            m_modbusTCPMaster->deleteLater();
            m_modbusTCPMaster = nullptr;
            return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Could not connect to Modbus server."));;
        }
        connect(info, &ThingSetupInfo::aborted, m_modbusTCPMaster, &QModbusTcpClient::deleteLater);
        connect(m_modbusTCPMaster, &QModbusTcpClient::destroyed, this, [this] {m_modbusTCPMaster = nullptr;});

        connect(m_modbusTCPMaster, &QModbusTcpClient::stateChanged, info, [info, thing, this] (QModbusDevice::State state) {

            if (state == QModbusDevice::State::ConnectedState) {
                QModbusDataUnit unit(QModbusDataUnit::RegisterType::HoldingRegisters, 1000, 7);
                QModbusReply *reply = m_modbusTCPMaster->sendReadRequest(unit, 0);
                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                connect(reply, &QModbusReply::finished, info, [info, thing, reply, this] {
                    if (reply->error() != QModbusDevice::NoError) {
                        return;
                    }
                    QVector<quint16> result = reply->result().values();
                    if (result.length() < 7) {
                        return;
                    }

                    qCDebug(dcUniPi()) << "Found Neuron";
                    qCDebug(dcUniPi()) << "     - Slave Address" << reply->serverAddress();
                    qCDebug(dcUniPi()) << "     - Firmware version" << result[0];
                    qCDebug(dcUniPi()) << "     - Number of IOs" << result[1];
                    qCDebug(dcUniPi()) << "     - Number of peripherals" << result[2];
                    qCDebug(dcUniPi()) << "     - Firmware Id" << result[3];
                    qCDebug(dcUniPi()) << "     - Hardware Id" << result[4];
                    qCDebug(dcUniPi()) << "     - Serial number" << (static_cast<quint32>(result[6])<<16 | result[5]);

                    QString modelString = thing->paramValue(neuronThingModelParamTypeId).toString();
                    Neuron::NeuronTypes type;
                    if (modelString == "S103") {
                        type = Neuron::NeuronTypes::S103;
                    } else if (modelString == "M103") {
                        type = Neuron::NeuronTypes::M103;
                    } else if (modelString == "M203") {
                        type = Neuron::NeuronTypes::M203;
                    } else if (modelString == "M303") {
                        type = Neuron::NeuronTypes::M303;
                    } else if (modelString == "M403") {
                        type = Neuron::NeuronTypes::M403;
                    } else if (modelString == "M503") {
                        type = Neuron::NeuronTypes::M503;
                    } else if (modelString == "M523") {
                        type = Neuron::NeuronTypes::M523;
                    } else if (modelString == "L203") {
                        type = Neuron::NeuronTypes::L203;
                    } else if (modelString == "L303") {
                        type = Neuron::NeuronTypes::L303;
                    } else if (modelString == "L403") {
                        type = Neuron::NeuronTypes::L403;
                    } else if (modelString == "L503") {
                        type = Neuron::NeuronTypes::L503;
                    } else if (modelString == "L513") {
                        type = Neuron::NeuronTypes::L513;
                    } else if (modelString == "L523") {
                        type = Neuron::NeuronTypes::L523;
                    } else if (modelString == "L533") {
                        type = Neuron::NeuronTypes::L533;
                    } else {
                        qCDebug(dcUniPi()) << "Unkown Neuron type" << modelString;
                        return info->finish(Thing::ThingErrorSetupFailed, tr("Neuon type not supported"));
                    }
                    m_neuron = new Neuron(type, m_modbusTCPMaster, this);

                    if (!m_neuron->init()) {
                        qCWarning(dcUniPi()) << "Could not load the modbus map";
                        m_neuron->deleteLater();
                        m_neuron = nullptr;
                        return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Error setting up Neuron Thing."));
                    }
                    connect(m_neuron, &Neuron::requestExecuted, this, &IntegrationPluginUniPi::onRequestExecuted);
                    connect(m_neuron, &Neuron::requestError, this, &IntegrationPluginUniPi::onRequestError);
                    connect(m_neuron, &Neuron::connectionStateChanged, this, &IntegrationPluginUniPi::onNeuronConnectionStateChanged);
                    connect(m_neuron, &Neuron::digitalInputStatusChanged, this, &IntegrationPluginUniPi::onNeuronDigitalInputStatusChanged);
                    connect(m_neuron, &Neuron::digitalOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronDigitalOutputStatusChanged);
                    connect(m_neuron, &Neuron::analogInputStatusChanged, this, &IntegrationPluginUniPi::onNeuronAnalogInputStatusChanged);
                    connect(m_neuron, &Neuron::analogOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronAnalogOutputStatusChanged);
                    connect(m_neuron, &Neuron::userLEDStatusChanged, this, &IntegrationPluginUniPi::onNeuronUserLEDStatusChanged);
                    connect(m_modbusTCPMaster, &QModbusTcpClient::stateChanged, this, &IntegrationPluginUniPi::onModbusTCPStateChanged);

                    thing->setStateValue(m_connectionStateTypeIds.value(thing->thingClassId()), (m_modbusTCPMaster->state() == QModbusDevice::ConnectedState));
                    return info->finish(Thing::ThingErrorNoError);
                });
            }
        });
    } else if(thing->thingClassId() == neuronExtensionThingClassId) {
        qCDebug(dcUniPi()) << "Setting up Neuron extension thing" << thing->name();

        NeuronExtension *neuronExtension;
        int slaveAddress = thing->paramValue(neuronExtensionThingSlaveAddressParamTypeId).toInt();
        QUuid modbusRtuMasterUuid = thing->paramValue(neuronExtensionThingModbusMasterUuidParamTypeId).toString();
        ModbusRtuMaster *modbusRtuMaster = hardwareManager()->modbusRtuResource()->getModbusRtuMaster(modbusRtuMasterUuid);
        int model =  thing->paramValue(neuronExtensionThingModelIdParamTypeId).toInt();
        neuronExtension = new NeuronExtension(NeuronExtension::ExtensionTypes(model), modbusRtuMaster, slaveAddress, this);

        /*
        } else {
            return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Error unrecognized extension type."));
        }
        if (!neuronExtension->init()) {
            qCWarning(dcUniPi()) << "Could not load the modbus map";
            neuronExtension->deleteLater();
            neuronExtension = nullptr;
            return info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Error loading modbus map."));
        }*/

        connect(neuronExtension, &NeuronExtension::requestExecuted, this, &IntegrationPluginUniPi::onRequestExecuted);
        connect(neuronExtension, &NeuronExtension::requestError, this, &IntegrationPluginUniPi::onRequestError);
        connect(neuronExtension, &NeuronExtension::connectionStateChanged, this, &IntegrationPluginUniPi::onNeuronExtensionConnectionStateChanged);
        connect(neuronExtension, &NeuronExtension::digitalInputStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionDigitalInputStatusChanged);
        connect(neuronExtension, &NeuronExtension::digitalOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionDigitalOutputStatusChanged);
        connect(neuronExtension, &NeuronExtension::analogInputStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionAnalogInputStatusChanged);
        connect(neuronExtension, &NeuronExtension::analogOutputStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionAnalogOutputStatusChanged);
        connect(neuronExtension, &NeuronExtension::userLEDStatusChanged, this, &IntegrationPluginUniPi::onNeuronExtensionUserLEDStatusChanged);

        m_neuronExtensions.insert(thing->id(), neuronExtension);
        //thing->setStateValue(m_connectionStateTypeIds.value(thing->thingClassId()), (m_modbusRTUMaster->state() == QModbusDevice::ConnectedState));

        return info->finish(Thing::ThingErrorNoError);
    } else if (thing->thingClassId() == digitalOutputThingClassId) {
        return info->finish(Thing::ThingErrorNoError);
    } else if (thing->thingClassId() == digitalInputThingClassId) {
        return info->finish(Thing::ThingErrorNoError);
    } else if (thing->thingClassId() == userLEDThingClassId) {
        return info->finish(Thing::ThingErrorNoError);
    } else if (thing->thingClassId() == analogInputThingClassId) {
        Thing *parent = myThings().findById(thing->parentId());
        if (parent->setupComplete()) {
            return info->finish(Thing::ThingErrorNoError);
        } else {
            connect(parent, &Thing::setupStatusChanged, info, [info] {
                return info->finish(Thing::ThingErrorNoError);
            });
        }
    } else if (thing->thingClassId() == analogOutputThingClassId) {
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

    if (thing->thingClassId() == analogInputThingClassId) {
        ThingClassId parentThingClassId = myThings().findById(thing->parentId())->thingClassId();
        QString circuit = thing->paramValue(analogInputThingCircuitParamTypeId).toString();
        connect(thing, &Thing::settingChanged, thing, [parentThingClassId, circuit, thing, this] (const ParamTypeId &paramTypeId, const QVariant &value) {
            if (paramTypeId == analogInputSettingsConfigurationParamTypeId) {
                if (parentThingClassId == neuronThingClassId) {
                    if (!m_neuron) {
                        return;
                    }
                    if (value.toString() == "Voltage") {
                        m_neuron->setAnalogInputConfiguration(circuit, NeuronCommon::AnalogIOConfiguration::Voltage);
                    } else if (value.toString() == "Current") {
                        m_neuron->setAnalogInputConfiguration(circuit, NeuronCommon::AnalogIOConfiguration::Current);
                    }
                    return;
                } else if (parentThingClassId == neuronExtensionThingClassId) {
                    NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->parentId());
                    if (!neuronExtension) {
                        return;
                    }
                    if (value.toString() == "Voltage") {
                        m_neuron->setAnalogInputConfiguration(circuit, NeuronCommon::AnalogIOConfiguration::Voltage);
                    } else if (value.toString() == "Current") {
                        m_neuron->setAnalogInputConfiguration(circuit, NeuronCommon::AnalogIOConfiguration::Current);
                    }
                    return;
                }
            }
        });
    } else if (thing->thingClassId() == analogOutputThingClassId) {
    }

    if (!m_reconnectTimer) {
        qCDebug(dcUniPi()) << "Creating reconnect timer";
        m_reconnectTimer = new QTimer(this);
        m_reconnectTimer->setSingleShot(true);
        connect(m_reconnectTimer, &QTimer::timeout, this, &IntegrationPluginUniPi::onReconnectTimer);
    }
}


void IntegrationPluginUniPi::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == digitalOutputThingClassId)  {

        if (action.actionTypeId() == digitalOutputPowerActionTypeId) {
            QString digitalOutputNumber = thing->paramValue(digitalOutputThingCircuitParamTypeId).toString();
            bool stateValue = action.param(digitalOutputPowerActionPowerParamTypeId).value().toBool();
            ThingClassId parentThingClassId = myThings().findById(thing->parentId())->thingClassId();

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
                QUuid requestId = m_neuron->setDigitalOutput(digitalOutputNumber, stateValue);
                if (requestId.isNull()) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                } else {
                    m_asyncActions.insert(requestId, info);
                    connect(info, &ThingActionInfo::aborted, this, [requestId, this](){m_asyncActions.remove(requestId);});
                }
                return;
            } else if (parentThingClassId == neuronExtensionThingClassId) {
                NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->parentId());
                if (!neuronExtension) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                QUuid requestId = neuronExtension->setDigitalOutput(digitalOutputNumber, stateValue);
                if (requestId.isNull()) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                } else {
                    m_asyncActions.insert(requestId, info);
                    connect(info, &ThingActionInfo::aborted, this, [requestId, this](){m_asyncActions.remove(requestId);});
                }
                return;
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
                QUuid requestId = m_neuron->setAnalogOutput(analogOutputNumber, analogValue);
                if (requestId.isNull()) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                } else {
                    m_asyncActions.insert(requestId, info);
                    connect(info, &ThingActionInfo::aborted, this, [requestId, this](){m_asyncActions.remove(requestId);});
                }
                return;
            } else if (parentThingClassId == neuronExtensionThingClassId) {
                NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->parentId());
                if (!neuronExtension) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                QUuid requestId = neuronExtension->setAnalogOutput(analogOutputNumber, analogValue);
                if (requestId.isNull()) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                } else {
                    m_asyncActions.insert(requestId, info);
                    connect(info, &ThingActionInfo::aborted, this, [requestId, this](){m_asyncActions.remove(requestId);});
                }
                return;
            }
        } else {
            qCWarning(dcUniPi()) << "Unhandled ActionTypeId" << action.actionTypeId();
            return info->finish(Thing::ThingErrorActionTypeNotFound);
        }
    } else if (thing->thingClassId() == userLEDThingClassId) {
        if (action.actionTypeId() == userLEDPowerActionTypeId) {
            QString userLED = thing->paramValue(userLEDThingCircuitParamTypeId).toString();
            bool stateValue = action.param(userLEDPowerActionPowerParamTypeId).value().toBool();
            ThingClassId parentThingClassId = myThings().findById(thing->parentId())->thingClassId();

            if (parentThingClassId == neuronThingClassId) {
                if (!m_neuron) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                QUuid requestId = m_neuron->setUserLED(userLED, stateValue);
                if (requestId.isNull()) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                } else {
                    m_asyncActions.insert(requestId, info);
                    connect(info, &ThingActionInfo::aborted, this, [requestId, this](){m_asyncActions.remove(requestId);});
                }
                return;
            } else if (parentThingClassId == neuronExtensionThingClassId) {
                NeuronExtension *neuronExtension = m_neuronExtensions.value(thing->parentId());
                if (!neuronExtension) {
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
                QUuid requestId = neuronExtension->setUserLED(userLED, stateValue);
                if (requestId.isNull()) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                } else {
                    m_asyncActions.insert(requestId, info);
                    connect(info, &ThingActionInfo::aborted, this, [requestId, this](){m_asyncActions.remove(requestId);});
                }
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
        m_neuron->deleteLater();
        m_neuron = nullptr;
    } else if ((thing->thingClassId() == uniPi1ThingClassId) || (thing->thingClassId() == uniPi1LiteThingClassId)) {
        if(m_unipi) {
            m_unipi->deleteLater();
            m_unipi = nullptr;
        }
    }

    if (myThings().isEmpty()) {
        qCDebug(dcUniPi()) << "Stopping timers";
        if (m_reconnectTimer) {
            m_reconnectTimer->stop();
            m_reconnectTimer->deleteLater();
            m_reconnectTimer = nullptr;
        }

        if (m_modbusTCPMaster) {
            m_modbusTCPMaster->disconnectDevice();
            m_modbusTCPMaster->deleteLater();
            m_modbusTCPMaster = nullptr;
        }
    }
}

void IntegrationPluginUniPi::onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value)
{
    qCDebug(dcUniPi()) << "Plugin configuration changed";
    if (paramTypeId == uniPiPluginPortParamTypeId) {
        if (m_modbusTCPMaster) {
            m_modbusTCPMaster->setConnectionParameter(QModbusDevice::NetworkAddressParameter, value.toString());
        }
    }

    if (paramTypeId == uniPiPluginAddressParamTypeId) {
        if (m_modbusTCPMaster) {
            m_modbusTCPMaster->setConnectionParameter(QModbusDevice::NetworkPortParameter, value.toInt());
        }
    }
    /*
               if (paramTypeId == uniPiPluginSerialPortParamTypeId) {
                   if (m_modbusRTUMaster) {
                       m_modbusRTUMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, value.toString());
                   }
               }

               if (paramTypeId == uniPiPluginBaudrateParamTypeId) {
                   if (m_modbusRTUMaster) {
                       m_modbusRTUMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, value.toInt());
                   }
               }

               if (paramTypeId == uniPiPluginParityParamTypeId) {
                   if (m_modbusRTUMaster) {
                       if (value == "Even") {
                           m_modbusRTUMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::Parity::EvenParity);
                       } else {
                           m_modbusRTUMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::Parity::NoParity);
                       }
                   }
               }
           */
}

void IntegrationPluginUniPi::onNeuronConnectionStateChanged(bool state)
{
    Thing *thing = myThings().filterByThingClassId(neuronThingClassId).first();
    if (!thing) {
        qCWarning(dcUniPi()) << "Could not find any Thing associated to Neuron obejct";
        return;
    }
    qCDebug(dcUniPi()) << "Neuron connection state changed" << thing->name() << state;
    thing->setStateValue(m_connectionStateTypeIds.value(thing->thingClassId()), state);
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

void IntegrationPluginUniPi::onNeuronUserLEDStatusChanged(const QString &circuit, bool value)
{
    Thing *neuronThing = myThings().filterByThingClassId(neuronThingClassId).first();
    if (!neuronThing) {
        qCWarning(dcUniPi()) << "Could not find any Thing associated to Neuron obejct";
        return;
    }

    foreach(Thing *thing, myThings().filterByParentId(neuronThing->id())) {
        if (thing->thingClassId() == userLEDThingClassId) {
            if (thing->paramValue(userLEDThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(userLEDPowerStateTypeId, value);
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

void IntegrationPluginUniPi::onRequestExecuted(const QUuid &requestId, bool success)
{
    if (m_asyncActions.contains(requestId)){
        ThingActionInfo *info = m_asyncActions.take(requestId);
        qCDebug(dcUniPi()) << "Request executed, pending requests:" << m_asyncActions.count();
        if (success){
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    }
}

void IntegrationPluginUniPi::onRequestError(const QUuid &requestId, const QString &error)
{
    qCDebug(dcUniPi()) << "Request error:" << error;
    if (m_asyncActions.contains(requestId)){
        ThingActionInfo *info = m_asyncActions.take(requestId);
        info->finish(Thing::ThingErrorHardwareNotAvailable, error);
    }
}

void IntegrationPluginUniPi::onNeuronExtensionUserLEDStatusChanged(const QString &circuit, bool value)
{
    NeuronExtension *neuronExtension = static_cast<NeuronExtension *>(sender());

    foreach(Thing *thing, myThings().filterByParentId(m_neuronExtensions.key(neuronExtension))) {
        if (thing->thingClassId() == userLEDThingClassId) {
            if (thing->paramValue(userLEDThingCircuitParamTypeId).toString() == circuit) {

                thing->setStateValue(userLEDPowerStateTypeId, value);
                return;
            }
        }
    }
}

void IntegrationPluginUniPi::onReconnectTimer()
{
    /*if(m_modbusRTUMaster) {
                   if (m_modbusRTUMaster->state() != QModbusDevice::State::ConnectedState) {
                       if (!m_modbusRTUMaster->connectDevice()) {
                           qCWarning(dcUniPi()) << "Reconnecing to modbus RTU master failed";
                           if (m_reconnectTimer) {
                               qCDebug(dcUniPi()) << "     - Starting reconnect timer";
                               m_reconnectTimer->start(5000);
                           }
                       }
                   }
               }*/
    if(m_modbusTCPMaster) {
        if (m_modbusTCPMaster->state() != QModbusDevice::State::ConnectedState) {
            if (!m_modbusTCPMaster->connectDevice()) {
                qCWarning(dcUniPi()) << "Reconnecing to modbus TCP master failed, trying again in 5 seconds";
                if (m_reconnectTimer) {
                    qCDebug(dcUniPi()) << "     - Starting reconnect timer";
                    m_reconnectTimer->start(5000);
                }
            }
        }
    }
}

void IntegrationPluginUniPi::onModbusTCPStateChanged(QModbusDevice::State state)
{
    bool connected = (state == QModbusDevice::State::ConnectedState);
    qCDebug(dcUniPi()) << "Modbus TCP status changed:" << state;

    if (!connected) {
        //try to reconnect in 10 seconds
        if (m_reconnectTimer) {
            qCDebug(dcUniPi()) << "     - Starting reconnect timer";
            m_reconnectTimer->start(5000);
        }
    }
}

void IntegrationPluginUniPi::onModbusRTUStateChanged(QModbusDevice::State state)
{
    bool connected = (state == QModbusDevice::State::ConnectedState);
    qCDebug(dcUniPi()) << "Modbus RTU status changed:" << state;

    if (!connected) {
        //try to reconnect in 10 seconds
        if (m_reconnectTimer) {
            qCDebug(dcUniPi()) << "     - Starting reconnect timer";
            m_reconnectTimer->start(5000);
        }
    }
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
