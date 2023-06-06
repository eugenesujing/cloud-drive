#ifndef FILES_H
#define FILES_H

#include <QWidget>
#include <QTimer>
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
    void on_newFolderPB_clicked();

    void on_freshPB_clicked();

    void on_switch_to_files_widget(int currRow);

    void on_deletePB_clicked();

    void on_renamePB_clicked();

    void on_double_clicked(const QModelIndex& index);

    void on_backPB_clicked();

    void on_uploadPB_clicked();

    void uploadBegin();

private:
    Ui::Files *ui;
    void deleteListItem();
    QTimer   timer;
    QString filePathUpload;

};

#endif // FILES_H
