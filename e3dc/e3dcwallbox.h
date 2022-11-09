#ifndef E3DCWALLBOX_H
#define E3DCWALLBOX_H

#include <QObject>
class e3dcWallbox: public QObject
{
    Q_OBJECT
public:
    enum Registers {
        RegisterCurrentCollectivePower = 40078
    };
    Q_ENUM(Registers);


    e3dcWallbox();
    float currentPower();
    void setCurrentPower(float currentPower);

signals:
    void updated();
    void currentPowerChanged(float currentPower);

private:

    float m_current_Power = 0;
    Registers m_current_Power_Register = RegisterCurrentCollectivePower;


    int m_wallboxID;
};



#endif // E3DCWALLBOX_H
