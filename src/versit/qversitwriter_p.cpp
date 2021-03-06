/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtVersit module of the Qt Toolkit.
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

#include "qversitwriter_p.h"

#include <QtCore/qbuffer.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qtextcodec.h>

#include "qvcard21writer_p.h"
#include "qvcard30writer_p.h"
#include "qversitdocumentwriter_p.h"
#include "qversitutils_p.h"

QT_BEGIN_NAMESPACE_VERSIT

/*! Constructs a writer. */
QVersitWriterPrivate::QVersitWriterPrivate()
    : mIoDevice(0),
    mState(QVersitWriter::InactiveState),
    mError(QVersitWriter::NoError),
    mIsCanceling(false),
    mDefaultCodec(0)
{
}

/*! Destroys a writer. */
QVersitWriterPrivate::~QVersitWriterPrivate()
{
}

/*! Links the signals from this to the signals of \a writer. */
void QVersitWriterPrivate::init(QVersitWriter* writer)
{
    qRegisterMetaType<QVersitWriter::State>("QVersitWriter::State");
    connect(this, SIGNAL(stateChanged(QVersitWriter::State)),
            writer, SIGNAL(stateChanged(QVersitWriter::State)), Qt::DirectConnection);
}

/*!
 * Do the actual writing and set the error and state appropriately.
 */
void QVersitWriterPrivate::write()
{
    bool canceled = false;

    // Try to get the type from the parameter to startWriting...
    QVersitDocument::VersitType type = documentType();

    foreach (const QVersitDocument& document, mInput) {
        if (isCanceling()) {
            canceled = true;
            break;
        }

        // Get type from the document if not specified in startWriting
        if (type == QVersitDocument::InvalidType)
            type = document.type();

        QScopedPointer<QVersitDocumentWriter> writer(writerForType(type, document));
        QTextCodec* codec = mDefaultCodec;
        if (codec == NULL) {
            if (type == QVersitDocument::VCard21Type) {
                codec = QTextCodec::codecForName("ISO-8859-1");
                writer->setAsciiCodec();
            } else {
                codec = QTextCodec::codecForName("UTF-8");
            }
        }
        writer->setCodec(codec);
        writer->setDevice(mIoDevice);
        if (!writer->encodeVersitDocument(document)) {
            setError(QVersitWriter::IOError);
            break;
        }
    }
    if (canceled)
        setState(QVersitWriter::CanceledState);
    else
        setState(QVersitWriter::FinishedState);
}

/*!
 * Inherited from QThread, called by QThread when the thread has been started.
 */
void QVersitWriterPrivate::run()
{
    write();
}

void QVersitWriterPrivate::setState(QVersitWriter::State state)
{
    mMutex.lock();
    mState = state;
    mMutex.unlock();
    emit stateChanged(state);
}

QVersitWriter::State QVersitWriterPrivate::state() const
{
    QMutexLocker locker(&mMutex);
    return mState;
}

void QVersitWriterPrivate::setError(QVersitWriter::Error error)
{
    QMutexLocker locker(&mMutex);
    mError = error;
}

QVersitWriter::Error QVersitWriterPrivate::error() const
{
    QMutexLocker locker(&mMutex);
    return mError;
}

/*!
 * Returns a QVersitDocumentWriter that can encode a QVersitDocument of type \a type.
 * The caller is responsible for deleting the object.
 */
QVersitDocumentWriter* QVersitWriterPrivate::writerForType(QVersitDocument::VersitType type, const QVersitDocument& document)
{
    switch (type) {
        case QVersitDocument::InvalidType:
        {
            // Neither startWriting or the document provided the type.
            // Need to infer the type from the document's componentType
            QString componentType(document.componentType());
            if (componentType == QStringLiteral("VCARD")) {
                return new QVCard30Writer(QVersitDocument::VCard30Type);
            } else if (componentType == QStringLiteral("VCALENDAR")
                    || componentType == QStringLiteral("VEVENT")
                    || componentType == QStringLiteral("VTODO")
                    || componentType == QStringLiteral("VJOURNAL")
                    || componentType == QStringLiteral("VTIMEZONE")
                    || componentType == QStringLiteral("VALARM")) {
                return new QVCard30Writer(QVersitDocument::ICalendar20Type);
            } else {
                return new QVCard30Writer(QVersitDocument::VCard30Type);
            }
        }
        case QVersitDocument::VCard21Type:
            return new QVCard21Writer(type);
        case QVersitDocument::VCard30Type:
            return new QVCard30Writer(type);
        default:
            return new QVCard30Writer(type);
    }
}

void QVersitWriterPrivate::setCanceling(bool canceling)
{
    QMutexLocker locker(&mMutex);
    mIsCanceling = canceling;
}

bool QVersitWriterPrivate::isCanceling() const
{
    QMutexLocker locker(&mMutex);
    return mIsCanceling;
}

void QVersitWriterPrivate::setDocumentType(QVersitDocument::VersitType type)
{
    QMutexLocker locker(&mMutex);
    mType = type;
}

QVersitDocument::VersitType QVersitWriterPrivate::documentType() const
{
    QMutexLocker locker(&mMutex);
    return mType;
}

#include "moc_qversitwriter_p.cpp"
QT_END_NAMESPACE_VERSIT
