import QtQuick 2.0
import Sailfish.Silica 1.0
import SportsTracker 1.0
import QtLocation 5.0
import "../util.js" as Util
import "../cover"

Page {
    id: page
    property Workout workout;
    property CoverPage cover;
    property bool toBeReset: false;

    signal stopWorkout;

    Component.onCompleted: {
        if (cover)
            cover.stopWorkout.connect(stopWorkout);
    }


    Column {
        anchors.fill: parent
        spacing: Theme.paddingMedium
        visible: workout.status === Workout.NotStarted

        PageHeader {
            title: qsTr("Workout")
        }

        ComboBox {
            label: qsTr("Sport: ")
            menu: ContextMenu {
                MenuItem { text: "Running" }
                MenuItem { text: "Cycling" }
                MenuItem { text: "Skiing" }
            }
        }
    }

    Column {
        anchors.fill: parent
        spacing: Theme.paddingMedium
        visible: workout.status === Workout.Tracking || workout.status === Workout.Paused

        PageHeader {
            title: qsTr("Workout")
        }


        Map {

            /* preferred: ["osm"]
            required.mapping: Plugin.AnyMappingFeatures
            required.geocoding: Plugin.AnyGeocodingFeatures */
            x: Theme.paddingLarge
            width: parent.width - 2 * x
            height: width * 0.75

        }

        Row {
            x: Theme.paddingLarge

            Column {
                spacing: Theme.paddingMedium

                Label {
                    text: qsTr("Time: ")
                    font.pixelSize: Theme.fontSizeSmall
                }

                Label {
                    text: qsTr("Current speed: ")
                    font.pixelSize: Theme.fontSizeSmall
                }

                Label {
                    text: qsTr("Average speed: ")
                    font.pixelSize: Theme.fontSizeSmall
                }

                Label {
                    text: qsTr("Distance: ")
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            Column {
                spacing: Theme.paddingMedium
                Label {
                    text: Util.timeToString(workout.duration)
                    font.pixelSize: Theme.fontSizeSmall
                }

                Label {
                    text: qsTr("%1 km/h").arg((workout.speed * 3.6).toLocaleString(Qt.locale() , "f", 1))
                    font.pixelSize: Theme.fontSizeSmall
                }

                Label {
                    text: qsTr("%1 km/h").arg((workout.avgSpeed * 3.6).toLocaleString(Qt.locale() , "f", 1))
                    font.pixelSize: Theme.fontSizeSmall
                }

                Label {
                    text: qsTr("%1 km").arg((workout.distance / 1000).toLocaleString(Qt.locale() , "f", 1))
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
        }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Theme.paddingLarge

        Button {
            visible: workout.status === Workout.NotStarted
            text: qsTr("Start")
            onClicked: workout.status = Workout.Tracking
        }

        Button {
            visible: workout.status === Workout.Tracking || workout.status === Workout.Paused
            text: qsTr("Stop")
            onClicked: stopWorkout()
        }

        Button {
            visible: workout.status === Workout.Tracking
            text: qsTr("Pause")
            onClicked: workout.status = Workout.Paused
        }

        Button {
            visible: workout.status === Workout.Paused
            text: qsTr("Resume")
            onClicked: workout.status = Workout.Tracking
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Inactive && toBeReset)
        {
            workout.reset();
            toBeReset = false;
        }
    }

    onStopWorkout: {
        workout.status = Workout.Stopped;
        workout.save();
        if (page.status === PageStatus.Active)
            pageStack.navigateBack();

        toBeReset = true;
    }

    Timer {
        interval: 60000
        running: workout.status === Workout.Tracking || workout.status === Workout.Paused
        repeat: true
        onTriggered: workout.save()
    }
}
