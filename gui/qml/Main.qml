import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import App

ApplicationWindow {
	id: root
	minimumWidth: 920
	minimumHeight: 460
	visible: true
	title: qsTr("OpenP2P")

	property Peer currentPeer: null
	property ListModel peers: ListModel {}

	PeerConnection {
		id: peerConn
		root: root
		peerModel: peers
	}

	RowLayout {
		anchors.fill: parent
		anchors.margins: 8
		spacing: 32

		Pane {
			id: sidebar
			padding: 8
			Layout.fillHeight: true
			Layout.minimumWidth: list.contentItem.childrenRect.width + (padding * 2)

			ColumnLayout {
				anchors.fill: parent
				spacing: 12

				Text {
					Layout.fillWidth: true
					text: "Saved Peers"
					horizontalAlignment: Text.AlignHCenter
				}

				ListView {
					id: list
					Layout.fillHeight: true
					Layout.fillWidth: true
					spacing: 8
					focus: true
					clip: true
					model: root.peers
					delegate: Component {
						RowLayout {
							spacing: 4
							Layout.alignment: Qt.AlignCenter

							Rectangle {
								width: 40
								height: 40
								radius: 5
								color: "blue"
							}

							Text {
								text: model.peer.name + " " + index
							}

							MouseArea {
								anchors.fill: parent
								hoverEnabled: true
								z: 0
								onEntered: this.cursorShape = Qt.PointingHandCursor
								onClicked: function () {
									list.currentIndex = index
									currentPeer = peers.get(index).peer
								}
							}
						}
					}
					highlight: Rectangle {
						color: "lightsteelblue"
						radius: 5
					}
				}
			}
		}

		ColumnLayout {
			Layout.alignment: Qt.AlignTop
			Layout.fillHeight: true
			Layout.fillWidth: true
			spacing: 8

			Row {
				spacing: 12

				Label {
					text: "Your IPv6:"
				}

				Column {
					id: ipList

					property var ipModel: []

					AddrLabel {
						id: addrLabel
						onIpsReceived: function (ips) {
							ipList.ipModel = ips
						}
					}

					Repeater {
						model: ipList.ipModel

						TextEdit {
							required property string modelData

							text: modelData
							readOnly: true
							selectByMouse: true
						}
					}
				}
			}

			RowLayout {
				TextField {
					id: peerIP
					placeholderText: "Peer's IP"
					width: 230
				}

				Button {
					text: "Connect"
				}
			}

			MessageHandler {
				peer: currentPeer
				peerModel: peers
			}
		}
	}
}
