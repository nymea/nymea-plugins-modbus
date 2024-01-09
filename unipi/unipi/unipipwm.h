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

#ifndef UNIPIPWM_H
#define UNIPIPWM_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>

class UniPiPwm : public QObject
{
    Q_OBJECT
public:
    enum Polarity {
        PolarityNormal,
        PolarityInversed,
        PolarityInvalid
    };

    explicit UniPiPwm(int chipNumber, QObject *parent = nullptr);
    ~UniPiPwm();

    static bool isAvailable();

    bool exportPwm();

    bool enable();
    bool disable();

    bool isEnabled();

    int chipNumber();

    long period();
    bool setPeriod(long nanoSeconds);

    double frequency();
    bool setFrequency(double kHz);

    long dutyCycle();
    bool setDutyCycle(long nanoSeconds);

    UniPiPwm::Polarity polarity();
    bool setPolarity(UniPiPwm::Polarity polarity);

    int percentage();
    bool setPercentage(int percentage);

private:
    int m_chipNumber;
    long m_period;
    long m_dutyCycle;
    QDir m_pwmDirectory;

    bool unexportPwm();
};


#endif // UNIPIPWM_H
