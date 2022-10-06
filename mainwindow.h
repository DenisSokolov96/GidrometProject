#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "outputwindow.h"
#include "calcwindow.h"
#include "settingdbwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_get_data_triggered();

    void on_btn_del_triggered();

    void on_btn_change_triggered();

    void on_btn_exit_triggered();

    void on_btn_avalanches_triggered();

    void on_btn_setting_db_triggered();

    void on_FilterGrphBtn_triggered();

    void on_developBtn_triggered();

    void on_reference_triggered();

    void on_buttonMap_triggered();

private:
    Ui::MainWindow *ui;
    OutputWindow *outputwindow;
    CalcWindow *calwindow;
    SettingDBWindow *settingwindow;
    void myToolTip();
};
#endif // MAINWINDOW_H
