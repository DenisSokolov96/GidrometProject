#include "mapworld.h"
#include "ui_mapworld.h"
#include <QGeoCoordinate>
#include <QGraphicsObject>
#include <QQuickItem>
#include <requestSql.h>
#//include <QQuickWidget>
#include <QQmlContext>
#include "MarkerModel.h"
#include <storage.h>
#include <QMessageBox>

//получаю ссылку на объект одиночки
Storage& storageMapWorld = Storage::Instance();

MapWorld::MapWorld(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapWorld)
{
    ui->setupUi(this);

    //получить данные по долготе и широте с сохранением в listCoordinates
    getLatAndLot();
    //всплывающие подсказки
    myToolTip();

    // для QWebWidget отображение google карты в программе    
    //QString gMapURL = "Россия";
    //gMapURL = "http://maps.google.com.sg/maps?q="+gMapURL+"&oe=utf-8&rls=org.mozilla:en-US:official&client=firefox-a&um=1&ie=UTF-8&hl=en&sa=N&tab=wl";
    //gMapURL = "https://www.google.com/maps";
    //ui->Map->setUrl(QUrl(gMapURL));

    // для QQuickWidget
    //ui->Map->setSource(QUrl(QStringLiteral("qrc:/rec/map.qml")));
    // для QQuickWidget
        ui->Map->setSource(QUrl(QStringLiteral("qrc:/rec/map.qml")));




        ui->Map->show();
        auto obj = ui->Map->rootObject();
        connect(this, SIGNAL(setCenter(QVariant,QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
//    QQuickWidget w;
//    MarkerModel model;
//    w.rootContext()->setContextProperty("MarkerModel", &model);

//    w.setSource(QUrl(QStringLiteral("qrc:/rec/map.qml")));
//    w.show();

}

MapWorld::~MapWorld()
{
    delete ui;
}

void MapWorld::getLatAndLot()
{
    QGeoCoordinate coordinate;

    RequestSql requestSql;
    //meteostations (meteostation_name latitude longitude)
    //hydroposts (hydropost_name latitude longitude)

    QStringList listNameTable = {"meteostations", "hydroposts"};
    QList<QStringList> listNameCol = {  {"latitude", "longitude", "meteostation_id"},
                                 {"latitude", "longitude", "hydropost_id"}  };


    for (int i =0; i < listNameTable.length(); i++)
    {
        QSqlQuery requestQuery = requestSql.getDataForSlctColms(listNameTable[i], listNameCol[i], "ALL");
        while (requestQuery.next())
        {
            QString str_lat = requestQuery.value(0).toString();
            QString str_lon = requestQuery.value(1).toString();            
            int id_coordinat = requestQuery.value(2).toInt();
            coordinate.setLatitude(str_lat.toDouble());
            coordinate.setLongitude(str_lon.toDouble());
            listCoordinates.append(coordinate);
            listIdCoordinates.append(id_coordinat);
        }
    }
}

//Задаю подсказки
void MapWorld::myToolTip()
{
    ui->plainTextEdit->setToolTip("Краткая информация о выбранном объекте");
    ui->plainTextEdit->setToolTipDuration(3000);
    ui->lineSearch->setToolTip("Поле ввода объекта для поиска");
    ui->lineSearch->setToolTipDuration(3000);
    ui->searchBtn->setToolTip("Кнопка для запуска поиска объектов");
    ui->searchBtn->setToolTipDuration(3000);    
}

//Собтие на нажатие кнопки поиска
void MapWorld::on_searchBtn_clicked()
{
    QStringList textInfoGeoObject = requvestInfo();
    ui->plainTextEdit->clear();
    foreach( QString str, textInfoGeoObject)
       ui->plainTextEdit->appendPlainText(str);
}

QStringList MapWorld::requvestInfo()
{
    RequestSql requestSql;
    QStringList textInfoGeoObject;
    QString lineSearch = ui->lineSearch->text();
    QStringList listNameTable = {"meteostations", "hydroposts"};    
    QList<QStringList> listNameCol = {  {"mountain_name", "meteostation_name"},
                                        {"hydropost_organ", "hydropost_name"} };
    if ( lineSearch.toInt()){
        listNameCol[0].append("meteostation_id");
        listNameCol[1].append("hydropost_id");
    }
    for (int i =0; i < listNameTable.length(); i++)
    {
        QSqlQuery requestQuery = requestSql.getDataWhere(listNameTable[i],listNameCol[i],lineSearch);

        while (requestQuery.next())
        {
            textInfoGeoObject.append( storageMapWorld.storageHeaderMap[listNameTable[i]] );
            QString strNameObject;
            for (int j = 0; !requestQuery.isNull(j); j++)
            {
                strNameObject = storageMapWorld.storageTableMap[listNameTable[i]][1][j];
                strNameObject += " :-> ";
                textInfoGeoObject.append( strNameObject + requestQuery.value(j).toString() );
            }
            textInfoGeoObject.append("\n");
            textInfoGeoObject.append("/*/---------/*/---------/*/---------/*/");
            textInfoGeoObject.append("\n");
        }
    }
    if (textInfoGeoObject.length() == 0) textInfoGeoObject.append("Данные по запросу: '" + lineSearch + "' не получены...");

    return textInfoGeoObject;
}
