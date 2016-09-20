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

#ifndef tst_QVERSITCONTACTIMPORTER_H
#define tst_QVERSITCONTACTIMPORTER_H

#include <QObject>
#include <QtVersit/qversitcontactimporter.h>
#include <QtVersit/qversitdocument.h>

QT_BEGIN_NAMESPACE_VERSIT
class QVersitContactImporter;
class QVersitContactImporterPrivate;
QT_END_NAMESPACE_VERSIT

QTVERSIT_USE_NAMESPACE

class MyQVersitContactImporterPropertyHandler;
class MyQVersitResourceHandler;

class tst_QVersitContactImporter : public QObject
{
    Q_OBJECT

private slots: // Tests
    void init();
    void cleanup();

    void testName();
    void testNameWithFormatted();
    void testAddress();
    void testAddressWithoutSubTypes();
    void testTel();
    void testTelWithoutSubTypes();
    void testEmail();
    void testUrl();
    void testUid();
    void testOrganizationName();
    void testOrganizationTitle();
    void testOrganizationLogo();
    void testOrganizationAssistant();
    void testOrganizationRole();
    void testTimeStamp();
    void testVersion();
    void testVersion_data();
    void testMultipleVersions();
    void testAnniversary();
    void testBirthday();
    void testGender();
    void testNickname();
    void testAvatarUrl();
    void testAvatarInvalid();
    void testGeo();
    void testNote();
    void testDisplayLabel();
    void testOnlineAccount();
    void testFamily();
    void testFavorite();
    void testFavorite_data();
    void testMultipleFavorites();
    void testSound();
    void testTag();
    void testExtendedDetail();
    void testExtendedDetail_data();
    void testMultipleExtendedDetails();
    void testPref();
    void testPropertyHandler();
    void testInvalidDocument();
    void testEmailWithContextOther();

private: // Utilities

    QVersitDocument createDocumentWithProperty(const QVersitProperty& property);

    QVersitDocument createDocumentWithNameAndPhoto(
        const QString& name,
        QByteArray image,
        const QString& photoType);

    void addVersionPropertyToDocument(QString sequenceNumber, QString extendedVersion, QVersitDocument &document);
    void addFavoritePropertyToDocument(QString favorite, QString index, QVersitDocument &document);
    void addExtendedDetailPropertyToDocument(QString name, QString data, QVersitDocument &document);

private:
    QVersitContactImporter* mImporter;
    MyQVersitContactImporterPropertyHandler* mPropertyHandler;
    MyQVersitResourceHandler* mResourceHandler;
};

#endif // tst_QVERSITCONTACTIMPORTER_H
