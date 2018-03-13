#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>

#include "ui_sortdialog.h" //生成之后需要添加的 在头文件中添加
namespace Ui {
class sortdialog;
}

class sortdialog : public QDialog, public Ui::sortdialog
{
    Q_OBJECT

public:
    explicit sortdialog(QWidget *parent = 0);
    void setColumnRange(QChar first,QChar last);
    ~sortdialog();

private:
    Ui::sortdialog *ui;
};

#endif // SORTDIALOG_H
