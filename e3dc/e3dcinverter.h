#ifndef E3DCINVERTER_H
#define E3DCINVERTER_H

#include <QObject>
class e3dcInverter: public QObject
{
    Q_OBJECT
public:

    enum Registers {
        RegisterCurrentPower = 40068,
        RegisterNetworkPointPower = 40074
    };
    Q_ENUM(Registers);

    e3dcInverter();

    float currentPower();
    void setCurrentPower(float currentPower);

signals:
    void updated();

    void currentPowerChanged(float currentPower);
    void networkPointPowerChanged(float networkPointPower);



private:


    float m_current_Power = 0;
    float m_network_Point_Power = 0;
    Registers m_current_Power_Register = RegisterCurrentPower;
    Registers m_network_Point_Power_Register = RegisterNetworkPointPower;



};

#endif // E3DCINVERTER_H
