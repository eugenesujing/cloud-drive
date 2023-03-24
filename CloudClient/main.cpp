#include "cloudclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CloudClient::getInstance().show();


    return a.exec();
}
