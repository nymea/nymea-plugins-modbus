/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by copyright law, and
* remains the property of nymea GmbH. All rights, including reproduction, publication,
* editing and translation, are reserved. The use of this project is subject to the terms of a
* license agreement to be concluded with nymea GmbH in accordance with the terms
* of use of nymea GmbH, available under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the terms of the GNU
* Lesser General Public License as published by the Free Software Foundation; version 3.
* this project is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License along with this project.
* If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under contact@nymea.io
* or see our FAQ/Licensing Information on https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef DEVICEPLUGINMYPV_H
#define DEVICEPLUGINMYPV_H

#include "devices/deviceplugin.h"
#include "plugintimer.h"
#include "modbustcpmaster.h"

#include <QUdpSocket>

class DevicePluginMyPv: public DevicePlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.DevicePlugin" FILE "devicepluginmypv.json")
    Q_INTERFACES(DevicePlugin)


public:
    explicit DevicePluginMyPv();

    void discoverDevices(DeviceDiscoveryInfo *info) override;
    void setupDevice(DeviceSetupInfo *info) override;
    void postSetupDevice(Device *device) override;
    void deviceRemoved(Device *device) override;
    void executeAction(DeviceActionInfo *info) override;

private:

    enum ElwaModbusRegisters {
        Power= 1000,
        WaterTemperature = 1001,
        TargetWaterTemperature = 1002,
        Status = 1003,
        ManuelStart = 1012
    };

    enum ElwaStatus {
        Heating = 2,
        Standby = 3,
        Boosted = 4,
        HeatFinished = 5,
        Setup = 9,
        ErrorOvertempFuseBlown = 201,
        ErrorOvertempMeasured = 202,
        ErrorOvertempElectronics = 203,
        ErrorHardwareFault = 204,
        ErrorTempSensor = 205
    };

    PluginTimer *m_refreshTimer = nullptr;
    QHash<Device *, ModbusTCPMaster *> m_modbusTcpMasters;

    void update(Device *device);

private slots:
    void onRefreshTimer();
};

#endif // DEVICEPLUGINMYPV_H

