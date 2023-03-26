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
//ui->onlineFriend = new Online;
    /*QVBoxLayout* mainVBL = new QVBoxLayout;
    mainVBL->addLayout(ui->topHBL);
    mainVBL->addLayout(ui->msgHBL);

    mainVBL->addWidget(ui->onlineFriend);


    setLayout(mainVBL);*/
    ui->onlineFriend->hide();
    connect(ui->onlineButton, SIGNAL(clicked(bool)),this,SLOT(showOnline()));
}

Friend::~Friend()
{
    delete ui;
}

void Friend::handleShowOnlineResult(pto *recvPto)
{
    if(recvPto == NULL) return;
    ui->onlineFriend->showOnlineResult(recvPto);
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
            sendPTO->totalSize = sizeof(pto);
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
