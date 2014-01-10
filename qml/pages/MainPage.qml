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


Page {
    function timeToString(t)
    {
        var s = t % 60;
        var m = Math.floor(t / 60) % 60
        var h = Math.floor(t / 3600)

        return "%1:%2:%3".arg(h).arg(m < 10 ? ("0" + m) : m).arg(s < 10 ? ("0"+ s) : s);
    }

    id: page

    ListModel {
        id: workoutData
        ListElement {
            date: "lorem ipsum"
            sport: "foobar"
            distance: 10
            time: 3000
        }
        ListElement {
            date: "lorem ipsum"
            sport: "foobar"
            distance: 11
            time: 3001
        }
        ListElement {
            date: "lorem ipsum"
            sport: "foobar"
            distance: 12
            time: 3600
        }
    }

    SilicaListView {
        id: workoutList
        anchors.fill: parent
        model: workoutData
        spacing: Theme.paddingMedium

        PullDownMenu {
            MenuItem {
                text: "Start tracking"
                onClicked: pageStack.navigateForward(PageStackAction.Animated)
            }
            MenuItem {
                text: "Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }
        }

        header: Column {
            width: page.width
            height: childrenRect.height + Theme.paddingLarge

            PageHeader {
                title: "Sports Tracker"
            }

            Label {
                x: Theme.paddingLarge
                text: workoutList.count + " workout(s)"
            }
            Row
            {
                spacing: Theme.paddingMedium
                Column
                {
                    Label {
                        x: Theme.paddingLarge
                        text: "Total time"
                    }
                    Label {
                        x: Theme.paddingLarge
                        text: "Total distance"
                    }
                }
                Column
                {
                    Label {
                        x: Theme.paddingLarge
                        text: "toto"
                        color: Theme.secondaryColor
                    }
                    Label {
                        x: Theme.paddingLarge
                        text: "toto"
                        color: Theme.secondaryColor
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: workoutList.count == 0
            text: "No items"
        }

        delegate: BackgroundItem {
            id: delegate
            height: column2.height

            Column {
                id: column2

                width: parent.width - 2 * x
                x: Theme.paddingLarge

                Label {
                    text: date
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
                            text: "Distance"
                            font.pixelSize: Theme.fontSizeSmall
                        }
                        Label {
                            text: distance.toLocaleString(Qt.locale() , "f", 2) + " km"
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                    Column {
                        width: (delegate.width - img.width)/3
                        Label {
                            text: "Time"
                            font.pixelSize: Theme.fontSizeSmall
                        }
                        Label {
                            text: timeToString(time)
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                    Column {
                        width: (delegate.width - img.width)/3
                        Label {
                            text: "Speed"
                            font.pixelSize: Theme.fontSizeSmall
                        }
                        Label {
                            text: time > 0 ? ((distance * 3600 / time).toLocaleString(Qt.locale() , "f", 1) + " km/h") : "N/A"
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }
}


