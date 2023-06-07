#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include <QFile>
#include "protocol.h"
#include <QTimer>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ~MyTcpSocket();
    static MyTcpSocket& getInstance();
    QString getName()const;
public slots:
    void onRecv();
    //when the connection between client and server is off, we set account online status to 0 and emit signals for server to delete this socket
    void socektOff();
    void sendDownloadFile();
signals:
    void clientOff(MyTcpSocket*);
private:
    QString socketName;
    void respond(QString respondMsg, int ret, ENUM_MSG_TYPE type);
    void loadFolder(QString fullPath, bool isOpen = false, QString fileName =NULL);
    QFile uploadFile;
    long long fileUploadSoFar;
    long long fileTotalSize;
    bool isUploading;
    char uploadFileName[32];
    int currUploadCount;
    QTimer downloadTimer;
    char downloadFileName[32];
    QFile fileDownload;
};

#endif // MYTCPSOCKET_H
