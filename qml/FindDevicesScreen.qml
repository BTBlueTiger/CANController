import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls 2.15

Rectangle {
    color: "#f0f0f0"
    anchors.fill: parent

    Column {
        anchors.centerIn: parent
        width: Math.min(parent.width * 0.95, 500)
        spacing: 32

        // Title
        Text {
            text: "Bluetooth Device Finder"
            font.pixelSize: 28
            font.bold: true
            color: "#222"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Busy Indicator
        Rectangle {
            width: 60
            height: 60
            color: "transparent"
            anchors.horizontalCenter: parent.horizontalCenter

            BusyIndicator {
                id: control
                anchors.centerIn: parent
                running: bleClient.isScanning
                visible: bleClient.isScanning
                contentItem: Item {
                    implicitWidth: 64
                    implicitHeight: 64
                    Item {
                        id: item
                        x: parent.width / 2 - 32
                        y: parent.height / 2 - 32
                        width: 64
                        height: 64
                        opacity: control.running ? 1 : 0

                        Behavior on opacity { OpacityAnimator { duration: 250 } }
                        RotationAnimator {
                            target: item
                            running: control.visible && control.running
                            from: 0
                            to: 360
                            loops: Animation.Infinite
                            duration: 1250
                        }
                        Repeater {
                            id: repeater
                            model: 6
                            Rectangle {
                                x: item.width / 2 - width / 2
                                y: item.height / 2 - height / 2
                                implicitWidth: 10
                                implicitHeight: 10
                                radius: 5
                                color: "#21be2b"
                                transform: [
                                    Translate { y: -Math.min(item.width, item.height) * 0.5 + 5 },
                                    Rotation {
                                        angle: index / repeater.count * 360
                                        origin.x: 5
                                        origin.y: 5
                                    }
                                ]
                            }
                        }
                    }
                }
            }
        }

        // Discover Button
        Button {
            id: discoverButton
            text: "Discover Devices"
            font.pixelSize: 18
            width: 220
            height: 48
            anchors.horizontalCenter: parent.horizontalCenter
            background: Rectangle {
                color: "#4CAF50"
                radius: 12
            }
            onClicked: bleClient.startScan()
        }

        // Device List
        Rectangle {
            width: parent.width
            height: 320
            color: "#e0e0e0"
            radius: 12
            border.color: "#cccccc"
            border.width: 1
            anchors.horizontalCenter: parent.horizontalCenter

            ListView {
                id: deviceListView
                anchors.fill: parent
                anchors.margins: 12
                model: bleClient.devices
                spacing: 14
                clip: true

                delegate: Rectangle {
                    width: deviceListView.width - 24
                    height: 60
                    color: "#fff"
                    radius: 8
                    border.color: "#bdbdbd"
                    border.width: 1
                    anchors.horizontalCenter: parent.horizontalCenter

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 18

                        ColumnLayout {
                            Layout.fillWidth: true
                            Text {
                                text: modelData.name
                                font.pixelSize: 18
                                color: "#222"
                                elide: Text.ElideRight
                            }
                            Text {
                                text: modelData.address
                                font.pixelSize: 13
                                color: "#888"
                                elide: Text.ElideRight
                            }
                        }

                        Button {
                            text: "Connect"
                            font.pixelSize: 15
                            width: 100
                            height: 40
                            background: Rectangle {
                                color: "#2196F3"
                                radius: 8
                            }
                            onClicked: bleClient.connectToDevice(modelData.address)
                        }
                    }
                }
            }
        }
    }
}