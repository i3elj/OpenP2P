import App
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: item

  property Peer peer
  required property PeerListModel peerModel
  required property ApplicationWindow root

  Connections {
	function onNewConnection(p) {
	  item.peer = p;

	  if (peerModel.contains(item.peer)) {
		server.activatePeer(item.peer);
		return;
	  }

	  newConnDialog.open();
	}

	target: server
  }

  Dialog {
	id: newConnDialog

	closePolicy: Popup.NoAutoClose
	modal: false
	title: qsTr("Nova requisição de conexão!")
	width: 580
	x: root.width - width - 32
	y: root.height - height - 76

	contentItem: ColumnLayout {
	  spacing: 12

	  Label {
		Layout.fillWidth: true
		text: item.peer ? item.peer.name + " quer se conectar com você." : ""
		wrapMode: Label.Wrap
	  }

	  GridLayout {
		columnSpacing: 20
		columns: 2
		rowSpacing: 12

		Label {
		  text: "Endereço:"
		}
		Label {
		  Layout.fillWidth: true
		  text: item.peer ? item.peer.addr : ""
		  wrapMode: Label.Wrap
		}
		Label {
		  text: "Porta:"
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
