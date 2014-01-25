import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker 1.0
import "../util.js" as Util

Page {
    id: page
    property var filename;

    SilicaFlickable {
        anchors.fill: parent

        BusyIndicator {
            anchors.centerIn: parent
            running: details.loading
            size: BusyIndicatorSize.Large
            visible: details.loading
        }

        Column {
            visible: !details.loading
            anchors.fill: parent
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Workout details")
            }

            Row {
                width: parent.width
                Label {
                    text: qsTr(details.sport)
                    width: parent.width - labelDate.width
                }
                Label {
                    id: labelDate
                    text: Qt.formatDateTime(details.date)
                }
            }

            Plot {
                id: toto
                width: parent.width
                height: width * 0.75

                WorkoutDetails {
                    id: details
                    filename: page.filename
                }
            }

            Component.onCompleted: {
                details.altitude.colour = "blue"; //Theme.primaryColor
                details.speed.colour = "green"; //Theme.secondaryColor
            }

            Grid {
                columns: 2
                spacing: Theme.paddingMedium

                Label {
                    text: qsTr("Distance")
                }

                Label {
                    text: Util.distanceToString(details.distance)
                }

                Label {
                    text: qsTr("Duration")
                }

                Label {
                    text: Util.timeToString(details.duration)
                }

                Label {
                    text: qsTr("Average speed")
                }

                Label {
                    text: Util.speedToString(details.distance / details.duration)
                }

                Label {
                    text: qsTr("Maximum speed")
                }

                Label {
                    text: Util.speedToString(details.maxSpeed)
                }
            }
        }
    }
}
