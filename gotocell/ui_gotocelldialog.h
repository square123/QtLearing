/********************************************************************************
** Form generated from reading UI file 'gotocelldialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOTOCELLDIALOG_H
#define UI_GOTOCELLDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GoToCellDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *GoToCellDialog)
    {
        if (GoToCellDialog->objectName().isEmpty())
            GoToCellDialog->setObjectName(QStringLiteral("GoToCellDialog"));
        GoToCellDialog->resize(249, 71);
        verticalLayout = new QVBoxLayout(GoToCellDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(GoToCellDialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(GoToCellDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        okButton = new QPushButton(GoToCellDialog);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setEnabled(false);

        horizontalLayout_2->addWidget(okButton);

        cancelButton = new QPushButton(GoToCellDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        horizontalLayout_2->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_2);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(lineEdit);
#endif // QT_NO_SHORTCUT

        retranslateUi(GoToCellDialog);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(GoToCellDialog);
    } // setupUi

    void retranslateUi(QDialog *GoToCellDialog)
    {
        GoToCellDialog->setWindowTitle(QApplication::translate("GoToCellDialog", "Go to Cell", Q_NULLPTR));
        label->setText(QApplication::translate("GoToCellDialog", "&Cell Location:", Q_NULLPTR));
        okButton->setText(QApplication::translate("GoToCellDialog", "OK", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("GoToCellDialog", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GoToCellDialog: public Ui_GoToCellDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOTOCELLDIALOG_H
