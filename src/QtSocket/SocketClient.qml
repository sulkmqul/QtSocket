import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import Treg

//socket client画面
Item {

    id:socketClientScreen

    StackView.onRemoved: {
        socketClientController.disconnectSocket()
    }

    MessageDialog {
        id:infoMessage
        text: ""
    }



    SocketClientController {
        id: socketClientController

        //レスポンスがあったとき
        onRecieveMessage:{
            textResponse.text += "[RCV] " + mes + "\n";
            scrollViewLog.ScrollBar.vertical.position = 1.0 - scrollViewLog.ScrollBar.vertical.size

        }
        //メッセージを送ったとき
        onSendMessage:{
            let t = "(TEXT) "
            if(textflag === false) {
                t = "(BYTE) "
            }
            textResponse.text += "[SND]" + t + mes + "\n";
            scrollViewLog.ScrollBar.vertical.position = 1.0 - scrollViewLog.ScrollBar.vertical.size

        }

        onRaiseError:{
            infoMessage.close()
            infoMessage.text = mes
            infoMessage.open()
        }
    }


    Column{
        id: columnCommand
        anchors.fill: parent
        anchors.margins: 20
        spacing:30

        //Host connect Setting Area
        Item {
            id: hostArea
            height: 30
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20

            TextField{
                id: textfieldHost
                anchors.left: parent.left
                anchors.right: textfieldPort.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                text: "localhost"
            }
            Text {
                anchors.left: textfieldHost.left
                anchors.bottom: textfieldHost.top
                text: qsTr("Host")
            }

            TextField{
                id: textfieldPort
                width: 100
                anchors.right: buttonConnect.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                validator: RegularExpressionValidator{
                    regularExpression:/[0-9]{1,5}/
                }
                text: "5001"
            }
            Text {
                anchors.left: textfieldPort.left
                anchors.bottom: textfieldPort.top
                text: qsTr("Port")
            }

            Button{
                id: buttonConnect
                width: 100
                anchors.right: buttonDisconnect.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                text: "接続"
                enabled: (socketClientController.connectState === 0)

                onClicked: {
                    socketClientController.connectSocket(textfieldHost.text, textfieldPort.text)
                    textResponse.text = ""
                }
            }

            Button{
                id: buttonDisconnect
                width: 100
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                text: "切断"
                enabled: (socketClientController.connectState === 2)
                onClicked: {
                    socketClientController.disconnectSocket()
                }
            }
        }


        // 送信コマンド
        Item {
            id: commandArea
            height: 30
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20

            RadioButton {
                id: radioSendTypeText
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                checked: true
                text: "Text"
                onCheckedChanged: {
                    //textfieldCommand.text = ""
                }
            }
            RadioButton {
                id: radioSendTypeByte
                anchors.left: radioSendTypeText.right
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                checked: false
                text: "byte"
                onCheckedChanged: {
                    //textfieldCommand.text = ""
                }

            }
            Text {
                anchors.left: radioSendTypeText.left
                anchors.bottom: radioSendTypeText.top
                text: qsTr("Send Type")
            }

            TextField{
                id: textfieldCommand
                anchors.left: radioSendTypeByte.right
                anchors.right: buttonSendCommand.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                validator: RegularExpressionValidator{

                    regularExpression: {
                        if(radioSendTypeByte.checked)
                        {
                            // /[a-fA-F0-9]+/
                        }
                        else{
                            //
                        }
                    }
                }
            }
            Text {
                anchors.left: textfieldCommand.left
                anchors.bottom: textfieldCommand.top
                text: qsTr("Command")
            }
            Button{
                id: buttonSendCommand
                anchors.right: parent.right
                anchors.top: commandArea.top
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                width: 150
                text: "送信"
                enabled: (socketClientController.connectState === 2)
                onClicked: {
                    socketClientController.sendCommand(textfieldCommand.text, radioSendTypeText.checked)
                }
            }
        }        


        //結果領域
        Item {
            id: responseArea
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            anchors.topMargin:30
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            height: 400

            Text {
                id:respTitle
                text: qsTr("返答")
            }

            ScrollView{
                id: scrollViewLog
                anchors.top: respTitle.bottom
                anchors.left: responseArea.left
                anchors.right: responseArea.right
                anchors.bottom: responseArea.bottom

                TextArea  {
                    id: textResponse
                    readOnly: true
                    background: Rectangle{
                        color:"lightgray"
                    }
                    wrapMode: Text.WordWrap
                    text: qsTr("")
                }
            }
        }

    }



}
