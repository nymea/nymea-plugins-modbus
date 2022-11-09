#ifndef SCHNEIDERWALLBOX_H
#define SCHNEIDERWALLBOX_H

#include <QObject>

#include "schneiderwallboxmodbustcpconnection.h"

class SchneiderWallbox : public QObject
{
    Q_OBJECT
public:
    explicit SchneiderWallbox(SchneiderWallboxModbusTcpConnection *modbusTcpConnection, QObject *parent = nullptr);
    ~SchneiderWallbox();

    SchneiderWallboxModbusTcpConnection *modbusTcpConnection();

    void update();

    bool enableOutput(bool state);
    bool setMaxAmpere(int ampereValue);

private:
    SchneiderWallboxModbusTcpConnection *m_modbusTcpConnection = nullptr;
    bool m_charging{false};
    SchneiderWallboxModbusTcpConnection::CPWState m_cpwState{SchneiderWallboxModbusTcpConnection::CPWStateEvseNotAvailable};

    // The variable is initialized with 0 in schneidermodbustcpconnection.h, and signal is just emitted on change.
    quint16 m_recievedLifeBitRegisterValue{0};

    quint16 m_chargeCurrentSetpoint{0}; // Unit is Ampere
    quint16 m_chargeCurrent{0};         // Unit is Ampere
    quint16 m_degradedMode{0};
    double m_stationIntensityPhaseX{0}; // Current, unit is Ampere
    double m_stationIntensityPhase2{0}; // Current, unit is Ampere
    double m_stationIntensityPhase3{0}; // Current, unit is Ampere
    quint16 m_phaseCount{0};
    double m_currentPower{0.0}; // Power of all phases combined, unit is Watt
    bool m_errorOccured{false};
    bool m_acknowledgeCommand{false};
    quint16 m_remoteCommandStatus{0};
    SchneiderWallboxModbusTcpConnection::RemoteCommand m_lastCommand{SchneiderWallboxModbusTcpConnection::RemoteCommandAcknowledgeCommand};

signals:
    void phaseCountChanged(quint16 phaseCount);
    void currentPowerChanged(double currentPower);
};

#endif // SCHNEIDERWALLBOX_H
