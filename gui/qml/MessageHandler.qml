import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import App

Frame {
	Layout.fillHeight: true
	Layout.fillWidth: true
	padding: 8
	visible: peer !== null

	property Peer prevPeer
	required property Peer peer
	required property ListModel peerModel

	onPeerChanged: function () {
		if (peer !== null) {
			// peerManager.saveChatHistory(prevPeer, msgModel)
			// let messages = peerManager.loadChatHistory(peer)
			// msgModel.set(messages)
			prevPeer = peer
		}
	}

	Connections {
		target: peer
		function onNewMsg(from, msg) {
			console.warn("received:", msg)
		}

		function onMsgSent(msg) {
			chatList.addMessage(true, msg)
			chatList.positionViewAtEnd()
			msgInput.clear()
		}
	}

	Connections {
		target: server
		function onNewMsg(from, msg) {
			let newMsg = {
				"sent": false,
				"text": msg
			}

			if (from === peer) {
				msgModel.append(newMsg)
				console.log(msg)
			} else {

				// peerManager.addNewMsg(from, newMsg)
			}
		}
	}

	Connections {
		id: sessionManager
		target: sessionManager
	}

	ColumnLayout {
		anchors.fill: parent

		ListView {
			id: chatList
			Layout.fillHeight: true
			Layout.fillWidth: true
			clip: true

			model: ListModel {
				id: msgModel
			}

			delegate: Label {
				text: model.text
				wrapMode: Label.Wrap
				width: chatList.width
				horizontalAlignment: model.sent ? Qt.AlignRight : Qt.AlignLeft
			}

			function addMessage(sent, text) {
				let newMsg = {
					"sent": sent,
					"text": text
				}
				msgModel.append(newMsg)
			}
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
					placeholderText: "Type your message here..."
					Keys.onReturnPressed: sendMessage()

					function sendMessage() {
						if (msgInput.text.length === 0) {
							return
						}

						// TODO: workout the `me` sending a msg to `peer`
						// peer.sendMsg(msgInput.text)
						chatList.addMessage(true, msgInput.text)
						chatList.positionViewAtEnd()
						msgInput.clear()
					}
				}
			}

			Button {
				text: "Send"
				Layout.alignment: Qt.AlignTop
				onClicked: msgInput.sendMessage()
			}
		}
	}
}
