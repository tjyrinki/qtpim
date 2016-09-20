/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Pim module.
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

#include "qcontactengineid.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactEngineId
  \relates QContactId
  \brief The QContactEngineId class uniquely identifies a contact within a
  particular engine plugin.

  \inmodule QtContacts
  \ingroup contacts-backends

  Clients of the Contacts API should never use this class.
  Every engine implementor must implement a class derived from
  QContactEngineId.

  This class is provided so that engine implementors can implement their own
  id class (which may contain arbitrary data, and which may implement the
  required functions in an arbitrary manner).
 */

/*!
  \fn QContactEngineId::~QContactEngineId()
  Cleans up any memory in use by this engine contact id.
 */

/*!
  \fn QContactEngineId::isEqualTo(const QContactEngineId* other) const
  Returns true if this id is equal to the \a other id; false otherwise.
  Note that when implementing this function, you do not have to check that the type is the same,
  since the function which calls this function (in QContactId) does that check for you.
 */

/*!
  \fn QContactEngineId::isLessThan(const QContactEngineId* other) const
  Returns true if this id is less than the \a other id; false otherwise.
  Note that when implementing this function, you do not have to check that the type is the same,
  since the function which calls this function (in QContactId) does that check for you.
 */

/*!
  \fn QContactEngineId::managerUri() const
  Returns the manager URI of the constructed manager which created
  the id.  If the contact which the id identifies has not been deleted,
  the id should still be valid in the manager identified by the
  manager URI returned by this function.
 */

/*!
  \fn QContactAbstractRequest::StorageLocation QContactEngineId::storageLocation() const

  Returns the storage location where the contact is from.
 */
QContactAbstractRequest::StorageLocation QContactEngineId::storageLocation() const
{
    return QContactAbstractRequest::UserDataStorage;
}

/*!
  \fn QContactEngineId::toString() const
  Serializes the id to a string.  It contains all of the information
  required to identify a particular contact in the manager which created
  the id, formatted according to the serialization format of the
  manager.
 */

/*!
  \fn QContactEngineId::clone() const
  Returns a deep-copy clone of this id.
  The caller takes ownership of the returned engine contact id.
 */

/*!
  \fn QContactEngineId::debugStreamOut(QDebug& dbg) const
  Streams this id out to the debug stream \a dbg.
 */

/*!
  \fn QContactEngineId::hash() const
  Returns the hash value of this id.
 */

QT_END_NAMESPACE_CONTACTS
