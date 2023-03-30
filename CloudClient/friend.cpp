#include "protocol.h"
#include "friend.h"
#include "ui_friend.h"
#include "cloudclient.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{

    ui->setupUi(this);

    ui->onlineFriend->hide();
    chatWidgets.clear();
    connect(ui->onlineButton, SIGNAL(clicked(bool)),this,SLOT(showOnline()));
}

Friend::~Friend()
{
    delete ui;
    for(int i=0; i<chatWidgets.size(); i++){
        delete chatWidgets[i];
    }
}

void Friend::handleShowOnlineResult(pto *recvPto)
{
    if(recvPto == NULL) return;
    ui->onlineFriend->showOnlineResult(recvPto);
}

void Friend::handleLoadFriendList(pto *recvPto)
{
    if(recvPto == NULL) return;
    int n = recvPto->msgSize/32;
    char temp[32] = {""};
    ui->friendList->clear();
    for(int i=0; i <n; i++){
        memcpy(temp, (char*)(recvPto->data)+i*32, 32);
        ui->friendList->addItem(temp);
    }
}

void Friend::showOnline()
{
    if(ui->onlineFriend->isHidden()){
        ui->onlineFriend->show();
    }else{
        ui->onlineFriend->hide();
    }
}

void Friend::on_onlineButton_clicked()
{
    pto* sendPTO = makePTO(0);
    if(sendPTO==NULL){
        qDebug()<<"makePTO failed for onlineButton";
    }else{
        sendPTO->msgType = ENUM_MSG_TYPE_SHOW_ONLINE_REQUEST;
        sendPTO->totalSize = sizeof(pto);
        sendPTO->msgSize = 0;
        CloudClient::getInstance().getSocket().write((char*)sendPTO, sendPTO->totalSize);
        qDebug()<<sendPTO->data<<"send online";
        free(sendPTO);
        sendPTO = NULL;
    }
}

void Friend::on_searchButton_clicked()
{
    QString name = QInputDialog::getText(this, "Search User", "User Name:");
    if(!name.isEmpty()){
        searchUserName = name;
        pto* sendPTO = makePTO(0);
        if(sendPTO){
            sendPTO->msgType = ENUM_MSG_TYPE_SEARCH_USER_REQUEST;
            memcpy(sendPTO->preData,name.toStdString().c_str(),32);
            qDebug()<<"search name = "<<sendPTO->preData;
            CloudClient::getInstance().getSocket().write((char*)sendPTO, sendPTO->totalSize);
            free(sendPTO);
            sendPTO = NULL;
        }else{
            qDebug()<<"malloc for searchButton failed";
        }
    }else{
        QMessageBox::warning(this, "Search User", "User name cannot be empty!");
    }

}

void Friend::on_refreshButton_clicked()
{
    CloudClient::getInstance().loadFriendList();
}

void Friend::on_deleteButton_clicked()
{
    if(ui->friendList->currentItem()){
        QString nameToBeDeleted = ui->friendList->currentItem()->text();
        pto* sendPTO = makePTO(0);
        if(sendPTO){
            sendPTO->msgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
            memcpy(sendPTO->preData,nameToBeDeleted.toStdString().c_str(),32);
            memcpy(sendPTO->preData +32,CloudClient::getInstance().getLoginName().toStdString().c_str(),32);
            CloudClient::getInstance().getSocket().write((char*)sendPTO, sendPTO->totalSize);
            free(sendPTO);
            sendPTO = NULL;
        }else{
            qDebug()<<"malloc for searchButton failed";
        }
    }else{
        QMessageBox::warning(this, "Delete Friend", "Please select a friend.");
    }
}

void Friend::on_messageButton_clicked()
{
    if(ui->friendList->currentItem()){
        QString friendName = ui->friendList->currentItem()->text();
        //check if there's a chat widget for this friend already or not
        for(int i=0; i<chatWidgets.size(); i++){
            if(chatWidgets[i]->getFriendName()==friendName){
                if(chatWidgets[i]->isHidden()){
                    chatWidgets[i]->show();
                    qDebug()<<"show chatWidget for"<<friendName;
                }else{
                    chatWidgets[i]->raise();
                    qDebug()<<"raise chatWidget for"<<friendName;
                }
                return;
            }
        }
        PrivateMessage* newPM = new PrivateMessage();
        newPM->init(CloudClient::getInstance().getLoginName(),friendName);
        chatWidgets.append(newPM);
        qDebug()<<"Successfully create a window PM for"<<friendName;
        newPM->show();
    }
    else{
        QMessageBox::warning(this, "Private Message", "Please select a friend.");
    }

}
