import QtQuick
import QtQuick.Window
import QtQuick.Controls

BasePage {
    stateShownIn: "ABOUT"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Image {
            source: "../img/title.png"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        CenteredText {
            text: qsTr("Desktop App\nVersion") + " " + Qt.application.version
            lineHeight: 1.5
            horizontalAlignment: Text.AlignHCenter
        }

        CenteredText {
            text: "<a href='https://leif.support/'>https://leif.support</a>"
            onLinkActivated: (link) => Qt.openUrlExternally(link);
        }

        CenteredText {
            text: "Copyright © Tim Stone 2022"
        }
    }
}
