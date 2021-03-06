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

//TESTED_COMPONENT=src/versit

#include "tst_qversit.h"
#include <QtContacts/qcontacts.h>
#include <QtOrganizer/qorganizer.h>
#include <QtVersit/qversitwriter.h>
#include <QtVersit/qversitreader.h>
#include <QtVersit/qversitcontactexporter.h>
#include <QtVersit/qversitcontactimporter.h>
#include <QtVersit/qversitcontacthandler.h>
#include <QtVersit/qversitdocument.h>
#include <QtVersit/private/qversitdefs_p.h>
#include <QtVersit/private/qversitreader_p.h>
#include <QtVersitOrganizer/qversitorganizerexporter.h>
#include <QtVersitOrganizer/qversitorganizerimporter.h>

#include <QtTest/QtTest>
#include <QDebug>
#include <QDir>
#include <QList>

QTCONTACTS_USE_NAMESPACE
QTORGANIZER_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

class MyQVersitResourceHandler : public QVersitResourceHandler
{
public:
    MyQVersitResourceHandler() : mIndex(0)
    {
    }

    bool saveResource(const QByteArray& contents, const QVersitProperty& property,
                      QString* location)
    {
        Q_UNUSED(property)
        *location = QString::number(mIndex++);
        mObjects.insert(*location, contents);
        return true;
    }

    bool loadResource(const QString &location, QByteArray *contents, QString *mimeType)
    {
        Q_UNUSED(location)
        Q_UNUSED(contents)
        Q_UNUSED(mimeType)
        return false;
    }

    int mIndex;
    QMap<QString, QByteArray> mObjects;
};

#ifndef TESTDATA_DIR
#define TESTDATA_DIR "./"
#endif

Q_DECLARE_METATYPE(QList<QContact>)
Q_DECLARE_METATYPE(QContact)
Q_DECLARE_METATYPE(QList<QOrganizerItem>)
Q_DECLARE_METATYPE(QOrganizerItem)

class tst_QVersit : public QObject
{
    Q_OBJECT

private slots: // Tests
    void testImportVCardFiles();
    void testImportVCardFiles_data();
    void testBackupVCard();
    void testBackupVCard_data();
    void testPreserveVCard();
    void testPreserveVCard_data();
    void testPreserveVCardWithBackup();
    void testPreserveVCardWithBackup_data();
    void testImportICalFiles();
    void testImportICalFiles_data();

private:
};

void tst_QVersit::testImportVCardFiles()
{
    QFETCH(QString, filename);
    QFETCH(QByteArray, charset);
    QFETCH(QList<QContact>, expectedContacts);

    filename = QLatin1String(TESTDATA_DIR "testdata_vcf/") + filename;
    QVersitReader reader;
    QFile file(filename);
    QVERIFY2(file.open(QIODevice::ReadOnly), filename.toLatin1());
    reader.setDevice(&file);
    if (charset != "") {
        reader.setDefaultCodec(QTextCodec::codecForName(charset));
    }
    QVERIFY(reader.startReading());
    QVERIFY(reader.waitForFinished());
    QList<QVersitDocument> documents = reader.results();
    QVERIFY(!documents.isEmpty());
    QCOMPARE(reader.error(), QVersitReader::NoError);
    QVersitContactImporter importer;
    MyQVersitResourceHandler resourceHandler;
    importer.setResourceHandler(&resourceHandler);
    QVERIFY(importer.importDocuments(documents));
    QList<QContact> contacts = importer.contacts();
    QVERIFY(!contacts.isEmpty());

    if (expectedContacts.size() > 0) {
        QCOMPARE(contacts.size(), expectedContacts.size());
        QListIterator<QContact> i(expectedContacts);
        foreach (QContact parsed, contacts) {
            QContact expected = i.next();
            QList<QContactDetail> expectedDetails = expected.details();
            foreach(QContactDetail expectedDetail, expectedDetails) {
                QContactDetail::DetailType type = expectedDetail.type();
                QContactDetail parsedDetail = parsed.detail(type);
                if (parsedDetail != expectedDetail) {
                    qDebug() << "Actual:" << parsedDetail.value(1);
                    qDebug() << "Expected:" << expectedDetail.value(1);
                    QCOMPARE(parsedDetail, expectedDetail);
                }
            }
        }
    }
}

void tst_QVersit::testImportVCardFiles_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QByteArray>("charset");
    QTest::addColumn<QList<QContact> >("expectedContacts");

    QTest::newRow("AAB4-MultipleAll.vcf") << QString::fromLatin1("AAB4-MultipleAll.vcf")
        << QByteArray("UTF-16BE") << QList<QContact>();
    QTest::newRow("AAB4-MultipleAscii.vcf") << QString::fromLatin1("AAB4-MultipleAscii.vcf")
        << QByteArray("") << QList<QContact>();
    QTest::newRow("AAB4-SingleCompany.vcf") << QString::fromLatin1("AAB4-SingleCompany.vcf")
        << QByteArray("") << QList<QContact>();
    QTest::newRow("AAB4-SingleExtensive.vcf") << QString::fromLatin1("AAB4-SingleExtensive.vcf")
        << QByteArray("") << QList<QContact>();
    QTest::newRow("AAB4-SingleNonAscii.vcf") << QString::fromLatin1("AAB4-SingleNonAscii.vcf")
        << QByteArray("UTF-16BE") << QList<QContact>();
    QTest::newRow("AAB4-SingleNonAsciiWithPhoto.vcf") << QString::fromLatin1("AAB4-SingleNonAsciiWithPhoto.vcf")
        << QByteArray("UTF-16BE") << QList<QContact>();
    QTest::newRow("AAB5-SingleNonAscii.vcf") << QString::fromLatin1("AAB5-SingleNonAscii.vcf")
        << QByteArray("UTF-8") << QList<QContact>();

    {
        QList<QContact> list;
        QContact contact;
        QContactName name;
        name.setCustomLabel(QLatin1String("Firstname Lastname"));
        name.setFirstName(QLatin1String("Firstname"));
        name.setLastName(QLatin1String("Lastname"));
        name.setPrefix(QLatin1String("Title"));
        name.setSuffix(QLatin1String("Suffix"));
        contact.saveDetail(&name);
        QContactOrganization org;
        org.setName(QLatin1String("Company Name"));
        org.setDepartment(QStringList(QLatin1String("Department Name")));
        org.setTitle(QLatin1String("Job title"));
        contact.saveDetail(&org);
        QContactNote note;
        note.setNote(QLatin1String("This is a note field.  Pretty boring."));
        contact.saveDetail(&note);
        QContactManagerEngine::setContactDisplayLabel(&contact, QLatin1String("Firstname Lastname"));
        list.append(contact);
        QContactUrl homeUrl;
        homeUrl.setUrl(QLatin1String("http://mywebpage.com"));
        homeUrl.setContexts(QContactDetail::ContextHome);
        contact.saveDetail(&homeUrl);
        QContactUrl workUrl;
        workUrl.setUrl(QLatin1String("http://workwebpage"));
        workUrl.setContexts(QContactDetail::ContextWork);
        contact.saveDetail(&workUrl);
        QTest::newRow("Entourage11-basic.vcf") << QString::fromLatin1("Entourage11-basic.vcf")
            << QByteArray("UTF-16BE") << list;
    }

    QTest::newRow("Entourage11-image.vcf") << QString::fromLatin1("Entourage11-image.vcf")
        << QByteArray("UTF-16BE") << QList<QContact>();

    QTest::newRow("Entourage11-nonascii.vcf") << QString::fromLatin1("Entourage11-nonascii.vcf")
        << QByteArray("UTF-16BE") << QList<QContact>();

    {
        QList<QContact> list;
        QContact contact;
        QContactName name;
        name.setCustomLabel(QLatin1String("first last"));
        name.setFirstName(QLatin1String("first"));
        name.setLastName(QLatin1String("last"));
        contact.saveDetail(&name);
        QContactOrganization org;
        org.setName(QLatin1String("Nokia"));
        org.setDepartment(QStringList(QLatin1String("Qt DF")));
        contact.saveDetail(&org);
        QContactManagerEngine::setContactDisplayLabel(&contact, QLatin1String("first last"));
        list.append(contact);
        QTest::newRow("Entourage12-basic.vcf") << QString::fromLatin1("Entourage12-basic.vcf")
            << QByteArray("") << list;
    }

    QTest::newRow("Entourage12-kevin.vcf") << QString::fromLatin1("Entourage12-kevin.vcf")
        << QByteArray("UTF-8") << QList<QContact>();
    QTest::newRow("Entourage12-nonascii.vcf") << QString::fromLatin1("Entourage12-nonascii.vcf")
        << QByteArray("UTF-8") << QList<QContact>();
    QTest::newRow("gmail.vcf") << QString::fromLatin1("gmail.vcf")
        << QByteArray("UTF-8") << QList<QContact>();

    {
        QContact contact;
        QContactName name;
        name.setFirstName(QLatin1String("name"));
        contact.saveDetail(&name);
        QContactFamily family;
        family.setChildren(QStringList(QLatin1String("Child1")));
        contact.saveDetail(&family);
        family.setChildren(QStringList(QLatin1String("Child2")) << QLatin1String("Child3"));
        contact.saveDetail(&family);
        QContactNickname nickname;
        nickname.setNickname(QLatin1String("Nick6"));
        contact.saveDetail(&nickname);
        nickname.setNickname(QLatin1String("Nick5"));
        contact.saveDetail(&nickname);
        nickname.setNickname(QLatin1String("Nick4"));
        contact.saveDetail(&nickname);
        nickname.setNickname(QLatin1String("Nick3"));
        contact.saveDetail(&nickname);
        nickname.setNickname(QLatin1String("Nick2"));
        contact.saveDetail(&nickname);
        nickname.setNickname(QLatin1String("Nick1"));
        contact.saveDetail(&nickname);
        QContactPhoneNumber assistantphone;
        assistantphone.setNumber(QLatin1String("1234"));
        assistantphone.setSubTypes(QList<int> () << QContactPhoneNumber::SubTypeAssistant);
        contact.saveDetail(&assistantphone);
        QContactGeoLocation geo;
        geo.setLatitude(32.0);
        geo.setLongitude(-64);
        contact.saveDetail(&geo);
        QContactManagerEngine::setContactDisplayLabel(&contact, QLatin1String("name"));
        QTest::newRow("test1.vcf") << QString::fromLatin1("test1.vcf")
            << QByteArray("UTF-8") << (QList<QContact>() << contact);
    }

    // A file with bad wrapping (no preceding space on a line continuation)
    QTest::newRow("badwrap.vcf") << QString::fromLatin1("badwrap.vcf")
        << QByteArray("UTF-8") << QList<QContact>();
}

void tst_QVersit::testBackupVCard()
{
    // Test that using the backup profile, a contact, when exported and imported again, is unaltered.
    QFETCH(QContact, contact);
    QVersitContactExporter exporter(QVersitContactHandlerFactory::ProfileBackup);
    QVERIFY(exporter.exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QList<QVersitDocument> documents = exporter.documents();
    QCOMPARE(documents.size(), 1);

    QByteArray documentBytes;
    QVersitWriter writer(&documentBytes);
    writer.startWriting(documents);
    writer.waitForFinished();

    QVersitReader reader(documentBytes);
    reader.startReading();
    reader.waitForFinished();
    QList<QVersitDocument> parsedDocuments = reader.results();
    QCOMPARE(parsedDocuments.size(), 1);

    QVersitContactImporter importer(QVersitContactHandlerFactory::ProfileBackup);
    QVERIFY(importer.importDocuments(parsedDocuments));
    QList<QContact> contacts = importer.contacts();
    QCOMPARE(contacts.size(), 1);
    if (contacts.first().details() != contact.details()) {
        qDebug() << "Versit documents:" << documentBytes;
        qDebug() << "Actual:" << contacts.first();
        qDebug() << "Expected:" << contact;
        QCOMPARE(contacts.first().details(), contact.details());
    }
}

enum Color { RED, GREEN, BLUE };

void tst_QVersit::testBackupVCard_data()
{
    QTest::addColumn<QContact>("contact");

    // The test contacts need at least a name (so the resulting vCard is valid and a display label (because
    // otherwise, the imported display label will be set from the name or something)
    {
        QContact contact;
        QContactName name;
        name.setCustomLabel(QLatin1String("name"));
        contact.saveDetail(&name);
        QContactManagerEngine::setContactDisplayLabel(&contact, QLatin1String("name"));
        QTest::newRow("just a name") << contact;
    }

    {
        QContact contact;
        QContactName name;
        name.setFirstName(QLatin1String("first"));
        name.setLastName(QLatin1String("last"));
        name.setCustomLabel(QLatin1String("custom"));
        name.setValue(123456, QLatin1String("RandomValue1"));
        name.setValue(123457, QLatin1String("RandomValue1"));
        contact.saveDetail(&name);
        QContactDetail customDetail1(QContactDetail::TypeExtendedDetail);
        customDetail1.setValue(QContactExtendedDetail::FieldName, QLatin1String("CustomDetail1"));
        customDetail1.setValue(123456, QLatin1String("Value11"));
        customDetail1.setValue(123457, QLatin1String("Value12"));
        contact.saveDetail(&customDetail1);
        QContactDetail customDetail2(QContactDetail::TypeExtendedDetail);
        customDetail2.setValue(QContactExtendedDetail::FieldName, QLatin1String("CustomDetail2"));
        customDetail2.setValue(123456, QLatin1String("Value21"));
        customDetail2.setValue(123457, QLatin1String("Value22"));
        contact.saveDetail(&customDetail2);
        contact.setType(QContactType::TypeContact);
        QContactManagerEngine::setContactDisplayLabel(&contact, QLatin1String("custom"));
        QTest::newRow("custom detail grouping") << contact;
    }

    // Test of some non-string fields
    QContact contact;
    QContactName name;
    name.setCustomLabel(QLatin1String("name"));
    contact.saveDetail(&name);
    QContactManagerEngine::setContactDisplayLabel(&contact, QLatin1String("name"));
    QContactDetail customDetail(QContactDetail::TypeExtendedDetail);
    customDetail.setValue(QContactExtendedDetail::FieldName, QLatin1String("CustomDetailBlob"));
    customDetail.setValue(QContactExtendedDetail::FieldData, QByteArray("blob"));
    contact.saveDetail(&customDetail);
    QTest::newRow("binary field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, QDate(2010, 5, 18));
    contact.saveDetail(&customDetail);
    QTest::newRow("date field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, QTime(11, 25));
    contact.saveDetail(&customDetail);
    QTest::newRow("time field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, QDateTime(QDate(2010, 5, 18), QTime(11, 25)));
    contact.saveDetail(&customDetail);
    QTest::newRow("datetime field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, (int)42);
    contact.saveDetail(&customDetail);
    QTest::newRow("integer field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, UINT_MAX);
    contact.saveDetail(&customDetail);
    QTest::newRow("unsigned integer field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, QUrl(QLatin1String("http://www.nokia.com/")));
    contact.saveDetail(&customDetail);
    QTest::newRow("url field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, (bool)true);
    contact.saveDetail(&customDetail);
    QTest::newRow("bool field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, (bool)false);
    contact.saveDetail(&customDetail);
    QTest::newRow("false bool field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, (double)3.14159265);
    contact.saveDetail(&customDetail);
    QTest::newRow("double field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, (float)3.14159265);
    contact.saveDetail(&customDetail);
    QTest::newRow("float field") << contact;

    customDetail.setValue(QContactExtendedDetail::FieldData, RED);
    contact.saveDetail(&customDetail);
    QTest::newRow("enum field") << contact;
}

/*! Returns a list of properties from \a document that have the name \a propertyName */
QList<QVersitProperty> findPropertiesByName(
        const QVersitDocument &document, const QString &propertyName)
{
    QList<QVersitProperty> result;
    foreach (const QVersitProperty& property, document.properties()) {
        if (property.name() == propertyName)
            result << property;
    }
    return result;
}

/*! Returns true iff supers.size() == subs.size() and for every i, supers[i] has a superset of the
 * properties of subs[i]. */
bool areSuperdocuments(const QList<QVersitDocument>& supers, const QList<QVersitDocument>& subs) {
    if (supers.size() != subs.size())
        return false;

    for (int i = 0; i < supers.size(); i++) {
        const QVersitDocument& super = supers[i];
        const QVersitDocument& sub = subs[i];

        if (sub.type() != super.type())
            return false;
        if (sub.componentType() != super.componentType())
            return false;

        foreach (const QVersitProperty& property, sub.properties()) {
            QList<QVersitProperty> matches(findPropertiesByName(super, property.name()));
            bool foundMatch = false;
            foreach (const QVersitProperty& candidate, matches) {
                if (candidate.valueType() == property.valueType()
                        && candidate.variantValue() == property.variantValue()) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                return false;
            }
        }
    }
    return true;
}

void tst_QVersit::testPreserveVCard()
{
    // Test that using the preserver profile, a document, when imported and exported again, is unaltered.
    QFETCH(QByteArray, documentBytes);

    QVersitReader reader(documentBytes);
    reader.startReading();
    reader.waitForFinished();
    QList<QVersitDocument> parsedDocuments = reader.results();

    QVersitDocument::VersitType versitType = parsedDocuments.first().type();
    QVersitContactImporter importer("Preserve");
    QVERIFY(importer.importDocuments(parsedDocuments));
    QList<QContact> contacts = importer.contacts();

    QVersitContactExporter exporter("Preserve");
    QVERIFY(exporter.exportContacts(contacts, versitType));
    QList<QVersitDocument> documents = exporter.documents();

    QByteArray writtenBytes;
    QVersitWriter writer(&writtenBytes);
    writer.startWriting(documents);
    writer.waitForFinished();

    if (!areSuperdocuments(documents, parsedDocuments)) {
        qDebug() << "Contacts:" << contacts;
        qDebug() << "Actual:" << writtenBytes;
        qDebug() << "Expected:" << documentBytes;
        QVERIFY2(false, "Actual vCards aren't supersets of Expected vCards");
    }
}

void tst_QVersit::testPreserveVCard_data()
{
    QTest::addColumn<QByteArray>("documentBytes");

    QTest::newRow("basic") << QByteArray(
            "BEGIN:VCARD\r\n"
            "VERSION:2.1\r\n"
            "FN:John\r\n"
            "END:VCARD\r\n");

    QTest::newRow("custom property") << QByteArray(
            "BEGIN:VCARD\r\n"
            "VERSION:2.1\r\n"
            "FN:John\r\n"
            "X-SOMETHING:Nothing\r\n"
            "END:VCARD\r\n");
}

void tst_QVersit::testPreserveVCardWithBackup()
{
    // Test that the preserver and backup profile work well together
    QFETCH(QByteArray, documentBytes);

    // Read them
    QVersitReader reader(documentBytes);
    reader.startReading();
    reader.waitForFinished();
    QList<QVersitDocument> parsedDocuments = reader.results();

    // Import them
    QVersitDocument::VersitType versitType = parsedDocuments.first().type();
    QVersitContactImporter importer(QStringList()
            << "Preserve"
            << QVersitContactHandlerFactory::ProfileBackup);
    QVERIFY(importer.importDocuments(parsedDocuments));
    QList<QContact> contacts = importer.contacts();

    // Make a little change
    QVERIFY(contacts.size() >= 1);
    QContactDetail customDetail(QContactDetail::TypeExtendedDetail);
    customDetail.setValue(QContactExtendedDetail::FieldName, QLatin1String("CUSTOMDETAIL"));
    customDetail.setValue(QContactExtendedDetail::FieldData, QLatin1String("value"));
    contacts[0].saveDetail(&customDetail);

    // Export them
    QVersitContactExporter exporter(QStringList()
            << QVersitContactHandlerFactory::ProfileBackup
            << "Preserve");
    QVERIFY(exporter.exportContacts(contacts, versitType));
    QList<QVersitDocument> documents = exporter.documents();

    QByteArray writtenBytes;
    QVersitWriter writer(&writtenBytes);
    writer.startWriting(documents);
    writer.waitForFinished();

    if (!areSuperdocuments(documents, parsedDocuments)) {
        qDebug() << "Contacts:" << contacts;
        qDebug() << "Actual:" << writtenBytes;
        qDebug() << "Expected:" << documentBytes;
        QVERIFY2(false, "Actual vCards aren't supersets of Expected vCards");
    }

    // Import them (again)
    QVERIFY(importer.importDocuments(documents));
    QList<QContact> contacts2 = importer.contacts();

    if (contacts != contacts2) {
        qDebug() << "Versit documents:" << writtenBytes;
        qDebug() << "Actual:" << contacts2;
        qDebug() << "Expected:" << contacts;
        QVERIFY(contacts == contacts2);
    }
}

void tst_QVersit::testPreserveVCardWithBackup_data() {
    testPreserveVCard_data();
}

void tst_QVersit::testImportICalFiles()
{
    QFETCH(QString, filename);
    QFETCH(QByteArray, charset);
    QFETCH(QList<QOrganizerItem>, expectedItems);

    filename = QLatin1String(TESTDATA_DIR "testdata_ics/") + filename;
    QVersitReader reader;
    QFile file(filename);
    QVERIFY2(file.open(QIODevice::ReadOnly), filename.toLatin1());
    reader.setDevice(&file);
    if (charset != "") {
        reader.setDefaultCodec(QTextCodec::codecForName(charset));
    }
    QVERIFY(reader.startReading());
    QVERIFY(reader.waitForFinished());
    QList<QVersitDocument> documents = reader.results();
    QCOMPARE(reader.error(), QVersitReader::NoError);
    QCOMPARE(documents.size(), 1);
    QVersitOrganizerImporter importer;
    QVERIFY(importer.importDocument(documents.first()));
    QList<QOrganizerItem> items = importer.items();

    if (expectedItems.size() > 0) {
        QCOMPARE(items.size(), expectedItems.size());
        QListIterator<QOrganizerItem> i(expectedItems);
        foreach (QOrganizerItem parsed, items) {
            QOrganizerItem expected = i.next();
            QList<QOrganizerItemDetail> expectedDetails = expected.details();
            foreach(QOrganizerItemDetail expectedDetail, expectedDetails) {
                QOrganizerItemDetail::DetailType name = expectedDetail.type();
                QOrganizerItemDetail parsedDetail = parsed.detail(name);
                if (parsedDetail != expectedDetail) {
                    qDebug() << "Detail: " << name;
                    qDebug() << "Actual:" << parsedDetail.values();
                    qDebug() << "Expected:" << expectedDetail.values();
                    QCOMPARE(parsedDetail, expectedDetail);
                }
            }
        }
    }
}

void tst_QVersit::testImportICalFiles_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QByteArray>("charset");
    QTest::addColumn<QList<QOrganizerItem> >("expectedItems");

    QTest::newRow("2010-FIFA-WorldCup.ics") << QString::fromLatin1("2010-FIFA-WorldCup.ics")
        << QByteArray("") << QList<QOrganizerItem>();
    QTest::newRow("2010-India-Holidays.ics") << QString::fromLatin1("2010-India-Holidays.ics")
        << QByteArray("") << QList<QOrganizerItem>();
    QTest::newRow("2010-US-Holidays.ics") << QString::fromLatin1("2010-US-Holidays.ics")
        << QByteArray("") << QList<QOrganizerItem>();
    QTest::newRow("Asiacup2010.ics") << QString::fromLatin1("Asiacup2010.ics")
        << QByteArray("") << QList<QOrganizerItem>();
    QTest::newRow("FinlandHolidays.ics") << QString::fromLatin1("FinlandHolidays.ics")
        << QByteArray("") << QList<QOrganizerItem>();
    QTest::newRow("india2010.ics") << QString::fromLatin1("india2010.ics")
        << QByteArray("") << QList<QOrganizerItem>();
    QTest::newRow("IndiaHolidays.ics") << QString::fromLatin1("IndiaHolidays.ics")
        << QByteArray("") << QList<QOrganizerItem>();
    QTest::newRow("worldt2009.ics") << QString::fromLatin1("worldt2009.ics")
        << QByteArray("") << QList<QOrganizerItem>();
}

QTEST_MAIN(tst_QVersit)

#include "tst_qversit.moc"
