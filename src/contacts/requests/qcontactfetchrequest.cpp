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

#include "qcontactfetchrequest.h"

#include "qcontactrequests_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactFetchRequest
  \brief The QContactFetchRequest class allows a client to asynchronously
    request contacts from a contacts store manager.


  For a QContactFetchRequest, the resultsAvailable() signal will be emitted when the resultant
  contacts (which may be retrieved by calling contacts()), are updated, as well as if
  the overall operation error (which may be retrieved by calling error()) is updated.

  Please see the class documentation of QContactAbstractRequest for more information about
  the usage of request classes and ownership semantics.


  \inmodule QtContacts

  \ingroup contacts-requests
 */

/*! Constructs a new contact fetch request whose parent is the specified \a parent */
QContactFetchRequest::QContactFetchRequest(QObject* parent)
    : QContactAbstractRequest(new QContactFetchRequestPrivate, parent)
{
}

/*! Frees any memory used by this request */
QContactFetchRequest::~QContactFetchRequest()
{
}

/*!
  Sets the storage location from where the contacts will be retrieved.

  \a storageLocations is a flag so it is possible to define multiple locations in it.
  However, some backend implementations may accept only one flag to be set. In case multiple
  flags are set fetching is done from the default storage location.
  \sa QContactAbstractRequest::StorageLocation
  \sa QContactAbstractRequest::StorageLocations
 */
void QContactFetchRequest::setStorageLocations(QContactAbstractRequest::StorageLocations storageLocations)
{
    Q_D(QContactFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_storageLocations = storageLocations;
}

/*!
  Returns the storage locations from where the contacts will be retrieved.
  \sa QContactAbstractRequest::StorageLocation
  \sa QContactAbstractRequest::StorageLocations

 */
QContactAbstractRequest::StorageLocations QContactFetchRequest::storageLocations() const
{
    Q_D(const QContactFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_storageLocations;
}

/*!
  Sets the contact filter used to determine which contacts will be retrieved to \a filter.
*/
void QContactFetchRequest::setFilter(const QContactFilter& filter)
{
    Q_D(QContactFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_filter = filter;
}

/*! Sets the sort order of the result to \a sorting.  Only has an effect if called prior to calling \c start()
*/
void QContactFetchRequest::setSorting(const QList<QContactSortOrder>& sorting)
{
    Q_D(QContactFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_sorting = sorting;
}

/*!
  Sets the fetch hint which may be used by the backend to optimize contact retrieval
  to \a fetchHint.  A client should not make changes to a contact which has been retrieved
  using a fetch hint other than the default fetch hint.  Doing so will result in information
  loss when saving the contact back to the manager (as the "new" restricted contact will
  replace the previously saved contact in the backend).
  \sa QContactFetchHint
 */
void QContactFetchRequest::setFetchHint(const QContactFetchHint &fetchHint)
{
    Q_D(QContactFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_fetchHint = fetchHint;
}

/*! Returns the filter that will be used to select contacts to be returned
*/
QContactFilter QContactFetchRequest::filter() const
{
    Q_D(const QContactFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_filter;
}

/*! Returns the sort ordering that will be used sort the results of this request
*/
QList<QContactSortOrder> QContactFetchRequest::sorting() const
{
    Q_D(const QContactFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_sorting;
}

/*!
  Returns the fetch hint which may be used by the backend to optimize contact retrieval.
  A client should not make changes to a contact which has been retrieved
  using a fetch hint other than the default fetch hint.  Doing so will result in information
  loss when saving the contact back to the manager (as the "new" restricted contact will
  replace the previously saved contact in the backend).
  \sa QContactFetchHint
 */
QContactFetchHint QContactFetchRequest::fetchHint() const
{
    Q_D(const QContactFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_fetchHint;
}

/*! Returns the list of contacts retrieved by this request
*/
QList<QContact> QContactFetchRequest::contacts() const
{
    Q_D(const QContactFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_contacts;
}

#include "moc_qcontactfetchrequest.cpp"

QT_END_NAMESPACE_CONTACTS
