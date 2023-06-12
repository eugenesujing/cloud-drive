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
   connect(&downloadTimer,SIGNAL(timeout()), this, SLOT(sendDownloadFile()));
   isUploading = false;
   fileUploadSoFar = 0;
   fileTotalSize = 0;
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
    qDebug()<<"Received a new message";
    if(isUploading){
        QByteArray buffer = readAll();
        QString respondMsg;
        uploadFile.write(buffer);


        fileUploadSoFar += buffer.size();
        qDebug()<<"fileUploadSoFar="<<fileUploadSoFar;

        int ret = 0;
        if(fileUploadSoFar==fileTotalSize){
            uploadFile.close();
            isUploading = false;
            respondMsg = QString("Successfully uploaded file %1").arg(uploadFileName);
            ret = 1;
            respond(respondMsg, ret,  ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND);
        }else if (fileUploadSoFar>fileTotalSize){
            uploadFile.close();
            isUploading = false;
            respondMsg = QString("Error when uploading file %1. Please try again").arg(uploadFileName);
            respond(respondMsg, ret,  ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND);
        }




    }else{
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

                char newFolderName[32] = {""};

                memcpy(newFolderName, recvPto->preData, 32);
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
                            ret = (recvPto->code==0? 1:2);
                        }else{
                            respondMsg = QString("Failed to create folder '%1'. Please try again.").arg(newFolderName);

                        }
                    }
                }else{

                    respondMsg = QString("System cannot find '%1'. Please try again.").arg(curPath);
                }
                respond(respondMsg, ret, ENUM_MSG_TYPE_NEW_FOLDER_RESPOND);
                free(curPath);
                curPath = NULL;
                break;
            }
            case ENUM_MSG_TYPE_LOAD_FOLDER_REQUEST:{
                char* curPath = new char[recvPto->msgSize];
                memcpy(curPath, recvPto->data, recvPto->msgSize);
                loadFolder(curPath, recvPto->code);
                delete [] curPath;
                break;
            }
            case ENUM_MSG_TYPE_DELETE_FILE_REQUEST:{
                char* curPath = new char[recvPto->msgSize];
                memcpy(curPath, recvPto->data, recvPto->msgSize);
                qDebug()<<"curPath="<<curPath;
                char fileName[32] = {""};
                memcpy(fileName, recvPto->preData, 32);
                QString fullPath = QString("%1/%2").arg(curPath).arg(fileName);

                QFileInfo fileInfo(fullPath);
                int ret = 0;
                if(fileInfo.isDir()){
                    QDir dir(fullPath);
                    ret = dir.removeRecursively();
                }else if(fileInfo.isFile()){
                    QFile file(fullPath);
                    ret = file.remove();
                }
                QString respondMsg;
                if(ret==1){
                    respondMsg = QString("File '%1' has been deleted").arg(fileName);
                }else{
                    respondMsg = QString("Failed to delete file '%1'. Please try again.").arg(fileName);
                }
                respond(respondMsg, ret, ENUM_MSG_TYPE_DELETE_FILE_RESPOND);
                delete[] curPath;
                break;
            }
            case ENUM_MSG_TYPE_RENAME_FILE_REQUEST:{
                char* curPath = new char[recvPto->msgSize];
                memcpy(curPath, recvPto->data, recvPto->msgSize);
                char fileName[32] = {""};
                memcpy(fileName, recvPto->preData, 32);
                char newFileName[32] = {""};
                memcpy(newFileName, recvPto->preData+32, 32);
                QString fullPath = QString("%1/%2").arg(curPath).arg(fileName);
                QString newFullPath = QString("%1/%2").arg(curPath).arg(newFileName);

                QFile file(fullPath);
                QDir dir(curPath);
                int ret = file.rename(newFullPath);
                QString respondMsg;
                if(ret == 1){
                    respondMsg = QString("Rename successfully!");
                }else if(dir.exists(newFileName)){
                    respondMsg = QString("This destination already contains a file called '%1'.").arg(newFileName);
                }
                else{
                    respondMsg = QString("Failed to rename '%1'. Please make sure the file is not currently opened by another application.").arg(fileName);
                }
                respond(respondMsg,ret,ENUM_MSG_TYPE_RENAME_FILE_RESPOND);

                delete[] curPath;
                break;
            }
            case ENUM_MSG_TYPE_OPEN_FILE_REQUEST:{
                char* curPath = new char[recvPto->msgSize];
                memcpy(curPath, recvPto->data, recvPto->msgSize);
                char fileName[32] = {""};
                memcpy(fileName, recvPto->preData, 32);
                QString fullPath = QString("%1/%2").arg(curPath).arg(fileName);
                qDebug()<<"open file fullPath="<<fullPath;
                QFileInfo info(fullPath);
                if(info.isDir()){
                    loadFolder(fullPath, recvPto->code, true, fileName);
                }else if(info.isFile()){
                    //download file
                }
                delete [] curPath;
                break;
            }
            case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST:{
                char* curPath = new char[recvPto->msgSize];
                memcpy(curPath, recvPto->data, recvPto->msgSize);
                memset(uploadFileName,0,32);
                long long fileSize = 0;
                memcpy(uploadFileName,recvPto->preData, 32);
                sscanf(recvPto->preData+32, "%lld",&fileSize);
                QString filePath = QString("%1/%2").arg(curPath).arg(uploadFileName);

                uploadFile.setFileName(filePath);
                QString respondMsg;
                if(uploadFile.open(QIODevice::WriteOnly)){

                   fileTotalSize = fileSize;
                   fileUploadSoFar = 0;
                   isUploading = true;
                   qDebug()<<"FileName ="<<uploadFileName;
                   qDebug()<<"fileTotalSize ="<<fileTotalSize;
                }else{
                    respondMsg = QString("Failed to upload '%1'.").arg(uploadFileName);
                    respond(respondMsg, 0, ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND);
                }

                delete [] curPath;
                break;
            }
            case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST:{
                char* curPath = new char[recvPto->msgSize];
                memcpy(curPath, recvPto->data, recvPto->msgSize);
                memset(downloadFileName,0,32);
                memcpy(downloadFileName, recvPto->preData, 32);
                QString filePathDownload = QString("%1/%2").arg(curPath).arg(downloadFileName);
                QString respondMsg;

                fileDownload.setFileName(filePathDownload);
                if(!fileDownload.open(QIODevice::ReadOnly)){
                    respondMsg = QString("Failed to load file %1").arg(downloadFileName);
                }

                pto* sendPto = makePTO(respondMsg.size());
                sendPto->msgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND;
                //get the fileSize and send it to the client
                QString fileSizeString = QString("%1").arg(fileDownload.size());
                memcpy(sendPto->preData, fileSizeString.toStdString().c_str(),32);
                memcpy(sendPto->data, respondMsg.toStdString().c_str(),respondMsg.size());
                qDebug()<<"fileSize="<<fileSizeString;
                write((char*)sendPto, sendPto->totalSize);
                if(respondMsg.size()==0){
                    downloadTimer.start(1000);
                }

                break;
            }
            case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:{
                char sender[32] = {""};
                int numberOfReceivers = 0;
                memcpy(sender, recvPto->preData, 32);
                qDebug()<<"here";
                sscanf(recvPto->preData +32, "%d", &numberOfReceivers);
                recvPto->msgType = ENUM_MSG_TYPE_SHARE_FILE_RESEND_REQUEST;
                qDebug()<<"here";

                char* shareFileFullPath = new char[recvPto->msgSize-32*numberOfReceivers];
                memcpy(shareFileFullPath,recvPto->data + 32*numberOfReceivers, recvPto->msgSize-32*numberOfReceivers);
                for(int i=0; i<numberOfReceivers; i++){
                    char temp[32] = {""};
                    memcpy(temp,recvPto->data +32*i,32);
                    if(operDB::getInstance().handleSearchUser(temp)==1){
                        QString respondMsg = QString("%1 is not online. Please try again later.").arg(temp);
                        respond(respondMsg, 0, ENUM_MSG_TYPE_SHARE_FILE_RESPOND);
                    }
                    MyTcpServer::getInstance().resend(temp, recvPto, true);
                }
                qDebug()<<"here";
                break;
            }
            case ENUM_MSG_TYPE_SHARE_FILE_RESEND_RESPOND:{
                char sender[32]={""};
                memcpy(sender,recvPto->preData,32);
                if(recvPto->code == 0){
                    recvPto->msgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
                    MyTcpServer::getInstance().resend(sender, recvPto);
                    recvPtoFreed = true;
                }else if(recvPto->code == 1){

                    int savePathSize;
                    sscanf(recvPto->preData+32, "%d", &savePathSize);
                    char* savePath = new char[savePathSize];
                    memcpy(savePath, recvPto->data, savePathSize);
                    char* sharePath = new char[recvPto->msgSize - savePathSize];
                    memcpy(sharePath, recvPto->data+savePathSize, recvPto->msgSize-savePathSize);
                    copyFile(sharePath,savePath);
                    //respond to the sender about the share result
                    QString respondMsg = QString("%1 has accepted your share request.").arg(socketName);
                    pto* respondPTOsender = makePTO(respondMsg.size()+1);
                    if(respondPTOsender==NULL){
                        qDebug()<<"malloc for sendPto failed on ENUM_MSG_TYPE_SHARE_FILE_RESEND_RESPOND";
                        return;
                    }
                    respondPTOsender->code = 1;
                    respondPTOsender->msgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
                    memcpy(respondPTOsender->data, respondMsg.toStdString().c_str(), respondMsg.size());
                    MyTcpServer::getInstance().resend(sender, respondPTOsender);

                    //respond to the receiver the share result
                    respondMsg = QString("File has been copied to %1").arg(savePath);
                    pto* respondPTOreceiver = makePTO(respondMsg.size()+1);
                    if(respondPTOreceiver==NULL){
                        qDebug()<<"malloc for sendPto failed on ENUM_MSG_TYPE_SHARE_FILE_RESEND_RESPOND";
                        return;
                    }
                    respondPTOreceiver->code = 1;
                    respondPTOreceiver->msgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
                    memcpy(respondPTOreceiver->data, respondMsg.toStdString().c_str(), respondMsg.size());
                    write((char*)respondPTOreceiver,respondPTOreceiver->totalSize);
                    free(respondPTOreceiver);
                    respondPTOreceiver = NULL;
                }
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

void MyTcpSocket::sendDownloadFile()
{
    downloadTimer.stop();
    QString respondMsg;

    char* buffer= new char[4096];
    memset(buffer, 0, 4096);
    qint64 readTotal = 0;

    while(true){
        readTotal = fileDownload.read(buffer, 4096);
        if(readTotal>0 && readTotal<=4096){
            write(buffer,readTotal);
            qDebug()<<"readTotal="<<readTotal;
        }else if(readTotal==0){

            break;
        }else{
            qDebug()<<"File download size is greater than file size";
            break;
        }

    }
    qDebug()<<"Finish downloading file";
    fileDownload.close();
    delete[] buffer;
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

//isOpen decides if its a load folder request or open file request
void MyTcpSocket::loadFolder(QString fullPath,int code, bool isOpen, QString fileName)
{
    QDir dir(fullPath);
    QFileInfoList fileInfoList = dir.entryInfoList();
    int fileCount = fileInfoList.size();
    pto* respPto = makePTO(fileCount*sizeof(fileInfo));
    if(respPto == NULL){
        qDebug()<<"malloc failed for ENUM_MSG_TYPE_LOAD_FOLDER_REQUEST";
        return;
    }
    respPto->msgType = ENUM_MSG_TYPE_LOAD_FOLDER_RESPOND;
    fileInfo* pInfo = NULL;
    if(isOpen){
        memcpy(respPto->preData, fileName.toStdString().c_str(), fileName.size());
        respPto->msgType = ENUM_MSG_TYPE_OPEN_FILE_RESPOND;
    }
    for(int i=0; i <fileCount; i++){
        pInfo = (fileInfo*)(respPto->data) +i;
        memcpy(pInfo->fileName,fileInfoList[i].fileName().toStdString().c_str(),fileInfoList[i].fileName().size());
        if(fileInfoList[i].isDir()){
            pInfo->fileType = 0;
        }else if(fileInfoList[i].isFile()){
            pInfo->fileType = 1;
        }
    }
    respPto->code = code;
    qDebug()<<"load folder code = "<<respPto->code;
    write((char*)respPto, respPto->totalSize);
    free(respPto);
    respPto = NULL;
}

void MyTcpSocket::copyFile(QString copyFilePath, QString pasteFilePath)
{
    QFileInfo file(copyFilePath);
    if(file.isDir()){
        QDir dir;
        dir.mkdir(pasteFilePath);

        dir.setPath(copyFilePath);
        for(QString file: dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files)){
           copyFile(QString("%1/%2").arg(copyFilePath).arg(file), QString("%1/%2").arg(pasteFilePath).arg(file));
        }
    }else if(file.isFile()){
        QFile::copy(copyFilePath,pasteFilePath);
    }
}
