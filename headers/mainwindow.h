#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include "database.h"
#include "dbdata.h"
#include "dialog.h"
#include "about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void ScreenDataFromDB(const QTableWidget *widget, int typeRequest);
    void ReceiveStatusConnectionToDB(bool status);

    void recieve_create_view(QSqlQueryModel * model);

private slots:

    void on_act_addData_triggered();
    void on_act_connect_triggered();
    void on_pb_request_clicked();
    void on_act_about_triggered();
    void on_act_addData2_triggered();

    void on_pb_clear_clicked();

signals:

    void sig_RequestToDb(QString request);

    void signal_make_query(int type);
    void signal_clear_query();

private:

    QVector<QString> dataForConnect; //Данные для подключения к БД.
    QMap<fieldsForConnect, QString> mapForConnect;

    Ui::MainWindow *ui;
    DbData *dataDb;
    DataBase* dataBase;
    QMessageBox* msg;
    About * about;
    Dialog * dialog;
    int connectorKey = 0;
    bool viewKey = false;

    QString request = "SELECT title, release_year, c.name  FROM film f "
                      "JOIN film_category fc on f.film_id = fc.film_id "
                      "JOIN category c on c.category_id  = fc.category_id";

    int get_type();

};
#endif // MAINWINDOW_H
