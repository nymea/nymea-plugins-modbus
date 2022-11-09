#include "schneiderwallbox.h"
#include "extern-plugininfo.h"

SchneiderWallbox::SchneiderWallbox(SchneiderWallboxModbusTcpConnection *modbusTcpConnection, QObject *parent) :
    QObject{parent},
    m_modbusTcpConnection{modbusTcpConnection}
{
    connect(modbusTcpConnection, &SchneiderWallboxModbusTcpConnection::cpwStateChanged, this, [this](SchneiderWallboxModbusTcpConnection::CPWState cpwState){
        m_cpwState = cpwState;
    });
    connect(modbusTcpConnection, &SchneiderWallboxModbusTcpConnection::remoteCommandStatusChanged, this, [this](quint16 remoteCommandStatus){
        m_remoteCommandStatus = remoteCommandStatus;
    });
    connect(modbusTcpConnection, &SchneiderWallboxModbusTcpConnection::maxIntensitySocketChanged, this, [this](quint16 maxIntensitySocket){
        m_chargeCurrent = maxIntensitySocket;
    });
    connect(modbusTcpConnection, &SchneiderWallboxModbusTcpConnection::remoteControllerLifeBitChanged, this, [this](quint16 remoteControllerLifeBit){
        qCDebug(dcSchneiderElectric()) << "Life bit register changed to" << remoteControllerLifeBit;
        m_recievedLifeBitRegisterValue = remoteControllerLifeBit;
    });
    connect(modbusTcpConnection, &SchneiderWallboxModbusTcpConnection::stationIntensityPhaseXChanged, this, [this](double stationIntensityPhaseX){
        m_stationIntensityPhaseX = stationIntensityPhaseX;
    });
    connect(modbusTcpConnection, &SchneiderWallboxModbusTcpConnection::stationIntensityPhase2Changed, this, [this](double stationIntensityPhase2){
        m_stationIntensityPhase2 = stationIntensityPhase2;
    });
    connect(modbusTcpConnection, &SchneiderWallboxModbusTcpConnection::stationIntensityPhase3Changed, this, [this](double stationIntensityPhase3){
        m_stationIntensityPhase3 = stationIntensityPhase3;
    });
    connect(modbusTcpConnection, &SchneiderWallboxModbusTcpConnection::degradedModeChanged, this, [this](quint16 degradedMode){
        m_degradedMode = degradedMode;
    });
}

// When deleting the object, make sure to end remote mode.
SchneiderWallbox::~SchneiderWallbox()
{
    if (m_recievedLifeBitRegisterValue != 2) {
        qCDebug(dcSchneiderElectric()) << "Terminating remote mode, sending life bit 2.";
        QModbusReply *reply = m_modbusTcpConnection->setRemoteControllerLifeBit(2);
        if (!reply) {
            qCWarning(dcSchneiderElectric()) << "Sending life bit failed because the reply could not be created.";
            m_errorOccured = true;
            return;
        }
        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
            qCWarning(dcSchneiderElectric()) << "Modbus reply error occurred while sending life bit" << error << reply->errorString();
            emit reply->finished(); // To make sure it will be deleted
        });
    }

    qCDebug(dcSchneiderElectric()) << "Deleting SchneiderWallbox object for device with address" << m_modbusTcpConnection->hostAddress();
    delete m_modbusTcpConnection;
}


SchneiderWallboxModbusTcpConnection *SchneiderWallbox::modbusTcpConnection()
{
    return m_modbusTcpConnection;
}

void SchneiderWallbox::update()
{
    if (!m_modbusTcpConnection->connected()) {
        return;
    }

    int phaseCount{1};
    double currentPower = m_stationIntensityPhaseX * 230;
    if (m_stationIntensityPhase2 > 0.1) {
        phaseCount++;
        currentPower += m_stationIntensityPhase2 * 230;
    }
    if (m_stationIntensityPhase3 > 0.1) {
        phaseCount++;
        currentPower += m_stationIntensityPhase3 * 230;
    }
    if (phaseCount != m_phaseCount) {
        m_phaseCount = phaseCount;
        emit phaseCountChanged(phaseCount);
    }
    if (currentPower != m_currentPower) {
        m_currentPower = currentPower;
        emit currentPowerChanged(currentPower);
    }

    qCDebug(dcSchneiderElectric()) << "Wallbox state is:" << m_cpwState;
    qCDebug(dcSchneiderElectric()) << "Recieved life bit:" << m_recievedLifeBitRegisterValue;
    qCDebug(dcSchneiderElectric()) << "Charge current limit set point:" << m_chargeCurrentSetpoint;
    qCDebug(dcSchneiderElectric()) << "Received charge current limit:" << m_chargeCurrent;
    qCDebug(dcSchneiderElectric()) << "Phase count:" << m_phaseCount;
    qCDebug(dcSchneiderElectric()) << "PhaseX:" << m_stationIntensityPhaseX;
    qCDebug(dcSchneiderElectric()) << "Phase2:" << m_stationIntensityPhase2;
    qCDebug(dcSchneiderElectric()) << "Phase3:" << m_stationIntensityPhase3;
    qCDebug(dcSchneiderElectric()) << "Current power (calculated from phase current):" << m_currentPower << " (in ampere:" << (m_currentPower / 230) << ")";

    int lifeBitSend{0};
    bool sendCommand{false};
    SchneiderWallboxModbusTcpConnection::RemoteCommand remoteCommandSend{SchneiderWallboxModbusTcpConnection::RemoteCommandAcknowledgeCommand};
    quint16 chargeCurrentSetpointSend{0};
    if (m_charging) {
        qCDebug(dcSchneiderElectric()) << "Wallbox is charging.";

        if (m_recievedLifeBitRegisterValue) {
            if (m_recievedLifeBitRegisterValue == 1) {
                qCWarning(dcSchneiderElectric()) << "Life bit not reset by wallbox. Connection error or wallbox malfunction!";
            } else if (m_recievedLifeBitRegisterValue == 2) {
                qCDebug(dcSchneiderElectric()) << "Life bit register at value 2, setting it to 1 now.";
            }
        }

        // Check if station is available, set available if it is not.
        if (m_cpwState == SchneiderWallboxModbusTcpConnection::CPWStateEvseNotAvailable) {
            qCDebug(dcSchneiderElectric()) << "Device state is ’EVSE not available’. Setting it to ’EVSE available’.";
            if (!m_acknowledgeCommand) {    // Wait till last command is acknowledged before sending next command.
                sendCommand = true;
                remoteCommandSend = SchneiderWallboxModbusTcpConnection::RemoteCommandSetEvcseAvailable;
            }
        }

        lifeBitSend = 1;

        if (m_degradedMode) {
            qCWarning(dcSchneiderElectric()) << "Wallbox is in degraded mode. No charge current control possible in degraded mode!";
        } else {
            chargeCurrentSetpointSend = m_chargeCurrentSetpoint;
        }
    } else {
        qCDebug(dcSchneiderElectric()) << "Wallbox is off.";

        // Remote control is needed to turn off the charge current. Wait until the charge current is 0 before switching off remote control.
        if (m_chargeCurrent == 0) {
            if (m_recievedLifeBitRegisterValue != 2) {
                // Setting lifebit to 2 means end of remote control. Simply stopping to send 1 will result in connection timeout, and the wallbox will log that as an error.
                lifeBitSend = 2;
                qCDebug(dcSchneiderElectric()) << "Ending remote mode. Sending life bit 2.";
            }
        } else {
            // Charge current is not 0. Still need send command to end charging, so keep wallbox in remote mode.
            qCDebug(dcSchneiderElectric()) << "Still need to turn off current. Current max is at:" << m_chargeCurrent;
            lifeBitSend = 1;
        }


        // Check status. Depending on status, send force stop command. Force stop command might be needed to release plug.
    }

    m_errorOccured = false;
    if (lifeBitSend) {
        QModbusReply *reply = m_modbusTcpConnection->setRemoteControllerLifeBit(lifeBitSend);
        if (!reply) {
            qCWarning(dcSchneiderElectric()) << "Sending life bit failed because the reply could not be created.";
            m_errorOccured = true;
            return;
        }
        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
            qCWarning(dcSchneiderElectric()) << "Modbus reply error occurred while sending life bit" << error << reply->errorString();
            emit reply->finished(); // To make sure it will be deleted
        });
    }

    if (m_acknowledgeCommand) {
        if ((m_remoteCommandStatus & 0x0800) == 0x0800) {
            qCWarning(dcSchneiderElectric()) << "Wallbox responded with ’error’ to last command.";
            m_errorOccured = true;
            // Try to fix it by sending RemoteCommandAcknowledgeCommand.
            sendCommand = true;
        } else if (static_cast<SchneiderWallboxModbusTcpConnection::RemoteCommand>(m_remoteCommandStatus) == m_lastCommand) {
            if (m_lastCommand == SchneiderWallboxModbusTcpConnection::RemoteCommandAcknowledgeCommand) {
                m_acknowledgeCommand = false;
            } else {
                // At this point, no other command should have been sent, so remoteCommandSend still has the initial value of RemoteCommandAcknowledgeCommand.
                sendCommand = true;
            }
        }
    }

    if (sendCommand) {
        qCDebug(dcSchneiderElectric()) << "Sending remote command:" << remoteCommandSend;
        QModbusReply *reply = m_modbusTcpConnection->setRemoteCommand(remoteCommandSend);
        if (!reply) {
            qCWarning(dcSchneiderElectric()) << "Sending remote command failed because the reply could not be created.";
            m_errorOccured = true;
            return;
        }
        m_lastCommand = remoteCommandSend;
        m_acknowledgeCommand = true;
        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
            qCWarning(dcSchneiderElectric()) << "Modbus reply error occurred while sending remote command" << error << reply->errorString();
            emit reply->finished(); // To make sure it will be deleted
        });
    }

    if (m_chargeCurrent != chargeCurrentSetpointSend) {
        qCDebug(dcSchneiderElectric()) << "Device max current:" << m_chargeCurrent <<", sending modbus command current: " << chargeCurrentSetpointSend;
        QModbusReply *reply = m_modbusTcpConnection->setMaxIntensitySocket(chargeCurrentSetpointSend);
        if (!reply) {
            qCWarning(dcSchneiderElectric()) << "Sending max intensity socket failed because the reply could not be created.";
            m_errorOccured = true;
            return;
        }
        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
            qCWarning(dcSchneiderElectric()) << "Modbus reply error occurred while sending max intensity socket" << error << reply->errorString();
            emit reply->finished(); // To make sure it will be deleted
        });
    }
}

bool SchneiderWallbox::enableOutput(bool state)
{
    if (!m_modbusTcpConnection->connected()) {
        qCDebug(dcSchneiderElectric()) << "Can't set charging on/off, device is not connected.";
        return false;
    }

    qCDebug(dcSchneiderElectric()) << "Setting wallbox output to" << state;

    m_charging = state;
    //update();  Don't do update here, because the wallbox does not react very fast to modbus writes.
    if (m_errorOccured) {
        return false;
    }
    return true;
}

bool SchneiderWallbox::setMaxAmpere(int ampereValue)
{
    if (!m_modbusTcpConnection->connected()) {
        qCDebug(dcSchneiderElectric()) << "Can't set current limit, device is not connected.";
        return false;
    }

    qCDebug(dcSchneiderElectric()) << "Setting wallbox max current to" << ampereValue;

    m_chargeCurrentSetpoint = ampereValue;
    //update();  Don't do update here, because the wallbox does not react very fast to modbus writes.
    if (m_errorOccured) {
        return false;
    }
    return true;
}
