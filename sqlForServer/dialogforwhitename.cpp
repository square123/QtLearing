#include "dialogforwhitename.h"
#include "ui_dialogforwhitename.h"

DialogForWhiteName::DialogForWhiteName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogForWhiteName)
{
    ui->setupUi(this);
}

DialogForWhiteName::~DialogForWhiteName()
{
    delete ui;
}
