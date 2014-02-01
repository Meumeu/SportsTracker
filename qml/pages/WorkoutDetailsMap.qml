import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker 1.0
import QtLocation 5.0
import QtPositioning 5.0

SplitViewPage {
    property WorkoutDetails details;

    id: page
    allowedOrientations: Orientation.All

    backgroundSize: page.height / 10

    foreground: Map {
        anchors.fill: parent;
        id: map
        plugin : Plugin {
            id: plugin
            allowExperimental: true
            preferred: ["osm"]
            required.mapping: Plugin.AnyMappingFeatures
            required.geocoding: Plugin.AnyGeocodingFeatures
        }

        center: QtPositioning.coordinate(43.5, 7)
        MapPolyline {
            id: track;
            line.color: "red"
            line.width: 3
            antialiasing: true
        }

        MouseArea {
            anchors.fill: parent
            onClicked: page.open = !page.open
        }
    }

    Component.onCompleted: {
        details.fillPolyLine(track);
    }
}
