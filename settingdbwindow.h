#ifndef SETTINGDBWINDOW_H
#define SETTINGDBWINDOW_H

#include <QDialog>

namespace Ui {
    class SettingDBWindow;
}

class SettingDBWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDBWindow(QWidget *parent = nullptr);
    ~SettingDBWindow();

private slots:
    void on_btn_connect_clicked();

private:
    Ui::SettingDBWindow *ui;
};

#endif // SETTINGDBWINDOW_H
