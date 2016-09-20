/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include "qorganizercollectionid.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif
#include <QtCore/qstringlist.h>

#include "qorganizercollectionengineid.h"
#include "qorganizermanager_p.h"
#include "qorganizeritemid_p.h"

#if !defined(Q_CC_MWERKS)
QT_BEGIN_NAMESPACE
template<> QTORGANIZER_PREPEND_NAMESPACE(QOrganizerCollectionEngineId) *QSharedDataPointer<QTORGANIZER_PREPEND_NAMESPACE(QOrganizerCollectionEngineId)>::clone()
{
    return d ? d->clone() : 0;
}
QT_END_NAMESPACE
#endif // Q_CC_MWERKS

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerCollectionId
    \brief The QOrganizerCollectionId class provides information that uniquely identifies a collection
           in a particular manager.
    \inmodule QtOrganizer
    \ingroup organizer-main

    It consists of a manager URI which identifies the manager which manages the collection, and the
    ID of the collection in that manager.
 */

/*!
    Constructs a new, null collection ID.
 */
QOrganizerCollectionId::QOrganizerCollectionId()
    : d(0)
{
}

/*!
    Cleans up the memory in use by the collection ID.
 */
QOrganizerCollectionId::~QOrganizerCollectionId()
{
}

/*!
    Constructs a manager-unique ID which wraps the given engine-unique \a engineId.

    This ID takes ownership of the \a engineId and will delete it when this ID is destructed. Engine
    implementors must not delete the \a engineId, otherwise undefined behaviour will occur.
 */
QOrganizerCollectionId::QOrganizerCollectionId(QOrganizerCollectionEngineId *engineId)
    : d(engineId)
{
}

/*!
    Constructs a new collection ID as a copy of \a other.
 */
QOrganizerCollectionId::QOrganizerCollectionId(const QOrganizerCollectionId &other)
    : d(other.d)
{
}

/*!
    Assigns the collection ID to be equal to \a other.
 */
QOrganizerCollectionId &QOrganizerCollectionId::operator=(const QOrganizerCollectionId &other)
{
    d = other.d;
    return *this;
}

/*!
    Returns true if it has the same manager URI and ID as \a other.
*/
bool QOrganizerCollectionId::operator==(const QOrganizerCollectionId &other) const
{
    if (d == other.d)
        return true;

    if (d && other.d)
        return d->managerUri() == other.d->managerUri() && d->isEqualTo(other.d);

    return false;
}

/*!
    Returns true if either the manager URI or ID of it is different to that of \a other.
 */
bool QOrganizerCollectionId::operator!=(const QOrganizerCollectionId &other) const
{
    return !(*this == other);
}

/*!
    Returns true if this ID is less than the \a other.

    This ID will be considered less than the \a other if the manager URI of this ID is alphabetically
    less than the manager URI of the \a other. If both IDs have the same manager URI, this ID will
    be considered less than the \a other if the engine ID is less than the engine ID of the \a other.

    The invalid, empty ID consists of an empty manager URI and a null engine ID, and hence will be
    less than any non-invalid ID.

    This operator is provided primarily to allow use of a QOrganizerCollectionId as a key in a QMap.
 */
bool QOrganizerCollectionId::operator<(const QOrganizerCollectionId &other) const
{
    // a null id is always less than a non-null id.
    if (d == 0 && other.d != 0)
        return true;

    if (d && other.d) {
        // ensure they're of the same type (and therefore comparable)
        if (d->managerUri() == other.d->managerUri())
            return d->isLessThan(other.d);

        // not the same type?  just compare the manager uri.
        return d->managerUri() < other.d->managerUri();
    }

    return false;
}

/*!
    Returns true if the engine ID part of this ID is null (default constructed); otherwise, returns false.
 */
bool QOrganizerCollectionId::isNull() const
{
    return (d == 0);
}

/*!
    \relates QOrganizerCollectionId
    Returns the hash value for \a key.
 */
uint qHash(const QOrganizerCollectionId &key)
{
    if (key.d)
        return key.d->hash();
    return 0;
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates QOrganizerCollectionId
    Outputs \a id to the debug stream \a dbg.
 */
QDebug operator<<(QDebug dbg, const QOrganizerCollectionId &id)
{
    dbg.nospace() << "QOrganizerCollectionId(";
    if (id.isNull())
        dbg.nospace() << "(null))";
    else
        id.d->debugStreamOut(dbg)  << ")";
    return dbg.maybeSpace();
}
#endif // QT_NO_DEBUG_STREAM

#ifndef QT_NO_DATASTREAM
/*!
    \relates QOrganizerCollectionId
    Streams \a collectionId to the data stream \a out.
 */
QDataStream &operator<<(QDataStream &out, const QOrganizerCollectionId &collectionId)
{
    out << (collectionId.toString());
    return out;
}

/*!
    \relates QOrganizerCollectionId
    Streams \a collectionId in from the data stream \a in.
 */
QDataStream &operator>>(QDataStream &in, QOrganizerCollectionId &collectionId)
{
    QString idString;
    in >> idString;
    collectionId = QOrganizerCollectionId::fromString(idString);
    return in;
}
#endif // QT_NO_DATASTREAM

/*!
    Returns the URI of the manager which contains the collection identified by this ID.
 */
QString QOrganizerCollectionId::managerUri() const
{
    return d ? d->managerUri() : QString::null;
}

/*!
    Serializes the ID to a string. The format of the string will be:
    "qtorganizer:managerName:constructionParams:serializedEngineLocalItemId".
 */
QString QOrganizerCollectionId::toString() const
{
    QString mgrName;
    QMap<QString, QString> params;
    QString engineId;

    if (d) {
        QOrganizerManager::parseUri(d->managerUri(), &mgrName, &params);
        engineId = d->toString();
    }

    // having extracted the params the name, we now need to build a new string.
    return buildIdString(mgrName, params, engineId);
}

/*!
    Deserializes the given \a idString.  Returns a default-constructed (null) collection ID if the
    given \a idString is not a valid, serialized collection ID, or if the manager engine from which
    the ID refers to could not be found.
 */
QOrganizerCollectionId QOrganizerCollectionId::fromString(const QString &idString)
{
    QString managerName;
    QMap<QString, QString> params;
    QString engineIdString;

    if (!parseIdString(idString, &managerName, &params, &engineIdString))
        return QOrganizerCollectionId(); // invalid idString given.

    QOrganizerCollectionEngineId *engineId = QOrganizerManagerData::createEngineCollectionId(managerName, params, engineIdString);
    return QOrganizerCollectionId(engineId);
}

QT_END_NAMESPACE_ORGANIZER
