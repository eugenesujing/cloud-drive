 #include "sharefile.h"
#include "ui_sharefile.h"
#include "home.h"
#include <QCheckBox>
#include <QDebug>

ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);
    friendList = new QWidget;
    ui->scrollArea->setWidget(friendList);

    buttonGroup = new QButtonGroup(friendList);
    buttonGroup->setExclusive(false);

    vLayout = new QVBoxLayout(friendList);
    vLayout->setAlignment(Qt::AlignTop);
    updateFriendList();
}

ShareFile::~ShareFile()
{
    delete ui;
}

void ShareFile::updateFriendList()
{

    QList<QAbstractButton*> buttons = buttonGroup->buttons();

    for(int j=0; j<buttons.size(); j++){
        qDebug()<<"j="<<j;
        QAbstractButton* temp =buttons[j];
        buttonGroup->removeButton(temp);
        vLayout->removeWidget(temp);
        delete temp;
    }

    QStringList friends = Home::getInstance().getFriend()->getFriendList();

    QCheckBox* check = NULL;


    for(int i=0; i<friends.size(); i++){

        check = new QCheckBox(friends[i]);
        QFont font = check->font();
        font.setPointSize(20);
        check->setFont(font);
        vLayout->addWidget(check);

        buttonGroup->addButton(check);

    }



}

void ShareFile::on_selectAll_clicked()
{
    for(auto button:buttonGroup->buttons()){
        button->setChecked(true);
    }

}

void ShareFile::on_clearSelection_clicked()
{
    while(buttonGroup->checkedButton()!= nullptr){
        buttonGroup->checkedButton()->setChecked(false);
    }
}
