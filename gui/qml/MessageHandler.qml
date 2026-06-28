import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import App

Frame {
	Layout.fillHeight: true
	Layout.fillWidth: true
	padding: 8
	visible: peer !== null
	background: Rectangle {
		color: palette.light
		border.color: palette.mid
		border.width: 1
	}

	property Peer prevPeer
	required property Peer peer
	required property PeerListModel peerModel

	onPeerChanged: function () {
		if (prevPeer !== null) {
			prevPeer.saveChat()
			prevPeer = peer
			return
		}

		prevPeer = peer
		// if (peer !== null) {
		// 	prevPeer.saveChat()
		// 	peer.loadChat()
		// }
	}

	Connections {
		target: peer

		function onMsgSent(msg) {
			msgInput.clear()
			chatList.positionViewAtEnd()
		}
	}

	ColumnLayout {
		anchors.fill: parent

		ListView {
			id: chatList
			Layout.fillHeight: true
			Layout.fillWidth: true
			clip: true

			model: peer === null ? ListModel : peer.chatModel

			delegate: Label {
				text: model.text
				wrapMode: Label.Wrap
				width: chatList.width
				horizontalAlignment: model.sent ? Qt.AlignRight : Qt.AlignLeft
			}

			// function addMessage(sent, text) {
			// 	let newMsg = {
			// 		"sent": sent,
			// 		"text": !text.endsWith("\n") ? text + "\n" : text
			// 	}
			// 	chatList.model.append(newMsg)
			// 	chatList.positionViewAtEnd()
			//}
		}

		RowLayout {
			width: parent.width

			ScrollView {
				Layout.alignment: Qt.AlignTop
				Layout.maximumHeight: 200
				Layout.fillWidth: true
				Layout.minimumHeight: 36

				TextArea {
					id: msgInput
					wrapMode: TextArea.Wrap
					placeholderText: "Digite sua mensagem aqui..."
					Keys.onReturnPressed: sendMessage()

					function sendMessage() {
						let cleanMessage = msgInput.text.trim()

						if (cleanMessage.length === 0)
							return

						peer.sendMsg(cleanMessage)
					}
				}
			}

			Button {
				text: "Enviar"
				Layout.alignment: Qt.AlignTop
				onClicked: msgInput.sendMessage()
			}
		}
	}
}
