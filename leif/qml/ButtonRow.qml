import QtQuick
import QtQuick.Window
import QtQuick.Controls

Row {
    id: buttonRow

    anchors.top: parent.top
    anchors.topMargin: 30
    anchors.left: parent.left
    anchors.leftMargin: 30
    spacing: 30

    NavButton {
        text: qsTr("Status")
        shownState: "STATUS"
    }

    NavButton {
        text: qsTr("Location")
        shownState: "LOCATION"
    }

    NavButton {
        text: qsTr("About")
        shownState: "ABOUT"
    }
}
