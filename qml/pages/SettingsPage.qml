import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker 1.0

Dialog {
    SilicaFlickable {
        anchors.fill: parent

        Column {
            anchors.fill: parent

            PageHeader {
                title: qsTr("Settings")
            }

            TextSwitch {
                id: sync
                width: parent.width
                text: qsTr("Synchronize")
                //description: ""
            }

            TextField {
                id: syncServer
                width: parent.width
                placeholderText: "toto"
                label: "titi"
            }
        }
    }

    onOpened: {
        sync.checked = Settings.value("sync", false);
        syncServer.text = Settings.value("syncServer", "");
    }

    onAccepted: {
        Settings.setValue("sync", sync.checked);
        Settings.setValue("syncServer", syncServer.text)
    }
}
