/********************************************************************************
** Form generated from reading UI file 'puyotet.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUYOTET_H
#define UI_PUYOTET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_puyotet
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *puyotet)
    {
        if (puyotet->objectName().isEmpty())
            puyotet->setObjectName("puyotet");
        puyotet->resize(800, 600);
        centralwidget = new QWidget(puyotet);
        centralwidget->setObjectName("centralwidget");
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(360, 190, 80, 24));
        puyotet->setCentralWidget(centralwidget);
        menubar = new QMenuBar(puyotet);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        puyotet->setMenuBar(menubar);
        statusbar = new QStatusBar(puyotet);
        statusbar->setObjectName("statusbar");
        puyotet->setStatusBar(statusbar);

        retranslateUi(puyotet);

        QMetaObject::connectSlotsByName(puyotet);
    } // setupUi

    void retranslateUi(QMainWindow *puyotet)
    {
        puyotet->setWindowTitle(QCoreApplication::translate("puyotet", "puyotet", nullptr));
        pushButton->setText(QCoreApplication::translate("puyotet", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class puyotet: public Ui_puyotet {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUYOTET_H
