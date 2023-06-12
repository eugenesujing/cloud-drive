#include "savefile.h"
#include "ui_savefile.h"
#include "cloudclient.h"
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>


SaveFile::SaveFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveFile)
{
    ui->setupUi(this);
    curPath = QString("./%1").arg(CloudClient::getInstance().getLoginName());


    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(double_clicked(QModelIndex)));
}

SaveFile::~SaveFile()
{
    deleteListItem();
    delete ui;
}

void SaveFile::init(QString senderName, QString path, QString fileName)
{
    sender = senderName;
    shareFileFullPath = path;
    shareFileName = fileName;
    loadFolder();
}

void SaveFile::deleteListItem()
{
    int count = ui->listWidget->count();
    QListWidgetItem* item = NULL;

    for(int i=0; i<count; i++){
        item = ui->listWidget->item(count-i-1);
        ui->listWidget->removeItemWidget(item);
        delete item;
    }
}

void SaveFile::updateFileList(pto *recvPto)
{
    qDebug()<<"in updateFileList saveFile";
    if(recvPto == NULL){
        qDebug()<<"recvPto is nullptr";
    }
    //clear all existing listItem and free memory before loading new list
    deleteListItem();
    int fileCount = recvPto->msgSize/(sizeof(fileInfo));
    fileInfo* pInfo = NULL;
    //starting from 2 to ignore './'  and '../'
    qDebug()<<"fileCount = "<<fileCount;
    for(int i=2; i<fileCount; i++){
        pInfo = (fileInfo*)(recvPto->data) +i;
        QListWidgetItem* listItem = new QListWidgetItem;
        listItem->setText(pInfo->fileName);
        if(pInfo->fileType==0){
            listItem->setIcon(QIcon(QPixmap(":/fileType/dir.jpeg")));
        }else if(pInfo->fileType==1){
            listItem->setIcon(QIcon(QPixmap(":/fileType/file.jpeg")));
        }
        ui->listWidget->addItem(listItem);
    }
}

void SaveFile::setCurPath(QString fileName)
{
    curPath = QString("%1/%2").arg(curPath).arg(fileName);
}



void SaveFile::double_clicked(const QModelIndex &index)
{
    qDebug()<<"on double clicked";
    QString fileName = index.data().toString();
    pto* sendPto = makePTO(curPath.size()+1);
    if(sendPto==NULL){
        qDebug()<<"malloc for sendPto failed on on_double_clicked";
        return;
    }
    memcpy(sendPto->data,curPath.toStdString().c_str(),curPath.size());
    memcpy(sendPto->preData, fileName.toStdString().c_str(), fileName.size());
    sendPto->msgType = ENUM_MSG_TYPE_OPEN_FILE_REQUEST;
    sendPto->code = 1;
    CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
    free(sendPto);
    sendPto = NULL;
}

void SaveFile::loadFolder()
{
    pto* sendPto = makePTO(curPath.size()+1);
    if(sendPto==NULL){
        qDebug()<<"malloc for sendPto failed on on_newFolderPB_clicked()";
        return;
    }
    memcpy(sendPto->data,curPath.toStdString().c_str(),curPath.size());
    sendPto->msgType = ENUM_MSG_TYPE_LOAD_FOLDER_REQUEST;
    sendPto->code = 1; // code 0 represents file system, 1 represents savefile system
    CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
    qDebug()<<"in loadFolder() saveFile";
    free(sendPto);
    sendPto = NULL;
}

void SaveFile::on_pushButton_2_clicked()
{
    QString root = QString("./%1").arg(CloudClient::getInstance().getLoginName());
    if(curPath != root){
        int index = curPath.lastIndexOf('/');
        curPath.remove(index, curPath.size()-index);
        qDebug()<<"curPath = "<<curPath;
        pto* sendPto = makePTO(curPath.size()+1);
        if(sendPto==NULL){
            qDebug()<<"malloc for sendPto failed on on_double_clicked";
            return;
        }

        memcpy(sendPto->data,curPath.toStdString().c_str(),curPath.size());
        sendPto->msgType = ENUM_MSG_TYPE_LOAD_FOLDER_REQUEST;
        sendPto->code = 1;
        CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
        CloudClient::getInstance().setCurPath(curPath);
        free(sendPto);
        sendPto = NULL;

    }
}

void SaveFile::on_cancel_clicked()
{
    int ret = QMessageBox::information(this,"Share File", "Are you sure you want to cancel this share?", QMessageBox::Yes, QMessageBox::No);
    if(ret==QMessageBox::Yes){
        qDebug()<<"cancel share";
        this->hide();
        QString respondMsg = QString("%1 refused your share file request.").arg(CloudClient::getInstance().getLoginName());
        pto* respondPto = makePTO(respondMsg.size()+1);
        respondPto->msgType = ENUM_MSG_TYPE_SHARE_FILE_RESEND_RESPOND;
        if(respondPto==NULL){
            qDebug()<<"malloc for sendPto failed on on_double_clicked";
            return;
        }
        memcpy(respondPto->preData, sender.toStdString().c_str(), 32);
        memcpy(respondPto->data,respondMsg.toStdString().c_str(), respondMsg.size());
        respondPto->code = 0;
        CloudClient::getInstance().getSocket().write((char*)respondPto, respondPto->totalSize);
    }else{
        this->raise();
    }
}

void SaveFile::on_newFolder_clicked()
{
    QInputDialog* qid = new QInputDialog;
    QString folderName = qid->getText(this, "New folder", "New folder name");
    if(!folderName.isEmpty()){
        if(folderName.size()>32){
            QMessageBox::warning(qid, "New folder", "Folder name must be less than 32 characters");
        }else{

            pto* sendPto = makePTO(curPath.size());
            if(sendPto==NULL){
                qDebug()<<"malloc for sendPto failed on on_newFolderPB_clicked()";
                return;
            }
            sendPto->msgType = ENUM_MSG_TYPE_NEW_FOLDER_REQUEST;
            memcpy(sendPto->preData, folderName.toStdString().c_str(),32);
            memcpy(sendPto->data, curPath.toStdString().c_str(), curPath.size());
            sendPto->code = 1;
            CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
            free(sendPto);
            sendPto = NULL;
        }
    }else{
        QMessageBox::warning(qid,  "Empty file name", "File name cannot be empty");
    }
    delete qid;
}

void SaveFile::on_confirm_clicked()
{
    this->hide();

    QString saveFileFullPath = QString("%1/%2").arg(curPath).arg(shareFileName);
    pto* respondPto = makePTO(saveFileFullPath.size()+shareFileFullPath.size()+2);
    respondPto->msgType = ENUM_MSG_TYPE_SHARE_FILE_RESEND_RESPOND;
    if(respondPto==NULL){
        qDebug()<<"malloc for sendPto failed on on_double_clicked";
        return;
    }
    memcpy(respondPto->preData, sender.toStdString().c_str(), 32);
    sprintf(respondPto->preData+32,"%d", saveFileFullPath.size()+1);
    memcpy(respondPto->data,saveFileFullPath.toStdString().c_str(), saveFileFullPath.size());
    memcpy(respondPto->data+ saveFileFullPath.size()+1,shareFileFullPath.toStdString().c_str(), shareFileFullPath.size());
    respondPto->code = 1;
    CloudClient::getInstance().getSocket().write((char*)respondPto, respondPto->totalSize);
}
