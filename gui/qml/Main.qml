import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import App

ApplicationWindow {
	id: root
	minimumWidth: 1080
	minimumHeight: 720
	visible: true
	title: qsTr("OpenP2P")

	property Peer currentPeer: null
	property PeerListModel peers: sessionManager.peers

	Component.onCompleted: timer.start()

	Timer {
		id: timer
		interval: 500
		repeat: false
		onTriggered: function() {
			if (!sessionManager.loadChats()) {
				console.warn("couldn't load peers chats")
			}

			server.tryReconnectAll()
		}
	}

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

			Connections {
				target: server

				// function onRejectedRemotely(peer) {
				// 	for (let i = 0; i < root.peers.count; ++i) {
				// 		if (root.peers.get(i).peer === peer) {
				// 			root.peers.remove(i)
				// 			break
				// 		}
				// 	}
				// }
			}

			ColumnLayout {
				anchors.fill: parent
				spacing: 12

				Text {
					Layout.fillWidth: true
					text: "Friend List"
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
						Item {
							width: ListView.view.width
							height: rowLayout.height + 8  // Add some padding

							MouseArea {
								id: mouseArea
								anchors.fill: parent
								hoverEnabled: true
								cursorShape: Qt.PointingHandCursor
								onClicked: {
									list.currentIndex = index
									currentPeer = peers.at(index)
								}
							}

							Rectangle {
								id: background
								anchors.fill: parent
								color:
										mouseArea.containsMouse ||
										model.peer.name === currentPeer.name ? "#f0f0f0" : "transparent"
								radius: 5
							}

							RowLayout {
								id: rowLayout
								anchors.centerIn: parent
								spacing: 4

								Rectangle {
									width: 10
									height: 10
									radius: 100
									color: model.peer.active ? "green" : "gray"
								}

								Text {
									text: model.peer.name
								}
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
					text: "Your IPv6:"
				}

				Column {
					id: ipList

					property var ipModel: []

					AddrLabel {
						id: addrLabel
						onIpsReceived: (ips) => ipList.ipModel = ips
					}

					RowLayout {
						Repeater {
							model: ipList.ipModel

							TextEdit {
								required property string modelData

								text: modelData
								readOnly: true
								selectByMouse: true
							}
						}

						TextField {
							id: selfPort
							placeholderText: "Default port: 7575"
							Component.onCompleted: text = self.port
						}

						Button {
							text: "Save"
							onClicked: self.port = selfPort.text
						}
					}
				}
			}

			RowLayout {
				id: selfNameSection
				spacing: 12

				Label {
					text: "Your Name:"
				}

				TextField {
					id: selfName
					placeholderText: "Jack Sparrow"
					Component.onCompleted: text = self.name
				}

				Button {
					text: "Save"
					onClicked: self.name = selfName.text
				}
			}

			RowLayout {
				id: peerIpSection

				TextField {
					id: peerIP
					placeholderText: "Peer's IP"
					Layout.fillWidth: true
				}

				TextField {
					id: peerPort
					placeholderText: "7575"
				}

				Button {
					text: "Connect"
					onClicked: function () {
						server.startNewConn(peerIP.text, Number(peerPort.text))
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
