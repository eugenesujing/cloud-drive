/********************************************************************************
** Form generated from reading UI file 'savefile.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEFILE_H
#define UI_SAVEFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SaveFile
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *pushButton_2;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QWidget *SaveFile)
    {
        if (SaveFile->objectName().isEmpty())
            SaveFile->setObjectName(QString::fromUtf8("SaveFile"));
        SaveFile->resize(635, 473);
        widget = new QWidget(SaveFile);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 611, 451));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 1);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        listWidget = new QListWidget(widget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(50);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        pushButton->setFont(font);

        horizontalLayout_2->addWidget(pushButton);

        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setFont(font);

        horizontalLayout_2->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(widget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setFont(font);

        horizontalLayout_2->addWidget(pushButton_4);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);


        retranslateUi(SaveFile);

        QMetaObject::connectSlotsByName(SaveFile);
    } // setupUi

    void retranslateUi(QWidget *SaveFile)
    {
        SaveFile->setWindowTitle(QCoreApplication::translate("SaveFile", "Form", nullptr));
        label->setText(QCoreApplication::translate("SaveFile", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">My Files</span></p></body></html>", nullptr));
        pushButton_2->setText(QCoreApplication::translate("SaveFile", "Back", nullptr));
        pushButton->setText(QCoreApplication::translate("SaveFile", "New Folder", nullptr));
        pushButton_3->setText(QCoreApplication::translate("SaveFile", "Confirm", nullptr));
        pushButton_4->setText(QCoreApplication::translate("SaveFile", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaveFile: public Ui_SaveFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEFILE_H
