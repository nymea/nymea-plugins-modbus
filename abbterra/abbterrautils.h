// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ABBTERRAUTILS_H
#define ABBTERRAUTILS_H

#include <QString>
#include <QVector>

#include <modbusdatautils.h>

namespace AbbTerraUtils {

struct DeviceInfo {
    bool valid = false;
    QString serialNumber;
    QString productName;
    QString firmwareVersion;
    double maxChargingCurrent = 32.0;
};

inline QString connectorTypeName(quint8 connectorType)
{
    switch (connectorType) {
    case 'G':
        return QStringLiteral("Cable");
    case 'P':
        return QStringLiteral("Outlet");
    case 'S':
        return QStringLiteral("Socket");
    case 'T':
        return QStringLiteral("Socket");
    default:
        return QString();
    }
}

inline int ratedPowerFromCode(quint8 ratedPowerCode)
{
    switch (ratedPowerCode) {
    case 7:
        return 7;
    case 11:
        return 11;
    case 22:
        return 22;
    default:
        return -1;
    }
}

inline DeviceInfo deviceInfoFromValues(quint64 serialNumberRaw, quint32 firmwareVersionRaw, quint32 maxCurrentRaw)
{
    DeviceInfo deviceInfo;

    const quint8 connectorType = static_cast<quint8>((serialNumberRaw >> 56) & 0xff);
    const quint8 ratedPowerCode = static_cast<quint8>((serialNumberRaw >> 48) & 0xff);
    const quint8 plantId = static_cast<quint8>((serialNumberRaw >> 40) & 0xff);
    const quint8 productionWeek = static_cast<quint8>((serialNumberRaw >> 24) & 0xff);
    const quint8 productionYear = static_cast<quint8>((serialNumberRaw >> 16) & 0xff);
    const quint8 uniqueHigh = static_cast<quint8>((serialNumberRaw >> 8) & 0xff);
    const quint8 uniqueLow = static_cast<quint8>(serialNumberRaw & 0xff);
    const int ratedPower = ratedPowerFromCode(ratedPowerCode);
    const QString connectorName = connectorTypeName(connectorType);

    if (ratedPower <= 0 || connectorName.isEmpty() || productionWeek == 0 || productionWeek > 53) {
        return deviceInfo;
    }

    deviceInfo.valid = true;
    deviceInfo.serialNumber = QStringLiteral("TACW%1-%2-%3%4-%5%6%7")
                                  .arg(ratedPower)
                                  .arg(plantId)
                                  .arg(productionWeek, 2, 10, QLatin1Char('0'))
                                  .arg(productionYear, 2, 10, QLatin1Char('0'))
                                  .arg(QChar(static_cast<char>(connectorType)))
                                  .arg(uniqueHigh, 2, 10, QLatin1Char('0'))
                                  .arg(uniqueLow, 2, 10, QLatin1Char('0'));
    deviceInfo.productName = QStringLiteral("ABB Terra AC %1 kW %2").arg(ratedPower).arg(connectorName);
    deviceInfo.firmwareVersion = QStringLiteral("%1.%2.%3")
                                     .arg((firmwareVersionRaw >> 24) & 0xff)
                                     .arg((firmwareVersionRaw >> 16) & 0xff)
                                     .arg((firmwareVersionRaw >> 8) & 0xff);
    if (maxCurrentRaw >= 6000 && maxCurrentRaw <= 32000) {
        deviceInfo.maxChargingCurrent = maxCurrentRaw / 1000.0;
    }

    return deviceInfo;
}

inline DeviceInfo deviceInfoFromRegisters(const QVector<quint16> &registers)
{
    if (registers.count() < 8) {
        return DeviceInfo();
    }

    return deviceInfoFromValues(
        ModbusDataUtils::convertToUInt64(registers.mid(0, 4), ModbusDataUtils::ByteOrderBigEndian),
        ModbusDataUtils::convertToUInt32(registers.mid(4, 2), ModbusDataUtils::ByteOrderBigEndian),
        ModbusDataUtils::convertToUInt32(registers.mid(6, 2), ModbusDataUtils::ByteOrderBigEndian)
    );
}

inline quint8 chargingStateCode(quint32 chargingStateRaw)
{
    return static_cast<quint8>((chargingStateRaw >> 8) & 0x7f);
}

inline bool chargingLimitedByCar(quint32 chargingStateRaw)
{
    return ((chargingStateRaw >> 15) & 0x1) == 0x1;
}

inline bool isVehiclePluggedIn(quint32 chargingStateRaw, quint32 socketLockState)
{
    const quint8 chargingState = chargingStateCode(chargingStateRaw);
    if (chargingState >= 3 && chargingState <= 6) {
        return true;
    }

    return socketLockState == 0x0101 || socketLockState == 0x0111;
}

inline bool isCharging(quint32 chargingStateRaw, quint32 activePower)
{
    return chargingStateCode(chargingStateRaw) == 6 || activePower > 100;
}

inline uint phaseCount(quint32 voltageL1, quint32 voltageL2, quint32 voltageL3)
{
    uint phases = 0;
    if (voltageL1 > 1000) {
        phases++;
    }
    if (voltageL2 > 1000) {
        phases++;
    }
    if (voltageL3 > 1000) {
        phases++;
    }
    return qMax(phases, 1u);
}

} // namespace AbbTerraUtils

#endif // ABBTERRAUTILS_H
