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

#ifndef INTEGRATIONPLUGINBGETECH_H
#define INTEGRATIONPLUGINBGETECH_H

#include "integrations/integrationplugin.h"
#include "plugintimer.h"

#include "../modbus/modbusrtumaster.h"

#include <QObject>
#include <QDateTime>
#include <QSerialPortInfo>

class IntegrationPluginBGetech : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginbgetech.json")
    Q_INTERFACES(IntegrationPlugin)

public:

    explicit IntegrationPluginBGetech();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;
    void thingRemoved(Thing *thing) override;

private:

    enum InputRegisters {
        Phase1ToNeutralVolts   = 1,
        Phase2ToNeutralVolts   = 3,
        Phase3ToNeutralVolts   = 5,
        Phase1Current          = 7,
        Phase2Current          = 9,
        Phase3Current          = 11,
        Phase1Power            = 13,
        Phase2Power            = 15,
        Phase3Power            = 17,
        Phase1ApparentPower    = 19,
        Phase2ApparentPower    = 21,
        Phase3ApparentPower    = 23,
        Phase1ReactivePower    = 25,
        Phase2ReactivePower    = 27,
        Phase3ReactivePower    = 29,
        Phase1PowerFactor      = 31,
        Phase2PowerFactor      = 33,
        Phase3PowerFactor      = 35,
        Phase1Angle            = 37,
        Phase2Angle            = 39,
        Phase3Angle            = 41,
        Frequency            = 71,
        ImportActiveEnergy   = 73,
        ExportActiveEnergy   = 75,
        ImportReactiveEnergy = 77,
        ExportReactiveEnergy = 79,
        TotalActiveEnergy    = 343,
        TotalReactiveEnergy  = 345
    };
    Q_ENUM(InputRegisters)

    enum HoldingRegisters {
        RelayPulseWidth     = 13,
        NetworkParityStop   = 19,
        NetworkPortNode     = 21,
        NetworkBaudRate     = 29,
        Pulse1Output        = 87,
        Pulse1Constant      = 63761,
        MeasurementMode     = 63776
    };
    Q_ENUM(HoldingRegisters)

    QHash<Thing *, ModbusRTUMaster *> m_connections;
    PluginTimer *m_pluginTimer = nullptr;
    QHash<QUuid, ThingActionInfo *> m_asyncActions;

    void readHoldingRegister(Thing *thing);
    void readInputRegister(Thing *thing);

private slots:
    void onConnectionStateChanged(bool status);
    void onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value);
    void onReceivedInputRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value);

    void onWriteRequestExecuted(const QUuid &requestId, bool success);
    void onWriteRequestError(const QUuid &requestId, const QString &error);
};

#endif // INTEGRATIONPLUGINWALLBE_H
