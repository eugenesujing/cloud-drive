#include "protocol.h"
#include "cloudclient.h"
#include "ui_cloudclient.h"
#include "home.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QString>

CloudClient::CloudClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CloudClient)
{
    ui->setupUi(this);
    loadConfig();

    connect(&mySocket, SIGNAL(connected()),this, SLOT(showConnected()));
    mySocket.connectToHost(QHostAddress(ip),port);
    connect(&mySocket, SIGNAL(readyRead()), this, SLOT(onRecv()));
}

CloudClient::~CloudClient()
{
    delete ui;
}

CloudClient &CloudClient::getInstance()
{
    static CloudClient instance;
    return instance;
}

QTcpSocket &CloudClient::getSocket()
{
    return mySocket;
}

QString CloudClient::getLoginName() const
{
    return loginName;
}

//Inform user when client is connected to server successfully
void CloudClient::showConnected()
{
    QMessageBox::information(this, "Connect to server", "Connected to server!");
}

//Load config from resource file
void CloudClient::loadConfig()
{
    QFile file(":/config.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll();
        QString strData = data.toStdString().c_str();

        file.close();
        strData.replace("\r\n"," ");

        QStringList strList = strData.split(" ");

        ip = strList[0];
        port = strList[1].toUShort();

    }
    else{
        QMessageBox::critical(this,"Load config", "Fail to load config");
    }
}

void CloudClient::loadFriendList()
{
    pto* sendPto = makePTO(0);
    if(sendPto==NULL){
        qDebug()<<"malloc for sendPto failed on CloudClient::loadFriendList()";
        return;
    }
    sendPto->msgType = ENUM_MSG_TYPE_FRESH_FRIENDLIST_REQUEST;
    mySocket.write((char*)sendPto,sendPto->totalSize);
    free(sendPto);
    sendPto = NULL;

}


/*void CloudClient::on_send_button_clicked()
{
    QString msgToBeSent = ui->lineEdit->text();
    if(!msgToBeSent.isEmpty()){
        pto* newPto = makePTO(msgToBeSent.size());
        if(newPto!=NULL){
            newPto->totalSize = sizeof (pto) + msgToBeSent.size();
            newPto->msgSize = msgToBeSent.size();
            memcpy(newPto->data,msgToBeSent.toStdString().c_str(),msgToBeSent.size());
            qDebug()<<newPto->data;
            mySocket.write((char*)newPto,newPto->totalSize);
            free(newPto);
            newPto = NULL;
        }
    }
}*/

void CloudClient::on_login_button_clicked()
{
    QString nameToBeSent = ui->name_input->text();
    loginName = nameToBeSent;
    QString pwdToBeSent = ui->pwd_input->text();
    if(!nameToBeSent.isEmpty() && !pwdToBeSent.isEmpty()){
        pto* newPto = makePTO(0);
        if(newPto!=NULL){
            newPto->totalSize = sizeof (pto);
            newPto->msgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
            //Since the name and pwd is small enough to be fitted into preData, we will use it to store the data
            memcpy(newPto->preData,nameToBeSent.toStdString().c_str(),nameToBeSent.size());
            memcpy(newPto->preData+32,pwdToBeSent.toStdString().c_str(),pwdToBeSent.size());
            mySocket.write((char*)newPto,newPto->totalSize);
            free(newPto);
            newPto = NULL;
        }else{
            qDebug()<<"malloc for newPto(Register) failed.";
        }
    }else{
        QMessageBox::critical(this,"Login In", "Name or Password is empty. Please reenter.");
    }
}

void CloudClient::on_register_button_clicked()
{
    QString nameToBeSent = ui->name_input->text();
    QString pwdToBeSent = ui->pwd_input->text();
    if(!nameToBeSent.isEmpty() && !pwdToBeSent.isEmpty()){
        pto* newPto = makePTO(0);
        if(newPto!=NULL){
            newPto->totalSize = sizeof (pto);
            newPto->msgType = ENUM_MSG_TYPE_REGISTER_REQUEST;
            //Since the name and pwd is small enough to be fitted into preData, we will use it to store the data
            memcpy(newPto->preData,nameToBeSent.toStdString().c_str(),nameToBeSent.size());
            memcpy(newPto->preData+32,pwdToBeSent.toStdString().c_str(),pwdToBeSent.size());
            mySocket.write((char*)newPto,newPto->totalSize);
            free(newPto);
            newPto = NULL;
        }
    }else{
        QMessageBox::critical(this,"Register New Account", "Name or Password is empty. Please reenter.");
    }
}

void CloudClient::onRecv()
{
    unsigned int ptoSize = 0;
    //get size of pto first
    mySocket.read((char*)&ptoSize, sizeof(unsigned int));
    unsigned int msgSize = ptoSize - sizeof (pto);

    pto* recvPto = makePTO(msgSize);
    if(recvPto!=NULL){
        recvPto->totalSize = ptoSize;
        qDebug()<<"totalSize="<<recvPto->totalSize;
        mySocket.read((char*)recvPto +sizeof(unsigned int), recvPto->totalSize-sizeof (unsigned int));

        //handle user request based on message type
        switch (recvPto->msgType) {
        case ENUM_MSG_TYPE_REGISTER_RESPOND:{
            char* respond = (char*)malloc(msgSize+1);
            memset(respond,0,msgSize+1);
            memcpy(respond,(char*)recvPto->data,msgSize);
            if(recvPto->code==1){
                QMessageBox::information(this, "Register New Account", respond);
            }else{
                QMessageBox::warning(this, "Register New Account", respond);
            }
            free(respond);
            respond = NULL;
            break;
        }
        case ENUM_MSG_TYPE_LOGIN_RESPOND:{

            if(recvPto->code!=1){
                char* respond = (char*)malloc(msgSize+1);
                memset(respond,0,msgSize+1);
                memcpy(respond,(char*)recvPto->data,msgSize);
                QMessageBox::warning(this, "Log In", respond);
                free(respond);
                respond = NULL;
                loginName.clear();
            }else{
                //load friendlist after login
                loadFriendList();
                Home::getInstance().show();
                hide();
                qDebug()<<"Login successfully.";
            }
            break;
        }
        case ENUM_MSG_TYPE_SHOW_ONLINE_RESPOND:{
            Home::getInstance().getFriend()->handleShowOnlineResult(recvPto);

            break;
        }
        case ENUM_MSG_TYPE_SEARCH_USER_RESPOND:{
            char* respond = (char*)malloc(msgSize+1);
            memset(respond,0,msgSize+1);
            memcpy(respond,(char*)recvPto->data,msgSize);
            if(recvPto->code>=0){
                QMessageBox::information(this, "Search User Result", respond);
            }else{
                QMessageBox::warning(this, "Search User Result", respond);
            }
            free(respond);
            respond = NULL;
            break;
        }
        case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:{
            char* respond = (char*)malloc(msgSize+1);
            memset(respond,0,msgSize+1);
            memcpy(respond,(char*)recvPto->data,msgSize);
            if(recvPto->code == -1){
                QMessageBox::warning(this, "Add Friend Request", respond);
            }else{
                QMessageBox::information(this, "Add Friend Request", respond);
            }
            free(respond);
            respond = NULL;
            break;
        }
        case ENUM_MSG_TYPE_ADD_FRIEND_RESEND_REQUEST:{
            //char searchName[32] = {' '};
            char loginName[32] = {' '};
            //memcpy(searchName, recvPto->preData, 32);
            memcpy(loginName, recvPto->preData+32, 32);
            QString req = QString("<%1> would like to connect.").arg(loginName);
            int ret = QMessageBox::information(this, "New Friend Request", req, QMessageBox::Yes, QMessageBox::No);
            pto* respPto = makePTO(0);
            if(respPto==NULL){
                qDebug()<<"malloc for sendPto failed on ENUM_MSG_TYPE_ADD_FRIEND_RESEND_REQUEST";
                return;
            }
            respPto->msgType = ENUM_MSG_TYPE_ADD_FRIEND_RESEND_RESPOND;
            respPto->code = ret;
            memcpy(respPto->preData, recvPto->preData, 64);

            mySocket.write((char*)respPto, respPto->totalSize);
            free(respPto);
            respPto = NULL;
            break;
        }
        case ENUM_MSG_TYPE_FRESH_FRIENDLIST_RESPOND:{
            Home::getInstance().getFriend()->handleLoadFriendList(recvPto);

            break;
        }
        case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND:{
            char* respond = (char*)malloc(msgSize+1);
            memset(respond,0,msgSize+1);
            memcpy(respond,(char*)recvPto->data,msgSize);
            if(recvPto->code != 1){
                QMessageBox::warning(this, "Delete Friend", respond);
            }else{
                QMessageBox::information(this, "Delete Friend", respond);
                //reload friendlist after deletion
                loadFriendList();
            }

            free(respond);
            respond = NULL;
            break;
        }
        default:
            break;
        }
      free(recvPto);
      recvPto = NULL;
    }
}
