/********************************************************************************
** Form generated from reading UI file 'cloudserver.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOUDSERVER_H
#define UI_CLOUDSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CloudServer
{
public:

    void setupUi(QWidget *CloudServer)
    {
        if (CloudServer->objectName().isEmpty())
            CloudServer->setObjectName(QString::fromUtf8("CloudServer"));
        CloudServer->resize(800, 600);

        retranslateUi(CloudServer);

        QMetaObject::connectSlotsByName(CloudServer);
    } // setupUi

    void retranslateUi(QWidget *CloudServer)
    {
        CloudServer->setWindowTitle(QCoreApplication::translate("CloudServer", "CloudServer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CloudServer: public Ui_CloudServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOUDSERVER_H
