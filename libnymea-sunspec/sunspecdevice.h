#ifndef SUNSPECDEVICE_H
#define SUNSPECDEVICE_H

#include <QObject>

#include "sunspec.h"

class SunSpecDevice : public QObject
{
    Q_OBJECT
public:
    explicit SunSpecDevice(SunSpec *connection, SunSpec::ModelId modelId, int modbusStartRegister, QObject *parent = nullptr);
    virtual ~SunSpecDevice() = default;

    SunSpec *connection() const;
    SunSpec::ModelId modelId() const;
    int modbusStartRegister() const;

    virtual void init() = 0;
    virtual void readModelHeader() = 0;
    virtual void readBlockData() = 0;

protected:
    SunSpec *m_connection = nullptr;
    SunSpec::ModelId m_modelId;
    uint m_modelLength = 0;
    uint m_modbusStartRegister = 40000;
    bool m_initFinishedSuccess = false;

signals:
    void initFinished(bool success);

};

#endif // SUNSPECDEVICE_H
