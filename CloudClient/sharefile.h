#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class ShareFile;
}

class ShareFile : public QWidget
{
    Q_OBJECT

public:
    explicit ShareFile(QWidget *parent = nullptr);
    ~ShareFile();

private:
    Ui::ShareFile *ui;
    QWidget* friendList;
    QButtonGroup* buttonGroup;

    void updateFriendList();
};

#endif // SHAREFILE_H
