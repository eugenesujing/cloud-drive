#ifndef FILES_H
#define FILES_H

#include <QWidget>
#include <QTimer>
#include  "protocol.h"
#include <QFile>

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
    bool writeDownloadFile(qint64 fileUploadSoFar, qint64 fileTotalSize);
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

    void on_downloadPB_clicked();

private:
    Ui::Files *ui;
    void deleteListItem();
    QTimer   timer;
    QString filePathUpload;
    QString fileNameDownload;
    QFile downloadFile;

};

#endif // FILES_H
