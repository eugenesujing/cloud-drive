#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QListWidget>

class Home : public QWidget
{
    Q_OBJECT
public:
    explicit Home(QWidget *parent = nullptr);

signals:

private:
    QListWidget* wlist;

};

#endif // HOME_H
