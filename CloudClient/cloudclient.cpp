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
    connect(&mySocket, SIGNAL(readyRead()), this, SLOT(onRecv()));
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


/*void CloudClient::on_send_button_clicked()
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
}*/

void CloudClient::on_login_button_clicked()
{
    QString nameToBeSent = ui->name_input->text();
    QString pwdToBeSent = ui->pwd_input->text();
    if(!nameToBeSent.isEmpty() && !pwdToBeSent.isEmpty()){
        pto* newPto = makePTO(0);
        if(newPto!=NULL){
            newPto->totalSize = sizeof (pto);
            newPto->msgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
            //Since the name and pwd is small enough to be fitted into preData, we will use it to store the data
            memcpy(newPto->preData,nameToBeSent.toStdString().c_str(),nameToBeSent.size());
            memcpy(newPto->preData+32,pwdToBeSent.toStdString().c_str(),pwdToBeSent.size());
            mySocket.write((char*)newPto,newPto->totalSize);
            free(newPto);
            newPto = NULL;
        }else{
            qDebug()<<"malloc for newPto(Register) failed.";
        }
    }else{
        QMessageBox::critical(this,"Login In", "Name or Password is empty. Please reenter.");
    }
}

void CloudClient::on_register_button_clicked()
{
    QString nameToBeSent = ui->name_input->text();
    QString pwdToBeSent = ui->pwd_input->text();
    if(!nameToBeSent.isEmpty() && !pwdToBeSent.isEmpty()){
        pto* newPto = makePTO(0);
        if(newPto!=NULL){
            newPto->totalSize = sizeof (pto);
            newPto->msgType = ENUM_MSG_TYPE_REGISTER_REQUEST;
            //Since the name and pwd is small enough to be fitted into preData, we will use it to store the data
            memcpy(newPto->preData,nameToBeSent.toStdString().c_str(),nameToBeSent.size());
            memcpy(newPto->preData+32,pwdToBeSent.toStdString().c_str(),pwdToBeSent.size());
            mySocket.write((char*)newPto,newPto->totalSize);
            free(newPto);
            newPto = NULL;
        }
    }else{
        QMessageBox::critical(this,"Register New Account", "Name or Password is empty. Please reenter.");
    }
}

void CloudClient::onRecv()
{
    unsigned int ptoSize = 0;
    mySocket.read((char*)&ptoSize, sizeof(unsigned int));
    unsigned int msgSize = ptoSize - sizeof (pto);

    pto* recvPto = makePTO(msgSize);
    if(recvPto!=NULL){
        recvPto->totalSize = ptoSize;
        mySocket.read((char*)recvPto +sizeof(unsigned int), ptoSize-sizeof (unsigned int));
        //handle user request based on message type
        switch (recvPto->msgType) {
        case ENUM_MSG_TYPE_REGISTER_RESPOND:{
            char* respond = (char*)malloc(msgSize+1);
            memset(respond,0,msgSize+1);
            memcpy(respond,(char*)recvPto->data,msgSize);
            if(recvPto->code==1){
                QMessageBox::information(this, "Register New Account", respond);
            }else{
                QMessageBox::warning(this, "Register New Account", respond);
            }
            free(respond);
            respond = NULL;
            break;
        }
        case ENUM_MSG_TYPE_LOGIN_RESPOND:{

            if(recvPto->code!=1){
                char* respond = (char*)malloc(msgSize+1);
                memset(respond,0,msgSize+1);
                memcpy(respond,(char*)recvPto->data,msgSize);
                QMessageBox::warning(this, "Log In", respond);
                free(respond);
                respond = NULL;
            }else{
                qDebug()<<"Login successfully.";
            }

            break;
        }
        default:
            break;
        }
      free(recvPto);
      recvPto = NULL;
    }
}
