import QtQuick
import QtQuick.Controls
import App

Window {
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("OpenP2P")

    Column {
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

        Row {
            TextField {
                placeholderText: "Peer's IP"
                width: 230
            }

            Button {
                text: "Connect"
            }
        }
    }
}
