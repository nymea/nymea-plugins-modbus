// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unipipwm.h"
#include "extern-plugininfo.h"

UniPiPwm::UniPiPwm(int chipNumber, QObject *parent) :
    QObject(parent),
    m_chipNumber(chipNumber),
    m_period(0),
    m_dutyCycle(0)
{
    m_pwmDirectory = QDir("/sys/class/pwm/pwmchip" + QString::number(chipNumber) + "/");
}

/*! Destructor for this UniPiPwm interface. */
UniPiPwm::~UniPiPwm()
{
    unexportPwm();
}

/*! Returns true, if the path \tt{/sys/class/pwm} exists and is not empty. */
bool UniPiPwm::isAvailable()
{
    QDir pwmDirectory("/sys/class/pwm");
    return pwmDirectory.exists() && !pwmDirectory.entryList().isEmpty();
}

bool UniPiPwm::exportPwm()
{
    QFile exportFile(m_pwmDirectory.path() + "/export");
    if (!exportFile.open(QIODevice::WriteOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not export pwm1 on chip" << m_chipNumber;
        return false;
    }

    QTextStream out(&exportFile);
    out << 1;
    exportFile.close();
    return true;
}

/*! Returns true, if this UniPiPwm interface has been enabled successfully. */
bool UniPiPwm::enable()
{
    QFile enableFile(m_pwmDirectory.path() + "/pwm1/enable");
    if (!enableFile.open(QIODevice::WriteOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not enable pwm1 on chip" << m_chipNumber;
        return false;
    }

    QTextStream out(&enableFile);
    out << 1;
    enableFile.close();
    return true;
}

/*! Returns true, if this UniPiPwm interface has been disabled successfully. */
bool UniPiPwm::disable()
{
    QFile enableFile(m_pwmDirectory.path() + "/pwm1/enable");
    if (!enableFile.open(QIODevice::WriteOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not disable pwm1 on chip" << m_chipNumber;
        return false;
    }

    QTextStream out(&enableFile);
    out << 0;
    enableFile.close();
    return true;
}

/*! Returns true, if this UniPiPwm interface is enabled. */
bool UniPiPwm::isEnabled()
{
    QFile enableFile(m_pwmDirectory.path() + "/pwm1/enable");
    if (!enableFile.open(QIODevice::ReadOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not read" << enableFile.fileName();
        return false;
    }
    QString value;
    QTextStream in(&enableFile);
    in >> value;
    enableFile.close();

    if (value == "1")
        return true;

    return false;
}

/*! Returns the period of this UniPiPwm. */
long UniPiPwm::period()
{
    // period = active + inactive time
    QFile periodFile(m_pwmDirectory.path() + "/pwm1/period");
    if (!periodFile.open(QIODevice::ReadOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not open" << periodFile.fileName();
        return false;
    }

    QString value;
    QTextStream in(&periodFile);
    in >> value;
    periodFile.close();
    m_period = value.toLong();
    return m_period;
}

/*! Returns true, if the period of this UniPiPwm has been set to \a nanoSeconds successfully. */
bool UniPiPwm::setPeriod(long nanoSeconds)
{
    // the current duty cycle can not be greater than the period
    if (dutyCycle() > nanoSeconds && !setDutyCycle(nanoSeconds))
        return false;

    // period = active + inactive time
    QFile periodFile(m_pwmDirectory.path() + "/pwm1/period");
    if (!periodFile.open(QIODevice::WriteOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not open" << periodFile.fileName();
        return false;
    }
    QTextStream out(&periodFile);
    out << QString::number(nanoSeconds);
    periodFile.close();
    m_period = nanoSeconds;
    return true;
}

/*! Returns the frequency [kHz] of the UniPiPwm. */
double UniPiPwm::frequency()
{
    return (100000000.0 / (period() * 1000));
}

/*! Returns true, if the frequency [kHz] of this Pwm has been set successfully to the given \a kHz. */
bool UniPiPwm::setFrequency(double kHz)
{
    // p = 1 / f
    long nanoSeconds = (long)(100000000 / (kHz * 1000));
    return setPeriod(nanoSeconds);
}

/*! Returns the duty cycle [ns] of the UniPiPwm. The duty cycle is the active time of one period. */
long UniPiPwm::dutyCycle()
{
    QFile dutyCycleFile(m_pwmDirectory.path() + "/pwm1/duty_cycle");
    if (!dutyCycleFile.open(QIODevice::ReadOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not open" << dutyCycleFile.fileName();
        return false;
    }

    QString value;
    QTextStream in(&dutyCycleFile);
    in >> value;
    dutyCycleFile.close();
    m_dutyCycle = value.toLong();
    return m_dutyCycle;
}

/*! Returns true, if the duty cycle [ns] of the UniPiPwm has been set successfully to the given \a nanoSeconds. The duty cycle is the active time of one period. */
bool UniPiPwm::setDutyCycle(long nanoSeconds)
{
    // can not be greater than period or negative
    if (nanoSeconds > m_period || nanoSeconds < 0) {
        qCWarning(dcUniPi()) << "ERROR: duty cycle has to be positive and smaller than the period";
        return false;
    }

    QFile dutyCycleFile(m_pwmDirectory.path() + "/pwm1/duty_cycle");
    if (!dutyCycleFile.open(QIODevice::WriteOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not open" << dutyCycleFile.fileName();
        return false;
    }
    QTextStream out(&dutyCycleFile);
    out << QString::number(nanoSeconds);
    dutyCycleFile.close();
    m_dutyCycle = nanoSeconds;
    return true;
}

/*! Returns the Polarity of this UniPiPwm. */
UniPiPwm::Polarity UniPiPwm::polarity()
{
    QFile polarityFile(m_pwmDirectory.path() + "/pwm1/polarity");
    if (!polarityFile.open(QIODevice::ReadOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not open" << polarityFile.fileName();
        return PolarityInvalid;
    }

    QString value;
    QTextStream in(&polarityFile);
    in >> value;
    polarityFile.close();

    if (value == "normal") {
        return PolarityNormal;
    } else if(value == "inversed") {
        return PolarityInversed;
    }

    return PolarityInvalid;
}

/*! Returns true, if the polarity of this UniPiPwm has been set to \a polarity successfully. */
bool UniPiPwm::setPolarity(UniPiPwm::Polarity polarity)
{
    if (polarity == UniPiPwm::PolarityInvalid)
        return false;

    // Note: the polarity can only be changed if the pwm is disabled.
    bool wasEnabled = isEnabled();
    if (wasEnabled && !disable())
        return false;

    QFile polarityFile(m_pwmDirectory.path() + "/pwm1/polarity");
    if (!polarityFile.open(QIODevice::WriteOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not open" << polarityFile.fileName();
        return false;
    }

    QTextStream out(&polarityFile);
    switch (polarity) {
    case PolarityNormal:
        out << "normal";
        break;
    case PolarityInversed:
        out << "inversed";
        break;
    default:
        break;
    }
    polarityFile.close();

    if (wasEnabled)
        enable();

    return true;
}

/*! Returns the current percentage of this UniPiPwm. */
int UniPiPwm::percentage()
{
    return (int)(dutyCycle() * (100.0 / period()) + 0.5);
}

/*! Returns true, if the percentage of this UniPiPwm has been set to \a percentage successfully. */
bool UniPiPwm::setPercentage(int percentage)
{
    long nanoSeconds = period() * (percentage / 100.0);
    return setDutyCycle(nanoSeconds);
}

/*! Returns true, if this UniPiPwm interface has been unexported successfully. */
bool UniPiPwm::unexportPwm()
{
    QFile unexportFile(m_pwmDirectory.path() + "/unexport");
    if (!unexportFile.open(QIODevice::WriteOnly)) {
        qCWarning(dcUniPi()) << "ERROR: could not unexport UniPiPwm" << m_chipNumber;
        return false;
    }

    QTextStream out(&unexportFile);
    out << 0;
    unexportFile.close();
    return true;
}
