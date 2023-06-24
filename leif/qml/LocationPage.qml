import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Leif 1.0
import Leif.Controllers 1.0

BasePage {
    stateShownIn: "LOCATION"

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

                Component.onCompleted: currentIndex = indexOfValue(SettingsController.country)
                onCurrentValueChanged: SettingsController.country = currentValue
            }

            RegionComboBox {
                id: control

                model: RegionModel {
                    id: regionModel

                    country: countryBox.currentValue
                }

                Component.onCompleted: {
                    regionModel.country = SettingsController.country;
                    currentIndex = indexOfValue(SettingsController.regionId);
                }

                onCurrentValueChanged: {
                    console.log("Current value changed!");
                    SettingsController.regionId = currentValue;
                }
            }
        }
    }
}
