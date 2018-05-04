#ifndef DIALOGFORWHITENAME_H
#define DIALOGFORWHITENAME_H

#include <QDialog>

namespace Ui {
class DialogForWhiteName;
}

class DialogForWhiteName : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForWhiteName(QWidget *parent = 0);
    ~DialogForWhiteName();

private:
    Ui::DialogForWhiteName *ui;
};

#endif // DIALOGFORWHITENAME_H
