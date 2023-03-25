#include "online.h"
#include "ui_online.h"
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
