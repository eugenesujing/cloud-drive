/********************************************************************************
** Form generated from reading UI file 'privatemessage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVATEMESSAGE_H
#define UI_PRIVATEMESSAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrivateMessage
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QListWidget *chatHistory;
    QHBoxLayout *horizontalLayout;
    QLineEdit *msgInput;
    QPushButton *msgSendPB;

    void setupUi(QWidget *PrivateMessage)
    {
        if (PrivateMessage->objectName().isEmpty())
            PrivateMessage->setObjectName(QString::fromUtf8("PrivateMessage"));
        PrivateMessage->resize(596, 423);
        widget = new QWidget(PrivateMessage);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 20, 571, 391));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        chatHistory = new QListWidget(widget);
        chatHistory->setObjectName(QString::fromUtf8("chatHistory"));

        verticalLayout->addWidget(chatHistory);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        msgInput = new QLineEdit(widget);
        msgInput->setObjectName(QString::fromUtf8("msgInput"));
        QFont font;
        font.setPointSize(14);
        msgInput->setFont(font);

        horizontalLayout->addWidget(msgInput);

        msgSendPB = new QPushButton(widget);
        msgSendPB->setObjectName(QString::fromUtf8("msgSendPB"));
        msgSendPB->setFont(font);

        horizontalLayout->addWidget(msgSendPB);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(PrivateMessage);

        QMetaObject::connectSlotsByName(PrivateMessage);
    } // setupUi

    void retranslateUi(QWidget *PrivateMessage)
    {
        PrivateMessage->setWindowTitle(QCoreApplication::translate("PrivateMessage", "Form", nullptr));
        msgSendPB->setText(QCoreApplication::translate("PrivateMessage", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrivateMessage: public Ui_PrivateMessage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATEMESSAGE_H
