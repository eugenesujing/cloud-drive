#include "cloudserver.h"
#include "ui_cloudserver.h"
#include "mytcpserver.h"

#include <QFile>
#include <QMessageBox>
#include <QHostAddress>

CloudServer::CloudServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CloudServer)
{
    ui->setupUi(this);

    loadConfig();

    MyTcpServer::getInstance().listen(QHostAddress(ip),port);
}

CloudServer::~CloudServer()
{
    delete ui;
}

void CloudServer::loadConfig()
{
    QFile file(":/config.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll();
        QString strData = data.toStdString().c_str();

        file.close();
        strData.replace("\r\n"," ");

        QStringList strList = strData.split(" ");

        ip = strList[0];
        port = strList[1].toUShort();

    }
    else{
        QMessageBox::critical(this,"Load config", "Fail to load config");
    }
}

