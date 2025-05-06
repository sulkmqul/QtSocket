#include "ServerResponseData.h"

ServerResponseData::ServerResponseData(QObject *parent)
    : QObject{parent}
{}

//読み込み
//引数：[0]=0=byte or 1=文字列 [1]=コマンド文字列 [2]=応答文字列
//返り値：なし
void ServerResponseData::Load(QStringList slist)
{
    int i = 0;

    //コマンドタイプ
    this->CommandType = slist[i].toInt();
    i++;
    //時間(ms)
    this->TimeMs = slist[i].toInt();
    i++;
    //Byte列として扱う
    if(this->CommandType == 0)
    {
        this->CommandByte = QByteArray::fromHex(slist[i].toUtf8());
        i++;
        this->ResponseByte = QByteArray::fromHex(slist[i].toUtf8());
        i++;
    }
    else {
        //文字列として扱う
        this->CommandByte = slist[i].toUtf8().data();
        i++;
        this->ResponseByte = slist[i].toUtf8().data();
        i++;
    }

    //データタイプ
    this->DataType = ECommandDataType::Response;
    if(this->TimeMs > 0 && this->CommandByte.length() <= 0)
    {
        this->DataType = ECommandDataType::AutoTimer;
    }

}

//コマンド確認
//引数：確認コマンド
//返り値：一致=true
bool ServerResponseData::CheckResponse(QByteArray command)
{
    //最後のNULLを無視してもよいかも

    if(this->CommandByte == command)
    {
        return true;
    }

    return false;
}

//応答コピー
//引数：コマンド、コマンド返答
//返り値：true=返答データあり
void ServerResponseData::CopyResponse(QByteArray *buf)
{
    *buf = this->ResponseByte;
}
