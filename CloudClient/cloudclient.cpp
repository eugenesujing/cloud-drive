#include "protocol.h"
#include "cloudclient.h"
#include "ui_cloudclient.h"
#include "home.h"

#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QString>
#include <QDir>
#include <QFileDialog>

CloudClient::CloudClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CloudClient)
{
    ui->setupUi(this);
    loadConfig();
    isDownloading = false;

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

QString CloudClient::getCurPath() const
{
    return curPath;
}

void CloudClient::setCurPath(QString newPath)
{
    curPath = newPath;
}

//Send a request to the server and share the file 'fileName' with friends in 'checkedFriendsName'
void CloudClient::startShareFile(QStringList checkedFriendsName, QString fileName)
{
    if(checkedFriendsName.size()==0){
        QMessageBox::warning(this, "Share File", "Please select a friend.");
        return;
    }
    int numberOfFriends = checkedFriendsName.size();
    QString fileFullPath = curPath+'/'+fileName;
    int msgLen = numberOfFriends*32 + fileFullPath.size() +1;
    pto* newPto =makePTO(msgLen);
    newPto->msgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    memcpy(newPto->preData, loginName.toStdString().c_str(), 32);
    sprintf(newPto->preData+32, "%d", numberOfFriends);
    for(int i=0; i<numberOfFriends; i++){
        memcpy(newPto->data +32*i, checkedFriendsName[i].toStdString().c_str(), 32);
    }
    memcpy(newPto->data +32*numberOfFriends, fileFullPath.toStdString().c_str(), fileFullPath.size());
    mySocket.write((char*)newPto, newPto->totalSize);

}


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
    qDebug()<<"Received a new message";
    if(isDownloading){
        bool ret = Home::getInstance().getFiles()->writeDownloadFile(fileUploadSoFar, fileTotalSize);
        if(ret){
            isDownloading = false;
        }
    }else{
        unsigned int ptoSize = 0;
        //get size of pto first
        mySocket.read((char*)&ptoSize, sizeof(unsigned int));
        unsigned int msgSize = ptoSize - sizeof (pto);

        pto* recvPto = makePTO(msgSize);
        if(recvPto!=NULL){
            recvPto->totalSize = ptoSize;
            qDebug()<<"totalSize="<<recvPto->totalSize;
            mySocket.read((char*)recvPto +sizeof(unsigned int), recvPto->totalSize-sizeof (unsigned int));
            qDebug()<<"msgType = "<<recvPto->msgType;
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
                    //set curPath as user's personal folder
                    curPath = QString("./%1").arg(loginName);
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
            case ENUM_MSG_TYPE_PRIVATE_MESSAGE_RESPOND:{
                char friendName[32] = {""};
                char* msg = (char*)malloc(recvPto->msgSize+1);
                memset(msg, 0, recvPto->msgSize+1);
                memcpy(msg, recvPto->data, recvPto->msgSize);
                memcpy(friendName, recvPto->preData+32, 32);
                qDebug()<<"friendName="<<friendName<<" message: "<<QString(msg)<<endl;
                Home::getInstance().getFriend()->newPrivateMessgae(friendName, msg);
                break;
            }
            case ENUM_MSG_TYPE_BROADCAST_RESPOND:{
                char friendName[32] = {""};
                char* msg = (char*)malloc(recvPto->msgSize+1);
                memset(msg, 0, recvPto->msgSize+1);
                memcpy(msg, recvPto->data, recvPto->msgSize);
                memcpy(friendName, recvPto->preData, 32);

                Home::getInstance().getFriend()->newBroadcastMessgae(friendName, msg);
                break;
            }
            case ENUM_MSG_TYPE_NEW_FOLDER_RESPOND:{
                char* respond = (char*)malloc(msgSize+1);
                memset(respond,0,msgSize+1);
                memcpy(respond,(char*)recvPto->data,msgSize);
                if(recvPto->code == 0){
                    QMessageBox::warning(this, "New Folder", respond);
                }else{
                    QMessageBox::information(this, "New Folder", respond);
                    if(recvPto->code == 1){
                        Home::getInstance().getFiles()->loadFiles();
                    }else if(recvPto->code == 2){
                        Home::getInstance().getSavaFile()->loadFolder();
                    }

                }
                free(respond);
                respond = NULL;
                break;
            }
            case ENUM_MSG_TYPE_LOAD_FOLDER_RESPOND:{
                qDebug()<<"load folder code = "<<recvPto->code;
                if(recvPto->code == 0){
                    Home::getInstance().getFiles()->updateFileList(recvPto);
                }else if(recvPto->code == 1){
                    Home::getInstance().getSavaFile()->updateFileList(recvPto);
                }

                break;
            }
            case ENUM_MSG_TYPE_DELETE_FILE_RESPOND:{
                char* respond = (char*)malloc(msgSize+1);
                memset(respond,0,msgSize+1);
                memcpy(respond,(char*)recvPto->data,msgSize);
                if(recvPto->code != 1){
                    QMessageBox::warning(this, "Delete File", respond);
                }else{
                    QMessageBox::information(this, "Delete File", respond);
                    Home::getInstance().getFiles()->loadFiles();
                }
                free(respond);
                respond = NULL;
                break;
            }
            case ENUM_MSG_TYPE_RENAME_FILE_RESPOND:{
                char* respond = (char*)malloc(msgSize+1);
                memset(respond,0,msgSize+1);
                memcpy(respond,(char*)recvPto->data,msgSize);
                if(recvPto->code != 1){
                    QMessageBox::warning(this, "Rename File", respond);
                }else{
                    QMessageBox::information(this, "Rename File", respond);
                    Home::getInstance().getFiles()->loadFiles();
                }
                free(respond);
                respond = NULL;
                break;
            }
            case ENUM_MSG_TYPE_OPEN_FILE_RESPOND:{
                char fileName[32] = {""};
                memcpy(fileName, recvPto->preData, 32);
                if(recvPto->code==0){

                    curPath = QString("%1/%2").arg(curPath).arg(fileName);
                    Home::getInstance().getFiles()->updateFileList(recvPto);


                }else if(recvPto->code == 1){
                    Home::getInstance().getSavaFile()->setCurPath(fileName);
                    Home::getInstance().getSavaFile()->updateFileList(recvPto);
                }

                break;
            }
            case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND:{
                char* respond = (char*)malloc(msgSize+1);
                memset(respond,0,msgSize+1);
                memcpy(respond,(char*)recvPto->data,msgSize);
                if(recvPto->code != 1){
                    QMessageBox::warning(this, "Upload File", respond);
                }else{
                    QMessageBox::information(this, "Upload File", respond);
                    Home::getInstance().getFiles()->loadFiles();
                }
                free(respond);
                respond = NULL;
                break;
            }
            case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND:{
                //if there is an error msg
                if(msgSize!=0){
                    char* respond = (char*)malloc(msgSize+1);
                    memset(respond,0,msgSize+1);
                    memcpy(respond,(char*)recvPto->data,msgSize);
                    QMessageBox::warning(this, "Download File", respond);
                    free(respond);
                    respond = NULL;
                }else{
                    qint64 fileSize = 0;
                    sscanf(recvPto->preData, "%lld",&fileSize);
                    fileUploadSoFar = 0;
                    fileTotalSize = fileSize;
                    isDownloading = true;
                    qDebug()<<"fileTotalSize="<<fileTotalSize;
                }
                break;

            }
            case ENUM_MSG_TYPE_SHARE_FILE_RESEND_REQUEST:{
                char sender[32] = {""};
                int numberOfReceivers = 0;
                memcpy(sender, recvPto->preData, 32);
                sscanf(recvPto->preData +32, "%d", &numberOfReceivers);

                QString filePath(recvPto->data+ 32*numberOfReceivers);

                int lastIndex = filePath.lastIndexOf('/');
                QString fileName = filePath.right(filePath.size()-lastIndex-1);
                int ret = QMessageBox::information(this, "Share File Request", QString("%1 would like to share file <%2>.").arg(sender).arg(fileName), QMessageBox::Yes, QMessageBox::No);
                if(ret==QMessageBox::Yes){
                    Home::getInstance().getSavaFile()->init(sender,filePath, fileName);
                    Home::getInstance().getSavaFile()->show();
                }else{
                    qDebug()<<"ret == no";
                    QString respondMsg = QString("%1 refused your share file request.").arg(loginName);
                    pto* respondPto = makePTO(respondMsg.size()+1);
                    if(respondPto==NULL){
                        qDebug()<<"malloc for sendPto failed on ENUM_MSG_TYPE_SHARE_FILE_RESEND_REQUEST";
                        return;
                    }
                    respondPto->msgType = ENUM_MSG_TYPE_SHARE_FILE_RESEND_RESPOND;
                    memcpy(respondPto->preData, sender, 32);
                    memcpy(respondPto->data,respondMsg.toStdString().c_str(), respondMsg.size());
                    respondPto->code = 0;
                    mySocket.write((char*)respondPto, respondPto->totalSize);
                }
                break;
            }
            case ENUM_MSG_TYPE_SHARE_FILE_RESPOND:{
                char* respond = (char*)malloc(msgSize+1);
                memset(respond,0,msgSize+1);
                memcpy(respond,(char*)recvPto->data,msgSize);
                QMessageBox::information(this, "Share File Request", respond);
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

}
