import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker.SportsTracker 1.0
import "../util.js" as Util

Page {
    id: page
    property var filename;

    SilicaFlickable {
        anchors.fill: parent

        Column {
            anchors.fill: parent

            PageHeader {
                title: qsTr("Workout details")
            }

            Plot {
                width: parent.width
                height: width
                data: [ PlotData {} ]
            }

            /*Row {
                Label {
                    text: "00/00/0000 00:00"
                }

                Label {
                    text: "foobar"
                }
            }

            Label {
                text: qsTr("Duration")
            }

            Label {
                text: "00:00:00"
            }

            Label {
                text: qsTr("Distance")
            }

            Label {
                text: "0 km"
            }

            Label {
                text: qsTr("Average speed")
            }

            Label {
                text: "0 km/h"
            }

            Label {
                text: qsTr("Maximum speed")
            }

            Label {
                text: "0 km/h"
            }*/
        }
    }
}
