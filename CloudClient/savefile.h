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
    void updateFileList(pto *recvPto);
private slots:
    void on_double_clicked(const QModelIndex& index);
private:
    Ui::SaveFile *ui;
    QString curPath;
    void loadFolder();
    void deleteListItem();

};

#endif // SAVEFILE_H
