#include "mytcpsocket.h"
#include "protocol.h"
#include "operdb.h"
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
        //handle user request based on message type
        switch (recvPto->msgType) {
        case ENUM_MSG_TYPE_REGISTER_REQUEST:{
            char name[32] = {' '};
            char pwd[32] = {' '};
            memcpy(name, recvPto->preData, 32);
            memcpy(pwd, recvPto->preData+32, 32);

            QString respondMsg;
            int ret = operDB::getInstance().handleRegister(name,pwd);
            if(ret==1){
                respondMsg = QString("Your new account <%1> has been registered successfully!").arg(name);
            }else if(ret==0){
                respondMsg = QString("User name <%1> has been registered by another user. Please use another name!").arg(name);

            }else if(ret==-1){
                respondMsg = QString("Name or password is empty! Cannot handle register request.");
                qDebug()<<"Name or password is empty! Cannot handle register request.";
            }

            pto* respPto = makePTO(respondMsg.size());
            if(respPto==NULL){
                qDebug()<<"malloc for respPto failed.";
                break;
            }

            respPto->totalSize = sizeof(pto) + respondMsg.size();
            respPto->msgType = ENUM_MSG_TYPE_REGISTER_RESPOND;
            strcpy(respPto->data, respondMsg.toStdString().c_str());
            if(ret==1){
                respPto->code = 1;
            }else{
                respPto->code = 0;
            }
            write((char*)respPto, respPto->totalSize);
            free(respPto);
            respPto = NULL;
            break;
        }
        default:
            break;
        }

    }
    free(recvPto);
    recvPto = NULL;
}
