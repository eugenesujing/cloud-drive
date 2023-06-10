#include "savefile.h"
#include "ui_savefile.h"
#include "cloudclient.h"
#include <QDebug>


SaveFile::SaveFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveFile)
{
    ui->setupUi(this);
    curPath = QString("./%1").arg(CloudClient::getInstance().getLoginName());
    loadFolder();

    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(on_double_clicked(QModelIndex)));
}

SaveFile::~SaveFile()
{
    deleteListItem();
    delete ui;
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

void SaveFile::on_double_clicked(const QModelIndex &index)
{
    QString fileName = index.data().toString();
    QString curPath = CloudClient::getInstance().getCurPath();
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
    free(sendPto);
    sendPto = NULL;
}
