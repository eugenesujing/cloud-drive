#include "mytcpsocket.h"
#include "protocol.h"
#include "operdb.h"
#include <QDebug>

MyTcpSocket::MyTcpSocket()
{
   connect(this, SIGNAL(readyRead()), this, SLOT(onRecv()));
   connect(this, SIGNAL(disconnected()), this, SLOT(socektOff()));
}

MyTcpSocket &MyTcpSocket::getInstance()
{
    static MyTcpSocket instance;
    return instance;
}

QString MyTcpSocket::getName() const
{
    return socketName;
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
            qDebug()<<ret;
            if(ret==1){
                respondMsg = QString("Your new account <%1> has been registered successfully!\0").arg(name);
            }else if(ret==0){
                respondMsg = QString("User name <%1> has been registered by another user. Please use another name!\0").arg(name);

            }else if(ret==-1){
                respondMsg = QString("Name or password is empty! Cannot handle register request.\0");
                qDebug()<<"Name or password is empty! Cannot handle register request.";
            }
            qDebug()<<respondMsg;
            pto* respPto = makePTO(respondMsg.size());
            if(respPto==NULL){
                qDebug()<<"malloc for respPto failed.";
                break;
            }

            respPto->totalSize = sizeof(pto) + respondMsg.size();
            respPto->msgType = ENUM_MSG_TYPE_REGISTER_RESPOND;
            strcpy(respPto->data, respondMsg.toStdString().c_str());
            respPto->code = ret;
            write((char*)respPto, respPto->totalSize);
            free(respPto);
            respPto = NULL;
            break;
        }
        case ENUM_MSG_TYPE_LOGIN_REQUEST:{
            char name[32] = {' '};
            char pwd[32] = {' '};
            memcpy(name, recvPto->preData, 32);
            memcpy(pwd, recvPto->preData+32, 32);

            QString respondMsg;
            int ret = operDB::getInstance().handleLogin(name,pwd);
            qDebug()<<ret;
            if(ret==1){
                respondMsg = QString("");
                socketName = name;
            }else if(ret==0){
                respondMsg = QString("Account <%1> is already logged in!\0").arg(name);

            }else if(ret==-1){
                respondMsg = QString("Incorrect name or password!\0");
            }
            qDebug()<<respondMsg;
            pto* respPto = makePTO(respondMsg.size());
            if(respPto==NULL){
                qDebug()<<"malloc for respPto failed.";
                break;
            }

            respPto->totalSize = sizeof(pto) + respondMsg.size();
            respPto->msgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
            strcpy(respPto->data, respondMsg.toStdString().c_str());
            respPto->code = ret;
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

//when the connection between client and server is off, we set account online status to 0 and emit signals for server to delete this socket
void MyTcpSocket::socektOff()
{
    if(!socketName.isEmpty()){
        operDB::getInstance().handleOffline(socketName.toStdString().c_str());
    }
    emit clientOff(this);
}
