#include "ServerResponseProvider.h"
#include"ServerResponseFile.h"
ServerResponseProvider::ServerResponseProvider(QObject *parent)
    : QObject{parent}
{}

//ファイル読み込み
//引数：読み込みファイルパス
//返り値：成功可否
bool ServerResponseProvider::LoadFile(QString filepath)
{    
    ServerResponseFile fp;

    this->ResponseCommandList.clear();
    this->CyclingCommandList.clear();

    //読み込み
    bool f = fp.LoadFile(filepath, &this->CommandList);
    if(f == false)
    {
        return false;
    }


    //コマンドを割り振る
    for(std::shared_ptr<ServerResponseData> data: this->CommandList)
    {
        if(data->GetDataType() == ECommandDataType::AutoTimer)
        {
            TimerData td;
            td.Timer = 0;
            td.RespData = data;
            this->CyclingCommandList.append(td);
        }
        else {
            this->ResponseCommandList.append(data);
        }
    }

    return true;
}


//レスポンスの割り出し
//引数：コマンド、コマンド返答
//返り値：true=返答データあり
bool ServerResponseProvider::FindResponse(QByteArray com, QByteArray *resp)
{
    for(std::shared_ptr<ServerResponseData> data: this->ResponseCommandList)
    {
        bool f = data->CheckResponse(com);
        if(f == true)
        {
            data->CopyResponse(resp);
            return true;
        }
    }

    return false;
}


//定期送信
//引数：今の時間、今回送信すべき値
//返り値：なし
void ServerResponseProvider::CyclingSend(qint64 timels, QList<QByteArray> *alist)
{
    alist->clear();

    for(int i=0; i<this->CyclingCommandList.count(); i++){
        TimerData *tdata = &this->CyclingCommandList[i];
        qint64 ms = timels - tdata->Timer;

        //時間を超えていた
        if(ms > tdata->RespData->GetTimeMs() )
        {
            tdata->Timer = timels;

            //返答コピー
            QByteArray buf;
            tdata->RespData->CopyResponse(&buf);
            alist->append(buf);
        }
    }

}

