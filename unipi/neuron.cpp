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

#include "neuron.h"
#include "extern-plugininfo.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QStandardPaths>
#include <QDirIterator>
#include <QElapsedTimer>

Neuron::Neuron(QObject *parent) : QThread(parent)
{
    qCDebug(dcUniPi()) << "Neuron: Creating Neuron connection";
    m_pollingTimer = new QTimer(this);
    m_outputPollingTimer = new QTimer(this);
}

Neuron::~Neuron()
{
    qCDebug(dcUniPi()) << "Neuron: Deleting Neuron connection";
}

bool Neuron::init()
{
    QDir test = QDir("/run/unipi/");
    if (!test.exists()) {
        qCWarning(dcUniPi()) << "Neuron: SysFs driver is not available";
        return false;
    }

    foreach (const QString &circuit, m_relayOutputs.keys()) {
        // Cleaning up relay outputs
        m_relayOutputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all relay outputs";
    QPair<uint8_t, uint8_t> value;
    foreach(value, discoverIOs("ro_*")) {
        QString circuit = QString("%1.%2").arg(value.first).arg(value.second);
        NeuronDigitalOutput *relay = new NeuronDigitalOutput(value.first, value.second, NeuronDigitalOutput::OutputTypeRelay, this);
        connect(relay, &NeuronDigitalOutput::valueChanged, this, [this, circuit] (bool value) {
            emit relayOutputStatusChanged(circuit, value);
        });
        m_relayOutputs.insert(circuit, relay);
    }

    foreach (const QString &circuit, m_digitalOutputs.keys()) {
        // Cleaning up digital outputs
        m_digitalOutputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all digital outputs";
    foreach(value, discoverIOs("do_*")) {
        QString circuit = QString("%1.%2").arg(value.first).arg(value.second);
        NeuronDigitalOutput *output = new NeuronDigitalOutput(value.first, value.second, NeuronDigitalOutput::OutputTypeDigital, this);
        connect(output, &NeuronDigitalOutput::valueChanged, this, [this, circuit] (bool value) {
            emit digitalOutputStatusChanged(circuit, value);
        });
        m_digitalOutputs.insert(circuit, output);
    }

    foreach (const QString &circuit, m_digitalInputs.keys()) {
        // Cleaning up digital inputs
        m_digitalInputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all digital inputs";
    foreach(value, discoverIOs("di_*")) {
        QString circuit = QString("%1.%2").arg(value.first).arg(value.second);
        NeuronDigitalInput *input = new NeuronDigitalInput(value.first, value.second, this);
        connect(input, &NeuronDigitalInput::valueChanged, this, [this, circuit] (bool value) {
            emit digitalInputStatusChanged(circuit, value);
        });
        m_digitalInputs.insert(circuit, input);
    }

    foreach (const QString &circuit, m_analogInputs.keys()) {
        // Cleaning up analog inputs
        m_analogInputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all analog inputs";
    foreach(value, discoverIOs("ai_*")) {
        QString circuit = QString("%1.%2").arg(value.first).arg(value.second);
        NeuronAnalogInput *input = new NeuronAnalogInput(value.first, value.second, this);
        connect(input, &NeuronAnalogInput::valueChanged, this, [this, circuit] (double value) {
            emit analogInputStatusChanged(circuit, value);
        });
        m_analogInputs.insert(circuit, input);
    }

    foreach (const QString &circuit, m_analogOutputs.keys()) {
        // Cleaning up analog outputs
        m_analogOutputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all analog outputs";
    foreach(value, discoverIOs("ao_*")) {
        QString circuit = QString("%1.%2").arg(value.first).arg(value.second);
        NeuronAnalogOutput *output = new NeuronAnalogOutput(value.first, value.second, this);
        connect(output, &NeuronAnalogOutput::valueChanged, this, [this, circuit] (double value) {
            emit analogOutputStatusChanged(circuit, value);
        });
        m_analogOutputs.insert(circuit, output);
    }
    return true;
}

void Neuron::run()
{  
    QElapsedTimer timer;
    timer.start();
    int m_pollingCounter = 0;
    while (1) {
        m_pollingCounter++;
        foreach (const QString &circuit, m_relayOutputs.keys()) {
            NeuronDigitalOutput *output = m_relayOutputs.value(circuit);
            bool value = output->getValue();
            if (!m_previousRelayOutputValue.contains(circuit) ||
                    (m_previousRelayOutputValue.contains(circuit) && (m_previousRelayOutputValue.value(circuit) != value))) {
                // Only emit status change of the circuit value has changed
                m_previousRelayOutputValue.insert(circuit, value);
                emit relayOutputStatusChanged(circuit, value);
            }
        }

        foreach (const QString &circuit, m_digitalInputs.keys()) {
            NeuronDigitalInput *input = m_digitalInputs.value(circuit);
            bool value = input->getValue();
            if (!m_previousDigitalInputValue.contains(circuit) ||
                    (m_previousDigitalInputValue.contains(circuit) && (m_previousDigitalInputValue.value(circuit) != value))) {
                m_previousDigitalInputValue.insert(circuit, value);
                emit digitalInputStatusChanged(circuit, value);
            }
        }

        foreach (const QString &circuit, m_userLeds.keys()) {
            NeuronDigitalOutput *output = m_userLeds.value(circuit);
            bool value = output->getValue();
            if (!m_previousUserLEDValue.contains(circuit) ||
                    (m_previousUserLEDValue.contains(circuit) && (m_previousUserLEDValue.value(circuit) != value))) {
                m_previousUserLEDValue.insert(circuit, value);
                emit userLEDStatusChanged(circuit, value);
            }
        }

        foreach (const QString &circuit, m_digitalOutputs.keys()) {
            NeuronDigitalOutput *output = m_digitalOutputs.value(circuit);
            bool value = output->getValue();
            if (!m_previousDigitalOutputValue.contains(circuit) ||
                    (m_previousDigitalOutputValue.contains(circuit) && (m_previousDigitalOutputValue.value(circuit) != value))) {
                m_previousDigitalOutputValue.insert(circuit, value);
                emit digitalOutputStatusChanged(circuit, value);
            }
        }

        foreach (const QString &circuit, m_analogOutputs.keys()) {
            NeuronAnalogOutput *output = m_analogOutputs.value(circuit);
            double value = output->getValue();
            if (!m_previousAnalogOutputValue.contains(circuit) ||
                    (m_previousAnalogOutputValue.contains(circuit) && (m_previousAnalogOutputValue.value(circuit) != value))) {
                m_previousAnalogOutputValue.insert(circuit, value);
                emit analogOutputStatusChanged(circuit, value);
            }
        }

        foreach (const QString &circuit, m_analogInputs.keys()) {
            NeuronAnalogInput *input = m_analogInputs.value(circuit);
            double value = input->getValue();
            if (!m_previousAnalogInputValue.contains(circuit) ||
                    (m_previousAnalogInputValue.contains(circuit) && (m_previousAnalogInputValue.value(circuit) != value))) {
                m_previousAnalogInputValue.insert(circuit, value);
                emit analogInputStatusChanged(circuit, value);
            }
        }
        if (QThread::currentThread()->isInterruptionRequested()) {
            qCDebug(dcUniPi()) << "Interrupt requested";
            return;
        }
        msleep(150);

        if (m_pollingCounter > 100) {
            qCDebug(dcUniPi()) << "Neuron polling cycle interval" << timer.elapsed()/100 << "milliseconds";
            timer.restart();
            m_pollingCounter = 0;
         }
    }
}

QList<QString> Neuron::relayOutputs()
{
    return m_relayOutputs.keys();
}

QList<QString> Neuron::digitalInputs()
{
    return m_digitalInputs.keys();
}

QList<QString> Neuron::digitalOutputs()
{
    return m_digitalOutputs.keys();
}

QList<QString> Neuron::analogInputs()
{
    return m_analogInputs.keys();
}

QList<QString> Neuron::analogOutputs()
{
    return m_analogOutputs.keys();
}

QList<QString> Neuron::userLEDs()
{
    return m_userLeds.keys();
}

bool Neuron::setRelayOutput(const QString &circuit, bool value)
{
    NeuronDigitalOutput *relay = m_relayOutputs.value(circuit);
    if (!relay) {
        qCDebug(dcUniPi()) << "Neuron: Relay" << circuit << "not available";
        return false;
    }
    relay->setValue(value);
    return true;
}

bool Neuron::setDigitalOutput(const QString &circuit, bool value)
{
    NeuronDigitalOutput *output = m_digitalOutputs.value(circuit);
    if (!output) {
        qCDebug(dcUniPi()) << "Neuron: Output" << circuit << "not available";
        return false;
    }
    output->setValue(value);
    return true;
}

bool Neuron::setAnalogOutput(const QString &circuit, double value)
{
    if (!m_analogOutputs.contains(circuit)) {
        qCWarning(dcUniPi()) << "Neuron: Analog output circuit not found" << circuit;
        return false;
    }
    m_analogOutputs.value(circuit)->setValue(value);
    return true;
}

bool Neuron::setUserLED(const QString &circuit, bool value)
{
    if (!m_userLeds.contains(circuit)) {
        qCWarning(dcUniPi()) << "Neuron: User LED circuit not found" << circuit;
        return false;
    }
    m_userLeds.value(circuit)->setValue(value);

    return true;
}

/*bool Neuron::relayOutputValue(const QString &circuit)
{
    return m_previousRelayOutputValue.value(circuit);
}

bool Neuron::digitalOutputValue(const QString &circuit)
{
    return m_previousDigitalOutputValue.value(circuit);
}

bool Neuron::userLEDValue(const QString &circuit)
{
    return m_previousUserLEDValue.value(circuit);
}

bool Neuron::digitalInputValue(const QString &circuit)
{
    return m_previousDigitalInputValue.value(circuit);
}

double Neuron::analogOutputValue(const QString &circuit)
{
    return m_previousAnalogOutputValue.value(circuit);
}

double Neuron::analogInputValue(const QString &circuit)
{
    return m_previousAnalogInputValue.value(circuit);
}*/


/*QUuid Neuron::setAnalogOutputConfiguration(const QString &circuit, NeuronCommon::AnalogIOConfiguration value)
{
    Q_UNUSED(circuit)
    Q_UNUSED(value)
    return "";
}

QUuid Neuron::setAnalogInputConfiguration(const QString &circuit, NeuronCommon::AnalogIOConfiguration value)
{
    Q_UNUSED(circuit)
    Q_UNUSED(value)
    return "";
}*/

QList<QPair<uint8_t, uint8_t> > Neuron::discoverIOs(const QString &filter)
{
    QList<QPair<uint8_t, uint8_t>> ioList;
    QDirIterator it("/run/unipi/", QStringList() << filter, QDir::Dirs, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        qDebug(dcUniPi()) << "      -" << filePath;

        bool ok = true;
        QStringList list = filePath.split('/').last().split('_');
        if (list.length() != 3) {
            qCWarning(dcUniPi()) << "Neuron: Discover IOs: File path cannot be parsed";
            continue;
        }
        uint8_t ioGroup = list[1].toInt(&ok);
        if (!ok) {
            qCWarning(dcUniPi()) << "Neuron: Discover IOs: Could not parse IO group";
        }
        uint8_t outputNumber = list[2].toInt(&ok);
        if (!ok) {
            qCWarning(dcUniPi()) << "Neuron: Discover IOs: Could not parse IO number";
        }
        ioList.append(QPair<uint8_t, uint8_t>(ioGroup, outputNumber));
    }
    return ioList;
}

NeuronIO::NeuronIO(QObject *parent) : QObject(parent)
{
}

bool NeuronIO::isAvailable()
{
    return m_valueFile.exists();
}

QString NeuronIO::directory() const
{
    return m_valueFile.fileName();
}

NeuronDigitalInput::NeuronDigitalInput(uint8_t ioGroup, uint8_t input, QObject *parent) : NeuronIO(parent)
{
    QString fileName("/run/unipi/");
    fileName.append(QString("io_group%1/").arg(ioGroup));
    fileName.append(QString("di_%1_%2/di_value").arg(ioGroup).arg(input, 2, 10, QLatin1Char('0')));
    qCDebug(dcUniPi()) << "NeuronDigitalInput: File name" << fileName;
    m_valueFile.setFileName(fileName);

    if (!m_valueFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCWarning(dcUniPi()) << "NeuronDigitalInput: Could not open value file:" << m_valueFile.errorString();
        return;
    }
}

bool NeuronDigitalInput::getValue()
{
    char value;
    m_valueFile.read(&value, 1);
    m_valueFile.seek(0);
    return (value == '1');
}

NeuronDigitalOutput::NeuronDigitalOutput(uint8_t ioGroup, uint8_t output, OutputType outputType, QObject *parent) : NeuronIO(parent)
{
    QString fileName("/run/unipi/");
    fileName.append(QString("io_group%1/").arg(ioGroup));
    switch (outputType) {
    case OutputTypeLed:
        fileName.append(QString("leds/unipi:green:uled-x%1/brightness").arg(output));
        break;
    case OutputTypeRelay:
        fileName.append(QString("ro_%1_%2/ro_value").arg(ioGroup).arg(output, 2, 10,  QLatin1Char('0')));
        break;
    case OutputTypeDigital:
        fileName.append(QString("do_%1_%2/do_value").arg(ioGroup).arg(output, 2, 10,  QLatin1Char('0')));
        break;
    }
    qCDebug(dcUniPi()) << "NeuronDigitalOutput: File name" << fileName;
    m_valueFile.setFileName(fileName);

    if (!m_valueFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qCWarning(dcUniPi()) << "NeuronDigitalOutput: Could not open value file:" << m_valueFile.errorString();
        return;
    }
}

void NeuronDigitalOutput::setValue(bool value)
{
    qCDebug(dcUniPi()) << "NeuronDigitalOutput: Set value" << value << "to" << directory();
    QTextStream out(&m_valueFile);
    out << (value ? "1" : "0");
}

bool NeuronDigitalOutput::getValue()
{
    char value;
    m_valueFile.read(&value, 1);
    m_valueFile.seek(0);
    return (value == '1');
}

NeuronAnalogOutput::NeuronAnalogOutput(uint8_t ioGroup, uint8_t output, QObject *parent) : NeuronIO(parent)
{
    QString fileName("/run/unipi/");
    fileName.append(QString("io_group%1/").arg(ioGroup));
    fileName.append(QString("ao_%1_%2/out_voltage0_raw").arg(ioGroup).arg(output));

    qCDebug(dcUniPi()) << "NeuronAnalogOutput: File name" << fileName;
    m_valueFile.setFileName(fileName);

    if (!m_valueFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qCWarning(dcUniPi()) << "NeuronAnalogOutput: Could not open value file:" << m_valueFile.errorString();
        return;
    }
}

void NeuronAnalogOutput::setValue(double value)
{
    qCDebug(dcUniPi()) << "NeuronAnalogOutput: Set value" << value << "to" << directory();
    QTextStream out(&m_valueFile);
    out << value;
}

double NeuronAnalogOutput::getValue()
{
    QString value;
    QTextStream in(&m_valueFile);
    in >> value;
    return value.toDouble();
}

NeuronAnalogInput::NeuronAnalogInput(uint8_t ioGroup, uint8_t input, QObject *parent) : NeuronIO(parent)
{
    QString fileName("/run/unipi/");
    fileName.append(QString("io_group%1/").arg(ioGroup));
    //fileName.append(QString("ai_%1_%2/in_current1_raw").arg(ioGroup).arg(input));
    fileName.append(QString("ai_%1_%2/in_voltage0_raw").arg(ioGroup).arg(input));

    qCDebug(dcUniPi()) << "NeuronAnalogInput: File name" << fileName;
    m_valueFile.setFileName(fileName);

    if (!m_valueFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCWarning(dcUniPi()) << "NeuronAnalogInput: Could not open value file:" << m_valueFile.errorString();
        return;
    }
}

double NeuronAnalogInput::getValue()
{
    QString value;
    QTextStream in(&m_valueFile);
    in >> value;
    return value.toDouble();
}
