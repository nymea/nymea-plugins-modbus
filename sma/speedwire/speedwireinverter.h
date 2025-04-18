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

#ifndef SPEEDWIREINVERTER_H
#define SPEEDWIREINVERTER_H

#include <QObject>
#include <QQueue>

#include <network/networkdevicemonitor.h>

#include "sma.h"
#include "speedwire.h"
#include "speedwireinterface.h"
#include "speedwireinverterreply.h"
#include "speedwireinverterrequest.h"


class SpeedwireInverter : public QObject
{
    Q_OBJECT
public:
    enum State {
        StateIdle,
        StateDisconnected,
        StateInitializing,
        StateLogin,
        StateGetInformation,
        StateQueryData
    };
    Q_ENUM(State)

    explicit SpeedwireInverter(SpeedwireInterface *speedwireInterface, NetworkDeviceMonitor *monitor, quint16 modelId, quint32 serialNumber, QObject *parent = nullptr);
    explicit SpeedwireInverter(SpeedwireInterface *speedwireInterface, const QHostAddress &address, quint16 modelId, quint32 serialNumber, QObject *parent = nullptr);

    NetworkDeviceMonitor *monitor() const;

    State state() const;

    bool reachable() const;

    Sma::DeviceClass deviceClass() const;
    QString modelName() const;

    double totalAcPower() const;

    double gridFrequency() const;

    double totalEnergyProduced() const;
    double todayEnergyProduced() const;

    double voltageAcPhase1() const;
    double voltageAcPhase2() const;
    double voltageAcPhase3() const;

    double currentAcPhase1() const;
    double currentAcPhase2() const;
    double currentAcPhase3() const;

    double powerAcPhase1() const;
    double powerAcPhase2() const;
    double powerAcPhase3() const;

    double powerDcMpp1() const;
    double powerDcMpp2() const;

    double voltageDcMpp1() const;
    double voltageDcMpp2() const;

    double currentDcMpp1() const;
    double currentDcMpp2() const;

    bool batteryAvailable() const;
    double batteryCycles() const;
    double batteryCharge() const;
    double batteryTemperature() const;
    double batteryCurrent() const;
    double batteryVoltage() const;

    // Query methods
    SpeedwireInverterReply *sendIdentifyRequest();
    SpeedwireInverterReply *sendLoginRequest(const QString &password = "0000", bool loginAsUser = true);
    SpeedwireInverterReply *sendLogoutRequest();
    SpeedwireInverterReply *sendSoftwareVersionRequest();
    SpeedwireInverterReply *sendDeviceTypeRequest();
    SpeedwireInverterReply *sendBatteryInfoRequest();

    // Start connecting
    void startConnecting(const QString &password = "0000");

public slots:
    void refresh();

signals:
    void reachableChanged(bool reachable);
    void loginFinished(bool success);
    void stateChanged(State state);
    void valuesUpdated();
    void batteryAvailableChanged(bool available);
    void batteryValuesUpdated();

private:
    SpeedwireInterface *m_speedwireInterface = nullptr;
    NetworkDeviceMonitor *m_monitor = nullptr;
    QHostAddress m_address;
    QString m_password;

    bool m_initialized = false;
    quint16 m_modelId = 0;
    quint32 m_serialNumber = 0;

    bool m_reachable = false;
    State m_state = StateDisconnected;
    quint8 m_packetId = 1;

    bool m_deviceInformationFetched = false;

    SpeedwireInverterReply *m_currentReply = nullptr;
    QQueue<SpeedwireInverterReply *> m_replyQueue;

    // Properties
    Sma::DeviceClass m_deviceClass = Sma::DeviceClassUnknown;
    QString m_modelName;
    QString m_softwareVersion;

    double m_totalAcPower = 0;
    double m_totalEnergyProduced = 0;
    double m_todayEnergyProduced = 0;

    double m_gridFrequency = 0;

    double m_voltageAcPhase1 = 0;
    double m_voltageAcPhase2 = 0;
    double m_voltageAcPhase3 = 0;

    double m_currentAcPhase1 = 0;
    double m_currentAcPhase2 = 0;
    double m_currentAcPhase3 = 0;

    double m_powerAcPhase1 = 0;
    double m_powerAcPhase2 = 0;
    double m_powerAcPhase3 = 0;

    double m_powerDcMpp1 = 0;
    double m_powerDcMpp2 = 0;

    double m_voltageDcMpp1 = 0;
    double m_voltageDcMpp2 = 0;

    double m_currentDcMpp1 = 0;
    double m_currentDcMpp2 = 0;

    bool m_batteryAvailable = false;

    double m_batteryCycles = 0;
    double m_batteryVoltage = 0;
    double m_batteryCurrent = 0;
    double m_batteryCharge = 0;
    double m_batteryTemperature = 0;

    void setState(State state);

    void sendNextReply();
    SpeedwireInverterReply *createReply(const SpeedwireInverterRequest &request);

    // Request builder function
    void buildDefaultHeader(QDataStream &stream, quint16 payloadSize = 38, quint8 control = 0xa0);
    void buildPacket(QDataStream &stream, quint32 command, quint16 packetId);

    // Send generic request for internal use
    SpeedwireInverterReply *sendQueryRequest(Speedwire::Command command, quint32 firstWord, quint32 secondWord);

    // Response process methods
    void processSoftwareVersionResponse(const QByteArray &response);
    void processDeviceTypeResponse(const QByteArray &response);
    void processAcPowerResponse(const QByteArray &response);
    void processAcVoltageCurrentResponse(const QByteArray &response);
    void processAcTotalPowerResponse(const QByteArray &response);
    void processDcPowerResponse(const QByteArray &response);
    void processDcVoltageCurrentResponse(const QByteArray &response);
    void processEnergyProductionResponse(const QByteArray &response);
    void processGridFrequencyResponse(const QByteArray &response);
    void processBatteryInfoResponse(const QByteArray &response);
    void processBatteryChargeResponse(const QByteArray &response);
    void processInverterStatusResponse(const QByteArray &response);

    void readUntilEndOfMeasurement(QDataStream &stream);
    double readValue(quint32 value, double divisor = 1.0);
    double readValue(qint32 value, double divisor = 1.0);

    void setReachable(bool reachable);
    void setBatteryAvailable(bool available);

private slots:
    void processData(const QHostAddress &senderAddress, quint16 senderPort, const QByteArray &data);

    void onReplyTimeout();
    void onReplyFinished();

};

#endif // SPEEDWIREINVERTER_H
