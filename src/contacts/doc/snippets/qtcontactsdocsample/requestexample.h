/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt Toolkit.
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


#ifndef REQUESTEXAMPLE_H
#define REQUESTEXAMPLE_H

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

#include <QObject>

#include <QtContacts/qcontactrequests.h>

//! [Class setup]
QTCONTACTS_USE_NAMESPACE
class AsyncRequestExample : public QObject
{
    Q_OBJECT

public:
    AsyncRequestExample();
    ~AsyncRequestExample();

public slots:
    void performRequests();

private slots:
    void contactFetchRequestStateChanged(QContactAbstractRequest::State newState);
    void contactSaveRequestStateChanged(QContactAbstractRequest::State newState);
    void contactRemoveRequestStateChanged(QContactAbstractRequest::State newState);
    void relationshipFetchRequestStateChanged(QContactAbstractRequest::State newState);
    void relationshipSaveRequestStateChanged(QContactAbstractRequest::State newState);
    void relationshipRemoveRequestStateChanged(QContactAbstractRequest::State newState);

private:
    QContactManager *m_manager;
    QContactFetchRequest m_contactFetchRequest;
    QContactSaveRequest m_contactSaveRequest;
    QContactRemoveRequest m_contactRemoveRequest;
    QContactRelationshipFetchRequest m_relationshipFetchRequest;
    QContactRelationshipSaveRequest m_relationshipSaveRequest;
    QContactRelationshipRemoveRequest m_relationshipRemoveRequest;
};
//! [Class setup]

class RequestExample : public QObject
{
    Q_OBJECT

public:
    RequestExample()
            : QObject(),
            m_previousLastIndex(-1),
            m_manager(0),
            m_fetchRequest(new QContactFetchRequest)
    {
    }

    ~RequestExample()
    {
        delete m_fetchRequest;
    }

    void setManager(QContactManager* manager)
    {
        m_manager = manager;
        m_fetchRequest->setManager(m_manager);
    }

private slots:
    void performRequest();
    void printContacts();
    void stateChanged(QContactAbstractRequest::State state);

private:
    int m_previousLastIndex;
    QContactManager *m_manager;
    QContactFetchRequest *m_fetchRequest;
};

#endif
