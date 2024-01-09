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

Neuron::Neuron(QObject *parent) : QThread{parent}
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
    QDir test = QDir("/run/unipi/") ;
    if (test.exists()) {
        qCWarning(dcUniPi()) << "Neuron: SysFs driver is deprecated, update the UniPi kernel module";
        return false;
    }

    QDir sysFsDir = QDir(m_unipiSysFsDir);
    if (!sysFsDir.exists()) {
        qCWarning(dcUniPi()) << "Neuron: SysFs driver is not available";
        return false;
    }

    foreach (auto circuit, m_relayOutputs.keys()) {
        // In case of re-init, cleaning up relay outputs
        m_relayOutputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all relay outputs";

    foreach(auto circuit, sysFsDir.entryList({"RO*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronDigitalOutput *relay = new NeuronDigitalOutput(circuit, NeuronDigitalOutput::OutputTypeRelay, this);
        connect(relay, &NeuronDigitalOutput::valueChanged, this, &Neuron::relayOutputStatusChanged);
        m_relayOutputs.insert(circuit, relay);
    }

    foreach(auto circuit, m_digitalOutputs.keys()) {
        // Cleaning up digital outputs
        m_digitalOutputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all digital outputs";
    foreach(auto circuit, sysFsDir.entryList({"DO*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronDigitalOutput *output = new NeuronDigitalOutput(circuit, NeuronDigitalOutput::OutputTypeDigital, this);
        connect(output, &NeuronDigitalOutput::valueChanged, this, &Neuron::digitalOutputStatusChanged);
        m_digitalOutputs.insert(circuit, output);
    }

    foreach(auto circuit, m_digitalInputs.keys()) {
        // Cleaning up digital inputs
        m_digitalInputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all digital inputs";
    foreach(auto circuit, sysFsDir.entryList({"DI*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronDigitalInput *input = new NeuronDigitalInput(circuit, this);
        connect(input, &NeuronDigitalInput::valueChanged, this, &Neuron::digitalInputStatusChanged);
        m_digitalInputs.insert(circuit, input);
    }

    foreach (auto circuit, m_analogInputs.keys()) {
        // Cleaning up analog inputs
        m_analogInputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all analog inputs";
    foreach(auto circuit, sysFsDir.entryList({"AI*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronAnalogInput *input = new NeuronAnalogInput(circuit, this);
        connect(input, &NeuronAnalogInput::valueChanged, this, &Neuron::analogInputStatusChanged);
        m_analogInputs.insert(circuit, input);
    }

    foreach (auto circuit, m_analogOutputs.keys()) {
        // Cleaning up analog outputs
        m_analogOutputs.take(circuit)->deleteLater();
    }
    qCDebug(dcUniPi()) << "Neuron: Discover all analog outputs";
    foreach(auto circuit, sysFsDir.entryList({"AO*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronAnalogOutput *output = new NeuronAnalogOutput(circuit, this);
        connect(output, &NeuronAnalogOutput::valueChanged, this, &Neuron::analogOutputStatusChanged);
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

        foreach (auto output, m_relayOutputs) {
            output->getValue();
        }
        foreach (auto input, m_digitalInputs) {
            input->getValue();
        }

        foreach (auto output, m_digitalOutputs) {
            output->getValue();
        }

        foreach (auto input, m_analogInputs) {
            input->getValue(); //Dont mind the value
        }

        if (isInterruptionRequested()) {
            qCDebug(dcUniPi()) << "Interrupt requested";
            return;
        }
        msleep(50);

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
        qCDebug(dcUniPi()) << "Neuron: Digital output" << circuit << "not available";
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

NeuronDigitalInput::NeuronDigitalInput(const QString &circuit, QObject *parent) : NeuronIO{parent}
{
    m_circuit = circuit;
    QString fileName = m_unipiSysFsDir;
    fileName.append(circuit);
    fileName.append("/value");
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
    bool bValue = (value == '1');
    if (m_currentValue != bValue) {
        m_currentValue = bValue;
        emit valueChanged(m_circuit, bValue);
    }
    return bValue;
}

NeuronDigitalOutput::NeuronDigitalOutput(const QString &circuit, OutputType outputType, QObject *parent) : NeuronIO{parent}
{
    m_circuit = circuit;
    QString fileName = m_unipiSysFsDir;
    switch (outputType) {
    case OutputTypeRelay:
        fileName.append(circuit);
        break;
    case OutputTypeDigital:
        fileName.append(circuit);
        break;
    }
    fileName.append("/value");
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
    bool bValue = (value == '1');
    if (m_currentValue != bValue) {
        m_currentValue = bValue;
        emit valueChanged(m_circuit, bValue);
    }
    return bValue;
}

NeuronAnalogOutput::NeuronAnalogOutput(const QString &circuit, QObject *parent) : NeuronIO(parent)
{
    m_circuit = circuit;
    QString fileName = m_unipiSysFsDir;
    fileName.append(circuit);
    fileName.append("/out_voltage_raw");

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
    out << value*1000;
}

NeuronAnalogInput::NeuronAnalogInput(const QString &circuit, QObject *parent) : NeuronIO(parent)
{
    m_circuit = circuit;
    QString fileName = m_unipiSysFsDir;
    fileName.append(circuit);
    fileName.append("/in_voltage_raw");

    qCDebug(dcUniPi()) << "NeuronAnalogInput: File name" << fileName;
    m_valueFile.setFileName(fileName);

    if (!m_valueFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCWarning(dcUniPi()) << "NeuronAnalogInput: Could not open value file:" << m_valueFile.errorString();
        return;
    }
}

double NeuronAnalogInput::getValue()
{
    if (!m_valueFile.isReadable()) {
        qCWarning(dcUniPi()) << "NeuronAnalogInput: File is not readable" << m_valueFile;
        return 0;
    }
    QByteArray array = m_valueFile.readAll();
    m_valueFile.seek(0);

    bool ok;
    double voltage = QString::fromLocal8Bit(array).toDouble(&ok)/1000.00;
    if (!ok) {
        qCWarning(dcUniPi()) << "NeuronAnalogInput: Could not parse analog input value";
        return 0;
    }
    voltage = (voltage<0) ? 0 : voltage;

    if (m_currentValue != voltage) {
        m_currentValue = voltage;
        emit valueChanged(m_circuit, voltage);
    }
    return voltage;
}
