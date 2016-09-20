/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef JSONDBPROCESS_H
#define JSONDBPROCESS_H

#include <QtTest/QtTest>
#include <QProcess>
#include <QTemporaryDir>
#include <QCoreApplication>
#include <QLibraryInfo>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class JsonDbProcess
{
public:
    JsonDbProcess()
    {
    }

    bool start(const QString &partitionsFilePath = QString(), bool killAllJsonDb = true) {

        if (killAllJsonDb) {
            if (system("killall jsondb") != 0) {
                qWarning() << Q_FUNC_INFO << "Unable to kill running jsondb instances.";
            }
        }

        QFileInfo partitionsFileInfo(partitionsFilePath);
        // Start new process
        QString jsondbPath = QLibraryInfo::location(QLibraryInfo::BinariesPath) + "/jsondb";
        if (!QFileInfo(jsondbPath).exists()) {
            qWarning() << Q_FUNC_INFO << "Cannot find JsonDb binaries.";
            return false;
        }

        QStringList args;
        if (!partitionsFilePath.isEmpty())
            args << "-config-path" << partitionsFileInfo.absolutePath();
        args << "-reject-stale-updates";
        m_process.start(jsondbPath, args);

        if (!m_process.waitForStarted()) {
            qWarning() << Q_FUNC_INFO << m_process.errorString();
            return false;
        } else {
            // Wait is needed so we get to know if the process was immediately exited
            QTest::qWait(100);

            // The process was immediately exited
            if (m_process.state() == QProcess::NotRunning) {
                // Maybe older JsonDb version is in use and -config-path argument is invalid?
                // TODO: remove this and the above wait later when it's safe
                m_process.start(jsondbPath, QStringList() << partitionsFileInfo.absolutePath());
                if (!m_process.waitForStarted()) {
                    qWarning() << Q_FUNC_INFO << m_process.errorString();
                    return false;
                }
            }
        }

        return true;
    }

    void terminate() {
        if (m_process.state() == QProcess::NotRunning)
            return;

        m_process.terminate();
        if (m_process.state() != QProcess::NotRunning && !m_process.waitForFinished()) {
            qWarning() << Q_FUNC_INFO << "JsonDb did not terminate cleanly.  Killing.";
            m_process.kill();
        }
    }

private:
    QProcess m_process;

};

#endif // JSONDBPROCESS_H
