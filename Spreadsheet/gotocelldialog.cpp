#include <QtWidgets>
#include "gotocelldialog.h"

GoToCellDialog::GoToCellDialog(QWidget *parent):QDialog(parent)
{
    setupUi(this);
    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    lineEdit->setValidator(new QRegExpValidator(regExp,this));//使用正则表达式来使其激活，非法输入都是不对的

    connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));//作为连接accept为1 reject为0
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
}

void GoToCellDialog::on_lineEdit_textChanged()
{
    okButton->setEnabled(lineEdit->hasAcceptableInput());
}
