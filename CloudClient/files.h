#ifndef FILES_H
#define FILES_H

#include <QWidget>

namespace Ui {
class Files;
}

class Files : public QWidget
{
    Q_OBJECT

public:
    explicit Files(QWidget *parent = nullptr);
    ~Files();

private slots:
    void on_pushButton_5_clicked();

    void on_newFolderPB_clicked();

private:
    Ui::Files *ui;
};

#endif // FILES_H
