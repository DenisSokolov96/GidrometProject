#ifndef REQUESTSQL_H
#define REQUESTSQL_H

#include <QString>
#include <QSqlQuery>

class RequestSql
{
public:
    RequestSql();
    QString createTables();
    QSqlQuery getAllTable(QString nameTable, QString strCntRep);
    QSqlQuery getNamesColumns(QString nameTable);
    QSqlQuery getNamesTables();
    QSqlQuery getDataForSlctColms(QString nameTable, QStringList listNameHeader, QString strCntRep);
    QSqlQuery getDataWhere(QString nameTable, QStringList listNameCol, QString strValue);
    bool setDataToDB(QString nameTable, QStringList listHeaders, QList<QStringList> listDatas);
    bool createTable(QStringList listParam);
    bool delTable(QString nameTable);
    bool clearTable(QString nameTable);
    int clearInfoSelect(QString nameTable, QString columnTable, QString strValue);
};

#endif // REQUESTSQL_H
