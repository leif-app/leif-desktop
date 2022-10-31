import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

LeifComboBox {
    id: control

    defaultIcon: "../img/globe.png"
    defaultText: qsTr("Select your country")
    textRole: "name"
    valueRole: "code"

    delegate: ItemDelegate {
        width: control.width
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
