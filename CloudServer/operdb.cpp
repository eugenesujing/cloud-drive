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
    myDB.setDatabaseName("C:\\Users\\eugen\\Documents\\cloud drive\\cloud-drive\\CloudServer\\cloudDB.db");
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

void operDB::handleOffline(const char *name)
{
    if(name == NULL){
        qDebug()<<"empty name for handleOffline()";
        return;
    }
    QString toBeExec = QString("update UserInfo set online=0 where name = \'%1\'").arg(name);
    QSqlQuery query;
    qDebug()<<toBeExec;
    query.exec(toBeExec);
    qDebug()<<"Successfully ran handleOffline()";
}

QStringList operDB::handleShowOnline(const char *name)
{
    QStringList res;
    res.clear();
    if(name == NULL){
        qDebug()<<"empty name for handleShowOnline()";
        return res;
    }
    QString toBeExec = QString("select name from UserInfo where online = 1 and name != \'%1\'").arg(name);
    QSqlQuery query;
    qDebug()<<toBeExec;
    query.exec(toBeExec);
    while(query.next()){
        res.append(query.value(0).toString());
    }
    return res;
}

int operDB::handleSearchUser(const char *name)
{
    if(name == NULL){
        return -1;
    }
    QString toBeExec = QString("select online from UserInfo where name=\'%1\'").arg(name);
    QSqlQuery query;
    qDebug()<<toBeExec;
    query.exec(toBeExec);
    if(query.next()){
        //the user being searched is offline
        if(query.value(0)==0){
            return 1;
        }else{
            //the user being searched is online
            return 2;
        }
    }else{
        //there is no matching account with the name and pwd provided
        return 0;
    }
}

int operDB::handleAddFriend(const char *searchName, const char *loginName)
{
    if(searchName == NULL || loginName == NULL){
        return -1;
    }
    QString toBeExec = QString("select * from Friend where (id=(select id from UserInfo where name =\'%1\') and friendId=( select id from UserInfo where name =\'%2\')) or(id=(select id from UserInfo where name =\'%3\') and friendId=( select id from UserInfo where name =\'%4\'))").arg(loginName).arg(searchName).arg(searchName).arg(loginName);
    QSqlQuery query;
    qDebug()<<toBeExec;
    query.exec(toBeExec);
    if(query.next()){
        return 0;//they are friends already
    }else{
        //they haven't been friends yet
        QString toBeExec = QString("select online from UserInfo where name=\'%1\'").arg(searchName);
        QSqlQuery query;
        qDebug()<<toBeExec;
        query.exec(toBeExec);
        if(query.next()){
            //the user being searched is offline
            if(query.value(0)==0){
                return 1;
            }else{
                //the user being searched is online
                return 2;
            }
        }else{
            //there is no matching account with the name and pwd provided
            return 3;
        }
    }
}

int operDB::handleAddFriendAgree(const char *searchName, const char *loginName)
{
    if(searchName == NULL || loginName == NULL){
        return -1;
    }
    QString toBeExec = QString("insert into Friend values((select id from UserInfo where name =\'%1\'),(select id from UserInfo where name =\'%2\'))").arg(searchName).arg(loginName);
    QSqlQuery query;
    qDebug()<<toBeExec;
    return query.exec(toBeExec);
}

QStringList operDB::handleFreshFriendList(const char *name)
{
    QStringList res;
    if(name == NULL){
        return res;
    }
    QString toBeExec = QString("select name from userinfo where id in (select friendid from friend where id = (select id from userinfo where name = \'%1\')) or id in (select id from friend where friendId = (select id from userinfo where name = \'%1\'))").arg(name);
    QSqlQuery query;
    qDebug()<<toBeExec;
    query.exec(toBeExec);
    while(query.next()){
        res.append(query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }
    return res;
}

int operDB::handleDeleteFriend(const char *friendName, const char *loginName)
{
    if(friendName == NULL || loginName == NULL){
        return -1;
    }
    QString toBeExec = QString("delete from friend where (id = (select id from userinfo where name = \'%1\') and friendid = (select id from userinfo where name = \'%2\')) or (id = (select id from userinfo where name = \'%2\') and friendid = (select id from userinfo where name = \'%1\'))").arg(friendName).arg(loginName);
    QSqlQuery query;
    qDebug()<<toBeExec;
    return query.exec(toBeExec);
}
