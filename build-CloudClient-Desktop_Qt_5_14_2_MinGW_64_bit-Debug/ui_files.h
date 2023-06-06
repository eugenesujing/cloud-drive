/********************************************************************************
** Form generated from reading UI file 'files.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILES_H
#define UI_FILES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Files
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout_2;
    QPushButton *backPB;
    QPushButton *newFolderPB;
    QPushButton *deletePB;
    QPushButton *renamePB;
    QPushButton *freshPB;
    QVBoxLayout *verticalLayout_3;
    QPushButton *uploadPB;
    QPushButton *downloadPB;
    QPushButton *sharePB;

    void setupUi(QWidget *Files)
    {
        if (Files->objectName().isEmpty())
            Files->setObjectName(QString::fromUtf8("Files"));
        Files->resize(672, 497);
        gridLayout = new QGridLayout(Files);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(Files);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setFrameShape(QFrame::WinPanel);

        verticalLayout->addWidget(label);

        listWidget = new QListWidget(Files);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, -1, 6, -1);
        backPB = new QPushButton(Files);
        backPB->setObjectName(QString::fromUtf8("backPB"));
        QFont font1;
        font1.setPointSize(11);
        backPB->setFont(font1);
        backPB->setAutoFillBackground(false);

        verticalLayout_2->addWidget(backPB);

        newFolderPB = new QPushButton(Files);
        newFolderPB->setObjectName(QString::fromUtf8("newFolderPB"));
        newFolderPB->setFont(font1);
        newFolderPB->setAutoFillBackground(false);

        verticalLayout_2->addWidget(newFolderPB);

        deletePB = new QPushButton(Files);
        deletePB->setObjectName(QString::fromUtf8("deletePB"));
        deletePB->setFont(font1);
        deletePB->setAutoFillBackground(false);

        verticalLayout_2->addWidget(deletePB);

        renamePB = new QPushButton(Files);
        renamePB->setObjectName(QString::fromUtf8("renamePB"));
        renamePB->setFont(font1);
        renamePB->setAutoFillBackground(false);

        verticalLayout_2->addWidget(renamePB);

        freshPB = new QPushButton(Files);
        freshPB->setObjectName(QString::fromUtf8("freshPB"));
        freshPB->setFont(font1);
        freshPB->setAutoFillBackground(false);

        verticalLayout_2->addWidget(freshPB);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(6, -1, 6, -1);
        uploadPB = new QPushButton(Files);
        uploadPB->setObjectName(QString::fromUtf8("uploadPB"));
        uploadPB->setFont(font1);
        uploadPB->setAutoFillBackground(false);

        verticalLayout_3->addWidget(uploadPB);

        downloadPB = new QPushButton(Files);
        downloadPB->setObjectName(QString::fromUtf8("downloadPB"));
        downloadPB->setFont(font1);
        downloadPB->setAutoFillBackground(false);

        verticalLayout_3->addWidget(downloadPB);

        sharePB = new QPushButton(Files);
        sharePB->setObjectName(QString::fromUtf8("sharePB"));
        sharePB->setFont(font1);
        sharePB->setAutoFillBackground(false);

        verticalLayout_3->addWidget(sharePB);


        horizontalLayout->addLayout(verticalLayout_3);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(Files);

        QMetaObject::connectSlotsByName(Files);
    } // setupUi

    void retranslateUi(QWidget *Files)
    {
        Files->setWindowTitle(QCoreApplication::translate("Files", "Form", nullptr));
        label->setText(QCoreApplication::translate("Files", "File", nullptr));
        backPB->setText(QCoreApplication::translate("Files", "Back", nullptr));
        newFolderPB->setText(QCoreApplication::translate("Files", "New Folder", nullptr));
        deletePB->setText(QCoreApplication::translate("Files", "Delete", nullptr));
        renamePB->setText(QCoreApplication::translate("Files", "Rename", nullptr));
        freshPB->setText(QCoreApplication::translate("Files", "Fresh", nullptr));
        uploadPB->setText(QCoreApplication::translate("Files", "Upload", nullptr));
        downloadPB->setText(QCoreApplication::translate("Files", "Download", nullptr));
        sharePB->setText(QCoreApplication::translate("Files", "Share", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Files: public Ui_Files {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILES_H
