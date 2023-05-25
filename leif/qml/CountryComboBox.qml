import QtQuick
import QtQuick.Window
import QtQuick.Controls

LeifComboBox {
    id: control

    defaultIcon: "../img/globe.png"
    defaultText: qsTr("Select your country")
    textRole: "name"
    valueRole: "code"

    delegate: ItemDelegate {
        width: control.width
        height: control.height
        palette.text: control.palette.buttonText
        font: control.font
        text: name
        highlighted: control.highlightedIndex === index
        icon.source: "../img/flags/" + code + ".png"
        icon.color: "transparent"
        spacing: control.spacing
        indicator: Image {
            visible: control.currentIndex === index
            source: control.highlightedIndex === index ? "../img/check_black.png" : "../img/check_white.png"
            x: control.width - width - control.rightInset - control.spacing
            y: control.topPadding + (control.availableHeight - height) / 2
        }
    }
}
