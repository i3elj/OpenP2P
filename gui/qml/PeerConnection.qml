import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import App

Item {
    required property ApplicationWindow root
    required property ListModel peerModel
    property Peer peer

    Connections {
        target: server

        function onNewConnection(p) {
            peer = p
            peer.name = "Fulano"
            newConnDialog.open()
        }
    }

    Dialog {
        id: newConnDialog
        title: "New Connection Request!"
        width: 580
        x: root.width - width - 32
        y: root.height - height - 76

        contentItem: ColumnLayout {
            spacing: 12

            Label {
                text: peer ? peer.name + " wants to connect with you." : ""
                wrapMode: Label.Wrap
                Layout.fillWidth: true
            }

            GridLayout {
                columns: 2
                columnSpacing: 20
                rowSpacing: 12

                Label {
                    text: "Address:"
                }
                Label {
                    text: peer ? peer.addrStr : ""
                    Layout.fillWidth: true
                    wrapMode: Label.Wrap
                }
                Label {
                    text: "Port Number:"
                }
                Label {
                    text: peer ? peer.port : ""
                }
            }
        }

        footer: DialogButtonBox {
            standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

            onAccepted: function () {
                server.setupPeer(peer)
                peerModel.append({ "peer": peer }   )
            }

            onRejected: server.rejectPeer(peer)
        }
    }
}
