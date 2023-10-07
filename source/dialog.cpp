#include "../headers/dialog.h"
#include "../headers/database.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // Default connection data y
    ui->txt_host_name->setText("981757-ca08998.tmweb.ru");
    ui->txt_bd_name->setText("netology_cpp");
    ui->txt_login->setText("netology_usr_cpp");
    ui->txt_pwd->setText("CppNeto3");
    ui->sb_port->setValue(5432);
}

Dialog::~Dialog()
{    
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    qDebug() << "DEBUG: in Dialog::on_buttonBox_accepted()";

    data.insert(hostName, ui->txt_host_name->text());
    data.insert(dbName, ui->txt_bd_name->text());
    data.insert(login, ui->txt_login->text());
    data.insert(pass, ui->txt_pwd->text());
    data.insert(port, ui->sb_port->text());

    sendData(data);
}

void Dialog::sendData(QMap<fieldsForConnect, QString> dbData)
{
    qDebug() << "DEBUG: in sendData(QMap<fieldsForConnect, QString> dbData)";

    emit signal_sendData(dbData);
}
