#include "changedatawindow.h"
#include "ui_changedatawindow.h"
#include <requestSql.h>
#include "QMessageBox"
#include <QFileDialog>
#include <QTextStream>
#include <myiostreamfile.h>
#include <storage.h>

//получаю ссылку на объект одиночки
Storage& storageChange = Storage::Instance();

ChangeDataWindow::ChangeDataWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeDataWindow)
{
    ui->setupUi(this);
    ui->selectTableBox->addItems(storageChange.storageHeaderMap.values());
    getNameTables();
    ui->addTableWidget->setAlternatingRowColors(true);
    connect(ui->addTableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(SlotSort(int)));
}

ChangeDataWindow::~ChangeDataWindow()
{
    delete ui;
}

//Получение списка имен таблиц с записью в выпадающее меню
void ChangeDataWindow::getNameTables()
{
    if (storageChange.storageHeaderMap.count() > 0)
    {
        //заполняю все выпадающие списки информацией
        foreach (QString key, storageChange.storageHeaderMap.keys())
        {
            ui->listNameTable->addItem(storageChange.storageHeaderMap[key]);
            ui->listNameTablesDelCrt->addItem(storageChange.storageHeaderMap[key]);
        }
        ui->listNameTable->setCurrentIndex(0);
        ui->listNameTablesDelCrt->setCurrentIndex(0);
    }
}

//Добавление строк в таблицу добавления данных для записи
void ChangeDataWindow::on_addRowBtn_clicked()
{
    ui->addTableWidget->insertRow(ui->addTableWidget->rowCount());
    ui->addTableWidget->setItem(ui->addTableWidget->rowCount()-1, 0, new QTableWidgetItem());
}

//При загрузке окна подготовка заголовков таблицы
void ChangeDataWindow::on_listNameTable_currentTextChanged(const QString &arg1)
{
    QString nameTable;
    foreach (QString key, storageChange.storageHeaderMap.keys()) {
        if (storageChange.storageHeaderMap[key] == ui->listNameTable->currentText())
        {
            nameTable = key;
            break;
        }
    }
    int columns = storageChange.storageTableMap[nameTable][0].count();
    ui->addTableWidget->setColumnCount(columns); //число колонок
    // прописываю заголовки
    ui->addTableWidget->setHorizontalHeaderLabels(storageChange.storageTableMap[nameTable][1]);
    ui->addTableWidget->setShowGrid(true); //сетка   
    // Разрешаем выделение построчно
    ui->addTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Растягиваем последнюю колонку на всё доступное пространство
    ui->addTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->addTableWidget->setRowCount(0);
    ui->addTableWidget->resizeColumnsToContents();
    //Выделение нескольких кортежей
    ui->addTableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
}

//Кнопка сохранения в бд
void ChangeDataWindow::on_pushData_clicked()
{
    QMessageBox::StandardButton reply;
    QString nameTableRus = ui->listNameTable->currentText();
    QString nameTableUs = storageChange.valToKey(nameTableRus);
    reply = QMessageBox::question(0, "Внимание", "Отправить данные в таблицу: " + nameTableRus + " ?");
    if (reply == QMessageBox::No) return;

    RequestSql requestSql;
    QStringList listHeaders = storageChange.storageTableMap[nameTableUs][0];
    int columns = ui->addTableWidget->columnCount();
    int rows= ui->addTableWidget->rowCount();
    int countRecord = 0;
    QList<QStringList> listDataToDB;
    for(int i = 0; i < rows; i++)
    {
        QStringList listDatas;
        for(int j = 0; j < columns; j++)        
        {
            if (ui->addTableWidget->item(i,j) == 0 || ui->addTableWidget->item(i,j)->text() == "") listDatas.append("NULL");
            else listDatas.append(ui->addTableWidget->item(i,j)->text());
        }
        listDataToDB.append(listDatas);
    }
    if (requestSql.setDataToDB(nameTableUs, listHeaders, listDataToDB))
        QMessageBox::information(0, "Информация", "Данные внесены в БД");
    else
        QMessageBox::warning(0, "Ошибка", "Ошибка записи в БД");

}

//Кнопка удаления таблиц по выбору
void ChangeDataWindow::on_delTblBtn_clicked()
{
    QString nameTableRus = ui->listNameTablesDelCrt->currentText();
    QString nameTableUs = storageChange.valToKey(nameTableRus);
    // проверка на удаление
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0, "Внимание", "Вы действительно хотите удалить таблицу: " + nameTableRus);
    if (reply == QMessageBox::Yes)
    {
        RequestSql requestSql;
        requestSql.delTable(nameTableUs);
        ui->listNameTablesDelCrt->clear();
        getNameTables();
    }
}

//Кнопка очистки таблиц по выбору
void ChangeDataWindow::on_clearTblBtn_clicked()
{
    QString nameTableRus = ui->listNameTablesDelCrt->currentText();
    QString nameTableUs = storageChange.valToKey(nameTableRus);
    // проверка на очистку
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0, "Внимание", "Вы действительно хотите очистить таблицу: " + nameTableRus);
    if (reply == QMessageBox::Yes)
    {
        RequestSql requestSql;
        requestSql.clearTable(nameTableUs);
    }
}

/*
 * Вообщем, надо делать вторую функцию для парсера, куда вывести:
 * 1 - разделители, то есть только точка с запятой
 * 2 - вложенный цикл поиска совпадений значений
 * 3 - вывод id, так как в самих данных его нет, он хранится отдельно
 * */
//Событие на загрузку данных из файла
void ChangeDataWindow::on_LoadFileBtn_clicked()
{    
    Myiostreamfile myiostreamfile;
    QList<QList<QString>> listData = myiostreamfile.loadFile("[,;]"); //"[,;]"


    if (listData.length() == 0){
        return;
    }
    else
    {
        //фильтрация        
        QStringList listNameHeader = storageChange.storageTableMap[storageChange.valToKey(ui->listNameTable->currentText())][1];
        refreshTable(listNameHeader);
        QList <int> filter;

        //поиск с какой строки начинается таблиц
        int idStart = 0;
        for (int i=0; i<listData.length(); i++)
        {
            if (listData[i][0].indexOf("#") == -1)
            {
                idStart = i;
                break;
            }
        }
                //поиск id

                //костыль для данных с метеостанциий
                for (int i=0; i < listData[idStart].length(); i++)
                {
                    for (int j=0; j < listNameHeader.length(); j++)
                    {
                        int index = listData[idStart][i].indexOf(listNameHeader[j]);
                        if (index != -1)
                            filter.append(i);
                    }
                }

        //Для документа, данные начинаются с idStart
        for (int i=0; i < listNameHeader.length(); i++)
        {
            int index = listData[idStart].indexOf(listNameHeader[i]);
            if (index != -1)
                filter.append(index);
        }

        //конец фильтрации

        //Добавление данных в табличку
        for (int lineindexCSV = idStart + 1, countRow = 0; lineindexCSV < listData.length(); lineindexCSV++, countRow++)
        {
            ui->addTableWidget->insertRow(ui->addTableWidget->rowCount());
            for (int j = 0; j < listData[lineindexCSV].length(); j++)
            {
                int index = filter.indexOf(j);
                if (index > -1)
                    ui->addTableWidget->setItem(countRow, index, new QTableWidgetItem(quotPars(listData[lineindexCSV][j])));
            }
        }
        ui->addTableWidget->resizeColumnsToContents();
    }
}

//Обновить заголовки таблицы при чтении из файла
void ChangeDataWindow::refreshTable(QStringList headerToken)
{
    //очистка таблицы
    ui->addTableWidget->setRowCount(0);
    int columns = headerToken.count();
    ui->addTableWidget->setColumnCount(columns); //число колонок
    // прописываю заголовки
    ui->addTableWidget->setHorizontalHeaderLabels(headerToken);
    ui->addTableWidget->resizeColumnsToContents();
}

//Проверка на ковычки
QString ChangeDataWindow::quotPars(QString strPars)
{
    QString newStr;

    for (int i=0; i<strPars.length(); i++)
    {
        if (strPars[i] != "'") newStr += strPars[i];
    }

    return newStr;
}

//Удаление выбранных строк
void ChangeDataWindow::delSelectedRow()
{
    QModelIndexList selectedRows = ui->addTableWidget->selectionModel()->selectedRows();
    while (!selectedRows.empty()) {
            ui->addTableWidget->removeRow(selectedRows[0].row());
            selectedRows = ui->addTableWidget->selectionModel()->selectedRows();
    }
}

//Событие на нажатие кнопки для удаления
void ChangeDataWindow::on_pushButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0, "Внимание", "Вы действительно хотите удалить выделенные элементы?");
    if (reply == QMessageBox::No) return;
    delSelectedRow();
}

void ChangeDataWindow::SlotSort(int iColumn)
{
    ui->addTableWidget->sortByColumn(iColumn);
}

//Событие на нажатие кнопки удаления данных из БД
void ChangeDataWindow::on_delBtn_clicked()
{
    delStrInDB();
}

//Метод удаления вводимых данных
void ChangeDataWindow::delStrInDB()
{
    RequestSql requestSql;    
    QString lineSearch = ui->lineDelEdit->text();
    QString nameTable = storageChange.valToKey(ui->selectTableBox->currentText());
    QStringList listNameCol = storageChange.storageTableMap[nameTable][0];    
    int res = 0;

    for (int j =0; j < listNameCol.length(); j++)
    {
        int checkRes = requestSql.clearInfoSelect(nameTable, listNameCol[j],lineSearch);
        if (checkRes > -1)
            res += checkRes;
    }

    if (res > 0) QMessageBox::information(0,"Выполнено", "Количество удалённых данных: " + QString::number(res));
    else QMessageBox::information(0,"Выполнено", "Информация не найдена!\n");

}
