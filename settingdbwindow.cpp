#include "settingdbwindow.h"
#include "ui_settingdbwindow.h"
#include "connection.h"
#include <storage.h>

//получаю ссылку на объект одиночки
Storage& storageSttng = Storage::Instance();

extern QMap<QString, QString> storageConctMap;

SettingDBWindow::SettingDBWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDBWindow)
{
    ui->setupUi(this);
    ui->host_name_edit->setText(storageConctMap["host_name"]);
    ui->name_db_edit->setText(storageConctMap["name_db"]);
    ui->user_edit->setText(storageConctMap["user"]);
    ui->password_edit->setText(storageConctMap["password"]);
    ui->port_edit->setText(storageConctMap["port"]);
}

SettingDBWindow::~SettingDBWindow()
{
    delete ui;
}

void SettingDBWindow::on_btn_connect_clicked()
{
    storageConctMap["host_name"] = ui->host_name_edit->text();
    storageConctMap["name_db"] = ui->name_db_edit->text();
    storageConctMap["user"] = ui->user_edit->text();
    storageConctMap["password"] = ui->password_edit->text();
    storageConctMap["port"] = ui->port_edit->text();

    if (createConnection())
    {
        storageSttng.getNameTableAndCol();
          this->close();
    }
}
