import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    Layout.fillHeight: true
    Layout.fillWidth: true
    padding: 8

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
            }

            function addMessage(text) { msgModel.append({"text": text}) }
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
                }
            }

            Button {
                text: "Send"
                Layout.alignment: Qt.AlignTop

                onClicked: function () {
                    if (msgInput.text.length === 0) return
                    chatList.addMessage(msgInput.text)
                    msgInput.clear()
                    chatList.positionViewAtEnd()
                }
            }
        }
    }
}
