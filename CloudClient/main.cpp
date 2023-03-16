#include "cloudclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CloudClient w;
    w.show();
    return a.exec();
}
