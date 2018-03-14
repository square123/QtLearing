#include "sortdialog.h"

//需要把头文件包含进去，但不是放在实现文件中

sortdialog::sortdialog(QWidget *parent) :
    QDialog(parent)//,
    //ui(new Ui::sortdialog) //这里要隐藏
{
    //ui->setupUi(this);  //这里要隐藏
   setupUi(this);  //感觉每次构造函数都是这个先起头
   secondaryGroupBox->hide();
   tertiaryGroupBox->hide();
   layout()->setSizeConstraint(QLayout::SetFixedSize);
   setColumnRange('A','Z');
}

void sortdialog::setColumnRange(QChar first, QChar last)
{
    primaryColumnCombo->clear();
    secondaryColumnCombo->clear();
    tertiaryColumnCombo->clear();

    secondaryColumnCombo->addItem(tr("None"));
    tertiaryColumnCombo->addItem(tr("None"));
    primaryColumnCombo->setMinimumSize(secondaryColumnCombo->sizeHint()); //sizeHint()函数可以返回布局系统试图认同的“理想”大小
    //上面这句是为了避免单字符和none的不一致性
    QChar ch =first;
    while(ch<=last)
    {
        primaryColumnCombo->addItem(QString(ch));
        secondaryColumnCombo->addItem(QString(ch));
        tertiaryColumnCombo->addItem(QString(ch));
        ch = ch.unicode()+1;
    }
}

sortdialog::~sortdialog() //这个是系统自动生成的，书上其实是没有这个的
{
    delete ui;
}
