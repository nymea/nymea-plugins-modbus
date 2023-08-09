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

#ifndef INTEGRATIONPLUGINMYPV_H
#define INTEGRATIONPLUGINMYPV_H

#include <integrations/integrationplugin.h>
#include <plugintimer.h>

#include <modbustcpmaster.h>

#include <QUdpSocket>
#include <QUuid>

class IntegrationPluginMyPv: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginmypv.json")
    Q_INTERFACES(IntegrationPlugin)


public:
    explicit IntegrationPluginMyPv();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

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
    QHash<Thing *, ModbusTcpMaster *> m_modbusTcpMasters;
    QHash<QUuid, ThingActionInfo *> m_asyncActions;

    void update(Thing *thing);

private slots:
    void onRefreshTimer();

    void onConnectionStateChanged(bool status);
    void onWriteRequestExecuted(QUuid requestId, bool success);
    void onWriteRequestError(QUuid requestId, const QString &error);

    void onReceivedHoldingRegister(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &values);
};

#endif // INTEGRATIONPLUGINMYPV_H

