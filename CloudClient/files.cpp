#include "files.h"
#include "ui_files.h"
#include  "protocol.h"
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
    delete ui;
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
