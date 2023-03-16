#ifndef CLOUDSERVER_H
#define CLOUDSERVER_H

#include <QWidget>
# include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class CloudServer; }
QT_END_NAMESPACE

class CloudServer : public QWidget
{
    Q_OBJECT

public:
    CloudServer(QWidget *parent = nullptr);
    ~CloudServer();

private:
    Ui::CloudServer *ui;
    QString ip;
    uint16_t port;

    //Load config from resource file
    void loadConfig();
};
#endif // CLOUDSERVER_H
