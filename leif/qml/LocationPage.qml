import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Leif 1.0

BasePage {
    stateShownIn: "LOCATION"

    SettingsModel {
        id: settings
    }

    Item {
        anchors.fill: parent
        anchors.margins: 30

        Column {
            spacing: 30
            anchors.centerIn: parent
            width: parent.width

            CenteredText {
                font.pointSize: 16
                text: qsTr("Leif needs your location to give the most accurate CO₂ readings.")
                wrapMode: Text.Wrap
                width: parent.width
            }

            CountryComboBox {
                id: countryBox

                model: CountryModel {}

                Component.onCompleted: currentIndex = indexOfValue(settings.country)
                onCurrentValueChanged: settings.country = currentValue
            }

            RegionComboBox {
                id: control

                model: RegionModel {
                    id: regionModel

                    country: countryBox.currentValue
                }

                Component.onCompleted: {
                    regionModel.country = settings.country;
                    currentIndex = indexOfValue(settings.regionId);
                }

                onCurrentValueChanged: {
                    console.log("Current value changed!");
                    settings.regionId = currentValue;
                }
            }
        }
    }
}
