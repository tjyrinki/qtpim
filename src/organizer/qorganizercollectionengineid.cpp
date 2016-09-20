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

#include "qorganizercollectionengineid.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerCollectionEngineId
    \brief The QOrganizerCollectionEngineId class uniquely identifies an item within a
           particular engine plugin.
    \inmodule QtOrganizer
    \ingroup organizer-backends

    Clients of the Organizer API should never use this class, while every engine implementor must
    implement a class derived from QOrganizerCollectionEngineId.

    This class is provided so that engine implementors can implement their own collection ID class
    (which may contain arbitrary data, and which may implement the required functions in an arbitrary
    manner).
 */

/*!
    Cleans up any memory in use by this engine collection ID.
 */
QOrganizerCollectionEngineId::~QOrganizerCollectionEngineId()
{
}

/*!
    \fn QOrganizerCollectionEngineId::isEqualTo(const QOrganizerCollectionEngineId *other) const

    Returns true if this ID is equal to the \a other; false otherwise.

    Note that when implementing this function, you do not have to check that the type is the same,
    since the function which calls this function (in QOrganizerCollectionId) does that check for you.
 */

/*!
    \fn QOrganizerCollectionEngineId::isLessThan(const QOrganizerCollectionEngineId *other) const

    Returns true if this id is less than the \a other; false otherwise.

    Note that when implementing this function, you do not have to check that the type is the same,
    since the function which calls this function (in QOrganizerCollectionId) does that check for you.
 */

/*!
    \fn QOrganizerCollectionEngineId::managerUri() const

    Returns the manager URI of the constructed manager which created the ID. If the collection which
    the ID identifies has not been deleted, the ID should still be valid in the manager identified by
    the manager URI returned by this function.
 */

/*!
    \fn QOrganizerCollectionEngineId::toString() const

    Serializes the id to a string. It contains all of the information required to identify a particular
    collection in the manager which created the ID, formatted according to the serialization format of
    the manager.
 */

/*!
    \fn QOrganizerCollectionEngineId::clone() const

    Returns a deep-copy clone of this ID. The caller takes ownership of the returned engine collection
    ID.
 */

/*!
    \fn QOrganizerCollectionEngineId::debugStreamOut(QDebug &dbg) const

    Streams this ID out to the debug stream \a dbg.
 */

/*!
    \fn QOrganizerCollectionEngineId::hash() const

    Returns the hash value of this ID.
 */

QT_END_NAMESPACE_ORGANIZER
