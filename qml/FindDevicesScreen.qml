import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls 2.15

Rectangle {
    color: "#f0f0f0" // Light background color

    Column {
        y: parent.height * 0.15
        width: parent.width  // Adjust width relative to the parent
        height: parent.height * 0.8// Adjust height relative to the parent
        spacing: 20
    

        Rectangle {
            width: 50 // Fixed width for placeholder
            height: 50 // Fixed height for placeholder
            anchors.horizontalCenter: parent.horizontalCenter
            color: "transparent" // Transparent background

            BusyIndicator {
                id: control
                anchors.centerIn: parent
                running: bleClient.isScanning
                visible: bleClient.isScanning // Only visible when running
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

                            Behavior on opacity {
                                OpacityAnimator {
                                    duration: 250
                                }
                            }

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
                                        Translate {
                                            y: -Math.min(item.width, item.height) * 0.5 + 5
                                        },
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


        Button {
            id: discoverButton
            text: "Discover Devices"
            font.pixelSize: 18
            width: 200
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            background: Rectangle {
                color: "#4CAF50" // Green button
                radius: 10
            }
            onClicked: {
                bleClient.startScan()
            }
        }

        ListView {
            id: deviceListView
            width: parent.width * 0.8
            height: parent.height * 0.5
            anchors.horizontalCenter: parent.horizontalCenter
            model: bleClient.devices
            spacing: 10
            clip: true

            delegate: Rectangle {
                width: parent.width
                height: 50
                color: "#ffffff" // White background for each item
                radius: 5
                border.color: "#cccccc" // Light gray border
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 20


                    Text {
                        text: modelData.name // Access the name property
                        font.pixelSize: 18
                        color: "#333333"
                        Layout.fillWidth: true
                    }

                    Text {
                        text: modelData.address // Access the address property
                        font.pixelSize: 14
                        color: "#666666"
                    }

                    Button {
                        text: "Connect"
                        font.pixelSize: 14
                        width: 100
                        height: 40
                        background: Rectangle {
                            color: "#2196F3" // Blue button
                            radius: 5
                        }
                        onClicked: {
                            bleClient.connectToDevice(model) // Pass the device info to connect
                        }
                    }
                }
            }
        }
    }
}
