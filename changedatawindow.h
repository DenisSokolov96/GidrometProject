#ifndef CHANGEDATAWINDOW_H
#define CHANGEDATAWINDOW_H

#include <QDialog>
#include <QMap>

namespace Ui {
class ChangeDataWindow;
}

class ChangeDataWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeDataWindow(QWidget *parent = nullptr);
    ~ChangeDataWindow();

private slots:
    void on_addRowBtn_clicked();

    void on_listNameTable_currentTextChanged(const QString &arg1);

    void on_pushData_clicked();

    void on_delTblBtn_clicked();

    void on_clearTblBtn_clicked();

    void on_LoadFileBtn_clicked();

    void on_pushButton_clicked();
    void SlotSort(int iColumn);

    void on_delBtn_clicked();

private:
    Ui::ChangeDataWindow *ui;
    void getNameTables();
    void refreshTable(QStringList lineToken);
    void delSelectedRow();
    void delStrInDB();
    QString valToKey(QString searhKey);
    QString quotPars(QString strPars);    
};

#endif // CHANGEDATAWINDOW_H
