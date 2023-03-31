#ifndef PRIVATEMESSAGE_H
#define PRIVATEMESSAGE_H

#include <QWidget>

namespace Ui {
class PrivateMessage;
}

class PrivateMessage : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateMessage(QWidget *parent = nullptr);
    ~PrivateMessage();
    void init(QString lName, QString fName);
    QString getFriendName();
    void addNewMessage(QString msg);
private slots:
    void on_msgSendPB_clicked();

private:
    Ui::PrivateMessage *ui;
    QString loginName;
    QString friendName;
};

#endif // PRIVATEMESSAGE_H
