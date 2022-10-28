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

    // для QQuickWidget
    MarkerModel model;
    ui->Map->rootContext()->setContextProperty("markerModel", &model);
    ui->Map->setSource(QUrl(QStringLiteral("qrc:/rec/map.qml")));
    ui->Map->show();

    auto obj = ui->Map->rootObject();
    connect(this, SIGNAL(setCoordinate(QVariant,QVariant, QVariant)), obj, SLOT(setCoordinate(QVariant, QVariant, QVariant)));

    for (int i = 0; i<listCoordinates.length(); i++){
        double lat = listCoordinates[i].latitude();
        double lon = listCoordinates[i].longitude();

        emit setCoordinate(lat,lon,listName[i]);
    }

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
    QList<QStringList> listNameCol = {  {"latitude", "longitude", "meteostation_id", "meteostation_name"},
                                 {"latitude", "longitude", "hydropost_id", "hydropost_name"}  };


    for (int i =0; i < listNameTable.length(); i++)
    {
        QSqlQuery requestQuery = requestSql.getDataForSlctColms(listNameTable[i], listNameCol[i], "ALL");
        while (requestQuery.next())
        {
            coordinate.setLatitude( requestQuery.value(0).toDouble() );
            coordinate.setLongitude( requestQuery.value(1).toDouble() );
            int id_coordinat = requestQuery.value(2).toInt();
            QString name = requestQuery.value(3).toString();

            listCoordinates.append(coordinate);
            QStringList listStr;
            switch (i) {
            case 0:              
                listStr.append(name);
                listStr.append(QString::number(id_coordinat));
                listStr.append("Метеостанция");
                listName.append(listStr);
                break;
            case 1:
                listStr.append(name);
                listStr.append(QString::number(id_coordinat));
                listStr.append("Гидропост");
                listName.append(listStr);
                break;
            default:
                break;
            }
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
    QList<QStringList> listNameCol = {  {"  mountain_name", "meteostation_name"},
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
