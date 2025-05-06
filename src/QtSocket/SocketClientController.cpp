#include "SocketClientController.h"


///
/// \brief コンストラクタ
/// \param parent
///
SocketClientController::SocketClientController(QQuickItem *parent) : QQuickItem(parent), ConnectState(0)
{
    Socket.reset();

}


///
/// \brief SocketClientController::connectSocket Socketの接続
/// \param host 接続先
/// \param port 接続先ポート番号
/// \return
///
bool SocketClientController::connectSocket(QString host, int port)
{
    if(ConnectState == 2 || ConnectState == 1)
    {
        qDebug() << "Already Connected";
        return false;
    }

    qDebug() << "Connect Start " << host << ":" << port;
    Socket.reset(new QTcpSocket());

    //接続
    connect(Socket.get(), &QTcpSocket::stateChanged, this, &SocketClientController::stateChangedSocket);
    connect(Socket.get(), &QTcpSocket::readyRead, this, &SocketClientController::readyReadSocket);
    connect(Socket.get(), &QTcpSocket::errorOccurred, this, &SocketClientController::errorOccurred);

    Socket->connectToHost(host, port);

    ConnectState = 1;
    emit connectStateChanged();

    return true;
}

///
/// \brief SocketClientController::disconnectSocket
/// \return
///
bool SocketClientController::disconnectSocket()
{
    if(ConnectState == 0)
    {
        qDebug()<< "already disconnected";
        return false;
    }

    Socket->disconnectFromHost();
    Socket.reset();


    return true;
}

//コマンド送信
//引数：送信メッセージ、テキスト可否(true=テキストで送る false=byte列に変換して送る)
//返り値：成功可否
bool SocketClientController::sendCommand(QString mes, bool textflag)
{
    QByteArray buf = mes.toUtf8().data();
    if(textflag == false)
    {
        buf = QByteArray::fromHex(mes.toUtf8());
        mes = buf.toHex();
    }
    if(buf.length() <= 0)
    {
        emit raiseError("byte変換失敗");
        return false;
    }


    Socket->write(buf);
    emit sendMessage(mes, textflag);

    return true;
}

///
/// \brief SocketClientController::stateChangedSocket 接続状況に変更があったとき
/// \param socketState
///
void SocketClientController::stateChangedSocket(QAbstractSocket::SocketState socketState)
{
    qDebug() << "stateChangedSocket " << socketState;

    //Socketの状態に応じて接続可否を設定しておく
    //接続に成功した
    if(socketState == QAbstractSocket::SocketState::ConnectedState)
    {
        ConnectState = 2;
        emit connectStateChanged();
    }
    //接続に失敗した
    if(socketState == QAbstractSocket::SocketState::UnconnectedState ||
        socketState == QAbstractSocket::SocketState::HostLookupState ||
        socketState == QAbstractSocket::SocketState::ClosingState)
    {
        ConnectState = 0;
        emit connectStateChanged();
    }
}

///
/// \brief SocketClientController::readyReadSocket Socketが読み込めるとき
///
void SocketClientController::readyReadSocket()
{
    QByteArray array = Socket->read(1024);
    QString sdata = array;
    qDebug() << "ready read = " << sdata;

    //受信データの送信
    emit recieveMessage(sdata);
}

///
/// \brief SocketClientController::errorOccurred
/// \param socketError
///
void SocketClientController::errorOccurred(QAbstractSocket::SocketError socketError)
{
    //
    qDebug() << "errorOccurred " << socketError;
    emit raiseError(QMetaEnum::fromType<QAbstractSocket::SocketError>().valueToKey(socketError));
}


