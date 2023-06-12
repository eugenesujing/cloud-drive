#include "files.h"
#include "ui_files.h"
#include "cloudclient.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

Files::Files(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Files)
{
    ui->setupUi(this);
    sFile = NULL;
    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(double_clicked(QModelIndex)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(uploadBegin()));
}

Files::~Files()
{
    deleteListItem();
    delete sFile;
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

            QString curPath = CloudClient::getInstance().getCurPath();
            pto* sendPto = makePTO(curPath.size());
            if(sendPto==NULL){
                qDebug()<<"malloc for sendPto failed on on_newFolderPB_clicked()";
                return;
            }
            sendPto->msgType = ENUM_MSG_TYPE_NEW_FOLDER_REQUEST;

            memcpy(sendPto->preData, folderName.toStdString().c_str(),32);
            memcpy(sendPto->data, curPath.toStdString().c_str(), curPath.size());
            sendPto->code = 0;
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

void Files::switch_to_files_widget(int currRow)
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
    QString curPath = CloudClient::getInstance().getCurPath();
    qDebug()<<"curPath="<<curPath;
    pto* sendPto = makePTO(curPath.size()+1);
    if(sendPto==NULL){
        qDebug()<<"malloc for sendPto failed on on_newFolderPB_clicked()";
        return;
    }
    memcpy(sendPto->data,curPath.toStdString().c_str(),curPath.size());
    sendPto->msgType = ENUM_MSG_TYPE_LOAD_FOLDER_REQUEST;
    sendPto->code = 0; // code 0 represents file system, 1 represents savefile system
    CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
    free(sendPto);
    sendPto = NULL;
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

bool Files::writeDownloadFile(qint64 fileUploadSoFar, qint64 fileTotalSize)
{
    QByteArray buffer =CloudClient::getInstance().getSocket().readAll();
    QString respondMsg;
    downloadFile.write(buffer);

    fileUploadSoFar += buffer.size();

    //qDebug()<<"currUploadCount="<<currUploadCount;
    //qDebug()<<"fileUploadSoFar="<<fileUploadSoFar;

    if(fileUploadSoFar==fileTotalSize){
        downloadFile.close();
        respondMsg = QString("Successfully downloaded file %1").arg(fileNameDownload);
        QMessageBox::information(this, "Download File", respondMsg);
        fileNameDownload.clear();
        return true;
    }else if (fileUploadSoFar>fileTotalSize){
        downloadFile.close();

        respondMsg = QString("Error in downloading file %1. Please try again").arg(fileNameDownload);
        QMessageBox::warning(this, "Download File", respondMsg);
        fileNameDownload.clear();
        return true;
    }

    return false;
}

void Files::hideShareFile()
{
    sFile->hide();
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
        free(sendPto);
        sendPto = NULL;
    }
}

void Files::on_renamePB_clicked()
{
    QListWidgetItem* listItem  = ui->listWidget->currentItem();
    if(listItem == NULL){
        QMessageBox::warning(this, "Rename file", "Please select a file");
    }else{
        QString currFile = listItem->text();
        QString curPath = CloudClient::getInstance().getCurPath();
        QString newFileName = QInputDialog::getText(this, "Rename file", "New Filename:");
        if(newFileName.size()>32){
            QMessageBox::warning(this, "Rename file", "File name must be less than 32 characters");
        }
        pto* sendPto = makePTO(curPath.size()+1);
        if(sendPto==NULL){
            qDebug()<<"malloc for sendPto failed on on_renamePB_clicked()";
            return;
        }
        memcpy(sendPto->data,curPath.toStdString().c_str(),curPath.size());
        memcpy(sendPto->preData, currFile.toStdString().c_str(), currFile.size());
        memcpy(sendPto->preData +32, newFileName.toStdString().c_str(), newFileName.size());
        sendPto->msgType = ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
        CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
        free(sendPto);
        sendPto = NULL;
    }
}

void Files::double_clicked(const QModelIndex& index)
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
    sendPto->code = 0;
    CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
    free(sendPto);
    sendPto = NULL;

}

void Files::on_backPB_clicked()
{
    QString curPath = CloudClient::getInstance().getCurPath();
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
        sendPto->code =0;
        CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);
        CloudClient::getInstance().setCurPath(curPath);
        free(sendPto);
        sendPto = NULL;

    }
}

void Files::on_uploadPB_clicked()
{
    filePathUpload = QFileDialog::getOpenFileName();
    if(filePathUpload.isEmpty()){
        QMessageBox::warning(this, "Upload File", "File name cannot be empty.");
    }else{
        int index = filePathUpload.lastIndexOf('/');
        QString fileName = filePathUpload.right(filePathUpload.size()-index-1);
        if(fileName.size()>32){
            QMessageBox::warning(this, "Upload File", "File name cannot be greater than 32 characters.");
        }else{
            QFile file(filePathUpload);
            long long fileSize = file.size();
            QString curPath = CloudClient::getInstance().getCurPath();
            pto* sendPto = makePTO(curPath.size()+1);
            sendPto->msgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
            memcpy(sendPto->preData, fileName.toStdString().c_str(),32);
            QString fileSizeString = QString("%1").arg(fileSize);
            memcpy(sendPto->preData+32, fileSizeString.toStdString().c_str(),32);
            memcpy(sendPto->data, curPath.toStdString().c_str(),curPath.size());
            CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);

            qDebug()<<"fileSize="<<fileSize;
            qDebug()<<"fileName="<<fileName;
            timer.start(1000);
            free(sendPto);
            sendPto = NULL;
        }
    }

}

void Files::uploadBegin()
{
    timer.stop();
    QFile file(filePathUpload);
    if(file.open(QIODevice::ReadOnly)){
        char* buffer= new char[4096];
        memset(buffer, 0, 4096);
        qint64 readTotal = 0;

        while(true){
            readTotal = file.read(buffer, 4096);
            if(readTotal>0 && readTotal<=4096){
                CloudClient::getInstance().getSocket().write(buffer,readTotal);
                qDebug()<<"readTotal="<<readTotal;
            }else if(readTotal==0){
                break;
            }else{
                QMessageBox::warning(this, "Upload File", "Failed to open file. Please try again.");
                break;
            }

        }
        qDebug()<<"Finish uploading file";
        file.close();
        delete[] buffer;
    }else{
        QMessageBox::warning(this, "Upload File", "Failed to open file. Please try again.");
    }

}

void Files::on_downloadPB_clicked()
{
    QListWidgetItem* listItem  = ui->listWidget->currentItem();
    if(listItem == NULL){
        QMessageBox::warning(this, "Download file", "Please select a file.");
    }else{
        QString filePathDownload = QFileDialog::getSaveFileName();
        if(filePathDownload.isEmpty()){
            QMessageBox::warning(this, "Download File", "File name cannot be empty.");
            return;
        }

        QString curPath = CloudClient::getInstance().getCurPath();
        fileNameDownload.clear();
        fileNameDownload = listItem->text();

        downloadFile.setFileName(filePathDownload);
        if(!downloadFile.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this, "Download File", QString("Failed to download %1").arg(fileNameDownload));
            return;
        }

        pto* sendPto = makePTO(curPath.size()+1);
        sendPto->msgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
        memcpy(sendPto->preData, fileNameDownload.toStdString().c_str(), 32);
        memcpy(sendPto->data, curPath.toStdString().c_str(), curPath.size());
        CloudClient::getInstance().getSocket().write((char*)sendPto, sendPto->totalSize);

        free(sendPto);
        sendPto = NULL;
    }
}

void Files::on_sharePB_clicked()
{
    QListWidgetItem* listItem  = ui->listWidget->currentItem();
    sFile = new ShareFile;
    sFile->setFileName(listItem->text());
    sFile->show();
}
