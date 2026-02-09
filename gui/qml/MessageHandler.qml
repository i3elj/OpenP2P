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
		if (prevPeer === null) {
			prevPeer = peer
		}

		if (peer !== null) {
			sessionManager.saveChat(prevPeer, msgModel)
			let messages = sessionManager.loadChat(peer)
			msgModel.clear()

			if (messages.length > 0) {
				msgModel.append(messages)
			}
		}

		prevPeer = peer
	}

	Connections {
		target: peer

		function onMsgSent(to, msg, success) {
			if (to === peer) {
				chatList.addMessage(true, msg)
			}
		}
	}

	Connections {
		target: server
		function onNewMsg(from, msg) {
			if (from === peer) {
				chatList.addMessage(false, msg)
			} else {
				// peerManager.addNewMsg(from, newMsg)
			}
		}
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
					"text": !text.endsWith("\n") ? text + "\n" : text
				}
				msgModel.append(newMsg)
				chatList.positionViewAtEnd()
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

						peer.sendMsg(msgInput.text)
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
