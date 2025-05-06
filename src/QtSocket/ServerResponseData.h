#ifndef SERVERRESPONSEDATA_H
#define SERVERRESPONSEDATA_H

#include <QObject>

enum ECommandDataType {
    Response,
    AutoTimer,
};

//サーバー返答データ
class ServerResponseData : public QObject
{
    Q_OBJECT
public:
    explicit ServerResponseData(QObject *parent = nullptr);

    //読み込み
    void Load(QStringList slist);

    //設定確認
    bool CheckResponse(QByteArray command);

    //応答コピー
    void CopyResponse(QByteArray *buf);

    //データタイプ取得
    ECommandDataType GetDataType(){ return DataType; };
    int GetTimeMs(){ return TimeMs; };
signals:


private:
    //データタイプ
    ECommandDataType DataType;
    //コマンドタイプ
    int CommandType;
    //応答時間(ms)
    int TimeMs;
    //コマンドByte
    QByteArray CommandByte;
    //レスポンスbyte
    QByteArray ResponseByte;
};

#endif // SERVERRESPONSEDATA_H
