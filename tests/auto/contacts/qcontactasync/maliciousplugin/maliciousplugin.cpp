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

#ifndef MALICIOUSPLUGINTARGET
#define MALICIOUSPLUGINTARGET contacts_maliciousplugin
#endif

#ifndef MALICIOUSPLUGINNAME
#define MALICIOUSPLUGINNAME maliciousplugin
#endif

#define makestr(x) (#x)
#define makename(x) makestr(x)

#include "maliciousplugin_p.h"
#include <QtContacts/qcontactmanager.h>

#include <QThread>
#include <QMutex>
#include <QSet>
#include <QDebug>

QTCONTACTS_USE_NAMESPACE

class MaliciousThreadObject : public QObject
{
    Q_OBJECT
public:
    MaliciousThreadObject() {}

public slots:
    void activateRequest(QContactAbstractRequest* req)
    {
        mutex.lock();
        if (activeRequests.contains(req)) {
            QContactManagerEngine::updateRequestState(req, QContactAbstractRequest::ActiveState);
        }
        mutex.unlock();
    }

    void finishRequest(QContactAbstractRequest* req)
    {
        QContactManager::Error errorResult = QContactManager::NoError;
        QMap<int, QContactManager::Error> errorsResult;
        QList<QContactId> idResult;
        QList<QContact> contactResult;
        QList<QContactRelationship> relResult;

        mutex.lock();
        if (activeRequests.contains(req)) {
            switch(req->type()) {
                case QContactAbstractRequest::ContactIdFetchRequest:
                    QContactManagerEngine::updateContactIdFetchRequest(static_cast<QContactIdFetchRequest*>(req), idResult, errorResult, QContactAbstractRequest::FinishedState);
                    break;

                case QContactAbstractRequest::ContactFetchRequest:
                    QContactManagerEngine::updateContactFetchRequest(static_cast<QContactFetchRequest*>(req), contactResult, errorResult, QContactAbstractRequest::FinishedState);
                    break;

                default:
                    QContactManagerEngine::updateRequestState(req, QContactAbstractRequest::FinishedState);
                    break;
            }
        }
        activeRequests.remove(req);
        mutex.unlock();
    }
public:
    QMutex mutex;

    QSet<QContactAbstractRequest*> activeRequests;
};

class MaliciousThread : public QThread
{
    Q_OBJECT;
public:
    MaliciousThread();
    ~MaliciousThread();
    QObject* eventLoopQuitHack;
};

MaliciousThread::MaliciousThread()
{
    eventLoopQuitHack = new QObject;
    eventLoopQuitHack->moveToThread(this);
    connect(eventLoopQuitHack, SIGNAL(destroyed(QObject*)), SLOT(quit()), Qt::DirectConnection);
}

MaliciousThread::~MaliciousThread()
{
    eventLoopQuitHack->deleteLater();
    wait();
}

MaliciousAsyncManagerEngine::MaliciousAsyncManagerEngine()
        : QContactManagerEngine()
{
    thread = new MaliciousThread();
    threadObject = new MaliciousThreadObject();
    threadObject->moveToThread(thread);

    connect(this, SIGNAL(doStartRequest(QContactAbstractRequest*)), threadObject, SLOT(activateRequest(QContactAbstractRequest*)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(doFinishRequest(QContactAbstractRequest*)), threadObject, SLOT(finishRequest(QContactAbstractRequest*)), Qt::BlockingQueuedConnection);

    thread->start();
}

MaliciousAsyncManagerEngine::~MaliciousAsyncManagerEngine()
{
    delete thread;
    delete threadObject;
}

QString MaliciousAsyncManagerEngine::managerName() const
{
    return QString(makename(MALICIOUSPLUGINNAME));
}

bool MaliciousAsyncManagerEngine::startRequest(QContactAbstractRequest* req)
{
    threadObject->mutex.lock();
    threadObject->activeRequests.insert(req);
    threadObject->mutex.unlock();

    // Spawn a thread to do stuff on another thread
    emit doStartRequest(req);
    emit doFinishRequest(req);

    return true;
}

bool MaliciousAsyncManagerEngine::cancelRequest(QContactAbstractRequest *req)
{
    updateRequestState(req, QContactAbstractRequest::CanceledState);
    QContactManagerEngine::cancelRequest(req);
    return true;
}

void MaliciousAsyncManagerEngine::requestDestroyed(QContactAbstractRequest *req)
{
    threadObject->mutex.lock();
    threadObject->activeRequests.remove(req);
    threadObject->mutex.unlock();
    QContactManagerEngine::requestDestroyed(req);
}

QString MaliciousEngineFactory::managerName() const
{
    return QString(makename(MALICIOUSPLUGINNAME));
}

QContactManagerEngine* MaliciousEngineFactory::engine(const QMap<QString, QString>& parameters, QContactManager::Error* error)
{
    Q_UNUSED(parameters);
    *error = QContactManager::NoError;
    return new MaliciousAsyncManagerEngine();
}

QContactEngineId* MaliciousEngineFactory::createContactEngineId(const QMap<QString, QString>& parameters, const QString& engineIdString) const
{
    Q_UNUSED(parameters);
    Q_UNUSED(engineIdString);
    return 0;
}

#include "maliciousplugin.moc"
