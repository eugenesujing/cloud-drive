#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QListWidget>
#include "friend.h"
#include "files.h"
#include "savefile.h"

class Home : public QWidget
{
    Q_OBJECT
public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();
    static Home& getInstance();
    Friend* getFriend()const;
    Files* getFiles()const;
    SaveFile* getSavaFile(int id)const;
    //widget type 0 is for share file widget, 1 for move file widget
    int initNewSaveFileWidget(QString sender, QString filePath, QString fileName, int widgetType);
signals:

private:
    QListWidget* wlist;
    Friend* friendWidget;
    Files* filesWidget;
    QList<SaveFile*> saveFileWidgets;
};

#endif // HOME_H
