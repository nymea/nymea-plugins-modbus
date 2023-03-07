/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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

#include "integrationpluginsenseair.h"
#include "plugininfo.h"
#include "hardwaremanager.h"
#include "hardware/modbus/modbusrtuhardwareresource.h"

IntegrationPluginSenseAir::IntegrationPluginSenseAir()
{

}

void IntegrationPluginSenseAir::discoverThings(ThingDiscoveryInfo *info)
{
    foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
        qCDebug(dcSenseAir()) << "Found RTU master resource" << modbusMaster;
        if (modbusMaster->connected() && modbusMaster->baudrate() == 9600 && modbusMaster->dataBits() == 8 && modbusMaster->stopBits() == 1 && modbusMaster->parity() == QSerialPort::NoParity) {
            ParamList parameters;
            ThingDescriptor thingDescriptor(s8ThingClassId, "Modbus RTU master", modbusMaster->serialPort());
            parameters.append(Param(s8ThingRtuMasterParamTypeId, modbusMaster->modbusUuid()));
            thingDescriptor.setParams(parameters);
            info->addThingDescriptor(thingDescriptor);
        } else {
            qCWarning(dcSenseAir()) << "Found configured resource" << modbusMaster << "but it is not connected. Skipping.";
        }
    }

    QString displayMessage;
    if (info->thingDescriptors().count() == 0) {
        displayMessage = QT_TR_NOOP("Please set up a Modbus RTU master with baudrate 9600, 8 data bits, 1 stop bits and no parity first.");
    }

    info->finish(Thing::ThingErrorNoError, displayMessage);
}

void IntegrationPluginSenseAir::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcSenseAir()) << "Setup" << thing << thing->params();


    if (m_s8Connections.contains(thing)) {
        qCDebug(dcSenseAir()) << "Reconfiguring existing thing" << thing->name();
        m_s8Connections.take(thing)->deleteLater();
    }

    ModbusRtuMaster *master = hardwareManager()->modbusRtuResource()->getModbusRtuMaster(thing->paramValue(s8ThingRtuMasterParamTypeId).toUuid());
    if (!master) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The Modbus RTU master is not available."));
        return;
    }

    SenseAirS8ModbusRtuConnection *s8Connection = new SenseAirS8ModbusRtuConnection(master, 0xfe, this);
    connect(info, &ThingSetupInfo::aborted, s8Connection, &SenseAirS8ModbusRtuConnection::deleteLater);

    connect(s8Connection, &SenseAirS8ModbusRtuConnection::reachableChanged, thing, [s8Connection, thing](bool reachable){
        qCDebug(dcSenseAir()) << "Reachable state changed" << reachable;
        if (reachable) {
            s8Connection->initialize();
        } else {
            thing->setStateValue(s8ConnectedStateTypeId, false);
        }
    });
    connect(s8Connection, &SenseAirS8ModbusRtuConnection::initializationFinished, info, [=](bool success){
        qCDebug(dcSenseAir()) << "Initialisation finished" << success;
        if (success) {
            qCDebug(dcSenseAir()) << "Meter status:" << s8Connection->meterStatus();
            m_s8Connections.insert(thing, s8Connection);
            info->finish(Thing::ThingErrorNoError);
        } else {
            delete s8Connection;
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    });
    connect(s8Connection, &SenseAirS8ModbusRtuConnection::initializationFinished, thing, [=](bool success){
        if (success) {
            thing->setStateValue(s8ConnectedStateTypeId, true);
        }
    });

    connect(s8Connection, &SenseAirS8ModbusRtuConnection::spaceCo2Changed, thing, [=](quint16 spaceCo2){
        qCDebug(dcSenseAir()) << "CO2 changed:" << spaceCo2;
        thing->setStateValue(s8Co2StateTypeId, spaceCo2);
    });

    s8Connection->update();
}

void IntegrationPluginSenseAir::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)
    if (!m_pluginTimer) {
        qCDebug(dcSenseAir()) << "Starting plugin timer...";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(5);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach(SenseAirS8ModbusRtuConnection *connection, m_s8Connections) {
                qCDebug(dcSenseAir()) << "Updating...";
                if (connection->reachable()) {
                    connection->update();
                }
            }
        });

        m_pluginTimer->start();

    }
}

void IntegrationPluginSenseAir::thingRemoved(Thing *thing)
{
    SenseAirS8ModbusRtuConnection *connection = m_s8Connections.take(thing);
    delete connection;


    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}
