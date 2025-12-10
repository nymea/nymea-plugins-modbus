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

#ifndef UNIPI_H
#define UNIPI_H

#include <QObject>
#include "mcp23008.h"
#include "mcp342xchannel.h"
#include "unipipwm.h"

#include <gpio.h>
#include <gpiomonitor.h>
#include "hardware/i2c/i2cmanager.h"

class I2CDevice;

class UniPi : public QObject
{
    Q_OBJECT


public:
    enum UniPiType {
        UniPi1,
        UniPi1Lite
    };

    explicit UniPi(I2CManager *i2cManager, UniPiType unipiType, QObject *parent = nullptr);
    ~UniPi() override;

    bool init();
    QString type();

    bool setDigitalOutput(const QString &cicuit, bool status);
    bool getDigitalOutput(const QString &circuit);
    bool getDigitalInput(const QString &circuit);

    bool setAnalogOutput(double value);
    bool getAnalogOutput();

    QList<QString> digitalInputs();
    QList<QString> digitalOutputs();
    QList<QString> analogInputs();
    QList<QString> analogOutputs();

private:
    I2CManager *m_i2cManager;

    UniPiType m_unipiType = UniPiType::UniPi1;
    MCP23008 *m_mcp23008 = nullptr;

    MCP342XChannel *m_analogInputChannel1 = nullptr;
    MCP342XChannel *m_analogInputChannel2 = nullptr;

    int getPinFromCircuit(const QString &cicuit);
    QHash<GpioMonitor *, QString> m_monitorGpios;
    UniPiPwm *m_analogOutput = nullptr;

signals:
    void digitalOutputStatusChanged(const QString &circuit, const bool &value);
    void digitalInputStatusChanged(const QString &circuit, const bool &value);
    void analogInputStatusChanged(const QString &circuit, double value);
    void analogOutputStatusChanged(double value);

private slots:
    void onInputValueChanged(const bool &value);
};

#endif // UNIPI_H
