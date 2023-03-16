#include "cloudserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CloudServer w;
    w.show();
    return a.exec();
}
