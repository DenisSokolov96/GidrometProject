#include "myiostreamfile.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>


Myiostreamfile::Myiostreamfile(QWidget *parent) : QDialog(parent)
{
}

void Myiostreamfile::saveToCSV(QString textData)
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Сохранить"), "",tr("Ссылка на документ (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
           return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
           QMessageBox::warning(this, tr("Ошибка сохранения"), file.errorString());
           return;
        }
        QTextStream out(&file);
        out << textData;
        file.close();
    }
    QMessageBox::information(this, "Успешно!", "Данные записаны в файл");
}

//Чтение файла (для пункта: 0 - Разработчики и 1 - Справка)
QString Myiostreamfile::readFileTxt(int flagIndex)
{
    QString line;
    QFile file(  ":/rec/textRead.txt" );
    if(file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        while(!file.atEnd())
        {
            line += file.readLine();
        }
    }
    else
    {
        switch (flagIndex) {
        case 0:
            return "Программу разработал Соколов Денис";
        case 1:
            return "Информация скрыта по просьбе правообладателя.";
        default:
            break;
        }
    }
    QStringList lst = line.split("***");
    return lst[flagIndex];
}

//Чтение из файла
QList<QList<QString>> Myiostreamfile::loadFile(QString strSplit)
{
    QList<QList<QString>> listData;

    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Загрузить"), "",tr("Ссылка на документ (*.csv);(*.xlsx);All Files (*)"));
    if (fileName.isEmpty())
    {
        QMessageBox::information(0, "Информация", "Файл не загружен");
        return listData;
    }
    else
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            bool firstIteration = false;
            while (!in.atEnd())
            {
                QString fileLine = in.readLine();
                QRegExp rx(strSplit);
                QStringList lineToken = fileLine.split(rx);
                if (!firstIteration)
                {
                    listData.append(lineToken);
                    //Запись имени файла в выпадающие меню
                    firstIteration = true;
                }
                else
                {
                    listData.append(lineToken);
                }
            }
            file.close();
        }
    }
    return listData;
}

