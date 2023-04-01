#include "mytcpsocket.h"
#include "mytcpserver.h"

#include "operdb.h"
#include <QDebug>
#include <QMessageBox>
#include <QDir>

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
    bool recvPtoFreed = false;
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
                QString curPath = QString("./%1").arg(name);
                QDir dir;
                dir.mkdir(curPath);

            }else if(ret==0){
                respondMsg = QString("User name <%1> has been registered by another user. Please use another name!\0").arg(name);

            }else if(ret==-1){
                respondMsg = QString("Name or password is empty! Cannot handle register request.\0");
                qDebug()<<"Name or password is empty! Cannot handle register request.";
            }
            qDebug()<<respondMsg;

            respond(respondMsg, ret,  ENUM_MSG_TYPE_REGISTER_RESPOND);
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
            respond(respondMsg, ret,  ENUM_MSG_TYPE_LOGIN_RESPOND);
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
            respond(respondMsg, ret, ENUM_MSG_TYPE_SEARCH_USER_RESPOND);
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
                MyTcpServer::getInstance().resend(searchName,resendPto);
                respondMsg = QString("Your friend request has been sent to <%1>.").arg(searchName);
            }else if(ret==-1){
                respondMsg = QString("Unknown Error.");
            }else if(ret==0){
                respondMsg = QString("Account <%1> is your friend already.").arg(searchName);
            }else if(ret==3){
                respondMsg = QString("Account <%1> does not exist.").arg(searchName);
            }
            qDebug()<<respondMsg;
            respond(respondMsg, ret, ENUM_MSG_TYPE_ADD_FRIEND_RESPOND);
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
            MyTcpServer::getInstance().resend(loginName, respPto);
            break;
        }
        case ENUM_MSG_TYPE_FRESH_FRIENDLIST_REQUEST:{
            QStringList res = operDB::getInstance().handleFreshFriendList(socketName.toStdString().c_str());
            pto* respPto = makePTO(res.size()*32);
            if(respPto==NULL){
                qDebug()<<"malloc for respPto failed.";
                break;
            }
            respPto->msgType = ENUM_MSG_TYPE_FRESH_FRIENDLIST_RESPOND;
            for(int i=0; i<res.size(); i++){
                memcpy((char*)(respPto->data)+i*32,res[i].toStdString().c_str(), res[i].size());
                qDebug()<<"respPto->data="<<(char*)(respPto->data)+32*i;
            }
            write((char*)respPto,respPto->totalSize);
            free(respPto);
            respPto = NULL;
            break;
        }
        case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:{
            char friendName[32] = {' '};
            char loginName[32] = {' '};
            memcpy(friendName, recvPto->preData, 32);
            memcpy(loginName, recvPto->preData+32, 32);

            int ret = operDB::getInstance().handleDeleteFriend(friendName,loginName);
            QString respondMsg;
            if(ret==1){
                respondMsg = QString("You have successfully deleted <%1>.").arg(friendName);
                QString resendMsg = QString("<%1> has deleted you.").arg(loginName);
                pto* resendPTO = makePTO(resendMsg.size());
                if(resendPTO==NULL){
                    qDebug()<<"malloc for respPto failed.";
                    break;
                }
                resendPTO->msgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
                resendPTO->code = 1;
                memcpy(resendPTO->data, resendMsg.toStdString().c_str(),resendMsg.size());
                MyTcpServer::getInstance().resend(friendName, resendPTO);
            }else{
                respondMsg = QString("System Error. Please try again.");
            }
            respond(respondMsg, ret, ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND);
            break;
        }
        case ENUM_MSG_TYPE_PRIVATE_MESSAGE_REQUEST:{
            char friendName[32] = {""};
            memcpy(friendName, recvPto->preData, 32);

            recvPto->msgType = ENUM_MSG_TYPE_PRIVATE_MESSAGE_RESPOND;
            MyTcpServer::getInstance().resend(friendName, recvPto);
            recvPtoFreed = true;

            break;
        }
        case ENUM_MSG_TYPE_BROADCAST_REQUEST:{
            recvPto->msgType = ENUM_MSG_TYPE_BROADCAST_RESPOND;
            MyTcpServer::getInstance().broadcast(recvPto);
            recvPtoFreed = true;
            break;
        }
        case ENUM_MSG_TYPE_NEW_FOLDER_REQUEST:{
            char loginName[32] = {""};
            char newFolderName[32] = {""};
            memcpy(loginName, recvPto->preData, 32);
            memcpy(newFolderName, recvPto->preData+32, 32);
            char* curPath = (char*)malloc(recvPto->msgSize+1);
            memset(curPath, 0, recvPto->msgSize+1);
            memcpy(curPath, recvPto->data, recvPto->msgSize);
            QDir dir(curPath);

            QString respondMsg;
            int ret = 0;
            if(dir.exists()){
                if(dir.exists(newFolderName)){
                    respondMsg = QString("This destination already contains a folder called '%1'.").arg(newFolderName);
                }else{
                    if(dir.mkdir(newFolderName)){
                        respondMsg = QString("Folder '%1' has been created successfuly.").arg(newFolderName);
                        ret = 1;
                    }else{
                        respondMsg = QString("Failed to create folder '%1'. Please try again.").arg(newFolderName);
                        ret = 2;
                    }
                }
            }else{
                ret = -1;
                respondMsg = QString("System cannot find '%1'. Please try again.").arg(curPath);
            }
            respond(respondMsg, ret, ENUM_MSG_TYPE_NEW_FOLDER_RESPOND);
            free(curPath);
            curPath = NULL;
            break;
        }
        default:
            break;
        }

    }
    if(recvPtoFreed == false){
        free(recvPto);
        recvPto = NULL;
    }

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

void MyTcpSocket::respond(QString respondMsg, int ret, ENUM_MSG_TYPE type)
{
    pto* respPto = makePTO(respondMsg.size());
    if(respPto==NULL){
        qDebug()<<"malloc for respPto failed.";
        return;
    }
    qDebug()<<respondMsg;
    respPto->msgType = type;
    respPto->code = ret;
    memcpy(respPto->data, respondMsg.toStdString().c_str(),respondMsg.size());
    write((char*)respPto,respPto->totalSize);
    free(respPto);
    respPto = NULL;
}
