#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>
#include <QButtonGroup>
#include <QVBoxLayout>

namespace Ui {
class ShareFile;
}

class ShareFile : public QWidget
{
    Q_OBJECT

public:
    explicit ShareFile(QWidget *parent = nullptr);
    ~ShareFile();

private slots:
    void on_selectAll_clicked();

    void on_clearSelection_clicked();

private:
    Ui::ShareFile *ui;
    QWidget* friendList;
    QButtonGroup* buttonGroup;
    QVBoxLayout *vLayout;
    void updateFriendList();
};

#endif // SHAREFILE_H
