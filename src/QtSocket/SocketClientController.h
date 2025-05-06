#ifndef SOCKETCLIENTCONTROLLER_H
#define SOCKETCLIENTCONTROLLER_H

#include <QObject>
#include <QQuickItem>
#include<QTcpSocket>

class SocketClientController : public QQuickItem
{
    Q_OBJECT
public:
    SocketClientController(QQuickItem *parent = nullptr);

    Q_PROPERTY(int connectState MEMBER ConnectState  NOTIFY connectStateChanged FINAL)

    //接続処理
    Q_INVOKABLE bool connectSocket(QString host, int port);
    //接続解除処理
    Q_INVOKABLE bool disconnectSocket();
    //コマンド送信
    Q_INVOKABLE bool sendCommand(QString mes, bool textflag);


signals:
    void connectStateChanged();

    void sendMessage(QString mes, bool textflag);
    void recieveMessage(QString mes);

    void raiseError(QString mes);


private:

    //接続socket
    std::unique_ptr<QTcpSocket> Socket;
    //接続状態 0=未接続 1=接続中 2=接続済み
    int ConnectState;

private slots:

    void stateChangedSocket(QAbstractSocket::SocketState socketState);
    void readyReadSocket();
    void errorOccurred(QAbstractSocket::SocketError socketError);

};

#endif // SOCKETCLIENTCONTROLLER_H
