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

#ifndef QCONTACTFETCHHINT_H
#define QCONTACTFETCHHINT_H

#include <QtCore/qshareddata.h>
#include <QtCore/qsize.h>
#include <QtCore/qstringlist.h>

#include <QtContacts/qcontactdetail.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactFetchHintPrivate;
class Q_CONTACTS_EXPORT QContactFetchHint {
public:
    QContactFetchHint();
    QContactFetchHint(const QContactFetchHint& other);
    ~QContactFetchHint();
    QContactFetchHint& operator=(const QContactFetchHint& other);

    QList<QContactDetail::DetailType> detailTypesHint() const;
    void setDetailTypesHint(const QList<QContactDetail::DetailType> &types);

    QStringList relationshipTypesHint() const;
    void setRelationshipTypesHint(const QStringList& relationshipTypes);

    QSize preferredImageSize() const;
    void setPreferredImageSize(const QSize& size);

    enum OptimizationHint {
        AllRequired = 0x0,
        NoRelationships = 0x1,
        NoActionPreferences = 0x2,
        NoBinaryBlobs = 0x4
        // any other optimization hints?
    };
    Q_DECLARE_FLAGS(OptimizationHints, OptimizationHint)

    OptimizationHints optimizationHints() const;
    void setOptimizationHints(OptimizationHints hints);

    int maxCountHint() const;
    void setMaxCountHint(int count);

private:
    QSharedDataPointer<QContactFetchHintPrivate> d;
};

#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream& operator<<(QDataStream& out, const QContactFetchHint& hint);
Q_CONTACTS_EXPORT QDataStream& operator>>(QDataStream& in, QContactFetchHint& hint);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContactFetchHint& hint);
#endif

Q_DECLARE_OPERATORS_FOR_FLAGS(QContactFetchHint::OptimizationHints)

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTFETCHHINT_H
