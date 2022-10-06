#include "requestSql.h"
#include <QMessageBox>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

RequestSql::RequestSql()
{    
}

//получение всех данных из таблцы
QSqlQuery RequestSql::getAllTable(QString nameTable, QString strCntRep)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM "+ nameTable + " LIMIT " + strCntRep);

    if (!query.exec()){
        QMessageBox::warning(0, "Ошибка чтения", query.lastError().text());
        return query;
    }   
    return query;
}

//получение выбранных данных из таблцы
QSqlQuery RequestSql::getDataForSlctColms(QString nameTable, QStringList listNameHeader, QString strCntRep)
{
    QSqlQuery query;
    QString strSQL = "";
    strSQL = "SELECT (";

    for (int i = 0; i < listNameHeader.length(); i++)
    {
        strSQL += listNameHeader[i];
        if (i+1 != listNameHeader.length()) strSQL += ", ";
    }
    strSQL += ") FROM " + nameTable + " LIMIT " + strCntRep;

    query.prepare(strSQL);

    if (!query.exec()){
        QMessageBox::warning(0, "Ошибка чтения", query.lastError().text());
        return query;
    }
    return query;
}

//получение информации по указанным значениям
QSqlQuery RequestSql::getDataWhere(QString nameTable, QStringList listNameCol, QString strValue)
{
    QSqlQuery query;
    QString strSQL = "";
    strSQL = "SELECT *";
    strSQL += " FROM " + nameTable;
    strSQL += " WHERE ";
    for (int i = 0; i < listNameCol.length(); i++)
    {
        strSQL += listNameCol[i] + "= '" + strValue + "'";
        if (i+1 != listNameCol.length()) strSQL += " OR ";
    }

    query.prepare(strSQL);

    if (!query.exec()){
        QMessageBox::warning(0, "Ошибка чтения", query.lastError().text());
        return query;
    }
    return query;
}

//получение имен таблиц в бд
QSqlQuery RequestSql::getNamesTables()
{
    QSqlQuery query;    
    query.prepare("SELECT table_name FROM information_schema.tables  WHERE table_schema='public' ORDER BY table_name;");

    if (!query.exec()){
        QMessageBox::warning(0, "Ошибка получения названий таблиц", query.lastError().text());
        return query;
    }
    return query;
}

//получение таблиц и колонок одним запросом, но не попорядку
//SELECT table_name, column_name FROM information_schema.columns WHERE table_schema='public'
//получение по имени таблицы имя колонки
QSqlQuery RequestSql::getNamesColumns(QString nameTable)
{
    QSqlQuery query;

    query.prepare("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '" + nameTable + "'");

    if (!query.exec()){
        QMessageBox::warning(0, "Ошибка получения заголовков в таблицах", query.lastError().text());
        return query;
    }
    return query;
}

//Записать данные в бд из таблицы ввода
bool RequestSql::setDataToDB(QString nameTable, QStringList listHeaders, QList<QStringList> listDatas)
{
    QString strSQL = "";
    strSQL = "INSERT INTO " + nameTable + " (";

    //Заголовки столбцов
    for (int i = 0; i < listHeaders.length(); i++)
    {
        strSQL += listHeaders[i];
        if (i+1 != listHeaders.length()) strSQL += ", ";
    }
    strSQL += ") VALUES ";

    //Данные записываю в строчку, сразу все.
    for (int i =0; i < listDatas.length(); i++)
    {
        strSQL += " (";
        for (int j = 0; j < listDatas[i].length(); j++)
        {
            if (listDatas[i][j] == "NULL") strSQL += listDatas[i][j];
            else strSQL += "'" + listDatas[i][j] + "'";
            if (j+1 != listDatas[i].count()) strSQL += ", ";
        }
        if (i + 1 < listDatas.count()) strSQL += "), ";
        else strSQL += ")";
    }

    QSqlQuery query;
    query.prepare(strSQL);

    if (!query.exec()){
        QMessageBox::warning(0, "Ошибка записи данных в БД", query.lastError().text());        
        return false;
    }
    return true;
}

//удаление таблицы
bool RequestSql::delTable(QString nameTable)
{
    QString strSQL;

    strSQL = "DROP TABLE " + nameTable;

    QSqlQuery query;
    query.prepare(strSQL);

    if (!query.exec()){
        QMessageBox::warning(0, "Ошибка удаления таблицы", query.lastError().text());
        return false;
    }
    return true;
}

//очистка таблицы
bool RequestSql::clearTable(QString nameTable)
{
    QString strSQL;

    strSQL = "DELETE FROM " + nameTable;

    QSqlQuery query;
    query.prepare(strSQL);

    if (!query.exec()){
        QMessageBox::warning(0, "Ошибка очистки таблицы", query.lastError().text());
        return false;
    }
    return true;
}

//удаление введеных данных
int RequestSql::clearInfoSelect(QString nameTable, QString columnTable, QString strValue)
{
    QString strSQL;

    strSQL = "DELETE FROM " + nameTable + " WHERE " + columnTable + " = '" + strValue + "'";

    QSqlQuery query;
    query.prepare(strSQL);

    if (!query.exec()){
        return -1;
    }
    if (query.numRowsAffected() > 0) return query.numRowsAffected();
    else return 0;
}
