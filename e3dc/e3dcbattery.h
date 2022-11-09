#ifndef E3DCBATTERY_H
#define E3DCBATTERY_H


#include <QObject>

class e3dcBattery : public QObject
{
    Q_OBJECT
public:

    enum Registers {
        RegisterCurrentPower = 40070,
        RegisterSOC = 40083
    };
    Q_ENUM(Registers);

    e3dcBattery();

    float currentPower();
    void setCurrentPower(float currentPower);

    quint16 SOC();
    void setSOC(quint16 SOC);


signals:
    void updated();

    void currentPowerChanged(float currentPower);
    void SOCChanged(quint16 SOC);

private:

    float m_currentPower = 0;
    quint16 m_SOC = 0;
    Registers m_current_Power_Register = RegisterCurrentPower;
    Registers m_network_Point_Power_Register = RegisterSOC;





};

#endif // E3DCBATTERY_H
