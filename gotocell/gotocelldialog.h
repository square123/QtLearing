#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H
#include <QDialog>
#include"ui_gotocelldialog.h"

class GoToCellDialog:public QDialog,public Ui::GoToCellDialog //最为便捷的方法：同时继承，并且由它实现确实的功能（通过简单地增加另外一个间接层就可以解决软件的任何问题）
{
    Q_OBJECT
public:
    GoToCellDialog(QWidget *parent = 0);
private slots:
    void on_lineEdit_textChanged();
};

#endif // GOTOCELLDIALOG_H

//setupUi()函数还会自动将那些符合on_objectName_signalName()命名惯例的任意槽与相应的objectName的signalName()信号连接在一起。
