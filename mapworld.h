#ifndef MAPWORLD_H
#define MAPWORLD_H

#include <QGeoCoordinate>
#include <QDialog>
#include <QVariant>

namespace Ui {
class MapWorld;
}

class MapWorld : public QDialog
{
    Q_OBJECT

public:
    explicit MapWorld(QWidget *parent = nullptr);
    QList<QGeoCoordinate> listCoordinates;
    QList<int> listIdCoordinates;
    ~MapWorld();

private:
    Ui::MapWorld *ui;    
    void getLatAndLot();
    void myToolTip();
    QStringList requvestInfo();

signals:   

private slots:
    void on_searchBtn_clicked();
};

#endif // MAPWORLD_H
