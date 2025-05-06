import QtQuick
import QtQuick.Controls

Item {
    width: 600
    height: 200
    anchors.centerIn: parent

    Row{

        anchors.fill: parent
        anchors.margins: 10

        // 左側のセクション
        Rectangle {
            width: parent.width / 2
            height: parent.height

            Button {
                id: buttonClient
                text: "Client"
                anchors.centerIn: parent
                anchors.margins: 10
                anchors.fill: parent
                font.pointSize: 30
                onClicked: {
                    stackViewMain.push("SocketClient.qml")
                }

            }
        }

        // 右側のセクション
        Rectangle {
            width: parent.width / 2
            height: parent.height

            Button {
                id: buttonServer
                text: "Server"
                anchors.centerIn: parent
                anchors.margins: 10
                anchors.fill: parent
                font.pointSize: 30
                onClicked: {
                    stackViewMain.push("SocketServer.qml")
                }
            }
        }
    }
}
