#ifndef TECHNIVOLTWALLBOX_H
#define TECHNIVOLTWALLBOX_H

#include <QObject>

#include "technivoltmodbustcpconnection.h"

class TechnivoltWallbox : public QObject
{
    Q_OBJECT
public:
    explicit TechnivoltWallbox(TechnivoltModbusTcpConnection *modbusTcpConnection, QObject *parent = nullptr);
    ~TechnivoltWallbox();

    TechnivoltModbusTcpConnection *modbusTcpConnection();

    void update();

    bool enableOutput(bool state);
    bool setMaxAmpere(int ampereValue);

private:
    TechnivoltModbusTcpConnection *m_modbusTcpConnection = nullptr;
    bool m_charging{false};

    quint16 m_chargeCurrentSetpoint{0}; // Unit is Ampere
    quint16 m_chargeCurrent{0};         // Unit is Ampere
    quint32 m_milliAmpPhase1{0};        // Current of phase 1, unit is milli Ampere
    quint32 m_milliAmpPhase2{0};        // Current of phase 2, unit is milli Ampere
    quint32 m_milliAmpPhase3{0};        // Current of phase 3, unit is milli Ampere
    quint16 m_phaseCount{0};
    bool m_errorOccured{false};

signals:
    void phaseCountChanged(quint16 phaseCount);
    void currentPowerChanged(double currentPower);
};

#endif // TECHNIVOLTWALLBOX_H
