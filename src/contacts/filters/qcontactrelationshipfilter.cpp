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

#include "qcontactrelationshipfilter.h"
#include "qcontactrelationshipfilter_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactRelationshipFilter
  \brief The QContactRelationshipFilter class provides a filter based
  around relationship criteria.


  \inmodule QtContacts

  \ingroup contacts-filters

  It may be used to select contacts which are involved in relationships
  which are of a certain type, or which involve certain contacts.

  One common use-case might be to select the contacts which are a member of a particular group.
  This use-case may be met with the following filter:

  \code
  QContactRelationshipFilter groupFilter;                               // select all contacts which are involved
  groupFilter.setRelationshipType(QContactRelationship::HasMember);     // in a group relationship
  groupFilter.setRelatedContact(groupContact());                   // with the group contact
  groupFilter.setRelatedContactRole(QContactRelationship::First); // where the group contact is the first participant
  \endcode

  Another common use-case might be to select the groups which a particular contact is a member of.
  This use-case may be met with the following filter:

  \code
  QContactRelationshipFilter whichGroupsFilter;                                 // select all contacts which are involved
  whichGroupsFilter.setRelationshipType(QContactRelationship::HasMember); // in a group relationship
  whichGroupsFilter.setRelatedContact(particularContact());                // with the particular contact
  whichGroupsFilter.setRelatedContactRole(QContactRelationship::Second);  // where the particular contact is the second participant
  \endcode

 */

Q_IMPLEMENT_CONTACTFILTER_PRIVATE(QContactRelationshipFilter)

/*!
  \fn QContactRelationshipFilter::QContactRelationshipFilter(const QContactFilter& other)
  Constructs a copy of \a other if possible, else constructs a new QContactRelationshipFilter.
 */

/*!
  Constructs a new relationship filter
 */
QContactRelationshipFilter::QContactRelationshipFilter()
    : QContactFilter(new QContactRelationshipFilterPrivate)
{
}

/*!
  Sets the type of relationship which a contact must have in order to match this filter to \a relationshipType
 */
void QContactRelationshipFilter::setRelationshipType(const QString& relationshipType)
{
    Q_D(QContactRelationshipFilter);
    d->m_relationshipType = relationshipType;
}

/*!
  Returns the type of relationship that a contact must have in order to match the filter
 */
QString QContactRelationshipFilter::relationshipType() const
{
    Q_D(const QContactRelationshipFilter);
    return d->m_relationshipType;
}

/*!
  Sets the contact with whom the tested contact must have a relationship in order for the tested contact to match this filter to be \a relatedContact
 */
void QContactRelationshipFilter::setRelatedContact(const QContact &relatedContact)
{
    Q_D(QContactRelationshipFilter);
    d->m_relatedContact = relatedContact;
}

/*!
  Returns the contact with whom the tested contact must have a relationship in order for the tested contact to match this filter
 */
QContact QContactRelationshipFilter::relatedContact() const
{
    Q_D(const QContactRelationshipFilter);
    return d->m_relatedContact;
}

/*!
  Sets the role in the relationship with the tested contact that the related contact must play in order for the tested contact to match this filter to be \a relatedContactRole
 */
void QContactRelationshipFilter::setRelatedContactRole(QContactRelationship::Role relatedContactRole)
{
    Q_D(QContactRelationshipFilter);
    d->m_relatedContactRole = relatedContactRole;
}

/*!
  Returns the role in the relationship with the tested contact that the related contact must play in order for the tested contact to match this filter
 */
QContactRelationship::Role QContactRelationshipFilter::relatedContactRole() const
{
    Q_D(const QContactRelationshipFilter);
    return d->m_relatedContactRole;
}


QT_END_NAMESPACE_CONTACTS
