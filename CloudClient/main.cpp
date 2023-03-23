#include "cloudclient.h"
#include "home.h"
#include "friend.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CloudClient w;
    //w.show();
    Home h;
    //h.show();
    Friend f;
    f.show();
    return a.exec();
}
