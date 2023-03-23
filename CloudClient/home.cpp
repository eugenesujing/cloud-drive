#include "home.h"


Home::Home(QWidget *parent) : QWidget(parent)
{
    wlist = new QListWidget(this);
    wlist->addItem("Friends");
    wlist->addItem("Resource");


}
