/********************************************************************************
** Form generated from reading UI file 'cloudclient.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOUDCLIENT_H
#define UI_CLOUDCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CloudClient
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QPushButton *register_button;
    QSpacerItem *verticalSpacer_2;
    QPushButton *login_button;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *name_label;
    QLineEdit *name_input;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *pwd_label;
    QLineEdit *pwd_input;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *CloudClient)
    {
        if (CloudClient->objectName().isEmpty())
            CloudClient->setObjectName(QString::fromUtf8("CloudClient"));
        CloudClient->resize(734, 575);
        widget = new QWidget(CloudClient);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(110, 90, 471, 311));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        register_button = new QPushButton(widget);
        register_button->setObjectName(QString::fromUtf8("register_button"));
        QFont font;
        font.setPointSize(16);
        register_button->setFont(font);

        verticalLayout_2->addWidget(register_button);

        verticalSpacer_2 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        login_button = new QPushButton(widget);
        login_button->setObjectName(QString::fromUtf8("login_button"));
        login_button->setFont(font);

        verticalLayout_2->addWidget(login_button);


        gridLayout->addLayout(verticalLayout_2, 1, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        name_label = new QLabel(widget);
        name_label->setObjectName(QString::fromUtf8("name_label"));
        QFont font1;
        font1.setPointSize(16);
        font1.setBold(false);
        font1.setWeight(50);
        name_label->setFont(font1);

        horizontalLayout->addWidget(name_label);

        name_input = new QLineEdit(widget);
        name_input->setObjectName(QString::fromUtf8("name_input"));
        QFont font2;
        font2.setPointSize(11);
        name_input->setFont(font2);

        horizontalLayout->addWidget(name_input);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pwd_label = new QLabel(widget);
        pwd_label->setObjectName(QString::fromUtf8("pwd_label"));
        pwd_label->setFont(font1);

        horizontalLayout_2->addWidget(pwd_label);

        pwd_input = new QLineEdit(widget);
        pwd_input->setObjectName(QString::fromUtf8("pwd_input"));
        pwd_input->setFont(font2);
        pwd_input->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(pwd_input);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(CloudClient);

        QMetaObject::connectSlotsByName(CloudClient);
    } // setupUi

    void retranslateUi(QWidget *CloudClient)
    {
        CloudClient->setWindowTitle(QCoreApplication::translate("CloudClient", "CloudClient", nullptr));
        register_button->setText(QCoreApplication::translate("CloudClient", "Register", nullptr));
        login_button->setText(QCoreApplication::translate("CloudClient", "Log In", nullptr));
        name_label->setText(QCoreApplication::translate("CloudClient", "Name:", nullptr));
        pwd_label->setText(QCoreApplication::translate("CloudClient", "Password:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CloudClient: public Ui_CloudClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOUDCLIENT_H
