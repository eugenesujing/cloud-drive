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

private:
    QSqlDatabase myDB;
    void initDB();

};

#endif // OPERDB_H
