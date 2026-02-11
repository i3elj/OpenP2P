import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import App

Item {
	id: item
	property Peer peer
	required property ApplicationWindow root
	required property ListModel peerModel

	Connections {
		target: server

		function onNewConnection(p) {
			item.peer = p
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

			onAccepted: function () {
				server.setupPeer(item.peer)
				peerModel.append({"peer": item.peer})
			}

			onRejected: server.rejectPeer(item.peer)
		}
	}
}
