#ifndef FILES_H
#define FILES_H

#include <QWidget>
#include  "protocol.h"

namespace Ui {
class Files;
}

class Files : public QWidget
{
    Q_OBJECT

public:
    explicit Files(QWidget *parent = nullptr);
    ~Files();

    static Files& getInstance();
    void loadFiles();
    void updateFileList(pto* recvPto);
private slots:
    void on_pushButton_5_clicked();

    void on_newFolderPB_clicked();

    void on_freshPB_clicked();

private:
    Ui::Files *ui;
    void deleteListItem();

};

#endif // FILES_H
