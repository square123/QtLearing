#include "dialogforsinglepeople.h"
#include "ui_dialogforsinglepeople.h"

DialogForSinglePeople::DialogForSinglePeople(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogForSinglePeople)
{
    ui->setupUi(this);
}

DialogForSinglePeople::~DialogForSinglePeople()
{
    delete ui;
}
