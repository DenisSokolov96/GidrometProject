#include "outputwindow.h"
#include "ui_outputwindow.h"
#include "QMessageBox"
#include <QSqlQueryModel>
#include "requestSql.h"
#include <myiostreamfile.h>
#include <storage.h>

//получаю ссылку на объект одиночки
Storage& storageOut = Storage::Instance();

OutputWindow::OutputWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OutputWindow)
{
    ui->setupUi(this);    
    getNameTables();
    ui->selectCntStr->addItem("Получить 50 записей");
    ui->selectCntStr->addItem("Получить 100 записей");
    ui->selectCntStr->addItem("Получить 200 записей");
    ui->selectCntStr->addItem("Получить 400 записей");
    ui->selectCntStr->addItem("Получить 800 записей");
    ui->selectCntStr->addItem("Получить 1600 записей");
    ui->selectCntStr->addItem("Получить 3200 записей");
    ui->selectCntStr->addItem("Получить все записи");
    ui->selectCntStr->setCurrentIndex(2);    
    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(SlotSort(int)));
}

OutputWindow::~OutputWindow()
{
    delete ui;
}

//Получение списка имен таблиц с записью в выпадающее меню
void OutputWindow::getNameTables()
{
    if (storageOut.storageHeaderMap.count() > 0)
    {
        foreach (QString key, storageOut.storageHeaderMap.keys())
              ui->nameTable->addItem(storageOut.storageHeaderMap[key]);
        ui->nameTable->setCurrentIndex(0);
    }
}

//Подготовка таблицы
void OutputWindow::setInfo(QSqlQuery requestQuery)
{
    QString nameTable = storageOut.valToKey(ui->nameTable->currentText());
    int columns = storageOut.storageTableMap[nameTable][0].count();
    ui->tableWidget->setColumnCount(columns); //число колонок
    ui->tableWidget->setHorizontalHeaderLabels(storageOut.storageTableMap[nameTable][1]); // прописываю заголовки
    ui->tableWidget->setShowGrid(true); //сетка    
    // Разрешаем выделение построчно
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //Выделение нескольких кортежей
    ui->tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    // Растягиваем последнюю колонку на всё доступное пространство
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    for(int i = 0; requestQuery.next(); i++){
        ui->tableWidget->insertRow(i);
        for(int j = 0; j < columns; j++){
            if (requestQuery.value(j).isNull()){
                ui->tableWidget->setItem(i,j, new QTableWidgetItem(""));
            } else {
                ui->tableWidget->setItem(i,j, new QTableWidgetItem(requestQuery.value(j).toString()));
            }
        }
    }

    // Ресайз колонок по содержимому
    ui->tableWidget->resizeColumnsToContents();
    //Цвет колонок
    ui->tableWidget->setAlternatingRowColors(true);   
}

//Сохранение данных в файл
void OutputWindow::on_saveToCSV_clicked()
{
    QString textData;
    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
        textData += storageOut.storageTableMap[storageOut.valToKey(ui->nameTable->currentText())][1][i] + ", ";
    textData += "\n";
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {        
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
                textData += ui->tableWidget->item(i, j)->text();
                textData += ", ";
        }
        textData += "\n";
    }
    //Запись в файл
    Myiostreamfile myiostreamfile;
    myiostreamfile.saveToCSV(textData);
}

void OutputWindow::on_getDataBtn_clicked()
{
    ui->tableWidget->setRowCount(0);    
    QString strCntRep = ui->selectCntStr->currentText();
    strCntRep = parsStrRep(strCntRep);
    RequestSql requestSql;
    QSqlQuery query = requestSql.getAllTable(storageOut.valToKey(ui->nameTable->currentText()), strCntRep);
    setInfo(query);
}

QString OutputWindow::parsStrRep(QString str)
{
    QString resultStr;
    QStringList list = str.split(' ');

    if (list[1] == "все")
        resultStr = "ALL";
    else
        resultStr = list[1];

    return resultStr;
}

void OutputWindow::delSelectedRow()
{
    QModelIndexList selectedRows = ui->tableWidget->selectionModel()->selectedRows();
    while (!selectedRows.empty()) {
            ui->tableWidget->removeRow(selectedRows[0].row());
            selectedRows = ui->tableWidget->selectionModel()->selectedRows();
    }
}

void OutputWindow::on_delSelectedBtn_clicked()
{    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0, "Внимание", "Вы действительно хотите удалить выделенные элементы?");
    if (reply == QMessageBox::No) return;
    delSelectedRow();
}

void OutputWindow::SlotSort(int iColumn)
{
    ui->tableWidget->sortByColumn(iColumn);
}
