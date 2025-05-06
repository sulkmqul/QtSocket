#ifndef SERVERRESPONSEPROVIDER_H
#define SERVERRESPONSEPROVIDER_H

#include <QObject>
#include "ServerResponseData.h"

typedef struct _TimerData {
    //コマンド
    std::shared_ptr<ServerResponseData> RespData;
    //最後に送った時間
    qint64 Timer;
} TimerData;

//サーバー管理
class ServerResponseProvider : public QObject
{
    Q_OBJECT



public:
    explicit ServerResponseProvider(QObject *parent = nullptr);

    //ファイル読み込み
    bool LoadFile(QString filepath);

    //レスポンスの割り出し
    bool FindResponse(QByteArray com, QByteArray *resp);

    //定期送信
    void CyclingSend(qint64 timels, QList<QByteArray> *alist);

signals:


private:

    //全コマンド
    QList<std::shared_ptr<ServerResponseData>> CommandList;

    //レンスポスコマンド
    QList<std::shared_ptr<ServerResponseData>> ResponseCommandList;
    //タイマーサイクリングコマンド
    QList<TimerData> CyclingCommandList;

};

#endif // SERVERRESPONSEPROVIDER_H
