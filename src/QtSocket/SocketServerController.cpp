#include "SocketServerController.h"

//最大接続数
const int MAX_CONNECTION = 1;

SocketServerController::SocketServerController(QObject *parent)
    : QObject(parent), ListenState(0)
{
    this->RespProvider.reset(new ServerResponseProvider());

    connect(&this->RespTimer, &QTimer::timeout, this, &SocketServerController::timeSpand);
    this->RespTimer.start(100);

    //タイマー開始
    this->Stopwatch.start();
}


//サーバー開始
//引数：ホスト、ポート
//返り値：なし
void SocketServerController::StartListen(QString host, int port)
{
    if(this->Server != NULL)
    {
        emit raiseError("開いています");
        return;
    }
    //作成
    this->Server.reset(new QTcpServer(this));

    //signal connect
    connect(this->Server.get(), &QTcpServer::newConnection, this, &SocketServerController::NewConnect);

    //listen開始
    //bool f = this->Server->listen(QHostAddress(host), port);
    bool f = this->Server->listen(QHostAddress::LocalHost, port);
    if(f == false)
    {
        emit raiseError("listen失敗");
        return;
    }

    //接続設定
    this->ListenState = 1;
    emit listenStateChanged();

    //ログ出力
    this->WriteServerLog("Listen開始");
    this->WriteServerLog("------------------------------------------------------------------");
}

//サーバー終了
//引数：なし
//返り値：なし
void SocketServerController::CloseServer()
{
    if(this->Server == NULL)
    {
        emit raiseError("閉じています");
        return;
    }

    //開かれているクライアントを閉じて終了する。
    for(QTcpSocket *soc : this->ClientList)
    {
        soc->close();
    }
    this->ClientList.clear();

    //サーバー終了
    this->Server->close();
    this->Server.reset();

    //ステータス終了
    this->ListenState = 0;
    emit listenStateChanged();

    this->WriteServerLog("------------------------------------------------------------------");
    this->WriteServerLog("Server終了");
}

//サーバー応答ファイルの読み込み
//引数：読み込みファイル
//返り値：成功可否
bool SocketServerController::LoadServerResponseFile(QString filepath)
{
    //qmlからわたるfiledialogの選択ファイルの余計なものがつくので消す
    filepath = filepath.remove("file:///");

    //ファイル読み込み
    bool f = this->RespProvider->LoadFile(filepath);
    if( f == false)
    {
        return false;
    }

    this->WriteServerLog("ServerResponseFile読み込み完了");
    return true;



}

//画面ログの書きこみ
//引数：メッセージ
//返り値：なし
void SocketServerController::WriteServerLog(QString mes)
{
    emit writeLogWindow(mes);
}
//クライアントログの書き込み
//引数：メッセージ
//返り値：なし
void SocketServerController::WriteClientLog(QTcpSocket *socket, QString mes)
{
    //クライアント情報を付与
    QString chost = socket->localAddress().toString();
    QString sport = QString::number(socket->localPort());

    QString m = chost + ":" + sport + " " + mes;
    emit writeLogWindow(m);
}

//QTcpServer::newConnectionのslots
//引数：なし
//返り値：なし
void SocketServerController::NewConnect()
{
    //対象の取得
    QTcpSocket *socket = this->Server->nextPendingConnection();

    if(this->ClientList.size()  >= MAX_CONNECTION)
    {
        socket->close();
        this->WriteClientLog(socket, "[ERROR] 同時接続数が最大です、拒否します");
        return;
    }

    //signal接続！
    connect(socket, &QTcpSocket::readyRead, this, &SocketServerController::readyReadSocket);
    connect(socket, &QTcpSocket::disconnected, this, &SocketServerController::disconnectedSocket);

    //追加
    this->ClientList.append(socket);

    QString chost = socket->localAddress().toString();
    QString sport = QString::number(socket->localPort());

    this->WriteServerLog("新規接続受け入れ(" + QString::number(this->ClientList.size()) + "):" + chost + ":" + sport);

}

//クライアントから何か送られてきた
//引数：なし
//返り値：なし
void SocketServerController::readyReadSocket()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(QObject::sender());
    if(socket == NULL)
    {
        qDebug() << "readyReadSocket sender 失敗";
        return;
    }

    QByteArray array = socket->read(1024);
    QString sdata = array;
    this->WriteClientLog(socket, "[RCVE] " + sdata);

    //コマンド解析して応答
    QByteArray resp;
    bool comres = this->RespProvider->FindResponse(array, &resp);
    if(comres == false)
    {
        return;
    }
    this->WriteClientLog(socket, "[RESP] " + sdata);

    //応答ありなら返信する
    socket->write(resp);
}

//クライアント接続解除
//引数：なし
//返り値：なし
void SocketServerController::disconnectedSocket()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(QObject::sender());
    if(socket == NULL)
    {
        qDebug() << "readyReadSocket sender 失敗";
        return;
    }

    bool f = this->ClientList.removeOne(socket);

    this->WriteClientLog(socket, "Disconnect(" + QString::number(this->ClientList.size()) + ")");
}

//タイマー処理
//引数：なし
//返り値：なし
void SocketServerController::timeSpand()
{
    QList<QByteArray> resplist;
    qint64 time = this->Stopwatch.elapsed();
    this->RespProvider->CyclingSend(time, &resplist);

    if(resplist.length() <= 0)
    {
        return;
    }

    //管理クライアントに対して、レスポンスを送信
    for(QTcpSocket *soc : this->ClientList)
    {
        for(int i=0; i<resplist.count(); i++)
        {
            soc->write(resplist[i]);
            QString sdata = resplist[i];
            this->WriteClientLog(soc, "[SEND] " + sdata);
        }
    }

}
