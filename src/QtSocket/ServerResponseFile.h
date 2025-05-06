#ifndef SERVERRESPONSEFILE_H
#define SERVERRESPONSEFILE_H

#include "ServerResponseData.h"
#include <QObject>



class ServerResponseFile : public QObject
{
    Q_OBJECT
public:
    ServerResponseFile();

    //ファイル読み込み
    bool LoadFile(QString filepath, QList<std::shared_ptr<ServerResponseData>> *alist);
};

#endif // SERVERRESPONSEFILE_H
