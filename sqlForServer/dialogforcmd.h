#ifndef DIALOGFORCMD_H
#define DIALOGFORCMD_H

#include <QDialog>

namespace Ui {
class DialogForCmd;
}

class DialogForCmd : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForCmd(QWidget *parent = 0);
    ~DialogForCmd();

private:
    Ui::DialogForCmd *ui;
};

#endif // DIALOGFORCMD_H
