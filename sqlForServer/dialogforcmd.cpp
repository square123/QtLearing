#include "dialogforcmd.h"
#include "ui_dialogforcmd.h"

DialogForCmd::DialogForCmd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogForCmd)
{
    ui->setupUi(this);
}

DialogForCmd::~DialogForCmd()
{
    delete ui;
}
