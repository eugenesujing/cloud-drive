#include "mytcpsocket.h"
#include "mytcpserver.h"
#include "protocol.h"
#include "operdb.h"
#include <QDebug>
#include <QMessageBox>

MyTcpSocket::MyTcpSocket()
{
   connect(this, SIGNAL(readyRead()), this, SLOT(onRecv()));
   connect(this, SIGNAL(disconnected()), this, SLOT(socektOff()));
}

MyTcpSocket::~MyTcpSocket()
{
    close();
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
        qDebug()<<"msgType = "<<recvPto->msgType;
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

            //respPto->totalSize = sizeof(pto) + respondMsg.size();
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
            qDebug()<<"Successfully makePTO for login request";
            //respPto->totalSize = sizeof(pto) + respondMsg.size();
            respPto->msgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
            qDebug()<<"Successfully makePTO for login request";
            strcpy(respPto->data, respondMsg.toStdString().c_str());
            qDebug()<<"Successfully makePTO for login request";
            respPto->code = ret;
            qDebug()<<"respPto->totoalSize = "<<respPto->totalSize;
            //write((char*)respPto, respPto->totalSize);
            qDebug()<<"Successfully sent msg for login request";
            free(respPto);
            respPto = NULL;
            break;
        }
        case ENUM_MSG_TYPE_SHOW_ONLINE_REQUEST:{
            qDebug()<<"SHOW_ONLINE";
            QStringList res = operDB::getInstance().handleShowOnline(socketName.toStdString().c_str());
            qDebug()<<"res.size="<<res.size();

            pto* respPto = makePTO(32*res.size());
            //respPto->msgSize = 32*res.size();
            respPto->msgType = ENUM_MSG_TYPE_SHOW_ONLINE_RESPOND;
            //respPto->totalSize = respPto->msgSize + sizeof(pto);
            for(int i=0; i<res.size(); i++){
                memcpy((char*)(respPto->data) + 32*i,res.at(i).toStdString().c_str(), res.at(i).size());
                qDebug()<<"respPto->data="<<(char*)(respPto->data)+32*i;
            }
            write((char*)respPto, respPto->totalSize);

            free(respPto);
            respPto = NULL;
            break;
        }
        case ENUM_MSG_TYPE_SEARCH_USER_REQUEST:{
            char name[32] = {' '};
            memcpy(name, recvPto->preData, 32);

            QString respondMsg;
            int ret = operDB::getInstance().handleSearchUser(name);
            qDebug()<<ret;
            if(ret==1){
                respondMsg = QString("Account <%1> is offline!").arg(name);
            }else if(ret==2){
                respondMsg = QString("Account <%1> is online!").arg(name);

            }else if(ret==-1){
                respondMsg = QString("Invalid or empty username!");
            }else if(ret==0){
                respondMsg = QString("Account <%1> does not exist!").arg(name);
            }
            qDebug()<<respondMsg;
            pto* respPto = makePTO(respondMsg.size());
            if(respPto==NULL){
                qDebug()<<"malloc for respPto failed.";
                break;
            }

            //respPto->totalSize = sizeof(pto) + respondMsg.size();
            respPto->msgType = ENUM_MSG_TYPE_SEARCH_USER_RESPOND;
            strcpy(respPto->data, respondMsg.toStdString().c_str());
            respPto->code = ret;
            write((char*)respPto, respPto->totalSize);
            free(respPto);
            respPto = NULL;
            break;
        }
        case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
            char searchName[32] = {' '};
            char loginName[32] = {' '};
            memcpy(searchName, recvPto->preData, 32);
            memcpy(loginName, recvPto->preData+32, 32);

            int ret = operDB::getInstance().handleAddFriend(searchName,loginName);
            QString respondMsg;
            qDebug()<<ret;
            if(ret==1){
                respondMsg = QString("Account <%1> is offline.").arg(searchName);
            }else if(ret==2){
                pto* resendPto = makePTO(0);
                memcpy(resendPto,recvPto,sizeof (pto));
                resendPto->msgType = ENUM_MSG_TYPE_ADD_FRIEND_RESEND_REQUEST;
                MyTcpServer::getInstance().resendAddFriendRequest(searchName,resendPto);
                respondMsg = QString("Your friend request has been sent to <%1>.").arg(searchName);
            }else if(ret==-1){
                respondMsg = QString("Unknown Error.");
            }else if(ret==0){
                respondMsg = QString("Account <%1> is your friend already.").arg(searchName);
            }else if(ret==3){
                respondMsg = QString("Account <%1> does not exist.").arg(searchName);
            }
            qDebug()<<respondMsg;
            pto* respPto = makePTO(respondMsg.size());
            if(respPto==NULL){
                qDebug()<<"malloc for respPto failed.";
                break;
            }

            //respPto->totalSize = sizeof(pto) + respondMsg.size();
            respPto->msgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respPto->data, respondMsg.toStdString().c_str());
            respPto->code = ret;
            write((char*)respPto, respPto->totalSize);
            free(respPto);
            respPto = NULL;
            break;
        }
        case ENUM_MSG_TYPE_ADD_FRIEND_RESEND_RESPOND:{
            char searchName[32] = {' '};
            char loginName[32] = {' '};
            memcpy(searchName, recvPto->preData, 32);
            memcpy(loginName, recvPto->preData+32, 32);

            QString respondMsg;
            int ret = 1;
            if(recvPto->code == QMessageBox::Yes){
                ret = operDB::getInstance().handleAddFriendAgree(searchName,loginName);
                if(ret==1){
                    respondMsg = QString("<%1> has accepted your friend request.").arg(searchName);
                }else{
                    respondMsg = QString("System Error. Your friend request to <%1> has failed to be completed.").arg(searchName);
                    ret = -1;
                }
            }else if(recvPto->code == QMessageBox::No){
                respondMsg = QString("<%1> has reclined your friend request.").arg(searchName);
            }

            pto* respPto = makePTO(respondMsg.size());
            if(respPto==NULL){
                qDebug()<<"malloc for respPto failed.";
                break;
            }

            respPto->code = ret;
            respPto->msgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respPto->data, respondMsg.toStdString().c_str());
            MyTcpServer::getInstance().resendAddFriendResendRespond(loginName, respPto);
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
    qDebug()<<"Successfully called handleOffLine()";
    qDebug()<<"SocketName = "<<socketName;
    emit clientOff(this);
    qDebug()<<"Successfully emit clientOff()";
}
