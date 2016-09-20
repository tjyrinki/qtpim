/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#ifndef QDECLARATIVECONTACTFETCHHINT_H
#define QDECLARATIVECONTACTFETCHHINT_H

#include <QtCore/qsize.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qvariant.h>

#include <QtQml/qqml.h>

#include <QtContacts/qcontactfetchhint.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactFetchHint : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> detailTypesHint READ detailTypesHint WRITE setDetailTypesHint NOTIFY fetchHintChanged )
    Q_PROPERTY(QStringList relationshipTypesHint READ relationshipTypesHint WRITE setRelationshipTypesHint NOTIFY fetchHintChanged )
    Q_PROPERTY(OptimizationHints optimizationHints READ optimizationHints WRITE setOptimizationHints NOTIFY fetchHintChanged )
    Q_PROPERTY(int imageWidth READ preferredImageWidth WRITE setPreferredImageWidth NOTIFY fetchHintChanged )
    Q_PROPERTY(int imageHeight READ preferredImageHeight WRITE setPreferredImageHeight NOTIFY fetchHintChanged )
    Q_FLAGS(OptimizationHints)
public:
    enum OptimizationHint {
        AllRequired = QContactFetchHint::AllRequired,
        NoRelationships = QContactFetchHint::NoRelationships,
        NoActionPreferences = QContactFetchHint::NoActionPreferences,
        NoBinaryBlobs = QContactFetchHint::NoBinaryBlobs
    };
    Q_DECLARE_FLAGS(OptimizationHints, OptimizationHint)

    QDeclarativeContactFetchHint(QObject* parent = 0);
    QList<int> detailTypesHint() const;
    void setDetailTypesHint(const QList<int> &detailTypes);

    QStringList relationshipTypesHint() const;
    void setRelationshipTypesHint(const QStringList& relationshipTypes);

    int preferredImageWidth() const;
    void setPreferredImageWidth(int w);

    int preferredImageHeight() const;
    void setPreferredImageHeight(int h);

    OptimizationHints optimizationHints() const;
    void setOptimizationHints(OptimizationHints hints);

    QContactFetchHint fetchHint() const;
    void setFetchHint(const QContactFetchHint& fetchHint);
signals:
    void fetchHintChanged();
private:
    QContactFetchHint m_fetchHint;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactFetchHint)

#endif // QDECLARATIVECONTACTFETCHHINT_H
