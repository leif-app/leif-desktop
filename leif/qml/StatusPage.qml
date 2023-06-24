import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Leif 1.0
import Leif.Controllers 1.0

BasePage {
    stateShownIn: "STATUS"

    property real sessionDisplay: CarbonController.sessionCarbon
    property real lifetimeDisplay: CarbonController.lifetimeCarbon

    Behavior on sessionDisplay {
        NumberAnimation {
            duration: 1000
            easing.type: Easing.InOutQuad
        }
    }

    Behavior on lifetimeDisplay {
        NumberAnimation {
            duration: 1000
            easing.type: Easing.InOutQuad
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 40

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            CenteredText {
                text: qsTr("Session")
                font.pointSize: 22
                font.weight: Font.Medium
            }

            Column {
                anchors.horizontalCenter: parent.horizontalCenter

                CenteredText {
                    text: sessionDisplay.toFixed(2)
                    font.pointSize: 40
                    color: "#FE2831"
                }

                CenteredText {
                    text: "gCO₂e"
                }

            }

        }

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            CenteredText {
                text: qsTr("Lifetime")
                font.pointSize: 22
                font.weight: Font.Medium
            }

            Column {
                anchors.horizontalCenter: parent.horizontalCenter

                CenteredText {
                    text: lifetimeDisplay.toFixed(1)
                    font.pointSize: 40
                }

                CenteredText {
                    text: "gCO₂e"
                }
            }
        }
    }
}
