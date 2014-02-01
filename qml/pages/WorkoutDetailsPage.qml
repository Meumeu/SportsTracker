import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker 1.0
import QtLocation 5.0
import "../util.js" as Util

Page {
    id: page
    property alias filename: details.filename;

    backNavigation: !toto.gesturing

    WorkoutDetails {
        id: details
    }

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
                height: width * 0.5 + Theme.paddingLarge
                paddingBottom: Theme.paddingLarge
                paddingTop: Theme.paddingMedium
                paddingLeft: Theme.paddingLarge*1.5
                paddingRight: Theme.paddingLarge*2.5
                font.pixelSize: Theme.fontSizeExtraSmall
                tickLength: Theme.paddingSmall
                interactive: false

                PlotData {
                    source: details.altitude
                    colour: "blue"
                    axisId:  PlotData.Right
                }

                PlotData {
                    source: details.speed
                    colour: "green"
                    axisId:  PlotData.Left
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: pageStack.push(Qt.resolvedUrl("WorkoutDetailsPlot.qml"), {details: details})
                }

            }

            /*Map {
                id: map
                width: parent.width - 2 * Theme.paddingLarge
                height: width * 0.5
                x: Theme.paddingLarge
                plugin : Plugin {
                    id: plugin
                    allowExperimental: true
                    preferred: ["osm"]
                    required.mapping: Plugin.AnyMappingFeatures
                    required.geocoding: Plugin.AnyGeocodingFeatures
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: pageStack.push(Qt.resolvedUrl("WorkoutDetailsMap.qml"), {details: details})
                }
            }*/

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
