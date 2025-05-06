#ifndef SOCKETSERVERCONTROLLER_H
#define SOCKETSERVERCONTROLLER_H

#include <QObject>

#include<QCoreApplication>
#include<QTcpServer>
#include<QTcpSocket>
#include<QDebug>
#include<QTimer>
#include"ServerResponseProvider.h"

class SocketServerController : public QObject
{
    Q_OBJECT

public:
    explicit SocketServerController(QObject *parent = nullptr);

    Q_PROPERTY(int listenState MEMBER ListenState  NOTIFY listenStateChanged FINAL)

    //サーバー開始
    Q_INVOKABLE void StartListen(QString host, int port);

    //サーバー終了
    Q_INVOKABLE void CloseServer();


    //サーバーコマンドファイルの読み込み
    Q_INVOKABLE bool LoadServerResponseFile(QString filepath);

signals:

    //ログ出力
    void writeLogWindow(QString mes);

    //エラー送信
    void raiseError(QString mes);

    //サーバー状態の変更
    void listenStateChanged();

private:

    //サーバー状態 0=open 1=open
    int ListenState;

    //サーバー
    std::unique_ptr<QTcpServer> Server;

    //管理クライアント一覧
    QList<QTcpSocket *> ClientList;

    //サーバーコマンド管理
    std::unique_ptr<ServerResponseProvider> RespProvider;

    QTimer RespTimer;
    QElapsedTimer Stopwatch;

    //接続管理
    void NewConnect();
    //画面ログの書きこみ
    void WriteServerLog(QString mes);
    //画面ログ書き込み
    void WriteClientLog(QTcpSocket *socket, QString mes);

private slots:


    void readyReadSocket();
    void disconnectedSocket();

    void timeSpand();
};

#endif // SOCKETSERVERCONTROLLER_H
