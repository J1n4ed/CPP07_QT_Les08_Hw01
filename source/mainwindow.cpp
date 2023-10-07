#include "../headers/mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Исходное состояние виджетов

    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    /*
     * Выделим память под необходимые объекты. Все они наследники
     * QObject, поэтому воспользуемся иерархией.
    */

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);
    about = new About(this);
    dialog = new Dialog(this);

    //Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    /*
     * Устанавливаем данные для подключениея к БД.
     * Поскольку метод небольшой используем лямбда-функцию.
     */
    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });

    /*
     * Соединяем сигнал, который передает ответ от БД с методом, который отображает ответ в ПИ
     */
     connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

    /*
     *  Сигнал для подключения к БД
     */
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    connect(dialog, &Dialog::signal_sendData, this, [&](QMap<fieldsForConnect, QString> recieveData){
        mapForConnect = recieveData;
    });

    connect(this, &MainWindow::signal_make_query, dataBase, &DataBase::recieve_make_query);
    connect(dataBase, &DataBase::signal_create_view, this, &MainWindow::recieve_create_view);
    connect(this, &MainWindow::signal_clear_query, dataBase, &DataBase::recieve_clear_query);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    qDebug() << "DEBUG: in on_act_addData_triggered()";

    //Отобразим диалоговое окно. Какой метод нужно использовать?
    connectorKey = 1;
    dataDb->show();
}

void MainWindow::on_act_addData2_triggered()
{
    qDebug() << "DEBUG: in on_act_addData2_triggered()";

    connectorKey = 2;
    dialog->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered()
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    qDebug() << "DEBUG: in on_act_connect_triggered()";

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");

       if (connectorKey == 1)
       {
           qDebug() << "DEBUG: connecting using QVector<>";
           auto conn1 = [&]{dataBase->ConnectToDataBase(dataForConnect);};
           QtConcurrent::run(conn1);
       }
       else if (connectorKey == 2)
       {
           qDebug() << "DEBUG: connecting using QMap<>";
           auto conn2 = [&]{dataBase->ConnectToDataBase(mapForConnect);};
           QtConcurrent::run(conn2);
       }
       else
       {
           qDebug() << "DEBUG: ConnectorKey in wrong or unknown state";
       }
    }
    else
    {
        qDebug() << "DEBUG: Disconnection...";
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }
}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{
    qDebug() << "DEBUG: in on_pb_request_clicked()";
    ///Тут должен быть код ДЗ

    int type = get_type();

    emit signal_make_query(type);
}

// ОЧИСТИТЬ
void MainWindow::on_pb_clear_clicked()
{
    qDebug() << "DEBUG: in on_pb_clear_clicked()";

    emit signal_clear_query();
}


/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
void MainWindow::ScreenDataFromDB(const QTableWidget *widget, int typeRequest)
{
    qDebug() << "DEBUG: in ScreenDataFromDB(const QTableWidget *widget, int typeRequest)";
    ///Тут должен быть код ДЗ
}
/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    qDebug() << "DEBUG: in ReceiveStatusConnectionToDB(bool status)";

    if(status)
    {
        qDebug() << "DEBUG: DB connected case";

        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else
    {
        qDebug() << "DEBUG: DB disconnected case";

        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }
}

void MainWindow::recieve_create_view(QSqlQueryModel *model)
{
    if ( !viewKey )
    {
        ui->tb_result->setModel(model);
        ui->tb_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        viewKey = true;
    }

    ui->tb_result->update();
}

void MainWindow::on_act_about_triggered()
{
    qDebug() << "DEBUG: in on_act_about_triggered()";

    about->show();
}

int MainWindow::get_type()
{
    qDebug() << "DEBUG: in get_type()";

    int type;
    type = ui->cb_category->currentIndex() + 1;
    return type;
}
