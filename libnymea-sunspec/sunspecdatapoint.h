#ifndef SUNSPECDATAPOINT_H
#define SUNSPECDATAPOINT_H

#include <QObject>
#include <QVector>

class SunSpecDataPoint
{
    Q_GADGET
public:
    enum Access {
        AccessReadOnly,
        AccessReadWrite
    };
    Q_ENUM(Access)

    enum DataType {
        Int16,
        Int32,
        Int64,
        UInt16,
        Raw16,
        UInt32,
        Acc16,
        Acc32,
        Acc64,
        BitField16,
        BitField32,
        BitField64,
        Enum16,
        Enum32,
        Float32,
        String,
        ScaleFactor,
        Pad,
        IpV4Address,
        IpV6Address,
        EUI48,
        Group,
        Sync
    };
    Q_ENUM(DataType)

    explicit SunSpecDataPoint();

    QString name() const;
    void setName(const QString &name);

    QString label() const;
    void setLabel(const QString &label);

    QString description() const;
    void setDescription(const QString &description);

    QString detail() const;
    void setDetail(const QString &detail);

    QString units() const;
    void setUnits(const QString &units);

    bool mandatory() const;
    void setMandatory(bool mandatory);

    Access access() const;
    void setAccess(Access access);

    quint16 addressOffset() const;
    void setAddressOffset(quint16 addressOffset);

    quint16 blockOffset() const;
    void setBlockOffset(quint16 blockOffset);

    DataType dataType() const;
    void setDataType(DataType dataType);

    int size() const;
    void setSize(int size);

    QString scaleFactorName() const;
    void setScaleFactorName(const QString &scaleFactorName);

    QVector<quint16> rawData() const;
    void setRawData(const QVector<quint16> &rawData);

    bool isValid() const;

    static DataType stringToDataType(const QString &typString);

private:
    QString m_name;
    QString m_label;
    QString m_description;
    QString m_detail;
    QString m_units;
    bool m_mandatory = false;
    Access m_access = AccessReadOnly;
    quint16 m_addressOffset = 0;
    quint16 m_blockOffset = 0;
    DataType m_dataType = DataType::UInt16;
    int m_size = 1;
    QString m_scaleFactorName;

    QVector<quint16> m_rawData;
};

#endif // SUNSPECDATAPOINT_H
