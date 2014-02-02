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
import harbour.sportstracker 1.0
import "../components"
import "../util.js" as Util


Page {
    id: mainPage

    SilicaListView {
        id: workoutList
        anchors.fill: parent
        model: WorkoutSummaryList

        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
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
                        color: Theme.secondaryColor
                        text: qsTr("Total time")
                    }
                    Label {
                        x: Theme.paddingLarge
                        color: Theme.secondaryColor
                        text: qsTr("Total distance")
                    }
                }
                Column
                {
                    Label {
                        x: Theme.paddingLarge
                        text: Util.timeToString(WorkoutSummaryList.totalTime)
                    }
                    Label {
                        x: Theme.paddingLarge
                        text: Util.distanceToString(WorkoutSummaryList.totalDistance)
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: workoutList.count == 0
            text: qsTr("No workouts")
        }

        delegate: ListItem {
            id: contentItem
            menu: contextMenu
            width: parent.width
            contentHeight: workout.height + Theme.paddingLarge

            ListView.onRemove: animateRemoval(contentItem)

            function remove() {
                remorseAction(qsTr("Deleting"), function()
                    {
                        WorkoutSummaryList.remove(index)
                    })
            }

            onClicked: pageStack.push(Qt.resolvedUrl("WorkoutDetailsPage.qml"), { filename: model.filename })

            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        text: qsTr("Delete")
                        onClicked: remove()
                    }
                    MenuItem {
                        text: qsTr("Share")
                    }
                }
            }

            WorkoutSummary {
                id: workout
                width: parent.width - 2 * x
                x: Theme.paddingLarge
                y: Theme.paddingLarge / 2

                distance: model.distance
                duration: model.duration
                date: model.date
                sport: model.sport
            }
        }

        VerticalScrollDecorator {}
    }
}
