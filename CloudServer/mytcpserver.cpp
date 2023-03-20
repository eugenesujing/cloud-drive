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
    connect(newSocket,SIGNAL(clientOff(MyTcpSocket*)),this,SLOT(freeSocket(MyTcpSocket*)));
    socketList.append(newSocket);
}

void MyTcpServer::freeSocket(MyTcpSocket *mySocket)
{
    QList<MyTcpSocket*>::iterator iter = socketList.begin();
    for(;iter != socketList.end(); iter++){
        if(*iter == mySocket){
            delete *iter;
            socketList.erase(iter);
        }
    }
    //display all sockets' user name
    for(iter=socketList.begin();iter!=socketList.end();iter++){
        qDebug()<<(*iter)->getName();
    }
    if(socketList.isEmpty()){
        qDebug()<<"socketlist is empty";
    }
}


