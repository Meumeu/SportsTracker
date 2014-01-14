/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import SportsTracker 1.0
import "../util.js" as Util


Page {
    id: mainPage

    SilicaListView {
        id: workoutList
        anchors.fill: parent
        model: WorkoutSummaryList
        spacing: Theme.paddingLarge

        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }
        }

        header: Column {
            width: mainPage.width
            height: childrenRect.height + Theme.paddingLarge

            PageHeader {
                title: "Sports Tracker"
            }

            Label {
                x: Theme.paddingLarge
                text: qsTr("%n workout(s)", "", workoutList.count)
            }
            Row
            {
                spacing: Theme.paddingMedium
                Column
                {
                    Label {
                        x: Theme.paddingLarge
                        text: qsTr("Total time")
                    }
                    Label {
                        x: Theme.paddingLarge
                        text: qsTr("Total distance")
                    }
                }
                Column
                {
                    Label {
                        x: Theme.paddingLarge
                        text: Util.timeToString(WorkoutSummaryList.totalTime)
                        color: Theme.secondaryColor
                    }
                    Label {
                        x: Theme.paddingLarge
                        text: qsTr("%1 km").arg((WorkoutSummaryList.totalDistance/1000).toLocaleString(Qt.locale() , "f", 2))
                        color: Theme.secondaryColor
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: workoutList.count == 0
            text: qsTr("No workouts")
        }

        delegate: BackgroundItem {
            id: delegate
            height: column2.height// + 2 * Theme.paddingLarge

            Column {
                id: column2

                width: parent.width - 2 * x
                x: Theme.paddingLarge
               // y: Theme.paddingLarge

                Label {
                    text: Qt.formatDateTime(display.date)
                    font.pixelSize: Theme.fontSizeSmall
                }
                Row {
                    Image {
                        id: img
                        source: "image://theme/icon-camera-sports"
                        height: column1.height
                        width: height + Theme.paddingLarge
                        fillMode: Image.PreserveAspectFit
                    }
                    Column {
                        id: column1
                        width: (delegate.width - img.width)/3
                        Label {
                            text: qsTr("Distance")
                            font.pixelSize: Theme.fontSizeSmall
                        }
                        Label {
                            text: qsTr("%1 km").arg((display.distance/1000).toLocaleString(Qt.locale() , "f", 2))
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                    Column {
                        width: (delegate.width - img.width)/3
                        Label {
                            text: qsTr("Time")
                            font.pixelSize: Theme.fontSizeSmall
                        }
                        Label {
                            text: Util.timeToString(display.time)
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                    Column {
                        width: (delegate.width - img.width)/3
                        Label {
                            text: qsTr("Speed")
                            font.pixelSize: Theme.fontSizeSmall
                        }
                        Label {
                            text: display.time > 0 ? qsTr("%1 km/h").arg((display.distance * 3.6 / display.time).toLocaleString(Qt.locale() , "f", 1)) : qsTr("N/A")
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }
}


