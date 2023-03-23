#include "friend.h"

#include "ui_friend.h"
#include <QVBoxLayout>


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
