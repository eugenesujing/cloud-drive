#include "files.h"
#include "ui_files.h"
#include "cloudclient.h"
#include <QInputDialog>
#include <QMessageBox>

Files::Files(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Files)
{
    ui->setupUi(this);
}

Files::~Files()
{
    deleteListItem();
    delete ui;
}

Files &Files::getInstance()
{
    static Files file;
    return file;
}

void Files::on_newFolderPB_clicked()
{
    QInputDialog* qid = new QInputDialog;
    QString folderName = qid->getText(this, "New folder", "New folder name");
    if(!folderName.isEmpty()){
        if(folderName.size()>32){
            QMessageBox::warning(qid, "New folder", "Folder name must be less than 32 characters");
        }else{
            QString loginName = CloudClient::getInstance().getLoginName();
            QString curPath = CloudClient::getInstance().getCurPath();
            pto* sendPto = makePTO(curPath.size());
            if(sendPto==NULL){
                qDebug()<<"malloc for sendPto failed on on_newFolderPB_clicked()";
                return;
            }
            sendPto->msgType = ENUM_MSG_TYPE_NEW_FOLDER_REQUEST;
            memcpy(sendPto->preData, loginName.toStdString().c_str(),32);
            memcpy(sendPto->preData+32, folderName.toStdString().c_str(),32);
            memcpy(sendPto->data, curPath.toStdString().c_str(), curPath.size());
            CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
            free(sendPto);
            sendPto = NULL;
        }
    }else{
        QMessageBox::warning(qid,  "Empty file name", "File name cannot be empty");
    }
    delete qid;
}

void Files::on_freshPB_clicked()
{
    loadFiles();
}

void Files::on_switch_to_files_widget(int currRow)
{   if(currRow == 1){
        loadFiles();
    }

}

void Files::deleteListItem()
{
    int count = ui->listWidget->count();
    QListWidgetItem* item = NULL;

    for(int i=0; i<count; i++){
        item = ui->listWidget->item(count-i-1);
        ui->listWidget->removeItemWidget(item);
        delete item;
    }
}

void Files::loadFiles()
{
    qDebug()<<"in loadFiles()";
    QString curPath = CloudClient::getInstance().getCurPath();
    qDebug()<<"curPath="<<curPath;
    pto* sendPto = makePTO(curPath.size()+1);
    if(sendPto==NULL){
        qDebug()<<"malloc for sendPto failed on on_newFolderPB_clicked()";
        return;
    }
    qDebug()<<"i'm here";
    memcpy(sendPto->data,curPath.toStdString().c_str(),curPath.size());
    sendPto->msgType = ENUM_MSG_TYPE_LOAD_FOLDER_REQUEST;
    qDebug()<<"sendPto->msgType = "<<sendPto->msgType;
    CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
    qDebug()<<"snedPto->data"<<sendPto->totalSize;
}

void Files::updateFileList(pto *recvPto)
{
    qDebug()<<"in updateFileList";
    if(recvPto == NULL){
        qDebug()<<"recvPto is nullptr";
    }
    //clear all existing listItem and free memory before loading new list
    deleteListItem();
    int fileCount = recvPto->msgSize/(sizeof(fileInfo));
    fileInfo* pInfo = NULL;
    //starting from 2 to ignore './'  and '../'
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

void Files::on_deletePB_clicked()
{
    QListWidgetItem* listItem  = ui->listWidget->currentItem();
    if(listItem == NULL){
        QMessageBox::warning(this, "Delete file", "Please select a file");
    }else{
        QString currFile = listItem->text();
        QString curPath = CloudClient::getInstance().getCurPath();
        pto* sendPto = makePTO(curPath.size()+1);
        if(sendPto==NULL){
            qDebug()<<"malloc for sendPto failed on on_deletePB_clicked";
            return;
        }
        memcpy(sendPto->data,curPath.toStdString().c_str(),curPath.size());
        memcpy(sendPto->preData, currFile.toStdString().c_str(), currFile.size());
        sendPto->msgType = ENUM_MSG_TYPE_DELETE_FILE_REQUEST;
        CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
    }
}
