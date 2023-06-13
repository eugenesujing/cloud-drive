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
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *back;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *newFolder;
    QPushButton *confirm;
    QPushButton *cancel;

    void setupUi(QWidget *SaveFile)
    {
        if (SaveFile->objectName().isEmpty())
            SaveFile->setObjectName(QString::fromUtf8("SaveFile"));
        SaveFile->resize(635, 473);
        layoutWidget = new QWidget(SaveFile);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 611, 451));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        back = new QPushButton(layoutWidget);
        back->setObjectName(QString::fromUtf8("back"));

        horizontalLayout->addWidget(back);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 1);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        listWidget = new QListWidget(layoutWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(50);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        newFolder = new QPushButton(layoutWidget);
        newFolder->setObjectName(QString::fromUtf8("newFolder"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        newFolder->setFont(font);

        horizontalLayout_2->addWidget(newFolder);

        confirm = new QPushButton(layoutWidget);
        confirm->setObjectName(QString::fromUtf8("confirm"));
        confirm->setFont(font);

        horizontalLayout_2->addWidget(confirm);

        cancel = new QPushButton(layoutWidget);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setFont(font);

        horizontalLayout_2->addWidget(cancel);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);


        retranslateUi(SaveFile);

        QMetaObject::connectSlotsByName(SaveFile);
    } // setupUi

    void retranslateUi(QWidget *SaveFile)
    {
        SaveFile->setWindowTitle(QCoreApplication::translate("SaveFile", "Form", nullptr));
        label->setText(QCoreApplication::translate("SaveFile", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">My Files</span></p></body></html>", nullptr));
        back->setText(QCoreApplication::translate("SaveFile", "Back", nullptr));
        newFolder->setText(QCoreApplication::translate("SaveFile", "New Folder", nullptr));
        confirm->setText(QCoreApplication::translate("SaveFile", "Confirm", nullptr));
        cancel->setText(QCoreApplication::translate("SaveFile", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaveFile: public Ui_SaveFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEFILE_H
