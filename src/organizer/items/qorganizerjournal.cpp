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

#include "qorganizerjournal.h"

#include <QtCore/qdatetime.h>

#include "qorganizerjournaltime.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerJournal
    \brief The QOrganizerJournal class supplies a journal which is associated with a particular point in time
    \inmodule QtOrganizer
    \ingroup organizer-items

    A journal consists of personal data which is associated with a particular point in time.
 */

/*!
    Sets the date time which this journal is associated with to \a dateTime.
 */
void QOrganizerJournal::setDateTime(const QDateTime &dateTime)
{
    QOrganizerJournalTime jtr = detail(QOrganizerItemDetail::TypeJournalTime);
    jtr.setEntryDateTime(dateTime);
    saveDetail(&jtr);
}

/*!
    Returns the date time associated with this journal.
 */
QDateTime QOrganizerJournal::dateTime() const
{
    QOrganizerJournalTime jtr = detail(QOrganizerItemDetail::TypeJournalTime);
    return jtr.entryDateTime();
}

QT_END_NAMESPACE_ORGANIZER
