#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <QWidget>
#include "protocol.h"

namespace Ui {
class SaveFile;
}

class SaveFile : public QWidget
{
    Q_OBJECT

public:
    explicit SaveFile(QWidget *parent = nullptr);
    ~SaveFile();
    void init(QString senderName, QString path, QString fileName, int widgetType, int widgetID);
    void updateFileList(pto *recvPto);
    void setCurPath(QString fileName);
    void loadFolder();
private slots:
    void double_clicked(const QModelIndex& index);
    void on_back_clicked();

    void on_cancel_clicked();

    void on_newFolder_clicked();

    void on_confirm_clicked();

private:
    Ui::SaveFile *ui;
    QString curPath;
    QString sender;
    QString shareFileFullPath;
    QString shareFileName;
    int type;
    int id;

    void deleteListItem();

};

#endif // SAVEFILE_H
