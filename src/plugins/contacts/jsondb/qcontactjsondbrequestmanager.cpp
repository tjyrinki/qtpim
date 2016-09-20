/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactjsondbrequestmanager.h"

#include <QtCore/qdebug.h>
#include <QtCore/qthread.h>

QT_BEGIN_NAMESPACE_CONTACTS

QContactJsonDbRequestManager::QContactJsonDbRequestManager()
{
    m_operationMutex = new QMutex();
}

QContactJsonDbRequestManager::~QContactJsonDbRequestManager()
{
    delete m_operationMutex;
}

void QContactJsonDbRequestManager::addRequest(QContactAbstractRequest* req, QList<QContact> items)
{
    QMutexLocker locker(m_operationMutex);
    if (!m_activeRequests.contains(req)) {
        QContactRequestData* newData = new QContactRequestData();
        newData->m_contactList = items;
        if (m_inactiveRequests.contains(req)) {
            newData->m_waitCondition = m_inactiveRequests.value(req);
            m_inactiveRequests.remove(req);
        } else {
            newData->m_waitCondition = 0;
        }
        m_activeRequests.insert(req, newData);
    }
}

void QContactJsonDbRequestManager::removeRequest(QContactAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);
    if (m_activeRequests.contains(req)) {
        delete m_activeRequests.value(req);
        m_activeRequests.remove(req);
    }
}

void QContactJsonDbRequestManager::addPrefetchRequest(QContactFetchRequest *prefetchReq, QContactSaveRequest *saveReq)
{
    QMutexLocker locker(m_operationMutex);
    if (!m_prefetchRequestsMap.contains(prefetchReq)) {
        m_prefetchRequestsMap.insert(prefetchReq, saveReq);
    }
}

QContactSaveRequest* QContactJsonDbRequestManager::removePrefetchRequest(QContactFetchRequest *prefetchReq)
{
    QMutexLocker locker(m_operationMutex);
    if (m_prefetchRequestsMap.contains(prefetchReq)) {
        return m_prefetchRequestsMap.take(prefetchReq);
    }
    return 0;
}

void QContactJsonDbRequestManager::addRequest(QJsonDbRequest *jsonDbRequest, RequestType requestType, QContactAbstractRequest *req, int contactIndex)
{
    QMutexLocker locker(m_operationMutex);
    m_jsonDbRequestTypeMap.insert(jsonDbRequest, requestType);
    if (m_activeRequests.contains(req)) {
        m_activeRequests.value(req)->m_jsonDbRequestMap.insert(jsonDbRequest, contactIndex);
    }
}

QContactAbstractRequest* QContactJsonDbRequestManager::removeRequest(QJsonDbRequest *jsonDbRequest, RequestType &requestType, int &contactIndex, QString *partitionName)
{
    QMutexLocker locker(m_operationMutex);
    if (m_jsonDbRequestTypeMap.contains(jsonDbRequest)) {
        requestType = m_jsonDbRequestTypeMap.value(jsonDbRequest);
        m_jsonDbRequestTypeMap.remove(jsonDbRequest);
        if (requestType == OrphanRequest || requestType == InvalidRequest)
            return 0;
    } else {
        qWarning() << Q_FUNC_INFO << "Could not find jsondbRequest:" << jsonDbRequest;
        requestType = InvalidRequest;
        return 0;
    }
    QList<QContactAbstractRequest*> reqList = m_activeRequests.keys();
    for (int i = 0; i < reqList.size(); i++) {
        QContactAbstractRequest* req = reqList.at(i);
        QMap<QJsonDbRequest*, int>* requestMap = &(m_activeRequests.value(req)->m_jsonDbRequestMap);
        if (requestMap->contains(jsonDbRequest)) {
            *partitionName = jsonDbRequest->partition();
            contactIndex = requestMap->value(jsonDbRequest);
            requestMap->remove(jsonDbRequest);
            return reqList.at(i);
        }
    }
    // request already deleted so mark this as orphan.
    requestType = OrphanRequest;
    return 0;
}


bool QContactJsonDbRequestManager::setWaitCondition(QContactAbstractRequest *req, QWaitCondition *waitCondition)
{
    // This function is called by JsonDbEngine, which is in another thread
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req)) {
        m_activeRequests.value(req)->m_waitCondition = waitCondition;
        return true;
    } else if (!m_inactiveRequests.contains(req)) {
        // Request and wait condition stored here if waitForRequestFinished function is called without starting
        // the request or after starting, but before slot JsonDbEngineRequestHandler::handleRequest is called.
        m_inactiveRequests.insert(req, waitCondition);
        return false;
    }
    //This should never happen...
    return false;
}

QWaitCondition* QContactJsonDbRequestManager::waitCondition(QContactAbstractRequest *req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req)) {
        return m_activeRequests.value(req)->m_waitCondition;
    }
    return 0;
}

void QContactJsonDbRequestManager::removeWaitCondition(QContactAbstractRequest *req)
{
    QMutexLocker locker(m_operationMutex);

    // The request is found only if waitForRequestFinished function was called for an Inactive request
    // which was never started.

    if (!m_inactiveRequests.contains(req)) {
        m_inactiveRequests.remove(req);
    }
}

void QContactJsonDbRequestManager::addContact(QContactAbstractRequest* req, QContact item, int contactIndex)
{
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req)) {
        m_activeRequests.value(req)->m_contactList.replace(contactIndex, item);
    }
}

QList<QContact> QContactJsonDbRequestManager::contacts(QContactAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req))
        return m_activeRequests.value(req)->m_contactList;
    else
        return QList<QContact>();
}



bool QContactJsonDbRequestManager::isRequestCompleted(QContactAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req)) {
        return m_activeRequests.value(req)->m_jsonDbRequestMap.isEmpty();
    }
    return true;
}

bool QContactJsonDbRequestManager::pendingPrefetchRequests(QContactSaveRequest *saveReq)
{
    QMutexLocker locker(m_operationMutex);
    return ((m_activeRequests.contains(saveReq)) &&
            (!m_prefetchRequestsMap.keys(saveReq).isEmpty()));
}

bool QContactJsonDbRequestManager::contains(QContactAbstractRequest *req) const
{
    QMutexLocker locker(m_operationMutex);
    return m_activeRequests.contains(req);// || m_inactiveRequests.contains(req);
}

#include "moc_qcontactjsondbrequestmanager.cpp"

QT_END_NAMESPACE_CONTACTS
