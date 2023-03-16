#include "cloudclient.h"
#include "ui_cloudclient.h"
#include "protocol.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QString>

CloudClient::CloudClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CloudClient)
{
    ui->setupUi(this);
    loadConfig();

    connect(&mySocket, SIGNAL(connected()),this, SLOT(showConnected()));
    mySocket.connectToHost(QHostAddress(ip),port);
}

CloudClient::~CloudClient()
{
    delete ui;
}

//Inform user when client is connected to server successfully
void CloudClient::showConnected()
{
    QMessageBox::information(this, "Connect to server", "Connected to server!");
}

//Load config from resource file
void CloudClient::loadConfig()
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


void CloudClient::on_send_button_clicked()
{
    QString msgToBeSent = ui->lineEdit->text();
    if(!msgToBeSent.isEmpty()){
        pto* newPto = makePTO(msgToBeSent.size());
        if(newPto!=NULL){
            newPto->totalSize = sizeof (pto) + msgToBeSent.size();
            newPto->msgSize = msgToBeSent.size();
            memcpy(newPto->data,msgToBeSent.toStdString().c_str(),msgToBeSent.size());
            qDebug()<<newPto->data;
            mySocket.write((char*)newPto,newPto->totalSize);
            free(newPto);
            newPto = NULL;
        }
    }
}
