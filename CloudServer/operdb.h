#ifndef OPERDB_H
#define OPERDB_H

#include <QObject>
#include <QSqlDatabase>

class operDB : public QObject
{
    Q_OBJECT
public:
    explicit operDB(QObject *parent = nullptr);

    static operDB& getInstance();
    ~operDB();
    void initDB();
    int handleRegister(const char* name, const char* pwd);
    int handleLogin(const char* name, const char* pwd);
    void handleOffline(const char* name);
private:
    QSqlDatabase myDB;


};

#endif // OPERDB_H
