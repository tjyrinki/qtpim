/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0

// These buttons are very ugly
Rectangle {
    id: container

    signal clicked
    property string image: ""
    property string text: ""
    property int padding: 2

    color: activePalette.button;
    smooth: true
    border.width: 1;
    border.color: Qt.darker(activePalette.dark);
    radius: 2;


    gradient: Gradient {
        GradientStop { position: 0.0; color: Qt.lighter(activePalette.button) }
        GradientStop { position: 1.0; color : mr.pressed ?  Qt.lighter(activePalette.light) : Qt.darker(activePalette.dark) }
    }
    width: stuff.width + 4 * padding
    height: stuff.height + 2 * padding

    Item {
        id: stuff
        width: childrenRect.width;
        height: childrenRect.height;
        y: padding
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            Image {
                id: imgItem;
                smooth: true
                width: source != "" ? 16 : 0; height: source != "" ? 16 : 0;
                source: container.image;
                opacity: source == "" ? 0 : 1;
            }

            Text {
                horizontalAlignment: Text.AlignHCenter
                id: txtItem; text: container.text; font.pixelSize: 14; color: activePalette.buttonText
            }
        }
    }

    // Don't make mr part of the size determination, since it uses anchors.fill
    MouseArea { id: mr; anchors.fill: parent; onClicked: container.clicked() }

}
