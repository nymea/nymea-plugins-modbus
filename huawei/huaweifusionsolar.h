#ifndef HUAWEIFUSIONSOLAR_H
#define HUAWEIFUSIONSOLAR_H

#include <QObject>
#include <QQueue>

#include "huaweimodbustcpconnection.h"

class HuaweiFusionSolar : public HuaweiModbusTcpConnection
{
    Q_OBJECT
public:
    explicit HuaweiFusionSolar(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);

    virtual void initialize() override;
    virtual void update() override;

private:
    QQueue<HuaweiModbusTcpConnection::Registers> m_registersQueue;

    int m_currentRegisterRequest = -1;

    void readNextRegister();
};

#endif // HUAWEIFUSIONSOLAR_H
