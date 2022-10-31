import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Leif 1.0

BasePage {
    stateShownIn: "STATUS"

    CarbonModel {
        id: carbonModel
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
                    text: carbonModel.sessionCarbon
                    font.pointSize: 40
                    color: "#FE2831"
                }

                CenteredText {
                    text: "grCO2"
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
                    text: carbonModel.lifetimeCarbon
                    font.pointSize: 40
                }

                CenteredText {
                    text: "grCO2"
                }
            }
        }
    }
}
