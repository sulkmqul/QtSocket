#include "ServerResponseFile.h"
#include <QFile>
#include <QTextStream>

ServerResponseFile::ServerResponseFile() {}


//ファイル読み込み
//引数：読み込みファイルパス、出力データ
//返り値：成功可否
bool ServerResponseFile::LoadFile(QString filepath, QList<std::shared_ptr<ServerResponseData>> *alist)
{
    QFile fp(filepath);
    //開く
    bool f = fp.open(QIODevice::ReadOnly | QIODevice::Text);
    if(f == false)
    {
        return false;
    }

    //StremOpen
    QTextStream rst(&fp);


    while(rst.atEnd() == false)
    {
        QString sline = rst.readLine();
        QStringList slist = sline.split(",");

        //指定の要素数ではない
        if( slist.count() != 4){
            continue;
        }

        //ここでデータ解析
        std::shared_ptr<ServerResponseData> data = std::make_shared<ServerResponseData>();
        data->Load(slist);

        alist->append(data);
    }

    fp.close();

    return true;
}
