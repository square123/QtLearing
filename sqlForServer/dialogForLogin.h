#ifndef DIALOGFORLOGIN_H
#define DIALOGFORLOGIN_H

#include <QDialog>
#include <string>

namespace Ui {
class DialogForLogin;
}

class DialogForLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForLogin(QWidget *parent = 0);
    ~DialogForLogin();
signals:
    void emitDataToFindDialog(std::string hostName,int port,std::string databaseName,std::string userName,std::string password);//传递数据给findDialog

private slots:
    void on_pushButton_link_clicked();

private:
    Ui::DialogForLogin *ui;
};

#endif // DIALOGFORLOGIN_H
