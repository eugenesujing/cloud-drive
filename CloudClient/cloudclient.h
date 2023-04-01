#ifndef CLOUDCLIENT_H
#define CLOUDCLIENT_H

#include <QWidget>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class CloudClient; }
QT_END_NAMESPACE

class CloudClient : public QWidget
{
    Q_OBJECT

public:
    CloudClient(QWidget *parent = nullptr);
    ~CloudClient();

    static CloudClient& getInstance();
    QTcpSocket& getSocket();
    QString getLoginName()const;
    void loadFriendList();
    QString getCurPath()const;
public slots:
    //Inform user when client is connected to server successfully
    void showConnected();
private slots:
    //void on_send_button_clicked();

    void on_register_button_clicked();

    void on_login_button_clicked();

    void onRecv();

private:
    Ui::CloudClient *ui;

    QString ip;
    uint16_t port;

    QTcpSocket mySocket;
    QString loginName;
    QString curPath;
    //Load config from resource file
    void loadConfig();

};
#endif // CLOUDCLIENT_H
