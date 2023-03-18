#include "cloudserver.h"
#include "operdb.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //CloudServer w;
    //w.show();
    operDB::getInstance().initDB();
    return a.exec();
}
