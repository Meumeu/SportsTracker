import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker 1.0

SplitViewPage {
    property WorkoutDetails details;

    id: page
    allowedOrientations: Orientation.All

    backgroundSize: page.isPortrait ? page.height / 10 : page.width / 5

    foreground: Plot {
        anchors.fill: parent

        paddingBottom: Theme.paddingLarge
        paddingTop: Theme.paddingMedium
        paddingLeft: Theme.paddingLarge*1.5
        paddingRight: Theme.paddingLarge*2.5
        font.pixelSize: Theme.fontSizeExtraSmall
        tickLength: Theme.paddingSmall

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

        onClicked: page.open = !page.open
    }
}
