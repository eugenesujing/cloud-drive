#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QList>

#include "mytcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();

    static MyTcpServer& getInstance();

    void incomingConnection(qintptr handle);
public slots:
    void freeSocket(MyTcpSocket* mySocket);
private:
    QList<MyTcpSocket*> socketList;
};

#endif // MYTCPSERVER_H
