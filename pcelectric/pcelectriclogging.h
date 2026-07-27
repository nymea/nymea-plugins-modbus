// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PCELECTRICLOGGING_H
#define PCELECTRICLOGGING_H

#include <QString>

#include <cmath>

namespace PcElectricLogging {

inline bool isInstantaneousEnergyState(const QString &stateName)
{
    return stateName == QStringLiteral("currentPower")
            || stateName.startsWith(QStringLiteral("currentPowerPhase"))
            || stateName.startsWith(QStringLiteral("voltagePhase"))
            || stateName.startsWith(QStringLiteral("currentPhase"));
}

inline bool isCumulativeEnergyState(const QString &stateName)
{
    return stateName == QStringLiteral("sessionEnergy")
            || stateName == QStringLiteral("totalEnergyConsumed");
}

inline bool isRelevantMeasurementChange(double lastInfoValue, double newValue)
{
    if (!std::isfinite(lastInfoValue) || !std::isfinite(newValue))
        return true;

    if (lastInfoValue == newValue)
        return false;

    if (lastInfoValue == 0.0 || newValue == 0.0)
        return true;

    return std::abs(newValue - lastInfoValue) >= std::abs(lastInfoValue) * 0.1;
}

}

#endif // PCELECTRICLOGGING_H
