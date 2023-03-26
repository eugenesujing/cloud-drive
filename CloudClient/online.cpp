#include "online.h"
#include "ui_online.h"
#include "protocol.h"
#include "cloudclient.h"
#include <QDebug>
Online::Online(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Online)
{
    ui->setupUi(this);
}

Online::~Online()
{
    delete ui;
}

void Online::showOnlineResult(pto *recvPto)
{
    if(recvPto == NULL) return;
    int n = recvPto->msgSize /32;
    qDebug()<<"n="<<n;
    char temp[32];
    for(int i=0; i<n; i++){
        memcpy(temp, (char*)(recvPto->data) + i*32, 32);
        ui->onlineWidget->addItem(temp);
        qDebug()<<temp;
    }
}

void Online::on_pushButton_clicked()
{
    QString name = ui->onlineWidget->currentItem()->text();
    pto* sendPTO = makePTO(0);
    QString loginName = CloudClient::getInstance().getLoginName();
    if(sendPTO){
        sendPTO->totalSize = sizeof(pto);
        sendPTO->msgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
        memcpy(sendPTO->preData,name.toStdString().c_str(),32);
        memcpy(sendPTO->preData+32,loginName.toStdString().c_str(),32);
        CloudClient::getInstance().getSocket().write((char*)sendPTO, sendPTO->totalSize);
        free(sendPTO);
        sendPTO = NULL;
    }else{
        qDebug()<<"malloc for sendPTO failed on add friend";
    }
}
