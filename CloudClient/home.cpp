#include "home.h"
#include <QStackedWidget>
#include <QHBoxLayout>

Home::Home(QWidget *parent) : QWidget(parent)
{
    wlist = new QListWidget(this);
    wlist->addItem("Friends");
    wlist->addItem("Resource");

    friendWidget = new Friend;
    resourceWidget = new Resource;
    QStackedWidget* sWidget = new QStackedWidget;
    sWidget->addWidget(friendWidget);
    sWidget->addWidget(resourceWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(wlist,1);
    mainLayout->addWidget(sWidget,5);

    setLayout(mainLayout);

    connect(wlist, SIGNAL(currentRowChanged(int)), sWidget, SLOT(setCurrentIndex(int)));
}

Home &Home::getInstance()
{
    static Home instance;
    return instance;
}

Friend *Home::getFriend() const
{
    return friendWidget;
}
