/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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

#ifndef TCP_MODBUSCONNECTION_H
#define TCP_MODBUSCONNECTION_H

#include <QObject>

#include "../libnymea-modbus/modbusdatautils.h"
#include "../libnymea-modbus/modbustcpmaster.h"
#include "e3dcinverter.h"
#include "e3dcbattery.h"
#include "e3dcwallbox.h"
#include "e3dcsmartmeter.h"

// information about e3DC is provided here: https://community.symcon.de/uploads/short-url/z6Yc7LiO6m9lJt8r5Aif539GbHI.pdf

class TCP_ModbusConnection : public ModbusTCPMaster
{
    Q_OBJECT
public:
    enum Registers {
        RegisterCurrentPower = 40068
    };
    Q_ENUM(Registers);

    enum Error {
        ConnectionNoError = 0,
        ConnectionWBError = 1,
        ConnectionInvError = 2,
        ConnectionBatError = 3
    };
    Q_ENUM(Error);

    // Wallbox_X_CTL gives a bitmask
    enum WallboxDataBitmask: quint16
    {
        ExistenceMask = 1,
        OperationType = 2




    };
    Q_ENUM(WallboxDataBitmask);


    explicit TCP_ModbusConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    ~TCP_ModbusConnection() = default;
    QList<int> activeWallbox();

    // Inverter
    //current Power Inverter [kW] - Address: 40068, Size: 2
    float currentPowerInv() const;
    //current Power Battery [kW] - Address: 40070, Size: 2
    float currentPowerBat() const;
    //current Power Wallbox [kW] - Address: 40078, Size: 2
    float currentPowerWB() const;
    //current Power NetworkPoint [kW] - Address: 40074, Size: 2
    float currentNetworkPointPower() const;
    //current SOC [%] - Address: 40068, Size: 2
    float currentSOC() const;

    // WallboxDiscovery Getter/Setter

    int WallboxActive();
    void setWallboxActive(int count);

    int WallboxInactive();
    void setWallboxInactive(int count);

    int MaximumAmountofWallboxes();


    bool discoveryRunning() const;
    void setDiscoveryRunning(bool discoveryRunning) const;

    virtual void initialize();

    //update
        //inverter data
    void updateCurrentPowerInv(e3dcInverter *inverter);
    void updateNetworkPointPower(e3dcSmartMeter *smartmeter);
        //wallbox data
    void updateCurrentPowerWB(e3dcWallbox* wallbox);
        //battery data
    void updateSOC(e3dcBattery* battery);
    void updateCurrentPowerBat(e3dcBattery* battery);

    //update devices
    void updateInverter(e3dcInverter * inverter);
    void updateBattery(e3dcBattery * battery);
    void updateWallbox(e3dcWallbox * wallbox);
    void updateSmartmeter(e3dcSmartMeter * smartmeter);


public slots:
    void startDiscovery();

signals:

    void initializationFinished();
    void discoveryFinished(bool success);
    void Countwallbox(bool count, int register);


    void currentPowerInvChanged(float currentPower);
    void currentPowerBatChanged(float currentPower);
    void currentPowerWBChanged(float currentPower);
    void currentNetworkPointPowerChanged(float currentNetworkPointPower);
    void currentSOCChanged(float SOC);

protected:
    // read
        //inverter data
    QModbusReply *readCurrentPowerInv();
        // Smartmeter data
    QModbusReply *readNetworkPointPower();
        //wallbox data
    QModbusReply *readCurrentPowerWB();
        //battery data
    QModbusReply *readCurrentPowerBat();
    QModbusReply *readSOC();

    // read Wallbox Control to check if wallbox exist
    QModbusReply *readWallboxControl(int Register);

    float m_currentPowerInv = 0;
    float m_currentPowerBat = 0;
    float m_currentPowerWB = 0;
    float m_SOC = 0;
    float m_networkPointPower = 0;

    // the key is the actual modbus adress
    QList<int> m_activeWallboxes;


private:
    int m_wallboxesActive = 0;
    int m_wallboxesInactive = 0;
    int m_maximumAmountofWallboxes = 7;

    quint16 m_slaveId = 1;
    QVector<QModbusReply *> m_pendingInitReplies;

    void verifyInitFinished();

    TCP_ModbusConnection::Error CheckWallboxes();


};

QDebug operator<<(QDebug debug, TCP_ModbusConnection *tCP_ModbusConnection);

#endif // TCP_MODBUSCONNECTION_H
