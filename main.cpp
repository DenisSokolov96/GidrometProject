#include "mainwindow.h"
#include <QApplication>

QMap<QString, QString> storageConctMap = {{"host_name", "localhost"},
                                          {"name_db", "postgres"},
                                          {"user", "postgres"},
                                          {"password", "1"},
                                          {"port", "5432"}};

int main(int argc, char *argv[])
{    
    QApplication app(argc, argv);
    //Запуск главного окна
    MainWindow w;
    w.show();
    return app.exec();
}
