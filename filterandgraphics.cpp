#include "filterandgraphics.h"
#include "ui_filterandgraphics.h"
#include <myiostreamfile.h>
#include <storage.h>
#include <requestSql.h>
#include <QChartView>

//получаю ссылку на объект одиночки
Storage& storageFilter = Storage::Instance();

FilterAndGraphics::FilterAndGraphics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterAndGraphics)
{
    ui->setupUi(this);
    getNameTables();
    //Всплывающие подсказки
    myToolTip();
    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(SlotSort(int)));
}

FilterAndGraphics::~FilterAndGraphics()
{
    delete ui;
}

/***Построение интерфейса графиков и вывод графика/ов***/

//Построить график
void FilterAndGraphics::on_createGraph_clicked()
{    
    if (listHeaderOutRUS.length() < 2) return;

    QList<double> listParamX;
    QList<double> listParamY;
    int idCol1 = 0, idCol2 = 1;
    for (int i=0; i < ui->tableWidget->rowCount(); i++)
    {

      listParamX.append(ui->tableWidget->item(i, idCol2)->text().toDouble());
      listParamY.append(i+1);

       //listParamY.append(ui->tableWidget->item(i, idCol1)->text().toDouble());
    }
    textTitle = "оX: " + listHeaderOutRUS[idCol1] + " / ";
    textTitle += "оY: " + listHeaderOutRUS[idCol2];

    QSplineSeries* series = new QSplineSeries();
    for (int i=0; i<listParamX.length(); i++)
        series->append(listParamY[i], listParamX[i]);

    QValueAxis *axisX = new QValueAxis();
    //axisX->setLabelFormat("%.2f");
    QValueAxis *axisY = new QValueAxis();
    //axisY->setLabelFormat("%.2f");

    chart = new QChart();
    chart->setTitle(textTitle);
    chart->addSeries(series);
    chart->legend()->setVisible(false);
    //chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);

    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

// Сохранение картинки по нажатию кнопки
void FilterAndGraphics::on_savePic_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Сохранить"), "",tr("Ссылка на документ (*.png);;All Files (*)"));
    if (fileName.isEmpty())
           return;
    else {
        try{
            QPixmap pixMap = QPixmap::grabWidget(ui->graphicsView);
            fileName.append(".png");
            pixMap.save(fileName);
        }catch(...){
            QMessageBox::warning(this, "Ошибка сохранения", "Файл не сохранен!");
        }
    }
    QMessageBox::information(this, "Успешно!", "График сохранен");
}

/***Построение фильтра и его применение к таблице***/

//Получение списка имен таблиц с записью в выпадающее меню
void FilterAndGraphics::getNameTables()
{
    if (storageFilter.storageHeaderMap.count() > 0)
    {
        foreach (QString key, storageFilter.storageHeaderMap.keys())
              ui->nameTables->addItem(storageFilter.storageHeaderMap[key]);
        ui->nameTables->setCurrentIndex(0);
    }
    //Устанавливаю разноцветные строки у таблицы
    ui->tableWidget->setAlternatingRowColors(true);
}

//Запись из таблицы в файл
void FilterAndGraphics::on_saveToCSVBtn_clicked()
{
    QString textData;
    //Добавляю заголовки
    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
        textData += storageFilter.storageTableMap[storageFilter.valToKey(ui->nameTables->currentText())][1][i] + ", ";
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

void FilterAndGraphics::on_getDataBtn_clicked()
{
    ui->tableWidget->setRowCount(0);
    RequestSql requestSql;
    nameTableOut = storageFilter.valToKey(ui->nameTables->currentText());
    listHeaderOutRUS = getInfoOutCheckBoxs();
    listHeaderOutUS = storageFilter.changeLanguage(nameTableOut, listHeaderOutRUS);
    QString countRowSpinBox;
    if (getAllOut()) countRowSpinBox = "ALL";
    else countRowSpinBox = QString::number(getInfoOutSpinBoxs());

    if (listHeaderOutUS.length() == 0)
    {
        QMessageBox::question(this, "Внимание", "Укажите, хотя бы, один столбец.",QMessageBox::Ok);
    } else {
        QSqlQuery query = requestSql.getDataForSlctColms(nameTableOut, listHeaderOutUS, countRowSpinBox);
        setInfo(query);
    }
}

//Подготовка таблицы и вывод данных
void FilterAndGraphics::setInfo(QSqlQuery requestQuery)
{
    int columns = listHeaderOutUS.length();
    ui->tableWidget->setColumnCount(columns); //число колонок
    ui->tableWidget->setHorizontalHeaderLabels(listHeaderOutRUS); // прописываю заголовки
    ui->tableWidget->setShowGrid(true); //сетка
    // Разрешаем выделение построчно
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //Выделение нескольких кортежей
    ui->tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    // Растягиваем последнюю колонку на всё доступное пространство
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    for(int i = 0; requestQuery.next(); i++)
    {
        ui->tableWidget->insertRow(i);
        for (int j = 0; requestQuery.value(j).isValid(); j++)
            ui->tableWidget->setItem(i,j, new QTableWidgetItem(requestQuery.value(j).toString()));
    }

    // Ресайз колонок по содержимому
    ui->tableWidget->resizeColumnsToContents();
}

//Создание и распределение QCheckBoxs
void FilterAndGraphics::addCheckBoxs()
{
    QString nameTable = storageFilter.valToKey(ui->nameTables->currentText());
    QStringList list = storageFilter.storageTableMap[nameTable][1];
    int countDown = 0;
    int countRight = 0;
    int listLength = list.length();
    for(auto column = list.begin(); column!=list.end(); ++column)
    {
        QCheckBox *checkBox = new QCheckBox(*column, this);
        checkBox->setChecked(false);
        ui->settingGridLayout->addWidget(checkBox, countDown, countRight, Qt::AlignLeft);
        countDown++;
        if (countDown * 2 > listLength)
        {
            countDown = 0;
            countRight = 1;
        }
    }
}

//Создание QCheckBox
void FilterAndGraphics::addCheckBox(QString str)
{
    int endEl = ui->settingGridLayout->count() + 1;
    QCheckBox *checkBox = new QCheckBox(str, this);
    checkBox->isCheckable();
    checkBox->setChecked(false);
    ui->settingGridLayout->addWidget(checkBox, endEl / 2, 0, Qt::AlignLeft);        
}

// Построение виджетов по выбору таблицы из выпадающего меню
void FilterAndGraphics::on_nameTables_currentTextChanged(const QString &arg1)
{
    delOldCheckBoxs();
    addCheckBoxs();
    //Кол-во выводимых элементов:
    addLabel("Кол-во выводимых строк: ");
    addSpinBox();
    //Кол-во выводимых элементов:
    addCheckBox("Показать все строки");
}

//Добавление SpinBox
void FilterAndGraphics::addSpinBox()
{
    int row = ui->settingGridLayout->count() / 2;

    QSpinBox *spinBox = new QSpinBox(this);
    spinBox->setMaximum(1000000);
    spinBox->setValue(500);
    ui->settingGridLayout->addWidget(spinBox, row , 1, Qt::AlignLeft);
}

//Добавление Label
void FilterAndGraphics::addLabel(QString str)
{
    int row = 0;
    if (ui->settingGridLayout->count() % 2 != 0)
        row = (ui->settingGridLayout->count() + 1) / 2;
    else row = ui->settingGridLayout->count() / 2;

    QLabel *label = new QLabel(this);
    label->setText(str);
    ui->settingGridLayout->addWidget(label, row, 0, Qt::AlignLeft);
}

//Очистка settingGridLayout от QCheckBoxs предыдущей таблицы
void FilterAndGraphics::delOldCheckBoxs()
{
    while( ui->settingGridLayout->count() )
    {
        QWidget* widget = ui->settingGridLayout->itemAt(0)->widget();
        if( widget )
        {
            ui->settingGridLayout->removeWidget(widget);
            delete widget;
        }
    }
}

//получение данных из виджетов CheckBoxs
QStringList FilterAndGraphics::getInfoOutCheckBoxs()
{
    QStringList listData;
    int index = 0;
    while( index < ui->settingGridLayout->count() )
    {
        QWidget* widget = ui->settingGridLayout->itemAt(index)->widget();
        if (qobject_cast<QCheckBox*>(widget))
        {
            QString strName = qobject_cast<QCheckBox*>(widget)->text();
            if ( qobject_cast<QCheckBox*>(widget)->isChecked() && strName != "Показать все строки")
            {
                listData.append( storageFilter.valToKey(strName) );
            }
        }
        index++;
    }
    return listData;
}

//получение данных из виджетов SpinBoxs
int FilterAndGraphics::getInfoOutSpinBoxs()
{
    int count;
    int index = 0;
    while( index < ui->settingGridLayout->count() )
    {
        QWidget* widget = ui->settingGridLayout->itemAt(index)->widget();
        if (qobject_cast<QSpinBox*>(widget))
        {
            QString strName = qobject_cast<QSpinBox*>(widget)->text();
            count = qobject_cast<QSpinBox*>(widget)->value();
            return count;
        }
        index++;
    }
    // может вернут 0 если не найдет элемента SpinBoxs
    return count;
}

// Проверка QCheckBox на вывод всех строк
bool FilterAndGraphics::getAllOut()
{
    int index = 0;
    while( index < ui->settingGridLayout->count() )
    {
        QWidget* widget = ui->settingGridLayout->itemAt(index)->widget();        
        if (qobject_cast<QCheckBox*>(widget))
        {
            if (qobject_cast<QCheckBox*>(widget)->text() == "Показать все строки" &&
                    qobject_cast<QCheckBox*>(widget)->isChecked())
                return true;
        }
        index++;
    }

    return false;
}

//Подсказки - toolTips
void FilterAndGraphics::myToolTip()
{
    ui->graphicsView->setToolTip("График зависимостей строится по первым двум столбцам данных");
    ui->graphicsView->setToolTipDuration(3000);
    ui->tableWidget->setToolTip("Таблица вывода выбранных данных по фильтру");
    ui->tableWidget->setToolTipDuration(3000);
}

//Удаление выбранных строк
void FilterAndGraphics::delSelectedRow()
{
    QModelIndexList selectedRows = ui->tableWidget->selectionModel()->selectedRows();
    while (!selectedRows.empty()) {
            ui->tableWidget->removeRow(selectedRows[0].row());
            selectedRows = ui->tableWidget->selectionModel()->selectedRows();
    }
}

//Событие на нажатие кнопки для удаления
void FilterAndGraphics::on_pushButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0, "Внимание", "Вы действительно хотите удалить выделенные элементы?");
    if (reply == QMessageBox::No) return;
    delSelectedRow();
}

void FilterAndGraphics::SlotSort(int iColumn)
{
    ui->tableWidget->sortByColumn(iColumn);
}
