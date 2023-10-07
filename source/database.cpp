#include "../headers/database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    dataBase = new QSqlDatabase();
    sqlModel = new QSqlQueryModel(this);
}

DataBase::~DataBase()
{
    delete dataBase;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{
    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    ///Тут должен быть код ДЗ

    bool status;
    status = dataBase->open( );
    emit sig_SendStatusConnection(status);
}

void DataBase::ConnectToDataBase(QMap<fieldsForConnect, QString> data)
{
    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    ///Тут должен быть код ДЗ

    bool status;
    status = dataBase->open( );
    emit sig_SendStatusConnection(status);
}

void DataBase::recieve_make_query(int type)
{
    qDebug() << "DEBUG: in recieve_make_query(int type)";

    if ( dataBase->isOpen() )
    {
        qDebug() << "DEBUG: DB check for open - TRUE";

        sqlQuery = new QSqlQuery();
        *sqlQuery = QSqlQuery(*dataBase);

        switch (type)
        {
        case requestAllFilms:

            qDebug() << "DEBUG: Setting model to all films..";

            if (sqlQuery->exec("SELECT title, description FROM film"))
            {
                sqlModel->setQuery(std::move(*sqlQuery));
            }

            break;
        case requestComedy:

            qDebug() << "DEBUG: Setting model to Comdey..";

            if (sqlQuery->exec("SELECT title, description FROM film f JOIN film_category fc ON f.film_id = fc.film_id JOIN category c ON c.category_id = fc.category_id WHERE c.name = 'Comedy'"))
            {
                sqlModel->setQuery(std::move(*sqlQuery));
            }

            break;
        case requestHorrors:

            qDebug() << "DEBUG: Setting model to Horrors..";

            if (sqlQuery->exec("SELECT title, description FROM film f JOIN film_category fc ON f.film_id = fc.film_id JOIN category c ON c.category_id = fc.category_id WHERE c.name = 'Horror'"))
            {
                sqlModel->setQuery(std::move(*sqlQuery));
            }

            break;
        default:

            qDebug() << "DEBUG: type in wrong state..";

            break;
        }

        delete sqlQuery;

        qDebug() << "DEBUG: Setting headers..";

        sqlModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Название"));
        sqlModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Описание"));

        qDebug() << "DEBUG: Emiting create_view..";

        emit signal_create_view(sqlModel);
    }
    else
    {
        qDebug() << "DEBUG: DB check for open - FALSE";
    }
}

void DataBase::recieve_clear_query()
{
    qDebug() << "DEBUG: in recieve_clear_query()";

    sqlQuery = new QSqlQuery();
    *sqlQuery = QSqlQuery(*dataBase);

    sqlModel->setQuery(std::move(*sqlQuery));

    sqlModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Название"));
    sqlModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Описание"));

    delete sqlQuery;

    emit signal_create_view(sqlModel);
}


/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();
}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request)
{
    ///Тут должен быть код ДЗ
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
