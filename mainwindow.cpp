#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include "changedatawindow.h"
#include "filterandgraphics.h"
#include <myiostreamfile.h>
#include <storage.h>
# include <mapworld.h>

//получаю ссылку на объект одиночки
Storage& storageMain = Storage::Instance();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Коментарии только для тестирования
    //Запуск окна подключения
    SettingDBWindow settingDBWindow;
    settingDBWindow.exec();

    //createConnection Только для разработки, сразу устанавливаем соединение
    //if (createConnection())
    //    storageMain.getNameTableAndCol();
    // END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //Функция для вывода подсказок
    myToolTip();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Получение данных
void MainWindow::on_btn_get_data_triggered()
{
    ui->statusbar->showMessage("Получение данных");
    OutputWindow window;
    window.showMaximized();
    window.exec();
}

//Отключение от БД
void MainWindow::on_btn_del_triggered()
{
    ui->statusbar->showMessage("Отключение об БД");
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0, "Внимание", "Вы действительно хотите отключиться от БД?");
    if (reply == QMessageBox::Yes)
        closeConnection();
}

//Добавлени/изменение данных
void MainWindow::on_btn_change_triggered()
{
    ChangeDataWindow window;
    window.showMaximized();
    window.exec();
    ui->statusbar->showMessage("Изменение данных");
}

//Выйти из программы
void MainWindow::on_btn_exit_triggered()
{
    QApplication::quit();
}

//НЕ РЕАЛИЗОВАН
//расчет лавинной опасности
void MainWindow::on_btn_avalanches_triggered()
{
     ui->statusbar->showMessage("Расчет лавинной опасности");
     CalcWindow window;     
     window.exec();
     ui->statusbar->showMessage("");
}

//Настройки подключения к БД
void MainWindow::on_btn_setting_db_triggered()
{
    ui->statusbar->showMessage("Настройки подключения к БД");
    SettingDBWindow settingDBWindow;    
    settingDBWindow.exec();
    ui->statusbar->showMessage("");
}

//Окно с фильтром и графиками
void MainWindow::on_FilterGrphBtn_triggered()
{
    ui->statusbar->showMessage("Фильтр и графики");
    FilterAndGraphics window;
    window.showMaximized();
    window.exec();
    ui->statusbar->showMessage("");
}

//Разработчики
void MainWindow::on_developBtn_triggered()
{
    ui->statusbar->showMessage("Разработчики");
    Myiostreamfile myiostreamfile;
    QString strInfo = myiostreamfile.readFileTxt(false);
    QMessageBox::information(0, "Разработчики", strInfo);
    ui->statusbar->showMessage("");
}

//Справка
void MainWindow::on_reference_triggered()
{
    ui->statusbar->showMessage("Справка");
    Myiostreamfile myiostreamfile;
    QString strInfo = myiostreamfile.readFileTxt(true);
    QMessageBox::information(0, "Справка", strInfo);
    ui->statusbar->showMessage("");
}

//Карта объектов
void MainWindow::on_buttonMap_triggered()
{
    ui->statusbar->showMessage("Карта объектов");
    MapWorld mapWorld;
    mapWorld.showMaximized();
    mapWorld.exec();
    ui->statusbar->showMessage("");
}

//Устанавливаю пояснения к картинкам
void MainWindow::myToolTip()
{
    QString toolTip = QString("<FONT COLOR=black>");
    toolTip += ("Лави́на — значительный объём снежной массы, падающей или соскальзывающей с крутых горных "
                "склонов со скоростью около 20—30 м/с. Сход лавины нередко сопровождается побочным "
                "эффектом в виде воздушной предлавинной волны, которая производит наибольшие разрушения.");
    toolTip += QString("</FONT>");
    ui->label->setToolTip(toolTip);

    toolTip = QString("<FONT COLOR=black>");
    toolTip += ("Па́водок — фаза водного режима реки, которая может многократно повторяться в различные "
                "сезоны года, характеризуется интенсивным увеличением расходов и уровней воды и "
                "вызывается дождями или обильным снеготаянием во время оттепелей. Периодически паводки "
                "не повторяются, и в этом их отличие от половодья.");
    toolTip += QString("</FONT>");
    ui->label_2->setToolTip(toolTip);

    toolTip = QString("<FONT COLOR=black>");
    toolTip += ("Наводне́ние — значительное затопление определённой территории земли в результате подъёма уровня\n"
                "воды в реке, озере, водохранилище или море, наносящее материальный ущерб экономике, социальной\n"
                "сфере и природной среде.");
    toolTip += QString("</FONT>");
    ui->label_3->setToolTip(toolTip);

    toolTip = QString("<FONT COLOR=black>");
    toolTip += ("Сель, или селевой поток — стремительный русловой поток, состоящий из смеси воды и обломков "
                "горных пород, внезапно возникающий в бассейнах небольших горных рек. Главной особенностью "
                "таких потоков служит высокая насыщенность обломочным материалом, которая составляет "
                "от 10 до 75% объема движущейся массы.");
    toolTip += QString("</FONT>");
    ui->label_4->setToolTip(toolTip);
}
