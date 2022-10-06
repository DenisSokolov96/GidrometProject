#ifndef OUTPUTDWINDOW_H
#define OUTPUTDWINDOW_H

#include <QDialog>
#include <QSqlQuery>
#include "QMap"

namespace Ui {
    class OutputWindow;
}

class OutputWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OutputWindow(QWidget *parent = nullptr);
    ~OutputWindow();    
    void setInfo(QSqlQuery requestSql);

private slots:
    void on_saveToCSV_clicked();

    void on_getDataBtn_clicked();

    void on_delSelectedBtn_clicked();
    void SlotSort(int iColumn);

private:
    Ui::OutputWindow *ui;
    void getNameTables();
    QString parsStrRep(QString str);
    QString valToKey(QString searhKey);
    void delSelectedRow();    
};

#endif // OUTPUTDWINDOW_H
