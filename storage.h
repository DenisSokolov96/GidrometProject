#ifndef STORAGE_H
#define STORAGE_H

#include <QMap>

class Storage
{
private:

    Storage(){}
    ~Storage(){}

public:

    static Storage& Instance()
    {
        // согласно стандарту, этот код ленивый и потокобезопасный
        static Storage s;
        return s;
    }

    Storage( const Storage& )= delete;
    Storage& operator=( Storage& )= delete;

    QMap<QString, QList<QStringList>> storageTableMap;    
    QMap<QString, QString> storageHeaderMap;

    QString valToKey(QString searhKey);
    void getNameTableAndCol();
    QStringList changeLanguage(QString nameTable, QStringList listLan);

};

#endif // STORAGE_H
