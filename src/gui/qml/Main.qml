import App
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
  id: root

  property Peer currentPeer: null
  property PeerListModel peers: sessionManager.peers

  height: 720
  title: qsTr("OpenP2P")
  visible: true
  width: 1080

  Component.onCompleted: timer.start()

  Timer {
	id: timer

	interval: 500
	repeat: false

	onTriggered: function () {
	  if (!sessionManager.loadChats()) {
		console.warn("couldn't load peers chats");
	  }

	  server.tryReconnectAll();
	}
  }

  PeerConnection {
	id: peerConn

	peerModel: peers
	root: root
  }

  RowLayout {
	anchors.fill: parent
	anchors.margins: 8
	spacing: 12

	Frame {
	  id: sidebar

	  Layout.fillHeight: true
	  Layout.minimumWidth: list.implicitWidth + (padding * 2)
	  padding: 8

	  background: Rectangle {
		border.color: palette.mid
		border.width: 1
		color: palette.light
	  }

	  ColumnLayout {
		anchors.fill: parent
		spacing: 12

		Text {
		  Layout.fillWidth: true
		  color: palette.text
		  font.pixelSize: 12
		  horizontalAlignment: Text.AlignHCenter
		  text: "Lista de Amigos"
		}

		ListView {
		  id: list

		  Layout.fillHeight: true
		  Layout.fillWidth: true

		  clip: true
		  focus: true
		  model: root.peers
		  spacing: 8

		  delegate: Item {
			height: rowLayout.height + 8
			width: ListView.view.width

			ContextMenu.menu: Menu {
			  MenuItem {
				text: "Remover"

				onTriggered: sessionManager.deletePeer(model.peer)
			  }
			}

			Rectangle {
			  id: background

			  anchors.fill: parent
			  color: mouseArea.containsMouse ? palette.highlight : "transparent"
			  radius: 5
			}

			Pane {
			  anchors.verticalCenter: parent.verticalCenter
			  background: null
			  padding: 8

			  RowLayout {
				id: rowLayout

				spacing: 8

				Rectangle {
				  color: model.peer.active ? "green" : "gray"
				  height: 10
				  radius: 100
				  width: 10
				}

				Text {
				  color: palette.text
				  text: model.peer.name
				}
			  }
			}

			MouseArea {
			  id: mouseArea

			  anchors.fill: parent
			  cursorShape: Qt.PointingHandCursor
			  hoverEnabled: true

			  onClicked: {
				list.currentIndex = index;
				currentPeer = peers.at(index);
			  }
			}
		  }
		}
	  }
	}

	ColumnLayout {
	  Layout.alignment: Qt.AlignTop
	  Layout.fillHeight: true
	  Layout.fillWidth: true
	  spacing: 8

	  RowLayout {
		id: selfIpSection

		spacing: 12

		Label {
		  text: "IPv6:"
		}

		Column {
		  id: ipList

		  property var ipModel: []

		  AddrLabel {
			id: addrLabel
			onIpsReceived: ips => ipList.ipModel = ips
		  }

		  RowLayout {
			Repeater {
			  model: ipList.ipModel

			  TextEdit {
				required property string modelData

				color: palette.text
				readOnly: true
				selectByMouse: true
				text: modelData
			  }
			}

			TextField {
			  id: selfPort
			  placeholderText: "Número da Porta (7575)"
			  Component.onCompleted: text = self.port
			}

			Button {
			  text: "Salvar"
			  onClicked: self.port = selfPort.text
			}
		  }
		}
	  }

	  RowLayout {
		id: selfNameSection
		spacing: 12

		Label {
		  text: "Seu Nome:"
		}

		TextField {
		  id: selfName
		  placeholderText: "Jack Sparrow"
		  Component.onCompleted: text = self.name
		}

		Button {
		  text: "Salvar"
		  onClicked: self.name = selfName.text
		}
	  }

	  RowLayout {
		id: peerIpSection

		TextField {
		  id: peerIP
		  Layout.fillWidth: true
		  placeholderText: "IP do Amigo"
		}

		TextField {
		  id: peerPort
		  placeholderText: "Número da Porta"
		}

		Button {
		  text: "Conectar"
		  onClicked: function () {
			server.startNewConn(peerIP.text, Number(peerPort.text));
		  }
		}
	  }

	  MessageHandler {
		peer: currentPeer
		peerModel: peers
	  }
	}
  }
}
