#ifndef SUNSPECMODEL_H
#define SUNSPECMODEL_H

#include <QObject>
#include <QTimer>

#include "sunspec.h"
#include "sunspecdatapoint.h"

class SunSpecModel : public QObject
{
    Q_OBJECT
public:
    explicit SunSpecModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    virtual ~SunSpecModel() = default;

    SunSpec *connection() const;

    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString label() const = 0;

    quint16 modelId() const;
    quint16 modelLength() const;
    quint16 modbusStartRegister() const;

    virtual void init();
    virtual void readModelHeader() = 0;
    virtual void readBlockData() = 0;

protected:
    SunSpec *m_connection = nullptr;
    QTimer m_initTimer;

    quint16 m_modelId;
    quint16 m_modelLength = 0;
    quint16 m_modbusStartRegister = 0;
    bool m_initialized = false;

    QVector<quint16> m_blockData;
    QVector<SunSpecDataPoint> m_dataPoints;

    void setInitializedFinished();

signals:
    void initFinished(bool success);

};

#endif // SUNSPECMODEL_H
