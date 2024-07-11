/********************************************************************************
** Form generated from reading UI file 'puyopuyo.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUYOPUYO_H
#define UI_PUYOPUYO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_puyopuyo
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *puyopuyo)
    {
        if (puyopuyo->objectName().isEmpty())
            puyopuyo->setObjectName("puyopuyo");
        puyopuyo->resize(400, 300);
        pushButton = new QPushButton(puyopuyo);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(170, 160, 80, 24));

        retranslateUi(puyopuyo);

        QMetaObject::connectSlotsByName(puyopuyo);
    } // setupUi

    void retranslateUi(QDialog *puyopuyo)
    {
        puyopuyo->setWindowTitle(QCoreApplication::translate("puyopuyo", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("puyopuyo", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class puyopuyo: public Ui_puyopuyo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUYOPUYO_H
