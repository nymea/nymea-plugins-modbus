#include "e3dcsmartmeter.h"

e3dcSmartMeter::e3dcSmartMeter()
{

}

int e3dcSmartMeter::networkPointPower()
{
    return m_network_Point_Power;
}

void e3dcSmartMeter::setNetworkPointPower(int networkPointPower)
{
    m_network_Point_Power = networkPointPower;
}
