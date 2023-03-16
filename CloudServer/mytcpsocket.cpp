#include "mytcpsocket.h"
#include "protocol.h"
#include <QDebug>

MyTcpSocket::MyTcpSocket()
{
   connect(this, SIGNAL(readyRead()), this, SLOT(onRecv()));
}

MyTcpSocket &MyTcpSocket::getInstance()
{
    static MyTcpSocket instance;
    return instance;
}

void MyTcpSocket::onRecv()
{
    unsigned int ptoSize = 0;
    this->read((char*)&ptoSize, sizeof(unsigned int));
    unsigned int msgSize = ptoSize - sizeof (pto);

    pto* recvPto = makePTO(msgSize);
    if(recvPto!=NULL){
        recvPto->totalSize = ptoSize;
        this->read((char*)recvPto +sizeof(unsigned int), ptoSize-sizeof (unsigned int));
        qDebug()<<recvPto->data;
    }
}
