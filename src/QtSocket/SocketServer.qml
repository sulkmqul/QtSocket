import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import Treg

//socket server画面
Item {
    id:socketServerScreen

    StackView.onRemoved: {

    }

    FileDialog {
        id: serverResponseFileSelectDialog
        onAccepted: {
            var f = socketServerController.LoadServerResponseFile(serverResponseFileSelectDialog.selectedFile)
            if(f === false){
                infoMessage.text = "読み込み失敗"
                infoMessage.open()
            }

        }
    }

    // 最後のログまでスクロールする
    function scrollToLastLog(){
        scrollViewLog.ScrollBar.vertical.position = 1.0 - scrollViewLog.ScrollBar.vertical.size
    }

    MessageDialog {
        id:infoMessage
        text: ""
    }

    //サーバー管理
    SocketServerController {
        id: socketServerController

        //ログを描くとき
        onWriteLogWindow:{
            textServerLog.text += mes + "\n";
            scrollToLastLog()

        }

        onRaiseError:{
            infoMessage.text = mes
            infoMessage.open()
        }

    }

    // 描画場所
    Column{
        anchors.fill: parent
        anchors.margins: 20
        spacing:20

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
                enabled: false
            }
            Text {
                anchors.left: textfieldHost.left
                anchors.bottom: textfieldHost.top
                text: qsTr("Host")
            }

            TextField{
                id: textfieldPort
                width: 100
                anchors.right: buttonListenStart.left
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
                id: buttonListenStart
                width: 100
                anchors.right: buttonClose.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                text: "Listen Start"
                enabled: (socketServerController.listenState === 0)

                onClicked: {
                    socketServerController.StartListen(textfieldHost.text, textfieldPort.text)                    
                }
            }

            Button{
                id: buttonClose
                width: 100
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                text: "Close"
                enabled: (socketServerController.listenState === 1)
                onClicked: {
                    socketServerController.CloseServer()
                }
            }
        }


        // サーバー制御部
        Item {
            id: configureArea
            height: 100
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            enabled: (socketServerController.listenState === 1)

            readonly property int configSizeW: 120
            readonly property int configSizeH: 35

            Column{
                spacing:5
                anchors.left: parent.left
                anchors.right: parent.right
                Row{
                    spacing: 10
                    Button {
                        id: buttonLoadComanndFile
                        width: configureArea.configSizeW
                        height: configureArea.configSizeH
                        text: "応答ファイル読み込み"
                        onClicked: {
                            serverResponseFileSelectDialog.open()
                        }
                    }
                    Button {
                        id: buttonDisconnectAll
                        width: configureArea.configSizeW
                        height: configureArea.configSizeH
                        text: "切断"
                        onClicked: {

                        }
                    }
                }
                //区切り線
                Rectangle{
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 1
                    color: "#D0D0D0"
                }

                Item {
                    id: commandArea
                    height: 50
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
                        anchors.bottom: buttonSendCommand.bottom
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
                        anchors.right: commandArea.right
                        //anchors.top: commandArea.top
                        anchors.bottom: commandArea.bottom
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        width: 150
                        text: "送信"
                        onClicked: {
                            //socketClientController.sendCommand(textfieldCommand.text, radioSendTypeText.checked)
                        }
                    }
                }


                /*

                //任意コマンド
                Row{
                    spacing: 10
                    Button {
                        id: buttonCommandA
                        width: configureArea.configSizeW
                        height: configureArea.configSizeH
                        text: "Command A"
                        onClicked: {

                        }
                    }
                    Button {
                        id: buttonCommandB
                        width: configureArea.configSizeW
                        height: configureArea.configSizeH
                        text: "Command B"
                        onClicked: {

                        }
                    }
                    Button {
                        id: buttonCommandC
                        width: configureArea.configSizeW
                        height: configureArea.configSizeH
                        text: "Command C"
                        onClicked: {

                        }
                    }
                    Button {
                        id: buttonCommandD
                        width: configureArea.configSizeW
                        height: configureArea.configSizeH
                        text: "Command D"
                        onClicked: {

                        }
                    }
                    Button {
                        id: buttonCommandE
                        width: configureArea.configSizeW
                        height: configureArea.configSizeH
                        text: "Command E"
                        onClicked: {

                        }
                    }
                }*/
            }

        }

        //ログ表示
        Item {
            id: logArea
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            anchors.topMargin:30
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            height: 350

            Text {
                id:respTitle
                text: qsTr("返答")
            }

            ScrollView{
                id: scrollViewLog
                anchors.top: respTitle.bottom
                anchors.left: logArea.left
                anchors.right: logArea.right
                anchors.bottom: logArea.bottom

                TextArea  {
                    id: textServerLog
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

