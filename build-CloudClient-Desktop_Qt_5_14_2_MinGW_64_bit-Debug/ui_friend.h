/********************************************************************************
** Form generated from reading UI file 'friend.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIEND_H
#define UI_FRIEND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <online.h>

QT_BEGIN_NAMESPACE

class Ui_Friend
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    Online *onlineFriend;
    QHBoxLayout *msgHBL;
    QLineEdit *broadcastText;
    QPushButton *broadcastPB;
    QHBoxLayout *topHBL;
    QTextEdit *broadcastHistory;
    QListWidget *friendList;
    QVBoxLayout *b;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
    QPushButton *onlineButton;
    QPushButton *searchButton;
    QPushButton *messageButton;

    void setupUi(QWidget *Friend)
    {
        if (Friend->objectName().isEmpty())
            Friend->setObjectName(QString::fromUtf8("Friend"));
        Friend->resize(641, 493);
        gridLayout_2 = new QGridLayout(Friend);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        onlineFriend = new Online(Friend);
        onlineFriend->setObjectName(QString::fromUtf8("onlineFriend"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(onlineFriend->sizePolicy().hasHeightForWidth());
        onlineFriend->setSizePolicy(sizePolicy);
        onlineFriend->setMinimumSize(QSize(570, 170));

        gridLayout->addWidget(onlineFriend, 2, 0, 1, 1);

        msgHBL = new QHBoxLayout();
        msgHBL->setObjectName(QString::fromUtf8("msgHBL"));
        broadcastText = new QLineEdit(Friend);
        broadcastText->setObjectName(QString::fromUtf8("broadcastText"));

        msgHBL->addWidget(broadcastText);

        broadcastPB = new QPushButton(Friend);
        broadcastPB->setObjectName(QString::fromUtf8("broadcastPB"));

        msgHBL->addWidget(broadcastPB);


        gridLayout->addLayout(msgHBL, 1, 0, 1, 1);

        topHBL = new QHBoxLayout();
        topHBL->setObjectName(QString::fromUtf8("topHBL"));
        broadcastHistory = new QTextEdit(Friend);
        broadcastHistory->setObjectName(QString::fromUtf8("broadcastHistory"));

        topHBL->addWidget(broadcastHistory);

        friendList = new QListWidget(Friend);
        friendList->setObjectName(QString::fromUtf8("friendList"));

        topHBL->addWidget(friendList);

        b = new QVBoxLayout();
        b->setObjectName(QString::fromUtf8("b"));
        deleteButton = new QPushButton(Friend);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));

        b->addWidget(deleteButton);

        refreshButton = new QPushButton(Friend);
        refreshButton->setObjectName(QString::fromUtf8("refreshButton"));

        b->addWidget(refreshButton);

        onlineButton = new QPushButton(Friend);
        onlineButton->setObjectName(QString::fromUtf8("onlineButton"));

        b->addWidget(onlineButton);

        searchButton = new QPushButton(Friend);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));

        b->addWidget(searchButton);

        messageButton = new QPushButton(Friend);
        messageButton->setObjectName(QString::fromUtf8("messageButton"));

        b->addWidget(messageButton);


        topHBL->addLayout(b);


        gridLayout->addLayout(topHBL, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(Friend);

        QMetaObject::connectSlotsByName(Friend);
    } // setupUi

    void retranslateUi(QWidget *Friend)
    {
        Friend->setWindowTitle(QCoreApplication::translate("Friend", "Form", nullptr));
        broadcastPB->setText(QCoreApplication::translate("Friend", "Broadcast", nullptr));
        deleteButton->setText(QCoreApplication::translate("Friend", "Delete", nullptr));
        refreshButton->setText(QCoreApplication::translate("Friend", "Refresh", nullptr));
        onlineButton->setText(QCoreApplication::translate("Friend", "Show Online", nullptr));
        searchButton->setText(QCoreApplication::translate("Friend", "Search", nullptr));
        messageButton->setText(QCoreApplication::translate("Friend", "Message", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Friend: public Ui_Friend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIEND_H
