import QtQuick 2.0
import Sailfish.Silica 1.0
import "../util.js" as Util

Column {
    id: workout

    property double distance;
    property double duration;
    property var date;

    Label {
        text: Qt.formatDateTime(date)
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
            width: (contentItem.width - img.width)/3
            Label {
                text: qsTr("Distance")
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
            }
            Label {
                text: Util.distanceToString(distance)
                font.pixelSize: Theme.fontSizeSmall
            }
        }
        Column {
            width: (contentItem.width - img.width)/3
            Label {
                text: qsTr("Time")
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
            }
            Label {
                text: Util.timeToString(duration)
                font.pixelSize: Theme.fontSizeSmall
            }
        }
        Column {
            width: (contentItem.width - img.width)/3
            Label {
                text: qsTr("Speed")
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
            }
            Label {
                text: duration > 0 ? Util.speedToString(distance / duration) : qsTr("N/A")
                font.pixelSize: Theme.fontSizeSmall
            }
        }
    }
}
