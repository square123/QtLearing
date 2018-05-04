#ifndef DIALOGFORSINGLEPEOPLE_H
#define DIALOGFORSINGLEPEOPLE_H

#include <QDialog>

namespace Ui {
class DialogForSinglePeople;
}

class DialogForSinglePeople : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForSinglePeople(QWidget *parent = 0);
    ~DialogForSinglePeople();

private:
    Ui::DialogForSinglePeople *ui;
};

#endif // DIALOGFORSINGLEPEOPLE_H
