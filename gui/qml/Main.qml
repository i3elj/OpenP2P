import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import App

ApplicationWindow {
    id: appwindow
    minimumWidth: 920
    minimumHeight: 460
    visible: true
    title: qsTr("OpenP2P")

    PeerConnection {
        window: appwindow
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8

        Pane {
            id: sidebar
            padding: 8
            Layout.fillHeight: true
            Layout.minimumWidth: list.contentItem.childrenRect.width + (padding * 2)

            ListView {
                id: list
                anchors.fill: parent
                spacing: 8
                focus: true
                clip: true
                model: 3
                delegate: Component {
                    RowLayout {
                        spacing: 4
                        Layout.alignment: Qt.AlignCenter
                        Rectangle {
                            width: 40
                            height: 40
                            color: "blue"
                        }
                        Text {
                            text: "Fulano de tal"
                        }
                    }
                }
                highlight: Rectangle {
                    color: "lightsteelblue"
                    radius: 5
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 8

            Row {
                spacing: 12

                Label {
                    text: "Your IPv6:"
                }

                Column {
                    id: ipList

                    property var ipModel: []

                    AddrLabel {
                        id: addrLabel
                        onIpsReceived: function (ips) {
                            ipList.ipModel = ips
                        }
                    }

                    Repeater {
                        model: ipList.ipModel

                        TextEdit {
                            required property string modelData

                            text: modelData
                            readOnly: true
                            selectByMouse: true
                        }
                    }
                }
            }

            RowLayout {
                TextField {
                    id: peerIP
                    placeholderText: "Peer's IP"
                    width: 230
                }

                Button {
                    text: "Connect"
                }
            }

            MessageHandler {}
        }
    }
}
