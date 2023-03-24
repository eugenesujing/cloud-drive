#include "protocol.h"
#include "friend.h"
#include "ui_friend.h"
#include "cloudclient.h"
#include <QVBoxLayout>
#include <QDebug>


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
        CloudClient::getInstance().getSocket().write((char*)sendPTO);
        free(sendPTO);
        sendPTO = NULL;
    }
}
