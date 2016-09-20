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

#ifndef QDECLARATIVECONTACTGEOLOCATION_H
#define QDECLARATIVECONTACTGEOLOCATION_H

#include <QtContacts/qcontactgeolocation.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactGeoLocation : public QDeclarativeContactDetail
{
    Q_OBJECT

    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY valueChanged)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY valueChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY valueChanged)
    Q_PROPERTY(double accuracy READ accuracy WRITE setAccuracy NOTIFY valueChanged)
    Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY valueChanged)
    Q_PROPERTY(double altitudeAccuracy READ altitudeAccuracy WRITE setAltitudeAccuracy NOTIFY valueChanged)
    Q_PROPERTY(double heading READ heading WRITE setHeading NOTIFY valueChanged)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY valueChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp WRITE setTimestamp NOTIFY valueChanged)
    Q_CLASSINFO("DefaultProperty", "label")
    Q_ENUMS(FieldType)
public:
    enum FieldType {
        Label = QContactGeoLocation::FieldLabel,
        Latitude = QContactGeoLocation::FieldLatitude,
        Longitude = QContactGeoLocation::FieldLongitude,
        Accuracy = QContactGeoLocation::FieldAccuracy,
        Altitude = QContactGeoLocation::FieldAltitude,
        AltitudeAccuracy = QContactGeoLocation::FieldAltitudeAccuracy,
        Heading = QContactGeoLocation::FieldHeading,
        Speed = QContactGeoLocation::FieldSpeed,
        Timestamp = QContactGeoLocation::FieldTimestamp
    };

    QDeclarativeContactGeoLocation(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactGeoLocation());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Geolocation;
    }

    void setLabel(const QString& v)
    {
        if (!readOnly() && v != label()) {
            detail().setValue(QContactGeoLocation::FieldLabel, v);
            emit valueChanged();
        }
    }
    QString label() const {return detail().value(QContactGeoLocation::FieldLabel).toString();}
    void setLatitude(double v)
    {
        if (!readOnly() && !qFuzzyCompare(v, latitude())) {
            detail().setValue(QContactGeoLocation::FieldLatitude, v);
            emit valueChanged();
        }
    }
    double latitude() const {return detail().value(QContactGeoLocation::FieldLatitude).toDouble();}
    void setLongitude(double v)
    {
        if (!readOnly() && !qFuzzyCompare(v, longitude())) {
            detail().setValue(QContactGeoLocation::FieldLongitude, v);
            emit valueChanged();
        }
    }
    double longitude() const {return detail().value(QContactGeoLocation::FieldLongitude).toDouble();}
    void setAccuracy(double v)
    {
        if (!readOnly() && !qFuzzyCompare(v, accuracy())) {
            detail().setValue(QContactGeoLocation::FieldAccuracy, v);
            emit valueChanged();
        }
    }
    double accuracy() const {return detail().value(QContactGeoLocation::FieldAccuracy).toDouble();}
    void setAltitude(double v)
    {
        if (!readOnly() && !qFuzzyCompare(v, altitude())) {
            detail().setValue(QContactGeoLocation::FieldAltitude, v);
            emit valueChanged();
        }
    }
    double altitude() const {return detail().value(QContactGeoLocation::FieldAltitude).toDouble();}
    void setAltitudeAccuracy(double v)
    {
        if (!readOnly() && !qFuzzyCompare(v, altitudeAccuracy())) {
            detail().setValue(QContactGeoLocation::FieldAltitudeAccuracy, v);
            emit valueChanged();
        }
    }
    double altitudeAccuracy() const {return detail().value(QContactGeoLocation::FieldAltitudeAccuracy).toDouble();}
    void setHeading(double v)
    {
        if (!readOnly() && v != heading()) {
            detail().setValue(QContactGeoLocation::FieldHeading, v);
            emit valueChanged();
        }
    }
    double heading() const {return detail().value(QContactGeoLocation::FieldHeading).toDouble();}
    void setSpeed(double v)
    {
        if (!readOnly() && !qFuzzyCompare(v, speed())) {
            detail().setValue(QContactGeoLocation::FieldSpeed, v);
            emit valueChanged();
        }
    }
    double speed() const {return detail().value(QContactGeoLocation::FieldSpeed).toDouble();}
    void setTimestamp(const QDateTime& v)
    {
        if (!readOnly() && v != timestamp()) {
            detail().setValue(QContactGeoLocation::FieldTimestamp, v);
            emit valueChanged();
        }
    }
    QDateTime timestamp() const {return detail().value(QContactGeoLocation::FieldTimestamp).toDateTime();}
signals:
    void valueChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactGeoLocation)

#endif // QDECLARATIVECONTACTGEOLOCATION_H
