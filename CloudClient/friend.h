#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include "online.h"
#include "privatemessage.h"

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
    //init a private message widget for friend, "friendName" is the name of the friend
    PrivateMessage* initPrivateMessageWidget(const char* friendName);

    void newPrivateMessgae(const char* friendName,char* message);
public slots:
    void showOnline();
private slots:
    void on_onlineButton_clicked();

    void on_searchButton_clicked();

    void on_refreshButton_clicked();

    void on_deleteButton_clicked();

    void on_messageButton_clicked();

private:
    Ui::Friend *ui;
    QString searchUserName;
    QList<PrivateMessage*> chatWidgets;
};

#endif // FRIEND_H
