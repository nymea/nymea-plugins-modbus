#ifndef MODBUSTCPCONNECTION_H
#define MODBUSTCPCONNECTION_H

#include <QObject>

class ModbusTcpMaster;

class ModbusTcpConnection : public QObject
{
    Q_OBJECT
public:
    explicit ModbusTcpConnection(QObject *parent = nullptr) : QObject{parent} { }
    virtual ~ModbusTcpConnection() = default;

    virtual ModbusTcpMaster *modbusTcpMaster() const = 0;

    virtual bool reachable() const = 0;
    virtual bool initializing() const = 0;

    virtual bool initialize() = 0;
    virtual bool update() = 0;

public slots:
    virtual bool connectDevice() = 0;
    virtual void disconnectDevice() = 0;
    virtual bool reconnectDevice() = 0;

signals:
    void reachableChanged(bool reachable);
    void initializationFinished(bool success);

};

#endif // MODBUSTCPCONNECTION_H
