import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    required property ApplicationWindow window

    Connections {
        target: server
        function onNewConnection(peer) {
            newConnDialog.peerName = "Fulano"
            newConnDialog.peerAddrs = peer.addrStr
            newConnDialog.peerPort = peer.port
            newConnDialog.open()
        }
    }

    Dialog {
        id: newConnDialog
        title: "New Connection Request!"
        width: 580
        x: window.width - width - 32
        y: window.height - height - 76

        property string peerName
        property string peerAddrs
        property int peerPort

        contentItem: ColumnLayout {
            spacing: 12

            Label {
                text: newConnDialog.peerName + " wants to connect with you."
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
                    text: newConnDialog.peerAddrs
                    Layout.fillWidth: true
                    wrapMode: Label.Wrap
                }
                Label {
                    text: "Port Number:"
                }
                Label {
                    text: newConnDialog.peerPort
                }
            }
        }

        footer: DialogButtonBox {
            standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

            onAccepted: server.setupPeer(newConnDialog.peerAddrs,
                                         newConnDialog.peerPort)
            onRejected: server.rejectPeer(newConnDialog.peerAddrs,
                                          newConnDialog.peerPort)
        }
    }
}
