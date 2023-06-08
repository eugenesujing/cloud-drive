#include "sharefile.h"
#include "ui_sharefile.h"
#include "home.h"
#include <QCheckBox>
#include <QDebug>

ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    qDebug()<<"here";
    ui->setupUi(this);
    friendList = new QWidget;
    ui->scrollArea->setWidget(friendList);
    qDebug()<<"here";
    updateFriendList();
    qDebug()<<"here";
    buttonGroup = new QButtonGroup(friendList);
    buttonGroup->setExclusive(false);
    qDebug()<<"here";
}

ShareFile::~ShareFile()
{
    delete ui;
}

void ShareFile::updateFriendList()
{
    /*qDebug()<<"inside";
    QList<QAbstractButton*> buttons = friendItems->buttons();
    qDebug()<<"inside";
    for(int j=0; j<buttons.size(); j++){
        qDebug()<<"j="<<j;
        QAbstractButton* temp =buttons[j];
        buttonGroup->removeButton(temp);
        delete temp;
    }*/
    //qDebug()<<buttonGroup;
    qDebug()<<"inside";
    QStringList friends = Home::getInstance().getFriend()->getFriendList();
    qDebug()<<"inside";
    QCheckBox* check = NULL;
    QVBoxLayout *vLayout = new QVBoxLayout(friendList);
    //qDebug()<<buttonGroup;
    for(int i=0; i<friends.size(); i++){
        qDebug()<<"i="<<i;
        check = new QCheckBox(friends[i]);
        vLayout->addWidget(check);
        qDebug()<<"i="<<i;
        buttonGroup->addButton(check);
        qDebug()<<"i="<<i;
    }
     qDebug()<<"inside";


}
