#include "technivoltwallbox.h"
#include "extern-plugininfo.h"

TechnivoltWallbox::TechnivoltWallbox(TechnivoltModbusTcpConnection *modbusTcpConnection, QObject *parent) :
    QObject{parent},
    m_modbusTcpConnection{modbusTcpConnection}
{
    connect(modbusTcpConnection, &TechnivoltModbusTcpConnection::hemsCurrentLimitChanged, this, [this](quint16 hemsCurrentLimit){
        m_chargeCurrent = hemsCurrentLimit;
    });
    connect(modbusTcpConnection, &TechnivoltModbusTcpConnection::currentPhase1Changed, this, [this](quint32 currentPhase1){
        m_milliAmpPhase1 = currentPhase1;
    });
    connect(modbusTcpConnection, &TechnivoltModbusTcpConnection::currentPhase2Changed, this, [this](quint32 currentPhase2){
        m_milliAmpPhase2 = currentPhase2;
    });
    connect(modbusTcpConnection, &TechnivoltModbusTcpConnection::currentPhase3Changed, this, [this](quint32 currentPhase3){
        m_milliAmpPhase3 = currentPhase3;
    });

}

TechnivoltWallbox::~TechnivoltWallbox()
{
    qCDebug(dcTechnivoltWallbox()) << "Deleting TechnivoltWallbox object for device with address" << m_modbusTcpConnection->hostAddress();
    delete m_modbusTcpConnection;
}


TechnivoltModbusTcpConnection *TechnivoltWallbox::modbusTcpConnection()
{
    return m_modbusTcpConnection;
}

void TechnivoltWallbox::update()
{
    if (!m_modbusTcpConnection->connected()) {
        return;
    }

    int phaseCount{0};
    quint32 milliAmpsTotal{0};
    if (m_milliAmpPhase1 > 50) {    // Use 50 mA as lower limit. Testing revealed small currents (~20 mA) can be present even when the vehicle is not charging.
        phaseCount++;
        milliAmpsTotal += m_milliAmpPhase1;
    }
    if (m_milliAmpPhase2 > 50) {
        phaseCount++;
        milliAmpsTotal += m_milliAmpPhase2;
    }
    if (m_milliAmpPhase3 > 50) {
        phaseCount++;
        milliAmpsTotal += m_milliAmpPhase3;
    }
    if (phaseCount < 1) {
        phaseCount = 1;
    }
    if (phaseCount != m_phaseCount) {
        m_phaseCount = phaseCount;
        emit phaseCountChanged(phaseCount);
    }

    qCDebug(dcTechnivoltWallbox()) << "Charge current limit set point:" << m_chargeCurrentSetpoint;
    qCDebug(dcTechnivoltWallbox()) << "Received charge current limit:" << m_chargeCurrent;
    qCDebug(dcTechnivoltWallbox()) << "Phase count:" << m_phaseCount;
    qCDebug(dcTechnivoltWallbox()) << "Phase1:" << m_milliAmpPhase1 << " mA";
    qCDebug(dcTechnivoltWallbox()) << "Phase2:" << m_milliAmpPhase2 << " mA";
    qCDebug(dcTechnivoltWallbox()) << "Phase3:" << m_milliAmpPhase3 << " mA";
    qCDebug(dcTechnivoltWallbox()) << "Current power:" << ((milliAmpsTotal * 230) / 1000) << " Watt (in Ampere:" << (milliAmpsTotal / 1000.0) << ")";

    quint16 chargeCurrentSetpointSend{0};
    if (m_charging) {
        chargeCurrentSetpointSend = m_chargeCurrentSetpoint;
    }

    m_errorOccured = false;
    if (m_chargeCurrent != chargeCurrentSetpointSend) {
        qCDebug(dcTechnivoltWallbox()) << "Device max current:" << m_chargeCurrent <<", sending modbus command current: " << chargeCurrentSetpointSend;
        QModbusReply *reply = m_modbusTcpConnection->setHemsCurrentLimit(chargeCurrentSetpointSend);
        if (!reply) {
            qCWarning(dcTechnivoltWallbox()) << "Sending current limit failed because the reply could not be created.";
            m_errorOccured = true;
            return;
        }
        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
            qCWarning(dcTechnivoltWallbox()) << "Modbus reply error occurred while sending max intensity socket" << error << reply->errorString();
            emit reply->finished(); // To make sure it will be deleted
        });
    }
}

bool TechnivoltWallbox::enableOutput(bool state)
{
    if (!m_modbusTcpConnection->connected()) {
        qCDebug(dcTechnivoltWallbox()) << "Can't set charging on/off, device is not connected.";
        return false;
    }

    qCDebug(dcTechnivoltWallbox()) << "Setting wallbox output to" << state;

    m_charging = state;
    //update();  Don't do update here, because the wallbox does not react very fast to modbus writes.
    if (m_errorOccured) {
        return false;
    }
    return true;
}

bool TechnivoltWallbox::setMaxAmpere(int ampereValue)
{
    if (!m_modbusTcpConnection->connected()) {
        qCDebug(dcTechnivoltWallbox()) << "Can't set current limit, device is not connected.";
        return false;
    }

    qCDebug(dcTechnivoltWallbox()) << "Setting wallbox max current to" << ampereValue;

    m_chargeCurrentSetpoint = ampereValue;
    //update();  Don't do update here, because the wallbox does not react very fast to modbus writes.
    if (m_errorOccured) {
        return false;
    }
    return true;
}
