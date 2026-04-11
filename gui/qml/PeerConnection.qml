import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import App

Item {
	id: item
	property Peer peer
	required property ApplicationWindow root
	required property PeerListModel peerModel

	Connections {
		target: server

		function onNewConnection(p) {
			item.peer = p

			if (peerModel.contains(item.peer)) {
				server.activatePeer(item.peer)
				return
			}

			newConnDialog.open()
		}
	}

	Dialog {
		id: newConnDialog
		title: "New Connection Request!"
		width: 580
		x: root.width - width - 32
		y: root.height - height - 76
		modal: false
		closePolicy: Popup.NoAutoClose

		contentItem: ColumnLayout {
			spacing: 12

			Label {
				text: item.peer ? item.peer.name + " wants to connect with you." : ""
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
					text: item.peer ? item.peer.addr : ""
					Layout.fillWidth: true
					wrapMode: Label.Wrap
				}
				Label {
					text: "Port Number:"
				}
				Label {
					text: item.peer ? item.peer.port : ""
				}
			}
		}

		footer: DialogButtonBox {
			standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

			onAccepted: server.acceptIncomingPeer(item.peer)
			onRejected: server.rejectIncomingPeer(item.peer)
		}
	}
}
