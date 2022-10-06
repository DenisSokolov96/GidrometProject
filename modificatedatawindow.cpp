#include "modificatedatawindow.h"
#include "ui_modificatedatawindow.h"

ModificateDataWindow::ModificateDataWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModificateDataWindow)
{
    ui->setupUi(this);
}

ModificateDataWindow::~ModificateDataWindow()
{
    delete ui;
}
