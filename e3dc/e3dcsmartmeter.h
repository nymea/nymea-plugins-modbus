#ifndef E3DCSMARTMETER_H
#define E3DCSMARTMETER_H

#include <QObject>
class e3dcSmartMeter: public QObject
{
   Q_OBJECT
public:
    e3dcSmartMeter();

    int networkPointPower();
    void setNetworkPointPower(int networkPointPower);

signals:

    void networkPointPowerChanged(int networkPointPower);


private:

    int m_network_Point_Power = 0;

};

#endif // E3DCSMARTMETER_H
