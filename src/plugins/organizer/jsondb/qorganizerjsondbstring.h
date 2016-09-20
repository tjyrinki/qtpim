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

#ifndef QORGANIZERJSONDBSTRING_H
#define QORGANIZERJSONDBSTRING_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qstring.h>

#include <QtOrganizer/qorganizerglobal.h>

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerJsonDbStr
{
public:
    // JsonDb manager name
    inline const static QString jsonDbManagerUri() { return QStringLiteral("qtorganizer:jsondb:"); }
    inline const static QString jsonDbManagerName() { return QStringLiteral("jsondb"); }

    inline const static QString jsonDbSchemaPrefix() { return QStringLiteral("com.nokia.mt.organizer."); }

    // JsonDb object property names
    inline const static QString jsonDbUuid() { return QStringLiteral("_uuid"); }
    inline const static QString jsonDbType() { return QStringLiteral("_type"); }
    inline const static QString jsonDbData() { return QStringLiteral("data"); }
    inline const static QString jsonDbCount() { return QStringLiteral("count"); }
    inline const static QString jsonDbError() { return QStringLiteral("error"); }
    inline const static QString jsonDbCode() { return QStringLiteral("code"); }
    inline const static QString jsonDbVersion() {return QStringLiteral("_version"); }
    inline const static QString jsonDbValue() { return QStringLiteral("value"); }

    // JsonDb query strings
    inline const static QString jsonDbQueryAllItems() { return QStringLiteral("[?_type in [\"com.nokia.mt.organizer.Event\", \"com.nokia.mt.organizer.Todo\", \"com.nokia.mt.organizer.EventOccurrence\", \"com.nokia.mt.organizer.TodoOccurrence\"]]"); }
    inline const static QString jsonDbQueryAllEventItems() { return QStringLiteral("[?_type in [\"com.nokia.mt.organizer.Event\", \"com.nokia.mt.organizer.EventOccurrence\"]]"); }
    inline const static QString jsonDbQueryEventTypeItems() { return QStringLiteral("[?_type=\"com.nokia.mt.organizer.Event\"]"); }
    inline const static QString jsonDbQueryEventOccurenceTypeItems() { return QStringLiteral("[?_type=\"com.nokia.mt.organizer.EventOccurrence\"]"); }
    inline const static QString jsonDbQueryAllTodoItems() { return QStringLiteral("[?_type in [\"com.nokia.mt.organizer.Todo\", \"com.nokia.mt.organizer.TodoOccurrence\"]]"); }
    inline const static QString jsonDbQueryTodoTypeItems() { return QStringLiteral("[?_type=\"com.nokia.mt.organizer.Todo\"]"); }
    inline const static QString jsonDbQueryTodoOccurenceTypeItems() { return QStringLiteral("[?_type=\"com.nokia.mt.organizer.TodoOccurrence\"]"); }

    inline const static QString jsonDbQueryAllCollections() { return QStringLiteral("[?_type=\"com.nokia.mt.organizer.Collection\"]"); }
    inline const static QString jsonDbQueryParentItems() { return QStringLiteral("[?_type in [\"com.nokia.mt.organizer.Event\", \"com.nokia.mt.organizer.Todo\"]][?recurrenceDates exists | recurrenceRules exists | exceptionDates exists | exceptionRules exists]"); }
    inline const static QString jsonDbQueryOccurrenceItems() { return QStringLiteral("[?_type in [\"com.nokia.mt.organizer.EventOccurrence\", \"com.nokia.mt.organizer.TodoOccurrence\"]]"); }
    inline const static QString jsonDbQueryOccurrenceItemsByParent() { return QStringLiteral("[?_type in [\"com.nokia.mt.organizer.EventOccurrence\", \"com.nokia.mt.organizer.TodoOccurrence\"]][?parentUuid = \"%1\"]"); }

    inline const static QString jsonDbQueryEventViews() { return QStringLiteral("[?_type=\"com.nokia.mt.organizer.EventView\"]"); }
    inline const static QString jsonDbQueryEventViewParentItems() { return QStringLiteral("[?_type=\"com.nokia.mt.organizer.EventView\"][?value.recurrenceRules exists]"); }

    inline const static QString jsonDbQueryCollectionUuidsTemplate() { return QStringLiteral("[?collectionUuid in [%1]]"); }
    inline const static QString jsonDbQueryUuidsTemplate() { return QStringLiteral("[?_uuid in [%1]]"); }

    inline const static QString jsonDbNotificationQuery() { return QStringLiteral("[?_type in [\"com.nokia.mt.organizer.Event\", \"com.nokia.mt.organizer.EventView\", \"com.nokia.mt.organizer.EventOccurrence\", \"com.nokia.mt.organizer.Todo\", \"com.nokia.mt.organizer.TodoOccurrence\", \"com.nokia.mt.organizer.Collection\"]]"); }
    inline const static QString jsonDbQueryEventStartDateTimeTemplate() { return QStringLiteral("[?startDateTime<=\"%1\"]"); }
    inline const static QString jsonDbQueryEventEndDateTimeTemplate() { return QStringLiteral("[?endDateTime>=\"%1\"]"); }

    // collections
    inline const static QString jsonDbCollectionType() { return QStringLiteral("com.nokia.mt.organizer.Collection"); }

    inline const static QString collectionDefaultFlag() { return QStringLiteral("isDefault"); }
    inline const static QString collectionDisplayName() { return QStringLiteral("displayName"); }
    inline const static QString collectionDescription() { return QStringLiteral("description"); }
    inline const static QString collectionColor() { return QStringLiteral("color"); }
    inline const static QString collectionImageUrl() { return QStringLiteral("imageUrl"); }

    inline const static QString defaultCollectionDisplayName() { return QStringLiteral("defaultCollection"); }

    // items
    inline const static QString itemCollectionUuid() { return QStringLiteral("collectionUuid"); }
    inline const static QString itemComments() { return QStringLiteral("comments"); }
    inline const static QString itemDescription() { return QStringLiteral("description"); }
    inline const static QString itemDisplayName() { return QStringLiteral("displayName"); }
    inline const static QString itemGuid() { return QStringLiteral("guid"); }
    inline const static QString itemTags() { return QStringLiteral("tags"); }
    inline const static QString itemPriority() { return QStringLiteral("priority"); }

    inline const static QString itemRecurrenceDates() { return QStringLiteral("recurrenceDates"); }
    inline const static QString itemRecurrenceRules() { return QStringLiteral("recurrenceRules"); }
    inline const static QString itemExceptionDates() { return QStringLiteral("exceptionDates"); }
    inline const static QString itemExceptionRules() { return QStringLiteral("exceptionRules"); }

    // item reminder
    inline const static QString itemReminder() { return QStringLiteral("reminder"); }
    inline const static QString itemReminderSecBeforeStart() { return QStringLiteral("secondsBeforeStart"); }
    inline const static QString itemReminderRepetitionCount() { return QStringLiteral("repetitionCount"); }
    inline const static QString itemReminderRepetitionDelay() { return QStringLiteral("repetitionDelay"); }
    inline const static QString itemReminderDataUrl() { return QStringLiteral("dataUrl"); }

    // item recurrence rules
    inline const static QString itemRecurrenceRuleFirstDayOfWeek() { return QStringLiteral("firstDayOfWeek"); }
    inline const static QString itemRecurrenceRuleMonthsOfYear() { return QStringLiteral("monthsOfYear"); }
    inline const static QString itemRecurrenceRuleFrequency() { return QStringLiteral("frequency"); }
    inline const static QString itemRecurrenceRulePositions() { return QStringLiteral("positions"); }
    inline const static QString itemRecurrenceRuleDaysOfMonth() { return QStringLiteral("daysOfMonth"); }
    inline const static QString itemRecurrenceRuleDaysOfYear() { return QStringLiteral("daysOfYear"); }
    inline const static QString itemRecurrenceRuleDaysOfWeek() { return QStringLiteral("daysOfWeek"); }
    inline const static QString itemRecurrenceRuleWeeksOfYear() { return QStringLiteral("weeksOfYear"); }
    inline const static QString itemRecurrenceRuleInterval() { return QStringLiteral("interval"); }
    inline const static QString itemRecurrenceRuleLimitCount() { return QStringLiteral("limitCount"); }
    inline const static QString itemRecurrenceRuleLimitDate() { return QStringLiteral("limitDate"); }

    // occurrence item parent
    inline const static QString itemOccurrenceParent() { return QStringLiteral("parentUuid"); }
    inline const static QString itemOccurrenceOriginalDate() { return QStringLiteral("originalDate"); }

    // events
    inline const static QString jsonDbEventType() { return QStringLiteral("com.nokia.mt.organizer.Event"); }
    inline const static QString jsonDbEventOccurrenceType() { return QStringLiteral("com.nokia.mt.organizer.EventOccurrence"); }

    inline const static QString eventStartDateTime() { return QStringLiteral("startDateTime"); }
    inline const static QString eventEndDateTime() { return QStringLiteral("endDateTime"); }
    inline const static QString eventIsAllDay() { return QStringLiteral("isAllDay"); }

    // event views
    inline const static QString jsonDbEventViewType() { return QStringLiteral("com.nokia.mt.organizer.EventView"); }
    inline const static QString eventIsSynthetic() { return QStringLiteral("isSynthetic"); }

    // event location
    inline const static QString eventLocation() { return QStringLiteral("location"); }

    inline const static QString eventLocationDisplayName() { return QStringLiteral("displayName"); }
    inline const static QString eventLocationGeo() { return QStringLiteral("geo"); }
    inline const static QString eventLocationGeoLatitude() { return QStringLiteral("latitude"); }
    inline const static QString eventLocationGeoLongitude() { return QStringLiteral("longitude"); }

    // event attendees
    inline const static QString eventAttendees() { return QStringLiteral("attendees"); }
    inline const static QString eventAttendeeName() { return QStringLiteral("name"); }
    inline const static QString eventAttendeeParticipationRole() { return QStringLiteral("participationRole"); }
    inline const static QString eventAttendeeParticipationStatus() { return QStringLiteral("participationStatus"); }
    inline const static QString eventAttendeeEmailAddress() { return QStringLiteral("emailAddress"); }
    inline const static QString eventAttendeeUuid() { return QStringLiteral("attendeeUuid"); }

    // event rsvp
    inline const static QString eventRsvp() { return QStringLiteral("rsvp"); }
    inline const static QString eventRsvpParticipationStatus() { return QStringLiteral("participationStatus"); }
    inline const static QString eventRsvpParticipationRole() { return QStringLiteral("participationRole"); }
    inline const static QString eventRsvpResponseRequirement() { return QStringLiteral("responseRequirement"); }
    inline const static QString eventRsvpResponseDeadline() { return QStringLiteral("responseDeadline"); }
    inline const static QString eventRsvpResponseDate() { return QStringLiteral("responseDate"); }
    inline const static QString eventRsvpOrganizerName() { return QStringLiteral("organizerName"); }
    inline const static QString eventRsvpOrganizerEmail() { return QStringLiteral("organizerEmail"); }

    // TODOs
    inline const static QString jsonDbTodoType() { return QStringLiteral("com.nokia.mt.organizer.Todo"); }
    inline const static QString jsonDbTodoOccurrenceType() { return QStringLiteral("com.nokia.mt.organizer.TodoOccurrence"); }

    inline const static QString todoStartDateTime() { return QStringLiteral("startDateTime"); }
    inline const static QString todoDueDateTime() { return QStringLiteral("dueDateTime"); }
    inline const static QString todoIsAllDay() { return QStringLiteral("isAllDay"); }
    inline const static QString todoFinishedDateTime() { return QStringLiteral("finishedDateTime"); }
    inline const static QString todoProgressPercentage() { return QStringLiteral("progressPercentage"); }
    inline const static QString todoStatus() { return QStringLiteral("status"); }

    //warning string
    inline const static QString warningCollectionRemove() { return QStringLiteral("Number of items deleted from jsondb is not equal to request!!"); }
    inline const static QString warningDefaultCollectionRemove() { return QStringLiteral("Default collection can not be removed!"); }
};

QT_END_NAMESPACE_ORGANIZER

#endif // QORGANIZERJSONDBSTRING_H
