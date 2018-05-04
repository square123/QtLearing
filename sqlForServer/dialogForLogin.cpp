#include "dialogForLogin.h"
#include "ui_dialogForLogin.h"

DialogForLogin::DialogForLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogForLogin)
{
    ui->setupUi(this);
    ui->lineEdit_password->setEchoMode(QLineEdit::PasswordEchoOnEdit);//输入后显示星号
    this->setFixedSize(this->width(),this->height());//固定尺寸
    //先设置一些默认值
    ui->lineEdit_ip->setText(QString("192.168.1.88"));
    ui->lineEdit_databaseName->setText(QString("project"));
    ui->lineEdit_port->setText(QString("3306"));
    ui->lineEdit_usrname->setText(QString("root"));
}

DialogForLogin::~DialogForLogin()
{
    delete ui;
}

void DialogForLogin::on_pushButton_link_clicked()
{
    //传递数据
    emit emitDataToFindDialog(ui->lineEdit_ip->text().toStdString(),ui->lineEdit_port->text().toInt(),ui->lineEdit_databaseName->text().toStdString(),ui->lineEdit_usrname->text().toStdString(),ui->lineEdit_password->text().toStdString());
    this->accept();//退出对话框
}
