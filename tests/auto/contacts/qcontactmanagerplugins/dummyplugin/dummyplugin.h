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
#ifndef DUMMYPLUGIN_H
#define DUMMYPLUGIN_H

#include <QtContacts/qcontactmanager.h>
#include <QtContacts/qcontactmanagerengine.h>
#include <QtContacts/qcontactmanagerenginefactory.h>
#include <QtCore/qscopedpointer.h>

QTCONTACTS_USE_NAMESPACE

class DummyEngine : public QContactManagerEngine
{
    Q_OBJECT
public:
    DummyEngine(const QMap<QString, QString>& parameters, QContactManager::Error* error);
    DummyEngine(const DummyEngine& other);
    ~DummyEngine();
    DummyEngine& operator=(const DummyEngine& other);
    QContactManagerEngine* clone();
    void deref();
    QString managerName() const;

    /* Contacts - Accessors and Mutators */
    QList<QContactId> contacts(QContactManager::Error* error) const;
    QContact contact(const QContactId& contactId, QContactManager::Error* error) const;
    QContact contact(const QContactId& contactId, const QContactFetchHint& fetchHint, QContactManager::Error* error) const;
    bool saveContact(QContact* contact, bool batch, QContactManager::Error* error);
    bool removeContact(const QContactId& contactId, bool batch, QContactManager::Error* error);

    /* Capabilities reporting */
    QStringList capabilities() const;
    QStringList fastFilterableDefinitions() const;
    QList<QVariant::Type> supportedDataTypes() const;

    QMap<QString, QString> managerParameters() const {return QMap<QString, QString>();}
    int managerVersion() const {return 0;}

    QList<QContactId> contactIds(const QContactFilter&, const QList<QContactSortOrder>&, QContactManager::Error* error) const
    {
        *error = QContactManager::NotSupportedError;
        return QList<QContactId>();
    }

    QList<QContact> contacts(const QContactFilter&, const QList<QContactSortOrder>&, const QContactFetchHint&, QContactManager::Error* error) const
    {
        *error = QContactManager::NotSupportedError;
        return QList<QContact>();
    }

    bool saveContacts(QList<QContact>*, QMap<int, QContactManager::Error>*, QContactManager::Error* error)
    {
        *error = QContactManager::NotSupportedError;
        return false;
    }

    bool removeContacts(const QList<QContactId>&, QMap<int, QContactManager::Error>*, QContactManager::Error* error)
    {
        *error = QContactManager::NotSupportedError;
        return false;
    }

    QContact conformingContact(const QContact&, QContactManager::Error* error)
    {
        *error = QContactManager::NotSupportedError;
        return QContact();
    }

    /* Synthesize the display label of a contact */
    virtual QString synthesizedDisplayLabel(const QContact&, QContactManager::Error* error) const
    {
        *error = QContactManager::NotSupportedError;
        return QString();
    }

    /* "Self" contact id (MyCard) */
    virtual bool setSelfContactId(const QContactId&, QContactManager::Error* error)
    {
        *error = QContactManager::NotSupportedError;
        return false;
    }

    QContactId selfContactId(QContactManager::Error* error) const
    {
        return QContactManagerEngine::selfContactId(error);
    }

    /* Relationships between contacts */
    virtual QList<QContactRelationship> relationships(const QString&, const QContactId&, QContactRelationship::Role, QContactManager::Error* error) const
    {
        *error = QContactManager::NotSupportedError;
        return QList<QContactRelationship>();
    }

    virtual bool saveRelationships(QList<QContactRelationship>*, QMap<int, QContactManager::Error>*, QContactManager::Error* error)
    {
        *error = QContactManager::NotSupportedError;
        return false;
    }

    virtual bool removeRelationships(const QList<QContactRelationship>&, QMap<int, QContactManager::Error>*, QContactManager::Error* error)
    {
        *error = QContactManager::NotSupportedError;
        return false;
    }

    /* Validation for saving */
    virtual bool validateContact(const QContact&, QContactManager::Error* error) const
    {
        *error = QContactManager::NotSupportedError;
        return false;
    }

    /* Asynchronous Request Support */
    virtual void requestDestroyed(QContactAbstractRequest*) {}
    virtual bool startRequest(QContactAbstractRequest*) {return false;}
    virtual bool cancelRequest(QContactAbstractRequest*) {return false;}
    virtual bool waitForRequestFinished(QContactAbstractRequest*, int) {return false;}

    virtual bool isRelationshipTypeSupported(const QString&, const QString&) const
    {
        return false;
    }

    virtual bool isFilterSupported(const QContactFilter&) const
    {
        return false;
    }

    virtual QList<QContactType::TypeValues> supportedContactTypes() const
    {
        return QList<QContactType::TypeValues>();
    }

};

class DummyEngineFactory : public QContactManagerEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QContactManagerEngineFactoryInterface" FILE "dummy.json")
    public:
        QContactManagerEngine* engine(const QMap<QString, QString>& parameters, QContactManager::Error* error);
        QContactEngineId* createContactEngineId(const QMap<QString, QString>& parameters, const QString& engineIdString) const;
        QString managerName() const;
};

#endif // DUMMYPLUGIN_H
