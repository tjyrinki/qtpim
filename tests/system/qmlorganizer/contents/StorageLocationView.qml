/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
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

import QtQuick 2.0

Item {
    anchors.centerIn: parent
    opacity: 0
    width: calendar.width;
    height: settingsView.height - settingsView.buttonTabsRow.height;

    Column {
        spacing: 5
        y: settingsView.buttonTabsRow.height

        Text {
            anchors { horizontalCenter: parent.horizontalCenter }
            color: "White"
            font { pointSize: 15; weight: Font.Bold }
            text: "Select the locations:"
        }

        ListView {
            width: calendar.width; height: calendar.height/5;
            model: calendar.storageLocationModel
            clip: true
            delegate: Component {
                Row {
                    anchors { horizontalCenter: parent.horizontalCenter }
                    spacing: 5
                    Text {
                        text: name
                        height: 30
                        color: "White"
                        font { weight: Font.Bold }
                    }
                    CheckBox {
                        checked: organizer.storageLocations & settingsView.mapStorageLocationStringToInt(name)
                        onClicked: {
                            var currentLocations = organizer.storageLocations;
                            var newLocation = settingsView.mapStorageLocationStringToInt(calendar.storageLocationModel.get(index).name);
                            if (checked)
                                organizer.storageLocations = currentLocations | newLocation;
                            else
                                organizer.storageLocations = currentLocations & ~newLocation;
                        }
                    }
                }
            }
        }

        Text {
            anchors { horizontalCenter: parent.horizontalCenter }
            color: "White"
            font { pointSize: 15; weight: Font.Bold }
            text: "Collections in selected location(s):"
        }

        ListView {
            width: calendar.width; height: calendar.height/8;
            model: organizer.collections
            clip:true
            delegate: Text {
                anchors { horizontalCenter: parent.horizontalCenter }
                text: name
                color: "white"
            }
        }

        Text {
            anchors { horizontalCenter: parent.horizontalCenter }
            color: "White"
            font { pointSize: 15; weight: Font.Bold }
            text: "Items in selected location(s):"
        }

        ListView {
            width: calendar.width; height: calendar.height/3;
            model: organizer.items
            clip:true
            delegate: Text {
                anchors { horizontalCenter: parent.horizontalCenter }
                text: displayLabel
                color: "white"
            }
        }
    }
}
