import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker 1.0

Dialog {
    SilicaFlickable {
        anchors.fill: parent
        VerticalScrollDecorator {}

        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            PageHeader {
                title: qsTr("Settings")
            }

            TextSwitch {
                id: syncEnabled
                width: parent.width
                text: qsTr("Synchronize")
                //description: ""
            }

            TextField {
                id: syncServer
                width: parent.width
                label: qsTr("Server")
                placeholderText: label
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }

            TextSwitch {
                id: syncSSL
                width: parent.width
                text: qsTr("Use SSL")
            }

            TextField {
                id: syncRootPath
                width: parent.width
                label: qsTr("Root path")
                placeholderText: label
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhUrlCharactersOnly | Qt.ImhNoAutoUppercase
            }

            TextField {
                id: syncUserName
                width: parent.width
                label: qsTr("User name")
                placeholderText: label
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }

            TextField {
                id: syncPassword
                width: parent.width
                label: qsTr("Password")
                placeholderText: label
                echoMode: TextInput.Password
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }

            ComboBox {
                id: syncPeriod
                width: parent.width
                label: qsTr("Sync every")
                menu: ContextMenu {
                    MenuItem { text: qsTr("15 minutes") }
                    MenuItem { text: qsTr("1 hour") }
                    MenuItem { text: qsTr("4 hours") }
                }
            }
        }
    }

    onOpened: {
        syncEnabled.checked = Settings.value("sync.enabled", false);
        syncSSL.checked = Settings.value("sync.ssl", false);
        syncServer.text = Settings.value("sync.server", "");
        syncRootPath.text = Settings.value("sync.rootpath", "/");
        syncUserName.text = Settings.value("sync.username", "");
        syncPassword.text = Settings.value("sync.password", "");

    }

    onAccepted: {
        Settings.setValue("sync.enabled", syncEnabled.checked);
        Settings.setValue("sync.ssl", syncSSL.checked);
        Settings.setValue("sync.server", syncServer.text);
        Settings.setValue("sync.rootpath", syncRootPath.text);
        Settings.setValue("sync.username", syncUserName.text);
        Settings.setValue("sync.password", syncPassword.text);
    }
}
