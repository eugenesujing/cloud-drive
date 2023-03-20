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
    myDB.setDatabaseName("C:\\Users\\eugen\\Documents\\CloudServer\\cloudDB.db");
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

int operDB::handleLogin(const char *name, const char *pwd)
{
    if(name == NULL || pwd == NULL){
        return -1;
    }
    QString toBeExec = QString("select online from UserInfo where name=\'%1\' and pwd=\'%2\'").arg(name).arg(pwd);
    QSqlQuery query;
    qDebug()<<toBeExec;
    query.exec(toBeExec);
    if(query.next()){
        //if the account is not online, we set the online status as online
        if(query.value(0)==0){
            toBeExec = QString("update UserInfo set online=1 where name = \'%1\'").arg(name);
            query.exec(toBeExec);
            return 1;
        }else{
            return 0;
        }
    }else{
        //there is no matching account with the name and pwd provided
        return -1;
    }
}
