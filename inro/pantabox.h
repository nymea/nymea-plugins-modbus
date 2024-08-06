#ifndef PANTABOX_H
#define PANTABOX_H

#include "pantaboxmodbustcpconnection.h"

class Pantabox : public PantaboxModbusTcpConnection
{
    Q_OBJECT
public:
    explicit Pantabox(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    explicit Pantabox(ModbusTcpMaster *modbusTcpMaster, quint16 slaveId, QObject *parent = nullptr);
    ~Pantabox() = default;

    virtual bool update() override;
    virtual bool initialize() override;

signals:

};

#endif // PANTABOX_H
