#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{
    socketList.clear();
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
    qDebug()<<"newSocket="<<newSocket;
    newSocket->setSocketDescriptor(handle);
    connect(newSocket,SIGNAL(clientOff(MyTcpSocket*)),this,SLOT(freeSocket(MyTcpSocket*)));
    socketList.append(newSocket);
}

void MyTcpServer::resendAddFriendRequest(const char *searchName, pto *sendPTO)
{
    if(searchName == NULL || sendPTO == NULL){
        return;
    }
    QList<MyTcpSocket*>::iterator iter = socketList.begin();
    for(;iter != socketList.end(); iter++){
        if((*iter)->getName() == searchName){
            (*iter)->write((char*)sendPTO,sendPTO->totalSize);
            break;
        }
    }
    free(sendPTO);
    sendPTO = NULL;
}

void MyTcpServer::resendAddFriendResendRespond(const char *loginName, pto *sendPTO)
{
    if(loginName == NULL || sendPTO == NULL){
        return;
    }
    QList<MyTcpSocket*>::iterator iter = socketList.begin();
    for(;iter != socketList.end(); iter++){
        if((*iter)->getName() == loginName){
            (*iter)->write((char*)sendPTO,sendPTO->totalSize);
            break;
        }
    }
    free(sendPTO);
    sendPTO = NULL;
}

void MyTcpServer::freeSocket(MyTcpSocket *mySocket)
{
    qDebug()<<"freeSocket()1";
    if(mySocket == NULL){
        return;
    }
    QList<MyTcpSocket*>::iterator iter = socketList.begin();
    for(;iter != socketList.end(); iter++){
        if(*iter == mySocket){
            //in case MyTcpSocket::socketOff() hasn't completed yet, we use deleteLater() to prevent server crash
            (*iter)->deleteLater();
            socketList.erase(iter);

            break;
        }
    }
    qDebug()<<"freeSocket()2";
    /*//display all sockets' user name
    for(iter=socketList.begin();iter!=socketList.end();iter++){
        qDebug()<<(*iter)->getName();
    }
    if(socketList.isEmpty()){
        qDebug()<<"socketlist is empty";
    }*/
}


