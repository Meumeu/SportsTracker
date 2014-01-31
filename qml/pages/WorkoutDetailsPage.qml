import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker 1.0
import "../util.js" as Util

Page {
    id: page
    property var filename;

    backNavigation: !toto.gesturing

    ListModel {
        id: sportsList
        ListElement {
            name: QT_TR_NOOP("Running")
        }
        ListElement {
            name: QT_TR_NOOP("Cycling")
        }
        ListElement {
            name: QT_TR_NOOP("Skiing")
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        interactive: false
        id: flickable

        BusyIndicator {
            anchors.centerIn: parent
            running: details.loading
            size: BusyIndicatorSize.Large
            visible: details.loading
        }

        Column {
            visible: !details.loading
            anchors.fill: parent
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Workout details")
            }

            Row {
                width: parent.width
                anchors.leftMargin: Theme.paddingLarge
                anchors.rightMargin: Theme.paddingLarge
                anchors.left: parent.left
                anchors.right: parent.right

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
                height: width * 0.75 + Theme.paddingLarge
                paddingBottom: Theme.paddingLarge
                paddingTop: Theme.paddingMedium
                paddingLeft: Theme.paddingLarge*1.5
                paddingRight: Theme.paddingLarge*2.5
                font.pixelSize: Theme.fontSizeExtraSmall

                tickLength: Theme.paddingSmall

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
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.paddingLarge
                anchors.rightMargin: Theme.paddingLarge

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
                    text: details.duration > 0 ? Util.speedToString(details.distance / details.duration) : qsTr("N/A")
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
