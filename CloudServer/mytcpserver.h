#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QList>

#include "mytcpsocket.h"
#include "protocol.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();

    static MyTcpServer& getInstance();

    void incomingConnection(qintptr handle);
    void resend(const char* name, pto* sendPTO);
public slots:
    void freeSocket(MyTcpSocket* mySocket);
private:
    QList<MyTcpSocket*> socketList;
};

#endif // MYTCPSERVER_H
