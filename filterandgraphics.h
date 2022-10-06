#ifndef FILTERANDGRAPHICS_H
#define FILTERANDGRAPHICS_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QtCharts>
#include <QSplineSeries>
#include <QSqlQuery>

namespace Ui {
class FilterAndGraphics;
}

class FilterAndGraphics : public QDialog
{
    Q_OBJECT

public:
    explicit FilterAndGraphics(QWidget *parent = nullptr);
    ~FilterAndGraphics();    

private slots:
    void on_createGraph_clicked();

    void on_saveToCSVBtn_clicked();

    void on_getDataBtn_clicked();

    void on_savePic_clicked();

    void on_nameTables_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();
    void SlotSort(int iColumn);

private:
    Ui::FilterAndGraphics *ui;
    QGraphicsScene      *scene;     // Объявляем сцену для отрисовки
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    QChart              *chart;

    void getNameTables();
    void setInfo(QSqlQuery requestQuery);
    void addCheckBoxs();
    void addCheckBox(QString str);
    void delOldCheckBoxs();
    void addSpinBox();
    void addLabel(QString str);
    QStringList getInfoOutCheckBoxs();
    int getInfoOutSpinBoxs();
    bool getAllOut();
    void myToolTip();
    void delSelectedRow();

    bool selectAll;
    QString textTitle;
    QString nameTableOut;
    QStringList listHeaderOutUS;
    QStringList listHeaderOutRUS;

};

#endif // FILTERANDGRAPHICS_H
