#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

extern QMap<QString, QString> storageConctMap;

inline bool createConnection()//подключение
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    db.setHostName(storageConctMap["host_name"]);
    db.setDatabaseName(storageConctMap["name_db"]);
    db.setUserName(storageConctMap["user"]);
    db.setPassword(storageConctMap["password"]);
    db.setPort(storageConctMap["port"].toInt());

    if (!db.open()){
        QMessageBox::warning(0, "Ошибка подключения", db.lastError().text());
        return false;
    }
    else
    {
        QMessageBox::information(0, "Успешно", "Соединение с БД установлено!");
        return true;
    }
}

inline void closeConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.close();
    if (!db.open())
        QMessageBox::warning(0, "Отключение", "Отключение выполнено");
    else
        QMessageBox::information(0, "Ошибка отключения", db.lastError().text());

}
#endif // CONNECTION_H
