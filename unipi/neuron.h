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

#ifndef NEURON_H
#define NEURON_H

#include <QThread>
#include <QMutex>

#include <QObject>
#include <QHash>
#include <QTimer>
#include <QUuid>
#include <QFile>
#include <QSocketNotifier>

class NeuronIO : public QObject
{
    Q_OBJECT

public:
    explicit NeuronIO(QObject *parent = nullptr);

    bool isAvailable();
    QString directory() const;

protected:
    QFile m_valueFile;
    QSocketNotifier *m_notifier = nullptr;
};

class NeuronDigitalOutput : public NeuronIO
{
    Q_OBJECT
public:

    enum OutputType {
        OutputTypeDigital,
        OutputTypeRelay,
        OutputTypeLed
    };
    Q_ENUM(OutputType)
    explicit NeuronDigitalOutput(uint8_t ioGroup, uint8_t output, OutputType outputType = OutputTypeDigital, QObject *parent = nullptr);

    void setValue(bool value);
    bool getValue();

private:
    bool m_currentValue = false;
signals:
    void valueChanged(bool value);
};

class NeuronDigitalInput : public NeuronIO
{
    Q_OBJECT
public:
    explicit NeuronDigitalInput(uint8_t ioGroup, uint8_t input, QObject *parent = nullptr);

    bool getValue();

private:
   bool m_currentValue = false;
signals:
    void valueChanged(bool value);
};

class NeuronAnalogInput : public NeuronIO
{
    Q_OBJECT
public:
    explicit NeuronAnalogInput(uint8_t ioGroup, uint8_t input, QObject *parent = nullptr);

    double getValue();

private:
    double m_currentValue = 0.00;

signals:
    void valueChanged(double value);
};

class NeuronAnalogOutput : public NeuronIO
{
    Q_OBJECT
public:
    explicit NeuronAnalogOutput(uint8_t ioGroup, uint8_t output, QObject *parent = nullptr);

    void setValue(double value);
    double getValue();

private:
    double m_currentValue = 0.00;

signals:
    void valueChanged(double value);
};

class Neuron : public QThread
{
    Q_OBJECT
public:

    explicit Neuron(QObject *parent = nullptr);
    ~Neuron();

    bool init();
    void startPolling(uint milliseconds = 200);

    QList<QString> digitalInputs();
    QList<QString> relayOutputs();
    QList<QString> digitalOutputs();
    QList<QString> analogInputs();
    QList<QString> analogOutputs();
    QList<QString> userLEDs();

    bool setRelayOutput(const QString &circuit, bool value);
    bool setDigitalOutput(const QString &circuit, bool value);
    bool setAnalogOutput(const QString &circuit, double value);
    bool setUserLED(const QString &circuit, bool value);

    /*bool relayOutputValue(const QString &circuit);
    bool digitalOutputValue(const QString &circuit);
    bool userLEDValue(const QString &circuit);
    bool digitalInputValue(const QString &circuit);
    double analogOutputValue(const QString &circuit);
    double analogInputValue(const QString &circuit);*/

    //QUuid setAnalogOutputConfiguration(const QString &circuit, NeuronCommon::AnalogIOConfiguration value);
    //QUuid setAnalogInputConfiguration(const QString &circuit, NeuronCommon::AnalogIOConfiguration value);

protected:
    void run() override;

private:
    QMutex m_valueMutex;
    QMutex m_stopMutex;

    QTimer *m_pollingTimer = nullptr;
    QTimer *m_outputPollingTimer = nullptr;

    QHash<QString, NeuronDigitalOutput *> m_userLeds;
    QHash<QString, NeuronDigitalOutput *> m_relayOutputs;
    QHash<QString, NeuronDigitalOutput *> m_digitalOutputs;
    QHash<QString, NeuronDigitalInput *> m_digitalInputs;
    QHash<QString, NeuronAnalogOutput *> m_analogOutputs;
    QHash<QString, NeuronAnalogInput *> m_analogInputs;

    QHash<QString, bool> m_previousDigitalInputValue;
    QHash<QString, bool> m_previousDigitalOutputValue;
    QHash<QString, bool> m_previousRelayOutputValue;
    QHash<QString, bool> m_previousUserLEDValue;
    QHash<QString, double> m_previousAnalogInputValue;
    QHash<QString, double> m_previousAnalogOutputValue;

    QList<QPair<uint8_t, uint8_t>> discoverIOs(const QString &filter);

signals:
    void relayOutputStatusChanged(const QString &circuit, bool value);
    void digitalOutputStatusChanged(const QString &circuit, bool value);
    void digitalInputStatusChanged(const QString &circuit, bool value);

    void analogInputStatusChanged(const QString &circuit, double value);
    void analogOutputStatusChanged(const QString &circuit, double value);

    void userLEDStatusChanged(const QString &circuit, bool value);
};
#endif // NEURON_H
