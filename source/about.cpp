#include "../headers/about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    QPixmap pix(":/images/duck");
    mo_about.setFileName(":/images/duck");
    ui->lable_image->setMovie(&mo_about);
    mo_about.start();
}

About::~About()
{
    delete ui;
}

void About::on_pb_close_about_clicked()
{
    this->close();
}

