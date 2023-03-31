#include "privatemessage.h"
#include "ui_privatemessage.h"
#include "protocol.h"
#include "cloudclient.h"
#include <QDebug>

PrivateMessage::PrivateMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateMessage)
{
    ui->setupUi(this);
}



PrivateMessage::~PrivateMessage()
{
    delete ui;
}

void PrivateMessage::init(QString lName, QString fName)
{
    loginName = lName;
    friendName = fName;
}

QString PrivateMessage::getFriendName()
{
    return friendName;
}

void PrivateMessage::addNewMessage(QString msg)
{
    ui->chatHistory->addItem(msg.toStdString().c_str());
}

void PrivateMessage::on_msgSendPB_clicked()
{
    QString msgToBeSent = ui->msgInput->text();
    ui->msgInput->clear();

    pto* respPto = makePTO(msgToBeSent.size());
    if(respPto==NULL){
        qDebug()<<"malloc for sendPto failed on ENUM_MSG_TYPE_ADD_FRIEND_RESEND_REQUEST";
        return;
    }
    respPto->msgType = ENUM_MSG_TYPE_PRIVATE_MESSAGE_REQUEST;
    memcpy(respPto->preData, friendName.toStdString().c_str(), 32);
    memcpy(respPto->preData+32, loginName.toStdString().c_str(), 32);
    memcpy(respPto->data, msgToBeSent.toStdString().c_str(), msgToBeSent.size());
    CloudClient::getInstance().getSocket().write((char*)respPto, respPto->totalSize);

    QString myMsg = QString("%1:\n    %2").arg(loginName).arg(msgToBeSent);
    ui->chatHistory->addItem(myMsg.toStdString().c_str());
    free(respPto);
    respPto = NULL;
}
