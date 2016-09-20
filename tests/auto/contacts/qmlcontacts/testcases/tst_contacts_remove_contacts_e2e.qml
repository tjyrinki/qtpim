/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtPim module of the Qt Toolkit.
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

import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

ContactsSavingTestCase {
    name: "ContactsRemoveContactsE2ETests"
    id: contactsRemoveContactsE2ETests

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
    }

    Contact {
        id: firstContact
    }

    Contact {
        id: secondContact
    }

    function init() {
        initTestForModel(model);
        emptyContacts(model);

        model.saveContact(firstContact);
        waitForContactsChanged();
        model.saveContact(secondContact);
        waitForContactsChanged();
    }

    function cleanup() {
        emptyContacts(model);
    }

    // Tests

    function test_removeEmptyList() {
        model.removeContacts([]);
        wait(500);

        compare(model.contacts.length, 2, "contacts.length");
    }

    function test_removeInvalidId() {
        model.removeContacts(["invalid"]);
        wait(500);

        compare(model.contacts.length, 2, "contacts.length");
    }

    function test_removeOneContact() {
        var id1 = model.contacts[0].contactId;
        var id2 = model.contacts[1].contactId;

        model.removeContacts([id1]);
        waitForContactsChanged();

        compare(model.contacts.length, 1, "contacts.length");
        compare(model.contacts[0].contactId, id2, "id of the second contact");
    }

    function test_removeMultipleContacts() {
        var id1 = model.contacts[0].contactId;
        var id2 = model.contacts[1].contactId;

        model.removeContacts([id1, id2]);
        waitForContactsChanged();
        if (model.contacts.length > 0)
            waitForContactsChanged();


        compare(model.contacts.length, 0, "contacts is empty");
    }

    function test_saveRemovedContactFails() {
        var id1 = model.contacts[0].contactId;
        var contactToFirstRemoveAndThenSave = model.contacts[0];

        model.removeContacts([id1]);
        waitForContactsChanged();

        var errorSpy = initTestForTargetListeningToSignal(model, "errorChanged");
        model.saveContact(contactToFirstRemoveAndThenSave);
        waitForTargetSignal(errorSpy);

        compare(model.error, "DoesNotExist", "model.error");
    }


    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitForContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);
    }

    function cleanupTestCase() {
        emptyContacts(model);
        finishTestForModel(model);
    }

    // Helpers

    function verifyIsUndefined(object) {
        verify(!object, "Object " + object + " is undefined");
    }
}
