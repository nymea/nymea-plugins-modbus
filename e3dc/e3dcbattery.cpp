#include "e3dcbattery.h"

e3dcBattery::e3dcBattery()
{

}

float e3dcBattery::currentPower()
{
    return m_currentPower;
}

void e3dcBattery::setCurrentPower(float currentPower)
{
    m_currentPower = currentPower;
}

quint16 e3dcBattery::SOC()
{
    return m_SOC;
}

void e3dcBattery::setSOC(quint16 SOC)
{
    m_SOC = SOC;
}


