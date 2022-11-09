/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#include "integrationplugine3dc.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>
#include "plugininfo.h"


IntegrationPluginTemplate::IntegrationPluginTemplate()
{
    // Connection Params
    m_connectionIpParamTypeIds.insert(e3dcConnectionThingClassId, e3dcConnectionThingIpAddressParamTypeId);
    m_connectionPortParamTypeIds.insert(e3dcConnectionThingClassId, e3dcConnectionThingPortParamTypeId);
    m_connectionMacAddressParamTypeIds.insert(e3dcConnectionThingClassId, e3dcConnectionThingMacAddressParamTypeId);
    m_connectionSlaveIdParamTypeIds.insert(e3dcConnectionThingClassId, e3dcConnectionThingSlaveIdParamTypeId);

    // Connection state
    m_connectedStateTypeIds.insert(e3dcConnectionThingClassId, e3dcConnectionConnectedStateTypeId);

    // Child things
    m_connectedStateTypeIds.insert(e3dcSmartmeterThingClassId, e3dcSmartmeterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(e3dcInverterThingClassId, e3dcInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(e3dcBatteryThingClassId, e3dcBatteryConnectedStateTypeId);
    m_connectedStateTypeIds.insert(e3dcWallboxThingClassId, e3dcWallboxConnectedStateTypeId);

    // Params
        //model
    m_modelIdParamTypeIds.insert(e3dcInverterThingClassId, e3dcInverterThingModelIdParamTypeId);

}

// find all devices in the network and display them. User needs to choose the correct device
void IntegrationPluginTemplate::discoverThings(ThingDiscoveryInfo *info)
{
    // Check if discovery is possible
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcE3dc()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    // do the discovery, find the hostname/Ipadress and the mac adress and display them
    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){

        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
            qCDebug(dcE3dc()) << "Found" << networkDeviceInfo;
            QString title;
            if (networkDeviceInfo.hostName().isEmpty()) {
                title = networkDeviceInfo.address().toString();
            } else {
                title = networkDeviceInfo.hostName() + " (" + networkDeviceInfo.address().toString() + ")";
            }

            QString description;
            if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                description = networkDeviceInfo.macAddress();
            } else {
                description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
            }

            qCDebug(dcE3dc()) << networkDeviceInfo;
            ThingDescriptor descriptor( info->thingClassId() , title, description);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(m_connectionMacAddressParamTypeIds.value(info->thingClassId()), networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcE3dc()) << "This thing already exists in the system." << existingThings.first() << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }



            ParamList params;
            params << Param(m_connectionIpParamTypeIds.value(info->thingClassId()), networkDeviceInfo.address().toString());
            params << Param(m_connectionMacAddressParamTypeIds.value(info->thingClassId()), networkDeviceInfo.macAddress());
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        // Discovery done
        info->finish(Thing::ThingErrorNoError);
    });


}

// Setup Thing (will always be invoked if a Thing gets added)
void IntegrationPluginTemplate::setupThing(ThingSetupInfo *info)
{

    // Identifiy which ThingClass the Thing belongs to and start the coresponding setupDevice function
    Thing *thing = info->thing();
    qCWarning(dcE3dc()) << "Setup" << thing << "with params: " << thing->params();

    if (thing->thingClassId() == e3dcConnectionThingClassId )
    {
        setupConnection(info);

    }else if (thing->thingClassId() == e3dcInverterThingClassId) {
       Thing *parentThing = myThings().findById(thing->parentId());
       if (parentThing->setupStatus() == Thing::ThingSetupStatusComplete) {
           setupInverter(info);
       } else {
           connect(parentThing, &Thing::setupStatusChanged, info, [this, info] {
               // if for some reason the Connection was not established yet, wait until the setupStatus changed
               setupInverter(info);
           });
       }
    }else if (thing->thingClassId() == e3dcWallboxThingClassId) {
       Thing *parentThing = myThings().findById(thing->parentId());
       if (parentThing->setupStatus() == Thing::ThingSetupStatusComplete) {
           setupWallbox(info);
       } else {
           connect(parentThing, &Thing::setupStatusChanged, info, [this, info] {
               setupWallbox(info);
           });
       }
    }else if (thing->thingClassId() == e3dcBatteryThingClassId) {
       Thing *parentThing = myThings().findById(thing->parentId());
       if (parentThing->setupStatus() == Thing::ThingSetupStatusComplete) {
           setupBattery(info);
       } else {
           connect(parentThing, &Thing::setupStatusChanged, info, [this, info] {
               setupBattery(info);
           });
       }

    }else if (thing->thingClassId() == e3dcSmartmeterThingClassId) {
       Thing *parentThing = myThings().findById(thing->parentId());
       if (parentThing->setupStatus() == Thing::ThingSetupStatusComplete) {
           setupSmartMeter(info);
       } else {
           connect(parentThing, &Thing::setupStatusChanged, info, [this, info] {
               setupSmartMeter(info);
           });
       }

   }else{
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
    }

}


// setup the Time and every Interval update YOUR values, so wallboxes only update the wallbox values
void IntegrationPluginTemplate::postSetupThing(Thing *thing)
{

    // setup a timer
    if (!m_pluginTimer){

        qCDebug(dcE3dc()) << "Starting plugin timer";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
        // everytime the timer is reached use the onPluginTimer function to "update" the devices
        connect(m_pluginTimer, &PluginTimer::timeout, this, &IntegrationPluginTemplate::onPluginTimer);
    }

    // Check if the Connection still exists
    if (m_e3DCConnections.contains(thing->id())) {
        TCP_ModbusConnection *TCPconnection = m_e3DCConnections.value(thing->id());
        if (!TCPconnection) {

            qCWarning(dcE3dc()) << "TCP_Connection not found for" << thing;
            return;
        }

        // if thing is either an inverter, a battery or a Wallbox
        // update the devices once.
        } else if (m_e3DCInverters.contains(thing)) {
            TCP_ModbusConnection *parentConnection =  m_e3DCConnections.value(thing->parentId());
            parentConnection->updateInverter(m_e3DCInverters.value(thing) );
        } else if (m_e3DCWallboxes.contains(thing)) {
            TCP_ModbusConnection *parentConnection =  m_e3DCConnections.value(thing->parentId());
            parentConnection->updateWallbox(m_e3DCWallboxes.value(thing));
        } else if (m_e3DCBatteries.contains(thing)) {
            TCP_ModbusConnection *parentConnection =  m_e3DCConnections.value(thing->parentId());
            parentConnection->updateBattery(m_e3DCBatteries.value(thing));
        } else if (m_e3DCSmartmeter.contains(thing)) {
            TCP_ModbusConnection *parentConnection =  m_e3DCConnections.value(thing->parentId());
            parentConnection->updateSmartmeter(m_e3DCSmartmeter.value(thing));

        }  else {
            Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
        }



}

// remove the Connection since it is the parent Object it will delete all of them
// children can be removed on their own aswell though
void IntegrationPluginTemplate::thingRemoved(Thing *thing)
{

    // we delete the Connection.
    // Since it is the Parent of the others, its also going to delete its children
    if (thing->thingClassId() == e3dcConnectionThingClassId && m_e3DCConnections.contains(thing->id())) {
        TCP_ModbusConnection *connection = m_e3DCConnections.take(thing->id());
        delete connection;
    }

    // We can also just delete one of its children
    if ((thing->thingClassId() == e3dcInverterThingClassId) && m_e3DCInverters.contains(thing)) {
        e3dcInverter *inverter = m_e3DCInverters.take(thing);
        delete inverter;
    }

    if ((thing->thingClassId() == e3dcBatteryThingClassId) && m_e3DCBatteries.contains(thing)) {
        e3dcBattery *batteries = m_e3DCBatteries.take(thing);
        delete batteries;
    }

    if ((thing->thingClassId() == e3dcWallboxThingClassId) && m_e3DCWallboxes.contains(thing)) {
        e3dcWallbox *wallbox = m_e3DCWallboxes.take(thing);
        delete wallbox;
    }

    if ((thing->thingClassId() == e3dcSmartmeterThingClassId) && m_e3DCSmartmeter.contains(thing)) {
        e3dcSmartMeter *smartmeter = m_e3DCSmartmeter.take(thing);
        delete smartmeter;
    }



    // obviously you also need to delete the Timer
    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

// update function everytime the PluginTimer sends the interval signal
void IntegrationPluginTemplate::onPluginTimer()
{


        foreach (Thing *thing, myThings()){

            Thing *parentThing = myThings().findById(thing->parentId());
            if (!parentThing){
                qCDebug(dcE3dc()) << "Thing does not have a parent or maybe it is the parent node itself";
                continue;
            }
            // get the Tcp connection of its parent
            TCP_ModbusConnection* TcpConnection = m_e3DCConnections.value(thing->parentId());

            // if thing is connected and it is the specific Device update this specific device
            qCDebug(dcE3dc()) << "Child connectionState";
            if (thing->stateValue("connected").toBool() &&  m_e3DCInverters.contains(thing) ) {
                TcpConnection->updateInverter(m_e3DCInverters.value(thing));
            }

            if (thing->stateValue("connected").toBool() && m_e3DCWallboxes.contains(thing) ) {
                TcpConnection->updateWallbox(m_e3DCWallboxes.value(thing));
            }

            if (thing->stateValue("connected").toBool() && m_e3DCBatteries.contains(thing) ) {
                TcpConnection->updateBattery(m_e3DCBatteries.value(thing));
            }

            if (thing->stateValue("connected").toBool() && m_e3DCSmartmeter.contains(thing) ) {
                TcpConnection->updateSmartmeter(m_e3DCSmartmeter.value(thing));
            }

        }


}

// setup the Modbus Connection, which is responsible of every child Thing (Inverter, WB, Battery)
void IntegrationPluginTemplate::setupConnection(ThingSetupInfo *info)
{

    Thing *thing = info->thing();
    QHostAddress hostAddress = QHostAddress(info->thing()->paramValue(m_connectionIpParamTypeIds.value(thing->thingClassId())).toString());
    int port = info->thing()->paramValue(m_connectionPortParamTypeIds.value(thing->thingClassId())).toInt();
    int slaveId = info->thing()->paramValue(m_connectionSlaveIdParamTypeIds.value(thing->thingClassId())).toInt();


    if (m_e3DCConnections.contains(thing->id())) {
        qCWarning(dcE3dc()) << "Reconfigure TCP_connection with new address" << hostAddress;
        TCP_ModbusConnection *parentConnection = m_e3DCConnections.value(thing->id());
        delete parentConnection;
    }


    // establish a new TCP_ModbusConnection
    TCP_ModbusConnection *TcpConnection = new TCP_ModbusConnection(hostAddress, port, slaveId, this);

    // Update all child things for this connection if the connectionState changed obviously only if the connectionState is true
    connect(TcpConnection, &TCP_ModbusConnection::connectionStateChanged, thing, [this, TcpConnection, thing] (bool connected) {
        qCDebug(dcE3dc()) << TcpConnection << (connected ? "connected" : "disconnected");
        // set connectionState of the parent
        thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), connected);

        // Update connected state of child things
        foreach (Thing *child, myThings().filterByParentId(thing->id())) {
            // set the connected state of all children
            child->setStateValue(m_connectedStateTypeIds.value(child->thingClassId()), connected);

            // Refresh childs if connected is true
            // Do this for every inverter, wallbox or battery
            // this only happens when the TCP_Connection gets established -> meaning its an initalizing read
            if (connected && m_e3DCInverters.contains(child) ) {
                TcpConnection->updateInverter(m_e3DCInverters.value(child));
            }

            if (connected && m_e3DCWallboxes.contains(child) ) {
                TcpConnection->updateWallbox(m_e3DCWallboxes.value(child));
            }

            if (connected && m_e3DCBatteries.contains(child) ) {
                TcpConnection->updateBattery(m_e3DCBatteries.value(child));
            }

            if (connected && m_e3DCSmartmeter.contains(child) ) {
                TcpConnection->updateSmartmeter(m_e3DCSmartmeter.value(child));
            }

        }
    });


    // Note: connectionStateChanged is a signal emitted by the TCPMaster class
    // This will only be casted during setup and later deleted, since the Slot is bounded to info, and this gets deleted after this
    connect(TcpConnection, &TCP_ModbusConnection::connectionStateChanged, info, [this, TcpConnection, info] (bool connected) {
        qCDebug(dcE3dc()) << "E3DC connected changed during setup:" << (connected ? "connected" : "disconnected");
        // only do something if the connectionStateChanged from false -> true
        if (connected) {

            // so we send to 7 register the question, whether or not the Wallbox exists.
            // We count the amount of responses we get and if we have 7 we can make sure that every response came back, and we know how many are active
            // we bound the variables to the TCP_Connection so multiple Connections can be setup.
            connect(TcpConnection, &TCP_ModbusConnection::Countwallbox, info,[this, TcpConnection](bool count, int WBregister){

                if (count){
                    TcpConnection->setWallboxActive(TcpConnection->WallboxActive()+1);
                }else{
                    TcpConnection->setWallboxInactive(TcpConnection->WallboxInactive()+1);
                }
                qCDebug(dcE3dc()) << "We got a respond of Wallbox on Register: " << WBregister;
                // if we got the information of all 7, we can say the discovery is finished
                if( ( TcpConnection->WallboxActive() + TcpConnection->WallboxInactive() ) == TcpConnection->MaximumAmountofWallboxes()){
                    emit TcpConnection->discoveryFinished(true);
                }

            });

            // this connection will only be triggered if startDiscovery is finished
            connect(TcpConnection, &TCP_ModbusConnection::discoveryFinished, info,[this, TcpConnection, info](bool success){
                if (success){

                    qCDebug(dcE3dc()) << "found something atleast";
                    // we can now add the Connection, since we now know that we got atleast a reply back
                    m_e3DCConnections.insert(info->thing()->id(), TcpConnection);
                    info->finish(Thing::ThingErrorNoError);
                    // now we add the children of the parent gateway Connection
                    processDiscoveryResult(info->thing(), TcpConnection);


                }else{

                    qCDebug(dcE3dc()) << " something went wrong during the Wallbox Discovery";
                }


            });
            // try to find out if wallboxes exist
            TcpConnection->startDiscovery();


        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    });

    // if the ThingSetup gets aborted delete the TcpConnection ...
    connect(info, &ThingSetupInfo::aborted, TcpConnection, &TCP_ModbusConnection::deleteLater);
    // ... and remove the Connection from m_e3dcConnections
    connect(TcpConnection, &TCP_ModbusConnection::destroyed, thing, [this, thing] {
        qCDebug(dcE3dc()) << "connection Destroyed";
        m_e3DCConnections.remove(thing->id());
        m_e3DCBatteries.clear();
        m_e3DCInverters.clear();
        m_e3DCSmartmeter.clear();
        m_e3DCWallboxes.clear();

    });

    if (!TcpConnection->connectDevice()) {
        qCWarning(dcE3dc()) << "Error connecting to e3dc device" << thing->name();
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        TcpConnection->deleteLater();
        return;
    }




}

// when the things get "autoAppeared" by the Connection we always going to Setup an Inverter, a Battery and a Smartmeter
// After the "ThingSetup" method of the Inverter Thing gets called, this setupInverter function is called to set everything up
void IntegrationPluginTemplate::setupInverter(ThingSetupInfo *info){


    Thing *thing = info->thing();

    TCP_ModbusConnection * TCPconnection = m_e3DCConnections.value(thing->parentId());
    // check if parent connection exists
    if (!TCPconnection) {
        qCWarning(dcE3dc()) << "Could not find E3dc parent connection for" << thing << "Deleting the Thing if exists";
        if(m_e3DCInverters.contains(thing)){
            e3dcInverter *inverter = m_e3DCInverters.take(thing);
            delete inverter;
        }
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }
    // we create an e3dcInverter Object, where we "save" the attributes/values we want to read from the Gateway
    e3dcInverter* inverter = new e3dcInverter();



//     everytime the Inverter gets updated, also update the thing
    connect(inverter, &e3dcInverter::currentPowerChanged, this, [thing](float currentPower){
        qCDebug(dcE3dc()) << thing << "Inverter current Power changed" << currentPower << "W";
        thing->setStateValue(e3dcInverterCurrentPowerStateTypeId, currentPower);
    });

    connect(inverter, &e3dcInverter::networkPointPowerChanged, this, [thing](float networkPointPower){
        qCDebug(dcE3dc()) << thing << "Inverter NetworkPointPower changed" << networkPointPower << "W";
        thing->setStateValue(e3dcInverterNetworkPointPowerStateTypeId, networkPointPower);
    });

    if(!m_e3DCInverters.contains(thing)){
        m_e3DCInverters.insert(thing, inverter);
    }
    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
    info->finish(Thing::ThingErrorNoError);
}

// same principal as in the inverter case
void IntegrationPluginTemplate::setupWallbox(ThingSetupInfo *info){

    Thing *thing = info->thing();
    TCP_ModbusConnection * TCPconnection = m_e3DCConnections.value(thing->parentId());
    // check if parent connection exists
    if (!TCPconnection) {
        qCWarning(dcE3dc()) << "Could not find E3dc parent connection for" << thing<< "Deleting the Thing";
        e3dcWallbox *wallbox = m_e3DCWallboxes.take(thing);
        delete wallbox;
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }


    e3dcWallbox* wallbox = new e3dcWallbox();
    m_e3DCWallboxes.insert(thing, wallbox);
    connect(wallbox, &e3dcWallbox::currentPowerChanged, this, [thing](float currentPower){
        qCDebug(dcE3dc()) << thing << "WB current Power changed" << currentPower << "W";
        thing->setStateValue(e3dcWallboxCurrentPowerStateTypeId, currentPower);
    });



    info->finish(Thing::ThingErrorNoError);
    return;

}

// same principal as in the inverter case
void IntegrationPluginTemplate::setupSmartMeter(ThingSetupInfo *info){

    Thing *thing = info->thing();
    TCP_ModbusConnection * TCPconnection = m_e3DCConnections.value(thing->parentId());
    // check if parent connection exists
    if (!TCPconnection) {
        qCWarning(dcE3dc()) << "Could not find E3dc parent connection for" << thing<< "Deleting the Thing";
        e3dcSmartMeter *smartmeter = m_e3DCSmartmeter.take(thing);
        delete smartmeter;
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }


    e3dcSmartMeter* smartmeter = new e3dcSmartMeter();


    connect(smartmeter, &e3dcSmartMeter::networkPointPowerChanged, this, [thing](float networkpointPower){
        qCDebug(dcE3dc()) << thing << "smartmeter networkpoint Power changed" << networkpointPower << "W";
        thing->setStateValue(e3dcSmartmeterCurrentPowerStateTypeId, networkpointPower);
    });


    m_e3DCSmartmeter.insert(thing, smartmeter);
    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
    info->finish(Thing::ThingErrorNoError);
    return;

}

// same principal as in the inverter case
void IntegrationPluginTemplate::setupBattery(ThingSetupInfo *info){

    Thing *thing = info->thing();
    TCP_ModbusConnection * TCPconnection = m_e3DCConnections.value(thing->parentId());
    // check if parent connection exists
    if (!TCPconnection) {
        qCWarning(dcE3dc()) << "Could not find E3dc parent connection for" << thing<< "Deleting the Thing";
        e3dcBattery *battery = m_e3DCBatteries.take(thing);
        delete battery;
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    e3dcBattery* battery = new e3dcBattery();


    connect(battery, &e3dcBattery::currentPowerChanged, this, [thing](float currentPower){
        qCDebug(dcE3dc()) << thing << "Battery current Power changed" << currentPower << "W";
        thing->setStateValue(e3dcBatteryCurrentPowerStateTypeId, currentPower);
    });

    connect(battery, &e3dcBattery::SOCChanged, this, [thing](float SOC){
        qCDebug(dcE3dc()) << thing << "WB SOC changed" << SOC << "%";
        thing->setStateValue(e3dcBatteryBatteryLevelStateTypeId, SOC);
    });

    m_e3DCBatteries.insert(thing, battery);
    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
    info->finish(Thing::ThingErrorNoError);
    return;


}


void IntegrationPluginTemplate::processDiscoveryResult(Thing *thing, TCP_ModbusConnection *TcpConnection)
{

   qCDebug(dcE3dc()) << "Processing discovery result from" << thing->name() << TcpConnection;

   // add Wallbox(es)
   // We can not test Wallbox as of right now, thats why whe disable the adding of Wallboxes for now.
   // When we will test wallboxes we will use this
//   if (TcpConnection->WallboxActive() >= 1 && myThings().filterByThingClassId(e3dcWallboxThingClassId).count() < 1){
//       // only add a Wallbox if ONE was found
//       qCDebug(dcE3dc()) << "Wallbox was found and will be setup " ;
//       ThingDescriptor WBdescriptor(e3dcWallboxThingClassId);
//       WBdescriptor.setParentId(thing->id());
//       WBdescriptor.setTitle("e3dc Wallbox");
//       // Add paramTypes
//       ParamList WBparams;
//       WBparams.append(Param(e3dcWallboxThingModelIdParamTypeId, "e3dc_Wallbox"));
//       WBdescriptor.setParams(WBparams);
//       emit autoThingsAppeared({WBdescriptor});
//   }

   // We assume that there is always an Inverter // add Inverter
   // We also only want one Inverter Thing, since we can only read the collective current Power
   // If e3dc would change things this might changes aswell
   if (myThings().filterByThingClassId(e3dcInverterThingClassId).count() < 1){
       qCDebug(dcE3dc()) << "Inverter will be setup " ;
       ThingDescriptor Invdescriptor(e3dcInverterThingClassId);
       Invdescriptor.setParentId(thing->id());
       Invdescriptor.setTitle("e3dc Inverter");
       // Add paramTypes
       ParamList Invparams;
       Invparams.append(Param(e3dcInverterThingModelIdParamTypeId, "e3dc_Inverter"));
       Invdescriptor.setParams(Invparams);
       emit autoThingsAppeared({Invdescriptor});
   }

   if(myThings().filterByThingClassId(e3dcBatteryThingClassId).count() < 1){
       // We assume there is always a battery // add Battery
       qCDebug(dcE3dc()) << "Battery will be setup " ;
       ThingDescriptor Batdescriptor(e3dcBatteryThingClassId);
       Batdescriptor.setParentId(thing->id());
       Batdescriptor.setTitle("e3dc Battery");
       // Add paramTypes
       ParamList Batparams;
       Batparams.append(Param( e3dcBatteryThingModelIdParamTypeId, "e3dc_Battery"));
       Batdescriptor.setParams(Batparams);
       emit autoThingsAppeared({Batdescriptor});
   }

   if(myThings().filterByThingClassId(e3dcSmartmeterThingClassId).count() < 1){
       // We assume there is always a battery // add Battery
       qCDebug(dcE3dc()) << "SmartMeter will be setup: ";
       ThingDescriptor SMdescriptor(e3dcSmartmeterThingClassId);
       SMdescriptor.setParentId(thing->id());
       SMdescriptor.setTitle("e3dc SmartMeter");
       // Add paramTypes
       ParamList SMparams;
       SMparams.append(Param(e3dcSmartmeterThingModelIdParamTypeId, "e3dc_SmartMeter"));
       SMdescriptor.setParams(SMparams);
       emit autoThingsAppeared({SMdescriptor});


   }



}
