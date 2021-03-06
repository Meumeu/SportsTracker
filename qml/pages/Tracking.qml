import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker 1.0
import QtLocation 5.0
import "../util.js" as Util
import "../cover"

Page {
    id: page
    property Workout workout;
    property CoverPage cover;
    property bool toBeReset: false;

    property MapPolyline track;

    signal stopWorkout;
    signal startWorkout;
    signal positionUpdate;

    Component.onCompleted: {
        cover.stopWorkout.connect(stopWorkout);
        cover.startWorkout.connect(startWorkout);

        workout.lastPositionChanged.connect(positionUpdate);

        map.zoomLevel = 14;
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

    Column {
        anchors.fill: parent
        spacing: Theme.paddingMedium
        visible: workout.status === Workout.NotStarted

        PageHeader {
            title: qsTr("Workout")
        }

        ComboBox {
            id: sportsCombo
            label: qsTr("Sport: ")
            menu: ContextMenu {
                Repeater {
                    model: sportsList
                    delegate: MenuItem {
                        text: qsTr(name)
                    }
                }
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
            id: map
            plugin : Plugin {
                id: plugin
                allowExperimental: true
                preferred: ["osm"]
                required.mapping: Plugin.AnyMappingFeatures
                required.geocoding: Plugin.AnyGeocodingFeatures
            }

            gesture.enabled: false

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
            onClicked: startWorkout()
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
        sportsCombo.currentIndex = 0
        if (page.status === PageStatus.Active)
        {
            pageStack.navigateBack();
            toBeReset = true;
        }
        else
        {
            workout.reset();
        }

        map.removeMapItem(track);
        track.destroy();
    }

    onStartWorkout: {
        workout.reset();
        workout.sport = sportsList.get(sportsCombo.currentIndex).name
        workout.status = Workout.Tracking;

        track = Qt.createQmlObject('import QtLocation 5.0; MapPolyline { }', map);
        track.line.color = "red";
        track.line.width = 3;
        track.antialiasing = true;
        map.addMapItem(track);
    }

    onPositionUpdate: {
        if (workout.status == Workout.Tracking)
        {
            track.addCoordinate(workout.lastPosition);
        }
        map.center = workout.lastPosition;
    }

    Timer {
        interval: 60000
        running: workout.status === Workout.Tracking || workout.status === Workout.Paused
        repeat: true
        onTriggered: workout.save()
    }
}
