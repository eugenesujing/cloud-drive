#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include "protocol.h"

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
signals:
    void clientOff(MyTcpSocket*);
private:
    QString socketName;
    void respond(QString respondMsg, int ret, ENUM_MSG_TYPE type);
    void loadFolder(QString fullPath, bool isOpen = false, QString fileName =NULL);
};

#endif // MYTCPSOCKET_H
