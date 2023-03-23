#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include "online.h"

namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    ~Friend();

public slots:
    void showOnline();
private:
    Ui::Friend *ui;

};

#endif // FRIEND_H
