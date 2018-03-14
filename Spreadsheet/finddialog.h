#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class FindDialog :public QDialog
{
    Q_OBJECT //对于所有定义了信号和槽的类，在类定义开始的Q_OBJECT都是必须的

public:
    FindDialog(QWidget *parent =0);//构造函数

//connect所使用的函数
signals: //实际上一个宏，C++预处理会在编译程序找到它之前把它转换成标准C++代码
    void findNext(const QString &str, Qt::CaseSensitivity cs); //Qt::CaseSensitivity 是一个枚举类型
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

private slots: //也是一个宏
    void findClicked();
    void enableFindButton(const QString &text);

//控件
private:
    QLabel *label;          //对于私有变量，使用了它们的类前置声明，这是可行的，因为他们都是指针，而且没有必要在头文件中就去访问它们，
    QLineEdit *lineEdit;    //因而编译程序无需这些类的完整定义。没有包含与这几个类相关的头文件而是使用了一些前置声明，这可以使编译过程
    QCheckBox *caseCheckBox;//更快一些
    QCheckBox *backwardCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;
};

#endif // FINDDIALOG_H
