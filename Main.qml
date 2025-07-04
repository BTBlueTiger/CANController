import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 2.15
import CanController

import "qml"

Window {

    property BLEClient bleClient: _bleClient



    width: 1080 / 2
    height: 2400 / 2
    visible: true
    title: qsTr("CANController")

    StackView {
        anchors.fill: parent
        initialItem: findDeviceScreen
    }


    Component {
        id: findDeviceScreen
        FindDevicesScreen {
            anchors.fill: parent
        }
    }
}
