#include "home.h"
#include <QStackedWidget>
#include <QHBoxLayout>

Home::Home(QWidget *parent) : QWidget(parent)
{
    wlist = new QListWidget(this);
    wlist->addItem("Friends");
    wlist->addItem("Files");

    friendWidget = new Friend;
    filesWidget = new Files;
    QStackedWidget* sWidget = new QStackedWidget;
    sWidget->addWidget(friendWidget);
    sWidget->addWidget(filesWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(wlist,1);
    mainLayout->addWidget(sWidget,5);

    saveFile = new SaveFile();
    saveFile->hide();

    setLayout(mainLayout);

    connect(wlist, SIGNAL(currentRowChanged(int)), sWidget, SLOT(setCurrentIndex(int)));
    connect(wlist, SIGNAL(currentRowChanged(int)), filesWidget, SLOT(switch_to_files_widget(int)));
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

Files *Home::getFiles() const
{
    return filesWidget;
}

SaveFile *Home::getSavaFile() const
{
    return saveFile;
}
