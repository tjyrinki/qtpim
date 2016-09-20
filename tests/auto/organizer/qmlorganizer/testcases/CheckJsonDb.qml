/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtPim module of the Qt Toolkit.
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

//TESTED_COMPONENT=src/organizer

import QtQuick 2.0
import QtTest 1.0
import QtJsonDb 1.0

Item {
    name: "A First thing to run"
    id: firstThingie

    // NOTE: Since Organizer jsondb-backend expects to have User- and System-partitions,
    // we must ensure they are there before testing.

    signal partitionsCreated
    property int creationRequestAmount: 0

    Partition {
        id: systemPartition
    }

    JsonDbListModel {
        id: partitionModel
        query: "[?_type=\"Partition\"]"
        roleNames: ["name", "_uuid"]
        partition: systemPartition
    }

    SignalSpy {
        id: partitionsCreatedSpy
        target: firstThingie
        signalName: "partitionsCreated"
    }

    function check_db() {
        var neededPartitions = ["com.nokia.mt.System", "com.nokia.mt.User"];
        for (var n=0;n<neededPartitions.length;n++) {
            var found = false;
            for (var p=0;p<partitionModel.count;p++) {
                if (partitionModel.get(p, "name") == neededPartitions[n]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                console.log("Partition "+neededPartitions[n]+" not found. Creating it..");
                systemPartition.create({_type :"Partition", name : neededPartitions[n]}, callback);
                creationRequestAmount++;
            }
        }
        if (creationRequestAmount) {
            partitionsCreatedSpy.wait()
        }
    }

    function callback(error, meta, response) {
        if (error) {
            console.log(response.status+" "+response.message);
        } else {
            creationRequestAmount--;
            if (!creationRequestAmount)
                partitionsCreated();
        }
    }
}

