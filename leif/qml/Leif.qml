import QtQuick
import QtQuick.Window
import QtQuick.Controls

ApplicationWindow {
    id: rootWindow

    property int sessionCarbon: 0
    property int lifeTimeCarbon: 27

    palette.link: "white"
    palette.text: "white"
    palette.windowText: "white"

    width: 768
    height: 480
    visible: true
    color: "#0f2732"
    title: qsTr("Leif - Not just green")

    Item {
        id: stateHandler

        state: "STATUS"

        states: [
            State {name: "STATUS"},
            State {name: "LOCATION"},
            State {name: "ABOUT"}
        ]
    }

    Behavior on sessionCarbon {
        NumberAnimation {
            duration: 500
            easing.type: Easing.InOutQuad
        }
    }

    Image {
        source: "../img/leif.png"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width * 0.55
        fillMode: Image.PreserveAspectFit
    }

    ButtonRow {}

    StatusPage {}

    LocationPage {}

    AboutPage {}
}
