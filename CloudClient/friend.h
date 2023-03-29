#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include "online.h"

namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    ~Friend();
    void handleShowOnlineResult(pto* recvPto);
    void handleLoadFriendList(pto* recvPto);
public slots:
    void showOnline();
private slots:
    void on_onlineButton_clicked();

    void on_searchButton_clicked();

    void on_refreshButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::Friend *ui;
    QString searchUserName;
};

#endif // FRIEND_H
