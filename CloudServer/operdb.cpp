#include "operdb.h"
#include <QMessageBox>
#include <QSqlQuery>

operDB::operDB(QObject *parent) : QObject(parent)
{
    myDB = QSqlDatabase::addDatabase("QSQLITE");
    initDB();
}

operDB &operDB::getInstance()
{
    static operDB instance;
    return instance;
}

operDB::~operDB()
{
    myDB.close();
}

void operDB::initDB()
{
    myDB.setHostName("localhost");
    myDB.setDatabaseName("");
    if(myDB.open()){
        QSqlQuery query;
        query.exec("");
        while(query.next()){

        }
    }
    else{
        QMessageBox::critical(NULL, "Open Database", "Failed To Open Database");
    }

}
