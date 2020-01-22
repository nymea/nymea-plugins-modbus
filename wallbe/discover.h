/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by copyright law, and
* remains the property of nymea GmbH. All rights, including reproduction, publication,
* editing and translation, are reserved. The use of this project is subject to the terms of a
* license agreement to be concluded with nymea GmbH in accordance with the terms
* of use of nymea GmbH, available under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the terms of the GNU
* Lesser General Public License as published by the Free Software Foundation; version 3.
* this project is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License along with this project.
* If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under contact@nymea.io
* or see our FAQ/Licensing Information on https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef DISCOVER_H
#define DISCOVER_H

#include <QObject>
#include <QProcess>
#include <QXmlStreamReader>
#include <QTimer>
#include <QHostAddress>

#include "host.h"

class Discover : public QObject
{
    Q_OBJECT
public:

    explicit Discover(QStringList arguments, QObject *parent = nullptr);
    ~Discover();
    bool getProcessStatus();

private:
    QTimer *m_timer;
    QStringList m_arguments;

    QProcess * m_discoveryProcess;
    QProcess * m_scanProcess;

    QXmlStreamReader m_reader;

    bool m_aboutToQuit;

    QStringList getDefaultTargets();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    // Process parsing
    QList<Host> parseProcessOutput(const QByteArray &processData);
    Host parseHost();

signals:
    void devicesDiscovered(QList<Host>);

private slots:
    void onTimeout();
};

#endif // DISCOVER_H
