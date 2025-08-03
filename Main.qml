import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 2.15
import CanController

import "qml"

Window {

    property BLEClient bleClient: _bleClient
    property SDLController sdlController: _sdlController
    width : 1080 / 2
    height : 2400 / 2
    visible: true
    title: qsTr("CANController")


    Connections {
        target: bleClient
        onIsDeviceConnectedChanged: {
            if (bleClient.isDeviceConnected) {
                stackView.replaceCurrentItem(deviceControl)
                height = 1080 / 2
                width = 2400 / 2
            } else {
                stackView.replaceCurrentItem(findDeviceScreen)
                width = 1080 / 2
                height = 2400 / 2
            }

        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: findDeviceScreen
    }


    Component {
        id: findDeviceScreen
        FindDevicesScreen {
            anchors.fill: parent
        }
    }

    Component {
        id: deviceControl
        DeviceControl {
            anchors.fill: parent
        }
    }
}
