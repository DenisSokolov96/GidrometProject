#ifndef CALCWINDOW_H
#define CALCWINDOW_H

#include <QDialog>

namespace Ui {
    class CalcWindow;
}

class CalcWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CalcWindow(QWidget *parent = nullptr);
    ~CalcWindow();

private:
    Ui::CalcWindow *ui;
};

#endif // CALCWINDOW_H
