#include "operdb.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
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
    myDB.setDatabaseName("C:\\Users\\eugen\\Documents\\CloudServer\\cloud.db");
    if(myDB.open()){
        QSqlQuery query;
        query.exec("select * from UserInfo");
        while(query.next()){

            QString data = QString("%1, %2, %3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
            qDebug()<<data;
        }
    }
    else{
        QMessageBox::critical(NULL, "Open Database", "Failed To Open Database");
    }

}

int operDB::handleRegister(const char *name, const char *pwd)
{
    if(name == NULL || pwd == NULL){
        return -1;
    }
    QString toBeExec = QString("insert into UserInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    QSqlQuery query;
    return query.exec(toBeExec);
}
