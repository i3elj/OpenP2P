import App
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
  required property Peer peer
  required property PeerListModel peerModel
  property Peer prevPeer

  Layout.fillHeight: true
  Layout.fillWidth: true
  padding: 8
  visible: peer !== null

  background: Rectangle {
	border.color: palette.mid
	border.width: 1
	color: palette.light
  }

  onPeerChanged: function () {
	if (prevPeer !== null) {
	  prevPeer.saveChat();
	  prevPeer = peer;
	  return;
	}

	prevPeer = peer;
  }

  Connections {
	target: peer

	function onMsgSent(msg) {
	  msgInput.clear();
	  chatList.positionViewAtEnd();
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
		horizontalAlignment: model.sent ? Qt.AlignRight : Qt.AlignLeft
		text: model.text
		width: chatList.width
		wrapMode: Label.Wrap
	  }
	}

	RowLayout {
	  width: parent.width

	  ScrollView {
		Layout.alignment: Qt.AlignTop
		Layout.fillWidth: true
		Layout.maximumHeight: 200
		Layout.minimumHeight: 36

		TextArea {
		  id: msgInput

		  function sendMessage() {
			let cleanMessage = msgInput.text.trim();

			if (cleanMessage.length === 0)
			  return;
			peer.sendMsg(cleanMessage);
		  }

		  placeholderText: "Digite sua mensagem aqui..."
		  wrapMode: TextArea.Wrap

		  Keys.onReturnPressed: sendMessage()
		}
	  }

	  Button {
		Layout.alignment: Qt.AlignTop
		text: "Enviar"

		onClicked: msgInput.sendMessage()
	  }
	}
  }
}
