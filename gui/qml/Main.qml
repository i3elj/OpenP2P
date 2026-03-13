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
																color: model.peer.active ? "blue" : "red"
														}

														Text {
																text: model.peer.name
														}

														MouseArea {
																anchors.fill: parent
																hoverEnabled: true
																z: 0
																onEntered: this.cursorShape = Qt.PointingHandCursor
																onClicked: function () {
																		list.currentIndex = index
																		currentPeer = peers.at(index)
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
