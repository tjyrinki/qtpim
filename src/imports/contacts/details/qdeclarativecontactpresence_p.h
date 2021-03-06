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

#ifndef QDECLARATIVECONTACTPRESENCE_H
#define QDECLARATIVECONTACTPRESENCE_H

#include <QtContacts/qcontactpresence.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactPresence : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QDateTime timestamp READ timestamp WRITE setTimestamp NOTIFY valueChanged)
    Q_PROPERTY(QString nickname READ nickname WRITE setNickname NOTIFY valueChanged)
    Q_PROPERTY(PresenceStateType state READ presenceState WRITE setPresenceState NOTIFY valueChanged)
    Q_PROPERTY(QString stateText READ presenceStateText WRITE setPresenceStateText NOTIFY valueChanged)
    Q_PROPERTY(QUrl imageUrl READ presenceStateImageUrl WRITE setPresenceStateImageUrl NOTIFY valueChanged)
    Q_PROPERTY(QString customMessage READ customMessage WRITE setCustomMessage NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_ENUMS(PresenceStateType)
    Q_CLASSINFO("DefaultProperty", "state")
public:
    enum FieldType {
        Timestamp = QContactPresence::FieldTimestamp,
        Nickname = QContactPresence::FieldNickname,
        State = QContactPresence::FieldPresenceState,
        StateText = QContactPresence::FieldPresenceStateText,
        ImageUrl = QContactPresence::FieldPresenceStateImageUrl,
        CustomMessage = QContactPresence::FieldCustomMessage
    };

    enum PresenceStateType {
        Unknown = QContactPresence::PresenceUnknown,
        Available = QContactPresence::PresenceAvailable,
        Hidden = QContactPresence::PresenceHidden,
        Busy = QContactPresence::PresenceBusy,
        Away = QContactPresence::PresenceAway,
        ExtendedAway = QContactPresence::PresenceExtendedAway,
        Offline = QContactPresence::PresenceOffline
    };

    QDeclarativeContactPresence(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactPresence());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Presence;
    }

    void setTimestamp(const QDateTime& v)
    {
        if (!readOnly() && v != timestamp()) {
            detail().setValue(QContactPresence::FieldTimestamp, v);
            emit valueChanged();
        }
    }
    QDateTime timestamp() const {return detail().value<QDateTime>(QContactPresence::FieldTimestamp);}
    void setNickname(const QString& v)
    {
        if (!readOnly() && v != nickname()) {
            detail().setValue(QContactPresence::FieldNickname, v);
            emit valueChanged();
        }
    }
    QString nickname() const {return detail().value(QContactPresence::FieldNickname).toString();}

    void setPresenceState(PresenceStateType v)
    {
        if (!readOnly() && v != presenceState()) {
            detail().setValue(QContactPresence::FieldPresenceState, static_cast<int>(v));
            emit valueChanged();
        }
    }

    PresenceStateType presenceState() const
    {
        return static_cast<PresenceStateType>(detail().value<int>(QContactPresence::FieldPresenceState));

    }

    void setPresenceStateText(const QString& v)
    {
        if (!readOnly() && v != presenceStateText()) {
            detail().setValue(QContactPresence::FieldPresenceStateText, v);
            emit valueChanged();
        }
    }
    QString presenceStateText() const {return detail().value(QContactPresence::FieldPresenceStateText).toString();}
    void setPresenceStateImageUrl(const QUrl& v)
    {
        if (!readOnly() && v != presenceStateImageUrl())  {
            detail().setValue(QContactPresence::FieldPresenceStateImageUrl, v);
            emit valueChanged();
        }
    }
    QUrl presenceStateImageUrl() const {return detail().value<QUrl>(QContactPresence::FieldPresenceStateImageUrl);}
    void setCustomMessage(const QString& v)
    {
        if (!readOnly() && v != customMessage()) {
            detail().setValue(QContactPresence::FieldCustomMessage, v);
            emit valueChanged();
        }
    }
    QString customMessage() const {return detail().value(QContactPresence::FieldCustomMessage).toString();}
signals:
    void valueChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactPresence)

#endif // QDECLARATIVECONTACTPRESENCE_H
