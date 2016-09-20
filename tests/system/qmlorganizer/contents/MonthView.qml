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
import "month.js" as Month
import QtOrganizer 5.0

Rectangle  {
    id:monthView
    property int month
    property int year
    property date startDay:new Date(year, month, 1)
    property int startWeekday:startDay.getDay()
    property var containItems: calendar.organizer.items ? calendar.organizer.containsItems(Month.dateOfThisDay(startDay, 1 - startWeekday), Month.dateOfThisDay(startDay, 43 - startWeekday), 86400) : undefined
    anchors.fill: parent

    Grid {
        id:container
        anchors.fill: parent
        columns: 7
        Repeater {
            model:["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"]
            Rectangle { width: container.width / 7
                        height: 35
                        color: "lightgray"
                        border.color: "#3f4947"
                        Text { text: modelData
                            font.bold: true
                            verticalAlignment: Text.AlignVCenter
                            style: Text.Sunken
                            styleColor: "#1365f3"
                            font.pointSize: 11
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                calendar.state = "WeekView";
                            }
                        }
            }
        }

        Repeater { model: 42
                   Rectangle {
                       id:dayContainer
                       radius:10
                       width: container.width / 7
                       height: (container.height - 35) / 6
                       color:  {
                                  if (Month.isToday(startDay,   index - startWeekday +1)) {
                                      dayContainer.radius= 20
                                      return "lightsteelblue";
                                  }
                                  else if (monthView.containItems[index]) {
                                      dayContainer.radius= 20
                                      return "yellow";
                                  }
                                  else {
                                      dayContainer.radius= 0
                                      return Month.getColorOfDay(startDay,   index - startWeekday +1);
                                  }
                              }
                       Text {
                           color: "#6ba24b";
                           text: Month.getDayOfMonth(startDay,   index - startWeekday +1)
                           font.bold: true
                           style: Text.Raised
                           font.pointSize: 10
                           anchors.centerIn: parent
                       }

                       MouseArea {
                           hoverEnabled:true
                           anchors.fill: parent
                           onEntered: {
                               dayContainer.border.color = "#1365f3"; // Set a dark blue surrounding border...
                               dayContainer.border.width = 3;
                           }
                           onExited: {
                               dayContainer.border.color = "#ffffff"; // Must reset the border and turn off else becomes graphics artifact
                           }
                           onReleased: {
                               dayContainer.border.color = "#ffffff"; // Must reset the border and turn off else becomes graphics artifact
                           }
                           onClicked: {
                               dayContainer.border.color = "#ffffff"; // Must reset the border and turn off else becomes graphics artifact
                               calendar.currentDate = new Date(calendar.year, calendar.month, index - startWeekday +1);
                               calendar.state = "DayView";
                           }
                       }
                   }
        }
    }
}
