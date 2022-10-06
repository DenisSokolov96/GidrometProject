#include "storage.h"
#include <requestSql.h>
#include <QSqlQuery>
#include <QVariant>

QString Storage::valToKey(QString searhKey)
{
    QString nameTable;
    foreach (QString key, storageHeaderMap.keys()) {
        if (storageHeaderMap[key] == searhKey)
            return key;
    }
    return searhKey;
}

void Storage::getNameTableAndCol()
{

    RequestSql requestSql;
    QSqlQuery query = requestSql.getAllTable("nameheaders","ALL");
    while(query.next())
    {
        QString name_us = query.value(1).toString();
        QString name_rus = query.value(2).toString();
        QString name_table = query.value(4).toString();
        QString field = query.value(3).toString();

        if (field == "table")
        {
            QStringList list = {};
            storageTableMap[name_table].append(list);
            storageTableMap[name_table].append(list);
            storageHeaderMap[name_table] = name_rus;
        }
        else if (field == "col")
        {
            storageTableMap[name_table][0].append(name_us);
            storageTableMap[name_table][1].append(name_rus);
        }
    }
}

QStringList Storage::changeLanguage(QString nameTable, QStringList listLan)
{
    QStringList listData;

    for (int i = 0; i < listLan.length(); i++)
    {
        int index = storageTableMap[nameTable][1].indexOf(listLan[i]);
        listData.append(storageTableMap[nameTable][0][index]);
    }
    return listData;
}


