#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{

}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"incoming connection";
    //generate a new socket for each new incoming connection
    //and this socket will be responsible for the data exchange with the client that just connected
    MyTcpSocket* newSocket = new MyTcpSocket;
    newSocket->setSocketDescriptor(handle);

    socketList.append(newSocket);
}


