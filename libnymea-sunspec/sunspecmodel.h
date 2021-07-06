#ifndef SUNSPECMODEL_H
#define SUNSPECMODEL_H

#include <QObject>
#include <QTimer>

#include "sunspec.h"
#include "sunspecdatapoint.h"

class SunSpecConnection;

class SunSpecModel : public QObject
{
    Q_OBJECT
public:
    explicit SunSpecModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength, QObject *parent = nullptr);
    virtual ~SunSpecModel() = default;

    SunSpecConnection *connection() const;

    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString label() const = 0;

    quint16 modelId() const;
    quint16 modelLength() const;
    quint16 modbusStartRegister() const;

    QHash<QString, SunSpecDataPoint> dataPoints() const;

    QVector<quint16> blockData() const;

    virtual void init();
    virtual void readBlockData();

    bool operator==(const SunSpecModel &other) const;

protected:
    SunSpecConnection *m_connection = nullptr;
    QTimer m_initTimer;

    quint16 m_modbusStartRegister = 0;
    quint16 m_modelId;
    quint16 m_modelLength = 0;

    bool m_initialized = false;

    QVector<quint16> m_blockData;
    QHash<QString, SunSpecDataPoint> m_dataPoints;

    void setInitializedFinished();

    virtual void processBlockData() = 0;

signals:
    void initFinished(bool success);
    void blockDataChanged(const QVector<quint16> blockData);

    void blockUpdated();

};

QDebug operator<<(QDebug debug, SunSpecModel *model);

#endif // SUNSPECMODEL_H
