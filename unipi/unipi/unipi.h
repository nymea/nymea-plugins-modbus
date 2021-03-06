/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
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
    QString type() const;

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
