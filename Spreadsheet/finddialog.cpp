//**重点** 在一个头文件中再包含一个大头文件不是一种良好的编程风格，尤其对于比较大的工程项目更是如此
#include <QtWidgets>  //为构成QtCore和QtGui组成部分的所有类进行了定义 warning:the <QtGui> has been replaced by <QtWidgets> in Qt5 **重点**
#include "finddialog.h"

FindDialog::FindDialog(QWidget *parent):QDialog(parent) //构造函数
{
    label = new QLabel(tr("Find &what:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit); //以行编辑器作为标签的伙伴 他可以在按下标签快捷键时接收焦点

    caseCheckBox = new QCheckBox(tr("Match &case")); //tr()函数是把他们翻译成其他语言的标记。在每个QObject对象以及包含有Q_OBJECT宏
    backwardCheckBox = new QCheckBox(tr("Search &backward"));//的子类都有该函数的声明，在每一个用户可见的字符串周围使用tr()函数是一个很不错的习惯

    findButton = new QPushButton(tr("&Find"));//&表示快捷键 “&”可以用来控制焦点 //所谓的快键键就是alt+ “&”后的第一个字母
    findButton->setDefault(true); //让Find按钮成为对话框的默认按钮
    findButton->setEnabled(false); //当禁用一个窗口部件时，它通常显示为灰色，并且不能和用户发生交互操作

    closeButton = new QPushButton(tr("Close"));

    //由于是从QObject继承过来的，因此可以省略connect函数中的QObject::前缀
    connect(lineEdit,SIGNAL(textChanged(const QString &)),this,SLOT(enableFindButton(const QString &)));
    connect(findButton,SIGNAL(clicked()),this,SLOT(findClicked())); //槽中是类中定义的函数
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));

    //开始定义布局关系了
    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(caseCheckBox);
    leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(findButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch(); //小弹簧是一个分割符（或者成为伸展器），用它来占据Find按钮和Close按钮所余下的空白区域，这样可以确保完全占用他们所在布局的上层空间

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Find"));
    setFixedHeight(sizeHint().height()); //QWidget::sizeHint()函数可以返回一个窗口部件所理想的尺寸大小
}

//析构函数在QT里不是必须的，以为Qt会在删除父对象的时候自动删除其所属的所有子对象

void FindDialog::findClicked()
{
    QString text = lineEdit->text();
    Qt::CaseSensitivity cs = caseCheckBox->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive;
    if(backwardCheckBox->isChecked())
    {
        emit findPrevious(text,cs);   //emit是Qt的关键字
    }else
    {
        emit findNext(text,cs);
    }
}

void FindDialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
}

