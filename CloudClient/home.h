#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QListWidget>
#include "friend.h"
#include "files.h"

class Home : public QWidget
{
    Q_OBJECT
public:
    explicit Home(QWidget *parent = nullptr);
    static Home& getInstance();
    Friend* getFriend()const;
signals:

private:
    QListWidget* wlist;
    Friend* friendWidget;
    Files* filesWidget;
};

#endif // HOME_H
