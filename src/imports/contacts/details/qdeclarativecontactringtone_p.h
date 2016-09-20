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

#ifndef QDECLARATIVECONTACTRINGTONE_H
#define QDECLARATIVECONTACTRINGTONE_H

#include <QtContacts/qcontactringtone.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactRingtone : public QDeclarativeContactDetail
{
    Q_OBJECT

    Q_PROPERTY(QUrl audioRingtoneUrl READ audioRingtoneUrl WRITE setAudioRingtoneUrl NOTIFY valueChanged)
    Q_PROPERTY(QUrl videoRingtoneUrl READ videoRingtoneUrl WRITE setVideoRingtoneUrl NOTIFY valueChanged)
    Q_PROPERTY(QUrl vibrationRingtoneUrl READ vibrationRingtoneUrl WRITE setVibrationRingtoneUrl NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_CLASSINFO("DefaultProperty", "audioRingtoneUrl")
public:
    enum FieldType {
        AudioRingtoneUrl = QContactRingtone::FieldAudioRingtoneUrl,
        VideoRingtoneUrl = QContactRingtone::FieldVideoRingtoneUrl,
        VibrationRingtoneUrl = QContactRingtone::FieldVibrationRingtoneUrl
    };

    QDeclarativeContactRingtone(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactRingtone());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Ringtone;
    }

    void setAudioRingtoneUrl(const QUrl& v)
    {
        if (!readOnly() && v != audioRingtoneUrl()) {
            detail().setValue(QContactRingtone::FieldAudioRingtoneUrl, v);
            emit valueChanged();
        }
    }
    QUrl audioRingtoneUrl() const {return detail().value<QUrl>(QContactRingtone::FieldAudioRingtoneUrl);}

    void setVideoRingtoneUrl(const QUrl& v)
    {
        if (!readOnly() && v != videoRingtoneUrl()) {
            detail().setValue(QContactRingtone::FieldVideoRingtoneUrl, v);
            emit valueChanged();
        }
    }
    QUrl videoRingtoneUrl() const {return detail().value<QUrl>(QContactRingtone::FieldVideoRingtoneUrl);}

    void setVibrationRingtoneUrl(const QUrl& v)
    {
        if (!readOnly() && v != vibrationRingtoneUrl()) {
            detail().setValue(QContactRingtone::FieldVibrationRingtoneUrl, v);
            emit valueChanged();
        }
    }
    QUrl vibrationRingtoneUrl() const {return detail().value<QUrl>(QContactRingtone::FieldVibrationRingtoneUrl);}
signals:
    void valueChanged();

};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactRingtone)

#endif // QDECLARATIVECONTACTRINGTONE_H
