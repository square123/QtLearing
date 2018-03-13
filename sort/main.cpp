#include "sortdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //qt的开始
    sortdialog *w = new sortdialog;
    w->setColumnRange('C','F');
    w->show();

    return a.exec(); //qt的循环
}
