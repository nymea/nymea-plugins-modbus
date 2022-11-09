#include "e3dcinverter.h"

e3dcInverter::e3dcInverter()
{

}

float e3dcInverter::currentPower()
{
    return m_current_Power;
}

void e3dcInverter::setCurrentPower(float currentPower)
{
    m_current_Power = currentPower;
}





