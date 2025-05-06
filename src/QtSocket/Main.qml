import QtQuick
import QtQuick.Controls

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Socket Connect")



    //HEADER
    Rectangle{
        id: mainHeader
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 50
        color: "gray"

        Button{
            id: buttonPrevPage
            text: "戻る"
            font.pointSize: 20
            anchors.right: parent.right
            anchors.top: parent.top
            width: 120
            height: 50
            visible: stackViewMain.depth > 1
            onClicked:{
                stackViewMain.pop()

            }
        }
    }

    StackView{
        id: stackViewMain
        anchors.top: mainHeader.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        initialItem: "MainMenu.qml"
        //initialItem: "SocketClient.qml"
        //initialItem: "SocketServer.qml"
    }

}
