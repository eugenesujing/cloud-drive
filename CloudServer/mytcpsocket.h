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
    void socketOff();
    void sendDownloadFile();
signals:
    void clientOff(MyTcpSocket*);
private:
    QString socketName;

    QFile uploadFile;
    long long fileUploadSoFar;
    long long fileTotalSize;
    bool isUploading;
    char uploadFileName[32];
    QTimer downloadTimer;
    char downloadFileName[32];
    QFile fileDownload;

    void respond(QString respondMsg, int ret, ENUM_MSG_TYPE type);
    void loadFolder(QString fullPath, pto* recvPto, bool isOpen = false, QString fileName =NULL);
    void copyFile(QString copyFilePath, QString pasteFilePath);
};

#endif // MYTCPSOCKET_H
